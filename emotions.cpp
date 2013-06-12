#include "emotions.h"

Emotions::Emotions()
{
    _dataPath=Sbs2Common::getRootAppPath()
            + "emotionClasses_"EMOTION_USERNAME"_"
            + QString::number(EMOTION_AROUSAL_ACCURACY)+"ar_"
            + QString::number(EMOTION_VALENCE_ACCURACY)+"val"
            +".dat";
    qDebug()<<"Data path : "<<_dataPath;

    /* Classifiers to fill (either by file in data path or by UI) */
    _arousalClassifier=new NaiveBayesClassifier();
    _valenceClassifier=new NaiveBayesClassifier();

    /* UI buttons */
    _guess=false;
    _record=false;
    _saveCalm=false;
    _saveJoy=false;
    _saveSad=false;
    _saveFear=false;

    /* These sets are used to record current to find later the emotion*/
    _arousalSet = new QMap<double,double>();
    _valenceSet = new QMap<double,double>();

    getClassifiers();
}

void Emotions::arousalValence(double arousal, double valence){
    arousal=(int)(arousal*pow10(EMOTION_AROUSAL_ACCURACY));
    valence=(int)(valence*pow10(EMOTION_VALENCE_ACCURACY));

    if(_record){
        insertValue(_arousalSet, arousal);
        insertValue(_valenceSet, valence);
    }

    if(_saveCalm){
        updateTrainedClass("calm", arousal, "positive", valence);
    }
    if(_saveJoy){
        updateTrainedClass("exited", arousal, "positive", valence);
    }
    if(_saveSad){
        updateTrainedClass("calm", arousal, "negative", valence);
    }
    if(_saveFear){
        updateTrainedClass("exited", arousal, "negative", valence);
    }

    if(_guess){
        curArousal = _arousalClassifier->classify(_arousalSet);
        curValence = _valenceClassifier->classify(_valenceSet);

        QString emotion;
        if(curArousal.contains("calm") && curValence.contains("positive")){
            emotion="calm";
        }else if(curArousal.contains("calm") && curValence.contains("negative")){
            emotion="sad";
        }else if(curArousal.contains("exited") && curValence.contains("positive")){
            emotion="joy";
        }else if(curArousal.contains("exited") && curValence.contains("negative")){
            emotion="fear";
        }else{
            emotion="try again later";
        }

        emit giveEmotion(QVariant(emotion));
        _guess=false;
    }
}

void Emotions::toggleSaveCalm(bool save){
    _saveCalm = save;
    resetCurrData();
}

void Emotions::toggleSaveJoy(bool save){
    _saveJoy = save;
    resetCurrData();
}

void Emotions::toggleSaveSad(bool save){
    _saveSad=save;
    resetCurrData();
}

void Emotions::toggleSaveFear(bool save){
    _saveFear=save;
    resetCurrData();
}

void Emotions::recordData(bool record){
    _record=record;
    if(!record){
        resetCurrData();
    }
}

void Emotions::guessEmotion(){
    _guess=true;
}

void Emotions::insertValue(QMap<double, double> *valueSet, double val){
    if(valueSet->contains(val)){
        valueSet->insert(val, valueSet->value(val)+1);
    }else{
        valueSet->insert(val, 1);
    }
}

void Emotions::updateTrainedClass(QString arousalStr, int arousalVal, QString valenceStr, int valenceVal){
    _arousalClassifier->addValueForClass(arousalStr, arousalVal);
    _valenceClassifier->addValueForClass(valenceStr, valenceVal);
}

void Emotions::storeClassifiers(){
    QFile file(_dataPath);

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Emotions : cannot open file emotionClasses.dat : "
                 << qPrintable(file.errorString()) << file.fileName()<< endl;
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_8);
    out<<*_arousalClassifier;
    out<<*_valenceClassifier;
}

void Emotions::getClassifiers(){
    QFile file(_dataPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Emotions : cannot open file emotionClasses.dat : "
                 << qPrintable(file.errorString()) << file.fileName() << endl;
        return;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_8);
    in>>*_arousalClassifier;
    in>>*_valenceClassifier;
}

void Emotions::resetCurrData(){
    _arousalSet->clear();
    _valenceSet->clear();
}
