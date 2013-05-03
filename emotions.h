#ifndef EMOTIONS_H
#define EMOTIONS_H

#include <QString>
#include <QObject>
#include <QDebug>
#include <QMapIterator>
#include <QVariant>
#include "naivebaiseclassifier.h"

#define NB_AROUSAL_VALENCE_BEFORE_CLASS 1000

class Emotions : public QObject{
    Q_OBJECT

private:
    NaiveBaiseClassifier* _arousalClassifier;
    NaiveBaiseClassifier* _valenceClassifier;

    QMap<QString,QMap<double, double>*>* _trainedArousalClasses; // calm / exited
    QMap<QString,QMap<double, double>*>* _trainedValenceClasses; // negative / positive

    QMap<double, double>* _totalArousalOccurrences;
    QMap<double, double>* _totalValenceOccurrences;

    bool _saveCalm;
    bool _saveJoy;
    bool _saveSad;
    bool _saveFear;

    double _sizeSet;
    int _nbInitDone;

    QMap<double, double>* _arousalSet;
    QMap<double, double>* _valenceSet;
    QString curArousal;
    QString curValence;

    void insertValueAndTotal(QMap<double, double>* valueSet, QMap<double, double>* totalSet, double val);
    void insertValue(QMap<double, double> *valueSet, double val);
    void updateTrainedClass(QString arousal, QString valence);

public:
    Emotions();

public slots:
    void arousalValence(double arousal, double valence);
    void initCalm();
    void initJoy();
    void initSad();
    void initFear();

signals:
    void giveEmotion(QVariant emotion);
};

#endif // EMOTIONS_H
