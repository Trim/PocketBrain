#include "mycallback.h"

MyCallback::MyCallback(QObject *parent) :
    Sbs2Callback(parent)
{
    filterReady=0;
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

    if(filterReady==0){
        initFilter();
    }
    sbs2DataHandler->filter2bands();

    // alpha band
    double f3_alpha=thisPacket->filteredValues["F3"];
    double f4_alpha=thisPacket->filteredValues["F4"];
    double af3_alpha=thisPacket->filteredValues["AF3"];
    double af4_alpha=thisPacket->filteredValues["AF4"];

    // beta band
    double f3_beta=thisPacket->filteredValues2ndBand["F3"];
    double f4_beta=thisPacket->filteredValues2ndBand["F4"];
    double af3_beta=thisPacket->filteredValues2ndBand["AF3"];
    double af4_beta=thisPacket->filteredValues2ndBand["AF4"];

    // Goal is to use af3/af4 because they are hairless copmared to f3/f4
    // So we'll apply same idea than colorOfMind but we use sensors otherwise

    // colorOfMind : (front beta power)/(front alpha power)
    double arousal = (af3_alpha/af3_beta + af4_alpha/af4_beta)/2;
    // colorOfMind : Valence = (left beta power)/(left alpha power) – (right beta power)/(right alpha power)
    double valence = (af4_alpha/af4_beta - af3_alpha/af3_beta);
    //arousal = af3_alpha;
    //valence=thisPacket->values["AF3"];

    //qDebug()<<"arousal : "<<arousal<<" and valence : "<<valence<<" emited";
    emit arousalValence(arousal,valence);
}

void MyCallback::initFilter(){
    lowFreq = 8;
    highFreq = 13;
    lowFreq2nd = 13;
    highFreq2nd = 30;

    qDebug() << "Mycallback : turn filter on...";
    sbs2DataHandler->turnFilterOn2bands(lowFreq, highFreq, 32, lowFreq2nd, highFreq2nd, 32);
    qDebug() << "Mycallback : filternow...";
    sbs2DataHandler->filter2bands();
    qDebug() << "Mycallback : filter activated !";
    filterReady=1;
}
