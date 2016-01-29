import QtQuick 2.2
import QtQuick.Controls 1.1

ApplicationWindow {
    id: window;
    visible: true
    width: 960
    height: 640
    title: qsTr("Receptor de Sinalização de Transito")

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    Item {
        objectName: "receivedSignsView"
        anchors.centerIn: parent;

        property alias image:signImage;
        property alias text:signMessage;

        function displaySign(signType, signText) {
            if (signType !== "")
                image.source = "qrc:///Resources/" + signType + ".png";
            else
                image.source = "";
            text.text = signText;
            // console.log("Got message:", msg)
        }

        Image {
            id: signImage
            objectName: "signImage"
            width: window.width - 100; height: window.height - 100;
            //width: 240; height: 240;
            fillMode: Image.PreserveAspectFit
            smooth: true
            //anchors.centerIn: parent
            anchors.horizontalCenter: parent.horizontalCenter;
            y: -(window.height/2);
            source: "Resources/UNK.png"

            //function updateImage() {
            //    source = "qrc:///R000.png";
            //    console.log('got some Items');
            //}
        }
        Text {
            id: signMessage
            objectName: "signMessage"
            text: qsTr("Hello World")
            font.pointSize: 40.0;
            anchors.horizontalCenter: parent.horizontalCenter;
            y: window.height/2 - font.pointSize*2;
            //y: 230;
            //top: parent.Center + 1000;
        }
    }
}
