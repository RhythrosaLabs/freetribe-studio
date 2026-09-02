# Electribe 2 Sampler Research

This note records the implementation baseline for Freetribe Studio. It keeps
the controller aligned with the ES2 workflow while avoiding unverified SysEx
writes.

## Verified workflow

- The ES2 is a 16-part pattern instrument. The tracker/editor model exposes
  the complete 64-step pattern, per-step on/off, velocity, gate, notes, and
  part-level sound parameters.
- The front-panel workflow is organized around Pattern, Part, Motion Sequence,
  Step Edit, Sound, Insert FX, Master FX, Global, and Data Utility operations.
- Sample projects use the `KORG/electribe sampler/Sample/` directory. An
  exported all-sample library is named `e2sSample.all`.
- Sample properties include name, category, one-shot/loop state, play level,
  tune/frequency, mono/stereo state, start/end/loop points, and slice points.
  Sliced samples can contain up to 64 slices and have per-step slice choices.
- The physical unit can be used over USB MIDI. A browser controller needs
  Web MIDI permission and should select an explicit output before sending
  messages. Elfer documents reading a pattern from the E2, editing it, and
  sending it back without saving to the device pattern slot.

## Studio coverage

The controller surface implements the verifiable host-side workflow: 16 part
selection, 64-step editing, page-specific controls for sound and FX/global/data
concepts, sample audio preview, MIDI file import, patch persistence, virtual
keyboard input, Web MIDI output selection, and guarded CC output logging.
`e2sSample.all` files are accepted as library artifacts and reported with their
size; full binary library editing remains a separate format implementation.

## Sources

- [Korg Electribe product and support materials](https://www.korg.com/)
- [Oe2sSLE](https://github.com/JonathanTaquet/Oe2sSLE), an open editor for
  `e2sSample.all`, including sample import, replacement, export, loops, slices,
  and memory-limit behavior.
- [e2sedit](https://github.com/daveschroeter/e2sedit), an editor documenting
  sample slots, categories, loop flags, tuning, loudness, and SD-card
  placement.
- [Elfer](https://github.com/maks/elfer), an open tracker describing 64-step
  pattern editing, USB MIDI connection, reading and sending patterns, and
  panel-control navigation.

## Not asserted as complete

The exact Korg SysEx schema, firmware update protocol, sample-library binary
encoder, and every hardware menu side effect require device capture/testing or
verified protocol documentation. The host UI must not claim those are complete
until messages are captured against an ES2 and round-tripped safely. USB, SD,
DMA, EMIFA/HostDMA, memory protection, RTOS scheduling, and embedded scripting
remain firmware features that require the target hardware and toolchain.
