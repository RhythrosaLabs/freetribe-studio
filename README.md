# Freetribe Studio

Freetribe Studio is a community fork of [Freetribe](https://github.com/bangcorrupt/freetribe),
with a browser-based companion for exploring the Electribe 2 performance
workflow alongside the original open-source firmware project. It aims to make
hardware experiments, patch design, MIDI routing, and audio module development
easier to explore.

This repository keeps the upstream firmware and its history while adding the
Freetribe Studio browser UI, native macOS plugin discovery tools, and related
research. Studio-specific changes are maintained here; firmware changes should
be contributed upstream when they are generally useful.

See
[Getting Started](https://bangcorrupt.github.io/freetribe-docs/getting-started/)
to jump right into the Freetribe API, or
[Architecture](https://bangcorrupt.github.io/freetribe-docs/architecture/) for a
deeper look at the system.

## Features

From a user application perspective, Freetribe is currently light on features.
Most of the hardware initialisation is complete, with driver stacks for much of
the system. Built on this is a set of services providing a high-level interface
to the device. Some basic examples are provided, showing how to integrate user
application code with the Freetribe kernel.

### Existing Features

- Serial MIDI input and output via TRS port.
- Set or clear a pixel anywhere in the vast 128x64 dot-matrix.
- Control backlight RGB (binary).
- Register callbacks for all of the panel controls.
- Set and toggle LEDs, with brightness control for those with support.
- Send commands to the Blackfin DSP and receive feedback.
- Process audio based on control input.
- Audio module API similar to many plugin formats.

### Planned Features

Some of these are in progress, most should be possible.

- High speed DSP control via EMIFA/HostDMA.
- DMA support for peripheral drivers.
- USB driver.
- SD card driver.
- DSP block processing.
- Memory protection.
- Dynamic loading of apps and modules.
- Preemptive scheduling using FreeRTOS.
- Embedded Lua and MicroPython.
- Support for sync ports.

## Freetribe Studio

The repository includes a dependency-free browser companion UI for exploring the
Freetribe performance workflow. It provides a live module cockpit, selectable
modules, step sequencer, MIDI and DSP telemetry, patch save/load with JSON
import/export, a virtual keyboard, optional Web MIDI access, roadmap view,
sample import with waveform preview, MIDI file import into the step pattern,
multi-sample library management, bulk upload/delete, sample metadata, sample
audition, beat-pad programming, roadmap view, device sync simulation, and a
browser-native screen recording control for making tutorial clips. It also
includes an advanced transport, non-destructive sample editor with trim/loop/
pitch/gain/filter preview, and a plugin bridge surface for native VST3 or Audio
Unit hosting.

To launch it locally:

```sh
python3 -m http.server 8080 --directory ui
```

Then open <http://localhost:8080>. The recording control uses the browser's
screen-sharing permission and downloads a WebM tutorial clip when stopped.

For macOS VST, VST3, and Audio Unit discovery, use the native companion server
instead:

```sh
npm run studio
```

It scans the standard user and system plugin folders and exposes the results to
the Studio scan control. To verify that installed Audio Units can be instantiated
by macOS, run `npm run probe:audio-units`. Native VST/VST3 processing still
requires a host SDK bridge; the browser fallback uses Web Audio.

The ES2 controller research and verified feature boundaries are documented in
[ELECTRIBE_2_SAMPLER_RESEARCH.md](ELECTRIBE_2_SAMPLER_RESEARCH.md).

The Studio downloads an actual imported `e2sSample.all` file byte-for-byte.
Uploaded WAV files are kept as browser samples and are not mislabeled as a
Korg library; binary `.all` creation requires the format encoder.

## Credit and Lineage

Freetribe Studio is based on and forked from
[bangcorrupt/freetribe](https://github.com/bangcorrupt/freetribe). Please see the
upstream project for its original authorship, hardware work, acknowledgements,
and community discussions. This fork retains the upstream AGPL-3.0 license and
continues to credit the open-source projects listed below.

## Support for You

If you need help with this project, please visit the
[Freetribe discussion forum](https://github.com/bangcorrupt/freetribe/discussions).

## Support for Me

Freetribe is free (as in GPL) and always will be. If you would like to support
my work you are most welcome to
[become a sponsor](https://github.com/sponsors/bangcorrupt). Freetribe exists
because people sponsored Hacktribe. Your support helps keep me motivated,
fuelled and focussed.

## Acknowledgements

Freetribe would be almost impossible without other open-source projects. The CPU
drivers are based on [StarterWare](https://www.ti.com/tool/STARTERWARE-SITARA)
by Texas Instruments. The hardware abstraction, build environment and code
examples provided the stepping-stone needed to get started.

In much the same way, the DSP drivers are based on
[monome/aleph](https://github.com/monome/aleph). This showed how to initialise
the Blackfin processor and configure peripherals. They also provide a public
domain DSP library, with many of the difficult maths problems packaged into
convenient unit generators.

MIDI input parsing is based on
[mikromodular/libmidi](https://github.com/mikromodular/libmidi), with
sysex/binary conversion borrowed from the
[Arduino MIDI library](https://github.com/FortySevenEffects/arduino_midi_library/blob/master/src/MIDI.cpp)
by Francois Best.

[UGUI](https://github.com/deividAlfa/UGUI) and
[micromenu](https://github.com/abcminiuser/micromenu-v2) provide a graphical
interface for user applications.

Special thanks to countless stackoverflow users.

## License

AGPL-3.0.
