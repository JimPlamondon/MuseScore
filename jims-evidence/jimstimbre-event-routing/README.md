# JiMSTimbre exact-note event routing

Date: 2026-08-20

## Why this exists

For a JiMStaff, MuseScore has already asked the JiMS Kernel for the exact sounding pitch before a synthesizer renders the note. JiMSTimbre must leave that fundamental alone and move only partials 2–N to the active Kernel partial map.

Mixed audio alone does not identify those fundamentals reliably. The ambiguity is especially visible in chords, where a partial of one voice can coincide with the fundamental or a partial of another. Estimating the notes again inside the effect throws away information that MuseScore already has.

## VST3 contract

JiMSTimbre remains an audio effect. VST3 (Virtual Studio Technology 3) lets it declare one default-active main event input bus with `AudioEffect::addEventInput`. MuseScore supplies the audio buffers and the optional note events together in `ProcessData`; each event has a block-relative sample offset. This is the standard VST3 mechanism for an effect that needs note-derived control metadata and does not make JiMSTimbre an instrument.

The relevant Steinberg interfaces are:

- `AudioEffect::addEventInput`: <https://steinbergmedia.github.io/vst3_doc/vstsdk/classSteinberg_1_1Vst_1_1AudioEffect.html>
- `ProcessData::inputEvents`: <https://steinbergmedia.github.io/vst3_doc/vstinterfaces/structSteinberg_1_1Vst_1_1ProcessData.html>
- VST3 note events: <https://steinbergmedia.github.io/vst3_doc/vstinterfaces/structSteinberg_1_1Vst_1_1Event.html>

VST3 does not require a generic host to copy instrument events to an insert effect. MuseScore does so deliberately because it owns both ends of this graph and its sequencer contains the authoritative JiMS-corrected note pitch. The instrument does not need to know that the copy exists.

## Data flow

MuseScore Basic uses the existing `AudioNoteEvent` sidecar populated by its FluidSynth sequencer. A VST3 instrument uses a shared `VstNoteEventBridge`: immediately after MuseScore successfully queues a standard VST3 Note On or Note Off event to the instrument, the bridge captures a lossless copy for the containing audio block. Both source paths therefore present the same source-independent note-event batch to `MixerChannel`.

After the instrument has rendered the audio block, `MixerChannel` gives that batch to each active insert effect immediately before processing the block. `VstFxProcessor` uses the same bridge mapping to produce standard VST3 Note On and Note Off events for JiMSTimbre, preserving pitch, tuning in cents, velocity, note ID, and sample offset. The distinct note ID preserves chords and overlapping unisons. Per-note expression events remain instrument-only because JiMSTimbre needs the exact sounding fundamental, not the instrument’s private performance controls.

`VstAudioClient` places the copied events in `ProcessData::inputEvents` and clears them after the process call so they cannot be replayed in the next block. JiMSTimbre converts `pitch + tuning` into an exact frequency. Its Digital Signal Processing (DSP) uses the active and recent note fundamentals to group and relocate partials according to the current Kernel-supplied partial map. Note-off fundamentals are retained briefly for release-tail processing. If a host supplies no events, the existing audio-analysis path remains available as a compatibility fallback.

## Effect readiness

Offline export previously waited for an asynchronously loaded VST3 instrument but not for an asynchronously loaded VST3 insert effect. When both JS80P and JiMSTimbre were present, rendering could start after JS80P became ready but before JiMSTimbre existed; the resulting on/off files were bit-identical because no effect instance processed them. Track-output readiness now includes every active insert. `SequencePlayer` waits until both the input instrument and output effect chain report ready, while failed plug-in loads settle explicitly rather than hanging the export.

## Real-time constraints

The audio thread uses bounded, preallocated event storage. The insert receives no score model, temperament implementation, or JiMS-specific event type through VST3. Temperament state and the partial map continue to arrive through the plug-in’s persisted parameter/state contract; note events identify only which already-correct fundamentals are sounding.

## Verification

The verified development app is `/Users/jim/Developer/JiMS/GitHub/musescore-jims-timbre-events/build.install610/mscore.app`. JiMSTimbre, JS80P 4.1.1, and Surge XT 1.3.4 were installed under `/Users/jim/Library/Audio/Plug-Ins/VST3/`. The third-party instruments were used as installed; neither bundle nor source tree was modified. Both installed bundles passed strict macOS code-signature verification after the tests. Surge XT’s executable SHA-256 remained `9f4feb5b990823e29a11955964e7807978929782dc56e140a6ee56a2d9aeb552`; JS80P’s executable SHA-256 was `81733f40ae3ee0b7a84f97365e609327c93b9319d4e5dab5c1fe991733e57ebf`.

The host route was accepted with MuseScore Basic: exact note events removed the original audible artifacts, and Jim passed the recorded by-ear comparison on 2026-08-20. An unmodified JS80P run over 24 syntonic generator widths also verified that the instrument and insert received the same lifecycle events and that JiMSTimbre could remain numerically transparent for the selected sine-dominant program. That run does not establish transparent retuning of arbitrary third-party programs, because later absolute-pitch and harmonic-rich tests showed that the tested third-party instruments did not consistently honor the supplied per-note tuning and that JiMSTimbre's broader spectral reconstruction target remained unmet.

Surge XT was also exercised without modification. It verified event delivery and partial-target computation, but deterministic harmonic-rich programs did not pass the strict Timbre Gap oracle. Surge XT therefore remains interoperability and falsification evidence rather than an accepted third-party transparency result.

The full JS80P evidence, including scores, informatively named `JiMSTimbre-off` and `JiMSTimbre-on` WAV files, manifests, logs, per-window measurements, and summary, is in `/Users/jim/Developer/JiMS/Temp/tfs_timbre_gap_evidence/third_party_js80p_chords_24`. The focused event trace showing independent instrument and effect clients receiving the same 48 chord lifecycle events is in `/Users/jim/Developer/JiMS/Temp/tfs_timbre_gap_evidence/third_party_js80p_chords_smoke/vst_event_trace_after_readiness.txt`.
