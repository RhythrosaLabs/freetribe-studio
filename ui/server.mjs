import http from 'node:http';
import {execFile} from 'node:child_process';
import {promisify} from 'node:util';
import fs from 'node:fs/promises';
import path from 'node:path';
import os from 'node:os';
import { fileURLToPath } from 'node:url';

const root = path.dirname(fileURLToPath(import.meta.url));
const port = Number(process.env.PORT || 8080);
const execFileAsync = promisify(execFile);
const pluginRoots = [
  path.join(os.homedir(), 'Library/Audio/Plug-Ins/VST3'),
  path.join(os.homedir(), 'Library/Audio/Plug-Ins/VST'),
  path.join(os.homedir(), 'Library/Audio/Plug-Ins/Components'),
  '/Library/Audio/Plug-Ins/VST3',
  '/Library/Audio/Plug-Ins/VST',
  '/Library/Audio/Plug-Ins/Components'
];

const formatFor = name => name.endsWith('.vst3') ? 'VST3' : name.endsWith('.vst') ? 'VST2' : 'Audio Unit';
const displayName = name => name.replace(/\.(vst3|vst|component)$/i, '');
async function discoverPlugins() {
  const plugins = [];
  for (const directory of pluginRoots) {
    try {
      const entries = await fs.readdir(directory, {withFileTypes:true});
      for (const entry of entries) {
        if (!entry.isDirectory() || !/\.(vst3|vst|component)$/i.test(entry.name)) continue;
        plugins.push({name:displayName(entry.name), format:formatFor(entry.name), path:path.join(directory, entry.name)});
      }
    } catch { /* Optional plugin directories may not exist. */ }
  }
  return plugins.sort((left, right) => left.name.localeCompare(right.name));
}
async function probeAudioUnits() {
  try {
    const {stdout} = await execFileAsync('/tmp/freetribe-audio-unit-probe', [], {timeout:10000});
    return {available:true, output:stdout};
  } catch (error) {
    return {available:false, output:error.stdout || '', error:'Build the probe with npm run probe:audio-units'};
  }
}

const server = http.createServer(async (request, response) => {
  if (request.url === '/api/plugins') {
    response.writeHead(200, {'content-type':'application/json', 'cache-control':'no-store', 'access-control-allow-origin':'*'});
    response.end(JSON.stringify({platform:process.platform, plugins:await discoverPlugins()}));
    return;
  }
  if (request.url === '/api/audio-unit-probe') {
    response.writeHead(200, {'content-type':'application/json', 'cache-control':'no-store', 'access-control-allow-origin':'*'});
    response.end(JSON.stringify(await probeAudioUnits()));
    return;
  }
  const requested = request.url === '/' ? 'index.html' : request.url.slice(1);
  const file = path.resolve(root, requested);
  if (!file.startsWith(`${root}${path.sep}`)) { response.writeHead(403); response.end('Forbidden'); return; }
  try {
    const contents = await fs.readFile(file);
    response.writeHead(200, {'content-type':requested.endsWith('.html') ? 'text/html' : 'text/plain'});
    response.end(contents);
  } catch { response.writeHead(404); response.end('Not found'); }
});

server.listen(port, () => console.log(`Freetribe Studio with plugin discovery: http://localhost:${port}`));