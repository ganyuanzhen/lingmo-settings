import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import LingmoUI 1.0 as LUI
import Lingmo.Settings 1.0
import "../"

RowLayout {
    id: updatetitlebar
    spacing: LUI.Units.largeSpacing

    signal sendCheckUpdate

    Item {
        width: 80
        height: 80

        Image {
            visible: (!control.isupdating) || (!control.ischeckingupdate)
            source: LUI.Theme.darkMode ? "qrc:/images/dark/changes-white" : "qrc:/images/light/changes"
            width: parent.width
            height: parent.height
            fillMode: Image.PreserveAspectCrop
            antialiasing: true
            smooth: true
            anchors.centerIn: parent
        }

        LUI.BusyIndicator {
            id: busyIndicator
            width: parent.width
            height: parent.height
            visible: (control.isupdating) || (control.ischeckingupdate)
            running: (control.isupdating) || (control.ischeckingupdate)
        }
    }

    Item {
        Layout.fillWidth: true
        Layout.fillHeight: true

        // Center promote text
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Label {
                id: updateText
                text: {
                    if ((!control.isupdating) && (!control.hasupdate_)) {
                        return qsTr("You're up to date");
                    } else if ((!control.isupdating) && (control.hasupdate_)) {
                        return qsTr("Updates available");
                    } else if ((control.isupdating)) {
                        return qsTr("Installing updates");
                    } else if ((control.ischeckingupdate)) {
                        return qsTr("Checking updates");
                    } else {
                        return qsTr("Error occurs in updating");
                    }
                }
                Layout.fillHeight: true
                width: parent.width
                font.pointSize: 20
                font.bold: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }

            Label {
                id: updateCheckTimeText
                text: "Last checked: Today, 18:37"
                Layout.fillHeight: true
                width: parent.width
                font.pointSize: 10
                font.bold: false
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }
        }
    }


    Button {
        id: checkUpdateButton
        width: 40
        text: qsTr("Check for updates")
        visible: true
        enabled: true
        onClicked: {
            control.ischeckingupdate = true;
            control.mUpdateManager.onUpdateDataReply.connect(control.handle_update_data)
            updatetitlebar.onSendCheckUpdate.connect(control.mUpdateManager.startCheckforUpdate);
            updatetitlebar.sendCheckUpdate();
        }
    }

    onSendCheckUpdate: {

    }
}

