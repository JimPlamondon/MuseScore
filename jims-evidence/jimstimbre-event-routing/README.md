# JiMSTimbre exact-note event routing

Date: 2026-08-20

## Why this exists

For a JiMStaff, MuseScore has already asked the JiMS Kernel for the exact sounding pitch before its built-in FluidSynth (“MuseScore Basic”) renders the note. JiMSTimbre must leave that fundamental alone and move only partials 2–N to the active Kernel partial map.

Mixed audio alone does not identify those fundamentals reliably. The ambiguity is especially visible in chords, where a partial of one voice can coincide with the fundamental or a partial of another. Estimating the notes again inside the effect throws away information that MuseScore already has.

## VST3 contract

JiMSTimbre remains an audio effect. VST3 (Virtual Studio Technology 3) lets it declare one default-active main event input bus with `AudioEffect::addEventInput`. MuseScore supplies the audio buffers and the optional note events together in `ProcessData`; each event has a block-relative sample offset. This is the standard VST3 mechanism for an effect that needs note-derived control metadata and does not make JiMSTimbre an instrument.

The relevant Steinberg interfaces are:

- `AudioEffect::addEventInput`: <https://steinbergmedia.github.io/vst3_doc/vstsdk/classSteinberg_1_1Vst_1_1AudioEffect.html>
- `ProcessData::inputEvents`: <https://steinbergmedia.github.io/vst3_doc/vstinterfaces/structSteinberg_1_1Vst_1_1ProcessData.html>
- VST3 note events: <https://steinbergmedia.github.io/vst3_doc/vstinterfaces/structSteinberg_1_1Vst_1_1Event.html>

VST3 does not require a generic host to copy instrument events to an insert effect. MuseScore does so deliberately because it owns both ends of this graph and its sequencer contains the authoritative JiMS-corrected note pitch.

## Data flow

For each Basic note, `FluidSequencer` creates a note-on and note-off metadata event alongside the existing MIDI (Musical Instrument Digital Interface) event. Both carry the same nominal pitch and tuning correction; the pair also receives a stable note identifier. `FluidSynth` collects the metadata at the exact block-relative offset at which it applies the corresponding MIDI event.

After Basic has rendered the audio block, `EventAudioSource` forwards the concrete synthesizer's note-event batch through the track-input wrapper. `MixerChannel` gives that batch to each active insert effect immediately before processing the block. `VstFxProcessor` translates it to standard VST3 Note On and Note Off events, preserving pitch, tuning in cents, velocity, note ID, and sample offset. `VstAudioClient` places the events in `ProcessData::inputEvents` and clears them after the process call so they cannot be replayed in the next block.

JiMSTimbre converts `pitch + tuning` into an exact frequency. Its Digital Signal Processing (DSP) uses the active and recent note fundamentals to group and relocate partials according to the current Kernel-supplied partial map. Note-off fundamentals are retained briefly for release-tail processing. If a host supplies no events, the existing audio-analysis path remains available as a compatibility fallback.

## Real-time constraints

The audio thread uses bounded, preallocated event storage. The insert receives no score model, temperament implementation, or JiMS-specific event type through VST3. Temperament state and the partial map continue to arrive through the plug-in’s persisted parameter/state contract; note events identify only which already-correct fundamentals are sounding.

## Verification target

The integration is accepted by building and installing JiMSTimbre, validating the VST3 bundle, building this fork under `~/Developer/JiMS/GitHub/`, and rendering deterministic scale and chord fixtures through MuseScore Basic with a zero-alignment effect control and with full JiMSTimbre alignment. The Fast Fourier Transform (FFT) oracle must confirm unchanged fundamentals and must measure the residual energy difference outside the intended partial relocation (the Timbre_Gap).

The verified development app is `/Users/jim/Developer/JiMS/GitHub/musescore-jims-timbre-events/build.install610/mscore.app`. Across 24 generator widths from 686 through 720 cents, the scale fundamental delta was 0.00284 cents at the 95th percentile and unexplained spectral-energy change was 0.561% at the 95th percentile. In the 24-width major-triad fixture, the corresponding results were 0.0178 cents and 0.810%. The aligned and control chord renders had the same −31.18 dBFS (decibels relative to full scale) 95th-percentile steady-state maximum sample step, providing no automated evidence that the effect added steady-state popping or crackling.
