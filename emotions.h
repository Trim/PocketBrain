#ifndef EMOTIONS_H
#define EMOTIONS_H

#include <QString>
#include <QObject>
#include <QDebug>
#include <QMapIterator>
#include <QVariant>
#include <QDateTime>
#include <QProcess>

#include "sbs2common.h"
#include "naivebayesclassifier.h"

// Number of decimals for arousal/valence (too high give bad results, too low too)
#define EMOTION_AROUSAL_ACCURACY 4
#define EMOTION_VALENCE_ACCURACY 5
#define EMOTION_USERNAME "adrien"

/* Set a movie title in EMOTION_MOVIE to store the recoreded data in a file
 * It will save : arousal, valence, emotion, time in a file with filename
 * ended by the EMOTION_MOVIE string
 */
#define EMOTION_MOVIE ""
#define EMOTION_PLOT_TIME 10000

class Emotions : public QObject{
    Q_OBJECT

private:
    QString _dataPath;

    /* Classifiers to fill (either by file in data path or by UI) */
    NaiveBayesClassifier* _arousalClassifier;
    NaiveBayesClassifier* _valenceClassifier;

    /* UI buttons */
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

    void insertValue(QMap<double, double> *valueSet, double val);
    void updateTrainedClass(QString arousalStr, int arousalVal, QString valenceStr, int valenceVal);
    void getClassifiers();
    void resetCurrData();

    QString getEmotion();
    void storeClass(QString klassName, NaiveBayesClassifier *classifier);
    QDateTime _lastPlot;

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
