# MeloPresto Score local macOS identity

MeloPresto Score is a local development application published by MeloPresto LLC, melopresto.com. Its bundle identifier is `com.melopresto.score`. Product metadata lives in `buildscripts/melopresto_identity.cmake`; the local product version is `0.0.0`. The inherited MuseScore engine and score-format versions remain unchanged. About identifies the product and the engine separately and retains upstream attribution. No Apple account, Developer ID, notarization, public release or distribution signing is part of this workflow.

The application uses its own Qt organization/application settings, application data, recent-file state, credentials directory and multi-process channel. Automatic and manual upstream application update checks are disabled. External MuseScore URL callbacks are not registered. Internal MuseScore namespaces, executable/build target names, MusicXML vocabulary, archive XML and cloud API contracts retain their existing identities.

## Native documents

`.meloscore` uses the existing native ZIP archive reader/writer, including score XML, styles, images, thumbnails, excerpts and audio/view settings. `.mscz` and `.mscx` remain supported. `.melosong` remains a different, structured timed-note format. This application does not claim to edit the other family formats merely because their names exist.

New local scores default to `.meloscore`; explicit existing score paths retain their suffix. Open/save/copy, command-line conversion, startup-score selection and template discovery recognize the new suffix. Backups retain `.meloscore~` in `.melopresto-backup`; recovery uses `.meloscore.autosave` and restores the original document path. Autosave files are internal recovery artifacts, not a separately advertised public document type.

The macOS bundle exports `com.melopresto.score.document` and `com.melopresto.score.backup`, with the document icon and the Owner handler rank. Inherited MuseScore declarations are imported and their handlers are Alternate. The preview helper source uses its own child bundle identifier and supports the native Score document type. It is excluded from the default local package: the current macOS extension host aborts when starting the ad-hoc signed helper. Use `--include-preview` only for a later explicitly validated experimental build; building or registering the helper alone does not prove the operating system can host it. The document icon and normal Finder opening remain available without a preview helper.

## Canonical artwork

`share/icons/melopresto/sources.json` records the canonical crescent and button source paths, hashes, palette and meanings. The crescent is the full Do-to-Do glyph without notation overlays. The button is the canonical button-field instrument geometry. These paths are copied byte-for-byte into checked source inputs; the generator applies uniform placement/scaling and rosewood, brass and bone colors. It does not implement musical geometry. The Jammer icon is artwork prepared for that application's later integration.

Run `python3 tools/melo/generate_app_icons.py --check` to verify the SVG masters. On macOS, regenerate raster and ICNS resources with `python3 tools/melo/generate_app_icons.py --render --node <node> --sharp-module <sharp-module>`. The renderer requires Node, sharp and iconutil; Python itself uses the standard library. Do not hand-edit the generated masters.

## Local build and packaging

Reuse the configured native build and its dependency cache. Configure `CMAKE_INSTALL_PREFIX` to a staging directory inside the source checkout, such as `package-local`, and set `MUSESCORE_REVISION` to the source commit being built. Build `MuseScoreStudio`, `MuseScoreQuickLookPreviewExtension`, `engraving_tests`, `project_test` and `muse_update_test`. The internal executable remains `mscore`; the installed outer bundle is `MeloPresto Score.app`.

Run `python3 tools/melo/package_local_macos.py --build build.release --output <new-path>/MeloPresto\ Score.app --macdeployqt <Qt>/bin/macdeployqt`. The script builds translation resources, installs into the configured staging directory, copies to a new destination, deploys runtime libraries/QML, preserves upstream QML plugin isolation, signs nested code with local ad-hoc signatures, then validates the bundle. Existing output destinations are refused. The helper needs linker header padding so deployment can relocate its libraries. No security settings or Apple credentials are changed.

Register the verified app with Launch Services after installing it in the user's Applications directory. Do not change stock MuseScore file defaults or modify its installation. If replacing an earlier local Score development build, verify its bundle identifier and preserve that build until the replacement passes launch checks.

## Verification

`python3 tools/melo/check_score_identity.py --app <app>` validates the built metadata and resources. `python3 tools/melo/check_meloscore_roundtrip.py --executable <executable> --fixture <score.mscx> --output <new-evidence-directory>` exercises real save/reopen, archive payload parity against equivalent `.mscz` operations, notation SVG parity, MIDI parity and malformed-input refusal. Add `--platform cocoa` for a packaged macOS app; the packaged app includes Cocoa, while the development Qt installation also offers offscreen rendering. The SVG comparison ignores only the exported filename title. Existing first-save style normalization is compared against the same MuseScore operation on the control file.

Native regression suites cover archive members, Unicode/multi-dot filenames, original recovery paths, backups and disabled manual/automatic upstream update checks. Run the broader engraving, project and update suites once after the focused checks pass. Preserve preexisting test-output files when running suites in a retained working directory. The hosted codestyle workflow additionally verifies canonical icon sources and centralized names; hosted native builds remain a deliberately dispatched workflow. Final owner acceptance covers icon appearance, launching both apps, the macOS Documents-folder privacy choice, normal Save/Save As, reopening via Finder, recovery and playback. The local startup scan uses the inherited user-selected Documents asset folders; a newly identified app receives its own macOS privacy prompt.

Windows/Linux packaging, Intel validation, additional family-file handlers, an update service, Spotlight indexing, settings import and distribution signing remain later work. Existing technical identifiers retained for upstream interoperability are intentional.

## Platform references

Apple distinguishes owned exported types from supported imported types in [Defining file and data types for your app](https://developer.apple.com/documentation/uniformtypeidentifiers/defining-file-and-data-types-for-your-app). Handler priority follows [LSHandlerRank](https://developer.apple.com/documentation/bundleresources/information-property-list/cfbundledocumenttypes/lshandlerrank). The UI reads product metadata through Qt's [Application properties](https://doc.qt.io/qt-6/qml-qtquick-application.html).
