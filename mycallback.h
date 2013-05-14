#ifndef MYCALLBACK_H
#define MYCALLBACK_H

#include <sbs2callback.h>
#include <sbs2datahandler.h>

class MyCallback : public Sbs2Callback
{
    Q_OBJECT
private:
    int lowFreq;
    int highFreq;
    int lowFreq2nd;
    int highFreq2nd;

    QVector<double>* maxValues;
    QVector<double>* minValues;

    void initFilter();
    int filterReady;

public:
    explicit MyCallback(QObject *parent = 0);
    void getData(Sbs2Packet *packet);

signals:
    void arousalValence(double arousal, double valence);
};
#endif // MYCALLBACK_H
