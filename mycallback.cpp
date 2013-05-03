#include "mycallback.h"

MyCallback::MyCallback(QObject *parent) :
    Sbs2Callback(parent)
{
}

/**
  Reimplemented callback method. This method sees single packets and handles them.
  */
void MyCallback::getData(Sbs2Packet *packet)
{
    //set current packet, increment counters
    setPacket(packet);

    if (currentPacketCounter%8 == 0){
        emit timeTick8();
    }

    double f3=thisPacket->values["F3"];
    double f4=thisPacket->values["F4"];
    double af3=thisPacket->values["AF3"];
    double af4=thisPacket->values["AF4"];

    changeBand("alpha");
    double f3_alpha=thisPacket->filteredValues["F3"];
    double f4_alpha=thisPacket->filteredValues["F4"];
    double af3_alpha=thisPacket->filteredValues["AF3"];
    double af4_alpha=thisPacket->filteredValues["AF4"];

    changeBand("beta");
    double f3_beta=thisPacket->filteredValues["F3"];
    double f4_beta=thisPacket->filteredValues["F4"];
    double af3_beta=thisPacket->filteredValues["AF3"];
    double af4_beta=thisPacket->filteredValues["AF4"];

    // Goal is to use af3/af4 because they are hairless copmared to f3/f4
    // So we'll apply same idea than colorOfMind but we use sensors otherwise

    // colorOfMind : double arousal = (f3_alpha/f3_beta + f4_alpha/f4_beta)/2;
    double arousal = (af3_alpha/af3_beta + af4_alpha/af4_beta)/2;
    // colorOfMind : double valence = ((f4-af4_alpha)/(f4-af4_beta) - (f3-af3_alpha)/(f3-af3_beta));
    double valence = ((af4-f4_alpha)/(af4-f4_beta) - (af3-f3_alpha)/(af3-f3_beta));

    //qDebug()<<"arousal : "<<arousal<<" and valence : "<<valence<<" emited";
    emit arousalValence(arousal,valence);
}

void MyCallback::changeBand(QString name)
{
    sbs2DataHandler->turnFilterOff();
    //qDebug()<<"MyCallBack : change band to "<<name;
    if (name.compare("alpha")){
        lowFreq = 8;
        highFreq = 13;
    }else if (name.compare("beta")){
        lowFreq = 13;
        highFreq = 30;
    }
    sbs2DataHandler->turnFilterOn(lowFreq, highFreq, 32);
    sbs2DataHandler->filter();
}
