#ifndef EMOTIONS_H
#define EMOTIONS_H

#include <QString>
#include <QObject>
#include <QDebug>
#include <QMapIterator>
#include <QVariant>
#include "sbs2common.h"
#include "naivebayesclassifier.h"

// Number of decimals for arousal/valence (too high give bad results, too low too)
#define EMOTION_AROUSAL_ACCURACY 4
#define EMOTION_VALENCE_ACCURACY 5

class Emotions : public QObject{
    Q_OBJECT

private:
    NaiveBayesClassifier* _arousalClassifier;
    NaiveBayesClassifier* _valenceClassifier;

    QMap<QString,QMap<double, double>*>* _trainedArousalClasses; // calm / exited
    QMap<QString,QMap<double, double>*>* _trainedValenceClasses; // negative / positive

    // Sets with value and with number of occurences over all the process and over all states
    QMap<double, double>* _totalArousalOccurrences;
    QMap<double, double>* _totalValenceOccurrences;

    bool _saveCalm;
    bool _saveJoy;
    bool _saveSad;
    bool _saveFear;
    bool _guess;
    bool _record;

    // Sets with value and current number of found values
    QMap<double, double>* _arousalSet;
    QMap<double, double>* _valenceSet;
    QString curArousal;
    QString curValence;

    void insertValueAndTotal(QMap<double, double>* valueSet, QMap<double, double>* totalSet, double val);
    void insertValue(QMap<double, double> *valueSet, double val);
    void updateTrainedClass(QString arousal, QString valence);
    void getClassifiers();
    void resetCurrData();

public:
    Emotions();

public slots:
    void arousalValence(double arousal, double valence);
    void toggleSaveCalm(bool save);
    void toggleSaveFear(bool save);
    void toggleSaveSad(bool save);
    void toggleSaveJoy(bool save);
    void guessEmotion();
    void recordData(bool record);
    void storeClassifiers();

signals:
    void giveEmotion(QVariant emotion);
};

#endif // EMOTIONS_H
