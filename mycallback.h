#ifndef MYCALLBACK_H
#define MYCALLBACK_H

#include <sbs2callback.h>

class MyCallback : public Sbs2Callback
{
    Q_OBJECT
private:
    int lowFreq;
    int highFreq;

    QVector<double>* maxValues;
    QVector<double>* minValues;

    void changeBand(QString band);

public:
    explicit MyCallback(QObject *parent = 0);
    void getData(Sbs2Packet *packet);

signals:
    void arousalValue(double value);
    void valenceValue(double value);

};

#endif // MYCALLBACK_H
