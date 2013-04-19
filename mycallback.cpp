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

    if (currentPacketCounter%8 == 0)
        emit timeTick8();

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

    double arousal = (f3_alpha/f3_beta + f4_alpha/f4_beta)/2;
    // With bipolar measure :
    double valence = ((f4-af4_alpha)/(f4-af4_beta) - (f3-af3_alpha)/(f3-af3_beta));

    emit arousalValue(arousal);
    emit valenceValue(valence);
}

void MyCallback::changeBand(QString name)
{
    qDebug()<<"MyCallBack : change band to "<<name;
    // Values found in emokit_dsp.c (see colorOfMind)
    if (name.compare("alpha")){
        lowFreq = 8;
        highFreq = 12;
    }else if (name.compare("beta")){
        lowFreq = 16;
        highFreq = 24;
    }
    sbs2DataHandler->turnFilterOn(lowFreq, highFreq, 32); //Need to understand what is "filterOrder" (last param.)
    sbs2DataHandler->filter();
}
