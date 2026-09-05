// SPDX-License-Identifier: GPL-3.0-only
// MuseScore-Studio-CLA-applies
pragma ComponentBehavior: Bound
import QtQuick
import Muse.Ui
import Muse.UiComponents
import MuseScore.Inspector
import "common"
InspectorSectionView {
    id: root
    required property JimsScoreSettingsModel model
    implicitHeight: controls.implicitHeight
    JimsTuningModel { id: tuning; Component.onCompleted: init() }
    Column {
        id: controls
        width: parent.width
        spacing: 10
        StyledTextLabel { text: qsTrc("inspector", "Tuning · M5 (major fifth), in cents") }
        TextInputField {
        id: centsInput
        property bool escaping: false
        onEscaped: { escaping = true; tuning.cancel(); inputField.text = tuning.cents.toLocaleString(Qt.locale(), "f", 3); Qt.callLater(function() { centsInput.escaping = false }) }
            width: parent.width
            currentText: tuning.cents.toLocaleString(Qt.locale(), "f", 3)
            navigation.panel: root.navigationPanel
            navigation.row: root.navigationRow(1)
            navigation.name: "Tuning"
            navigation.accessible.name: qsTrc("inspector", "Tuning in cents")
            onTextEditingFinished: function(newText) { if (!escaping) tuning.acceptText(newText) }
        }
        StyledTextLabel {
            width: parent.width
            visible: tuning.error.length > 0
            text: "⚠ " + tuning.error
            wrapMode: Text.WordWrap
            Accessible.role: Accessible.AlertMessage
            Accessible.name: text
        }
        StyledTextLabel { text: qsTrc("inspector", "Melody part") }
        StyledDropdown {
            width: parent.width
            model: [ { text: qsTrc("inspector", "Soprano"), value: 0 }, { text: qsTrc("inspector", "Alto"), value: 1 }, { text: qsTrc("inspector", "Tenor"), value: 2 }, { text: qsTrc("inspector", "Bass"), value: 3 } ]
            currentIndex: root.model.settings.melody ?? 0
            navigation.panel: root.navigationPanel
            navigation.row: root.navigationRow(2)
            navigation.accessible.name: qsTrc("inspector", "Melody part")
            onActivated: function(index, value) { root.model.setOption("melody", value) }
        }
        CheckBox {
            width: parent.width
            text: qsTrc("inspector", "Hide empty octave bands")
            checked: root.model.settings.elide ?? false
            navigation.panel: root.navigationPanel
            navigation.row: root.navigationRow(3)
            onClicked: root.model.setOption("elide", !checked)
        }
        CheckBox {
            width: parent.width
            text: qsTrc("inspector", "Show all octaves in the first system")
            checked: root.model.settings.firstSystem ?? true
            navigation.panel: root.navigationPanel
            navigation.row: root.navigationRow(4)
            onClicked: root.model.setOption("firstSystem", !checked)
        }
        FlatButton {
            id: glossaryButton
            width: parent.width
            property bool expanded: false
            text: qsTrc("inspector", "Glossary")
            navigation.panel: root.navigationPanel
            navigation.row: root.navigationRow(5)
            onClicked: expanded = !expanded
        }
        StyledTextLabel {
            width: parent.width
            visible: glossaryButton.expanded
            text: qsTrc("inspector", "M5: major fifth, the tuning generator. Cents: hundredths of a semitone. Re0: the reference Re pitch. Hollow stacks: empty octave bands hidden to save space. Prime-limit guide lines: reference pitches distinguished by labels and dash patterns. Scale dots: markers for notes in the chosen scale.")
            horizontalAlignment: Text.AlignLeft
            wrapMode: Text.WordWrap
        }
    }
}
