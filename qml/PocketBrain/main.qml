import QtQuick 1.0

Rectangle {
    id: page //unique id of the main object
    width: 640 //width of the window
    height: 640 //height of the window
    color: "black" //background color of the window
    property int value: 0 //value of one EEG channel
    property int filteredValue:0 // filtered value
    property int powerValue: 0 //power value of one EEG channel
    property int packetCounter: 0 //used for visualizing activity

    /**
      Function receiving time ticks from C++ code.
      */
    function timeTick()
    {
        page.packetCounter = (page.packetCounter + 1)%16;
    }

    function channelValues(value_, filtered_)
    {
        page.value = value_;
        page.filteredValue = filtered_;
    }

    Rectangle
    {
        id: textRectangle;
        anchors.centerIn: parent;
        width:300;
        height:80;
        color:"black";
        Text{
            anchors.left: textRectangle.left;
            font.pixelSize: 60;
            color: "white";
            text: page.value;
        }
        Text{
            anchors.right: textRectangle.right;
            font.pixelSize: 60;
            color: "red";
            text: page.filteredValue;
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

    Rectangle
    {
        //activity visualization
        color: "blue"
        height: 33
        width: height
        opacity: page.packetCounter/16.0
    }

}
