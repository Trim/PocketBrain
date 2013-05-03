import QtQuick 1.0

Rectangle {
    id: page //unique id of the main object
    width: 640 //width of the window
    height: 640 //height of the window
    color: "black" //background color of the window
    property string emotion: "wait..." ;
    property int packetCounter: 0 ;//used for visualizing activity

    /**
      Function receiving time ticks from C++ code.
      */
    function timeTick()
    {
        page.packetCounter = (page.packetCounter + 1)%16;
    }

    function updateEmotion(emotion_)
    {
        page.emotion = emotion_;
    }

    signal initCalm;
    signal initSad;
    signal initFear;
    signal initJoy;

    Rectangle
    {
        id: textRectangle;
        anchors.centerIn: parent;
        width:640;
        height:150;
        color:"black";
        Text{
            anchors.left: textRectangle.left;
            font.pixelSize: 60;
            color: "white";
            text: "I think you are : \n\t"+emotion;
        }
    }

    Rectangle
    {
        id: initDiv;
        anchors.bottom: parent.bottom;
        width:640;
        height:50;
        color:"black";

        Rectangle{
            id:initCalmDiv;
            anchors.left: initDiv.left;
            width:160;
            height:50;
            border.color:"white";
            color:"black";
            Text{
                anchors.centerIn: initCalmDiv;
                font.pixelSize: 20;
                color: "white";
                text: "InitCalm";
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    page.initCalm();
                }
            }
        }

        Rectangle{
            id:initSadDiv;
            anchors.left:initCalmDiv.right;
            width:160;
            height:50;
            border.color: "white";
            color:"black";
            Text{
                anchors.centerIn: initSadDiv;
                font.pixelSize: 20;
                color: "white";
                text: "InitSad";
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    page.initSad();
                }
            }
        }

        Rectangle{
            id:initFearDiv;
            width:160;
            height:50;
            border.color:"white";
            color:"black";
            anchors.left: initSadDiv.right

            Text{
                anchors.centerIn: initFearDiv;
                font.pixelSize: 20;
                color: "white";
                text: "InitFear";
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    page.initFear();
                }
            }
        }

        Rectangle{
            id:initJoyDiv;
            width:160;
            height:50;
            border.color:"white";
            color:"black";
            anchors.left: initFearDiv.right
            Text{
                anchors.centerIn: initJoyDiv;
                font.pixelSize: 20;
                color: "white";
                text: "InitJoy";
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    page.initJoy();
                }
            }
        }
    }

    Rectangle
    {
        //quit button
        color: "red"
        anchors.right: parent.right
        anchors.top: parent.top
        width: 50
        height: 50
        Text
        {
            anchors.centerIn: parent
            font.pixelSize: 40
            color: "white";
            text: "X"
        }
        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                Qt.quit();
            }
        }
    }
}
