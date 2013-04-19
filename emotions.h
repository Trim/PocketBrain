#ifndef EMOTIONS_H
#define EMOTIONS_H

#include <QString>
#include <QTimer>
#include <math.h>

#define UNSET_VALUE INFINITY

class Emotions : public QObject{
    Q_OBJECT
public:
    Emotions();
private:
    double arousal;
    double arousal_4s;
    double arousal_20s;
    double arousal_max;
    double arousal_min;
    bool arousal_wait_4s;
    bool arousal_wait_20s;

    double valence;
    double valence_4s;
    double valence_20s;
    double valence_max;
    double valence_min;
    bool valence_wait_4s;
    bool valence_wait_20s;

    double max_diff_arousal;
    double max_diff_valence;

    double max_arousal;
    double min_arousal;

    double max_valence;
    double min_valence;

    void resetValues();
    void computeEmotion();

public slots:
    void compute_arousal(double value);
    void compute_valence(double value);
    void wait_arousal_4s();
    void wait_arousal_20s();
    void wait_valence_4s();
    void wait_valence_20s();

signals:
    void giveEmotion(QString emotion);
};

#endif // EMOTIONS_H
