# Generated chord evidence

MeloPresto Harmony objects retain the immutable `melo.chord-evidence.v1` derivation with a generated or manual origin. Native persistence, MusicXML, and Music Encoding Initiative (MEI) import/export preserve the record. The public record schema and musical validation belong to the Kernel.

`Harmony::meloEvidenceError` gathers the actual supporting notes across the recorded interval and obtains their sounding frequencies through the Kernel. The Kernel checks membership, register, bass, duration coverage, and annotation position. A stale generated name uses the existing invalid-name display and accessibility feedback and prevents verified interchange export. Undo restores validity when the score evidence is restored. A manual edit retains generation history but is identified as manual.

The MusicXML tests exercise native/interchange persistence, supporting-note edits, undo/redo, manual history, and refused exports. The MEI tests exercise evidence round trips and stale-export refusal, including a name anchored to a note inside a chord. Private corpus checks supply an optional score path to the dedicated local test; its absence is an explicit skip, not private-corpus coverage in hosted builds.
