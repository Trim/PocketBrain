import QtQuick 1.0

Rectangle {
    id: page //unique id of the main object
    width: 640 //width of the window
    height: 640 //height of the window
    color: "black" //background color of the window
    property string emotion: "... please train me" ;
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
        infoText.text = "You are "+emotion_;
    }

    signal toggleSaveCalm(bool saving);
    signal toggleSaveSad(bool saving);
    signal toggleSaveFear(bool saving);
    signal toggleSaveJoy(bool saving);
    signal guessEmotion();
    signal storeClassifiers();
    signal recordData(bool record);

    Rectangle
    {
        id: guessRectangle;
        anchors.centerIn: parent;
        width:parent.width/2;
        height:150;
        color:"black";
        border.color: "white";
        Text{
            font.pixelSize: 40;
            color: "white";
            text: "Guess my emotion";
            width:parent.width;
            height:parent.height;
            wrapMode: Text.Wrap;
            horizontalAlignment: Text.AlignHCenter;
            verticalAlignment: Text.AlignVCenter;
        }
        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                if(!recordButton.record){
                    infoText.text = "Record some data before I can guess";
                }else{
                    guessEmotion();
                    infoText.text = "";
                }
            }
        }
    }

    Rectangle
    {
        id: initDiv;
        anchors.bottom: parent.bottom;
        width:parent.width;
        height:80;
        color:"black";

        Rectangle{
            id:initCalmDiv;
            anchors.left: initDiv.left;
            width:parent.width/4;
            height:parent.height;
            border.color:"white";
            color:"black";
            property bool saving: false;
            Text{
                id:initCalmText;
                anchors.top:parent.top;
                font.pixelSize: 20;
                width:parent.width;
                height:parent.height-font.pixelSize;
                color: "white";
                text: "Start train";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            Text{
                anchors.top: initCalmText.baseline;
                width:parent.width;
                font.pixelSize: initCalmText.font.pixelSize;
                color: "white";
                text: "Calm";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    if(!initSadDiv.saving && !initFearDiv.saving && !initJoyDiv.saving){
                        infoText.text="";
                        parent.saving=!parent.saving;
                        page.toggleSaveCalm(parent.saving);

                        if(parent.saving){
                            initCalmText.text="Stop train"; // it's the next action
                        }else{
                            initCalmText.text="Start train";
                        }
                    }else{
                        infoText.text="Please stop other training before starting this one";
                    }
                }
            }
        }


        Rectangle{
            id:initSadDiv;
            anchors.left:initCalmDiv.right;
            width:parent.width/4;
            height:parent.height;
            border.color:"white";
            color:"black";
            property bool saving: false;
            Text{
                id:initSadText;
                anchors.top:parent.top;
                font.pixelSize: 20;
                width:parent.width;
                height:parent.height-font.pixelSize;
                color: "white";
                text: "Start train";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            Text{
                anchors.top: initSadText.baseline;
                width:parent.width;
                font.pixelSize: initSadText.font.pixelSize;
                color: "white";
                text: "Sad";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    if(!initCalmDiv.saving && !initFearDiv.saving && !initJoyDiv.saving){
                        infoText.text="";
                        parent.saving=!parent.saving;
                        page.toggleSaveSad(parent.saving);

                        if(parent.saving){
                            initSadText.text="Stop train"; // it's the next action
                        }else{
                            initSadText.text="Start train";
                        }
                    }else{
                        infoText.text="Please stop other training before starting this one";
                    }
                }
            }
        }

        Rectangle{
            id:initFearDiv;
            anchors.left:initSadDiv.right;
            width:parent.width/4;
            height:parent.height;
            border.color:"white";
            color:"black";
            property bool saving: false;
            Text{
                id:initFearText;
                anchors.top:parent.top;
                font.pixelSize: 20;
                width:parent.width;
                height:parent.height-font.pixelSize;
                color: "white";
                text: "Start train";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            Text{
                anchors.top: initFearText.baseline;
                width:parent.width;
                font.pixelSize: initFearText.font.pixelSize;
                color: "white";
                text: "Fear";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    if(!initCalmDiv.saving && !initSadDiv.saving && !initJoyDiv.saving){
                        infoText.text="";
                        parent.saving=!parent.saving;
                        page.toggleSaveFear(parent.saving);

                        if(parent.saving){
                            initFearText.text="Stop train"; // it's the next action
                        }else{
                            initFearText.text="Start train";
                        }
                    }else{
                        infoText.text="Please stop other training before starting this one";
                    }
                }
            }
        }

        Rectangle{
            id:initJoyDiv;
            anchors.left: initFearDiv.right
            width:parent.width/4;
            height:parent.height;
            border.color:"white";
            color:"black";
            property bool saving: false;
            Text{
                id:initJoyText;
                anchors.top:parent.top;
                font.pixelSize: 20;
                width:parent.width;
                height:parent.height-font.pixelSize;
                color: "white";
                text: "Start train";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            Text{
                anchors.top: initJoyText.baseline;
                width:parent.width;
                font.pixelSize: initJoyText.font.pixelSize;
                color: "white";
                text: "Joy";
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    if(!initCalmDiv.saving && !initFearDiv.saving && !initSadDiv.saving){
                        infoText.text="";
                        parent.saving=!parent.saving;
                        page.toggleSaveJoy(parent.saving);

                        if(parent.saving){
                            initJoyText.text="Stop train"; // it's the next action
                        }else{
                            initJoyText.text="Start train";
                        }
                    }else{
                        infoText.text="Please stop other training before starting this one";
                    }
                }
            }
        }
    }

    Rectangle{
        id:infoDiv;
        width:parent.width;
        height:50;
        color:"black";
        anchors.bottom: initDiv.top;
        Text{
         id:infoText;
         color:"red";
         text:"";
         horizontalAlignment: Text.AlignHCenter;
         verticalAlignment: Text.AlignVCenter;
         width:parent.width;
         height:parent.height;
         font.pixelSize: 24;
        }
    }

    Rectangle{
        id:topbar;
        anchors.top:parent.top;
        anchors.left:parent.left;
        color:"black";
        height:50;
        width:parent.width;

        Rectangle{
            id:storeButton;
            color:"black";
            border.color: "white";
            anchors.left:parent.left;
            anchors.top:parent.top;
            height:parent.height;
            width:150;
            Text{
                anchors.centerIn: parent
                font.pixelSize: 20
                color: "white";
                text: "Store conf."
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    if(!initCalmDiv.saving && !initFearDiv.saving && !initSadDiv.saving && !initJoyDiv.saving){
                        infoText.text="";
                        page.storeClassifiers();
                    }else{
                        infoText.text="Please stop training before saving";
                    }
                }
            }
        }

        Rectangle{
            id:recordButton;
            color:"black";
            border.color: "white";
            anchors.left:storeButton.right;
            anchors.top:parent.top;
            height:parent.height;
            width:150;
            property bool record: false;
            Text{
                id:recordText;
                anchors.centerIn: parent
                font.pixelSize: 20
                color: "white";
                text: "Start record"
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    parent.record=!parent.record;
                    recordData(parent.record);
                    if(parent.record){
                        recordText.text="Stop record"; // it's the next action
                    }else{
                        recordText.text="Start record";
                    }
                }
            }
        }

        Rectangle
        {
            //quit button
            anchors.right: parent.right
            anchors.top: parent.top
            width: 50
            height: parent.height;
            color:"black"
            border.color: "white"
            Text
            {
                anchors.centerIn: parent
                font.pixelSize: parent.height;
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

}
