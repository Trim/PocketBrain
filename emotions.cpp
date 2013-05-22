#include "emotions.h"

Emotions::Emotions()
{
    _arousalClassifier=new NaiveBaiseClassifier();
    _valenceClassifier=new NaiveBaiseClassifier();
    _saveCalm=false;
    _saveJoy=false;
    _saveSad=false;
    _saveFear=false;
    _arousalSet = new QMap<double,double>();
    _valenceSet = new QMap<double,double>();

    _totalArousalOccurrences = new QMap<double, double>();
    _totalValenceOccurrences = new QMap<double, double>();
    _trainedArousalClasses = new QMap<QString, QMap<double, double>*>();
    _trainedValenceClasses = new QMap<QString, QMap<double, double>*>();

    _guess=false;
    _record=false;
    _saveCalm=false;
    _saveJoy=false;
    _saveSad=false;
    _saveFear=false;

    getClassifiers();
}

void Emotions::arousalValence(double arousal, double valence){
    arousal=(int)(arousal*pow10(EMOTION_AROUSAL_ACCURACY));
    valence=(int)(valence*pow10(EMOTION_VALENCE_ACCURACY));

    //qDebug()<<"arousal : "<<arousal<<" and valence : "<<valence<<" received";

    if(_record){
        insertValueAndTotal(_arousalSet, _totalArousalOccurrences, arousal);
        insertValueAndTotal(_valenceSet, _totalValenceOccurrences, valence);
    }

    if(_saveCalm){
        updateTrainedClass("calm", "positive");
    }
    if(_saveJoy){
        updateTrainedClass("exited", "positive");
    }
    if(_saveSad){
        updateTrainedClass("calm", "negative");
    }
    if(_saveFear){
        updateTrainedClass("exited", "negative");
    }

    if(_guess){
        foreach(QString klass, _trainedArousalClasses->keys()){
            qDebug()<<"Emo : klass "<<klass<<" has "<<_trainedArousalClasses->value(klass)->size()<<" values";
        }
        if(_arousalClassifier==NULL){
            _arousalClassifier = new NaiveBaiseClassifier(_trainedArousalClasses, _totalArousalOccurrences);
        }
        foreach(QString klass, _trainedValenceClasses->keys()){
            qDebug()<<"Emo : klass "<<klass<<" has "<<_trainedValenceClasses->value(klass)->size()<<" values";
        }
        if(_valenceClassifier==NULL){
            _valenceClassifier = new NaiveBaiseClassifier(_trainedValenceClasses, _totalValenceOccurrences);
        }

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

void Emotions::insertValueAndTotal(QMap<double, double> *valueSet, QMap<double, double> *totalSet, double val){
    insertValue(valueSet, val);

    if(totalSet->contains(val)){
        totalSet->insert(val, totalSet->value(val)+1);
    }else{
        totalSet->insert(val, 1);
    }
}

void Emotions::insertValue(QMap<double, double> *valueSet, double val){
    if(valueSet->contains(val)){
        valueSet->insert(val, valueSet->value(val)+1);
    }else{
        valueSet->insert(val, 1);
    }
}

void Emotions::updateTrainedClass(QString arousal, QString valence){
    if(_trainedArousalClasses->contains(arousal)){
        QMapIterator<double, double> arousalIt(*_arousalSet);
        while (arousalIt.hasNext()) {
            arousalIt.next();
            double curKey = arousalIt.key();
            double curVal = arousalIt.value();
            if(_trainedArousalClasses->value(arousal)->contains(curKey)){
                _trainedArousalClasses->value(arousal)->insert(curKey,
                                                               _trainedArousalClasses->value(arousal)->value(curKey)+curVal);
            }else{
                _trainedArousalClasses->value(arousal)->insert(curKey,curVal);
            }
        }
    }else{
        _trainedArousalClasses->insert(arousal, _arousalSet);
    }

    if(_trainedValenceClasses->contains(valence)){
        QMapIterator<double, double> valenceIt(*_valenceSet);
        while (valenceIt.hasNext()) {
            valenceIt.next();
            double curKey = valenceIt.key();
            double curVal = valenceIt.value();
            if(_trainedValenceClasses->value(valence)->contains(curKey)){
                _trainedValenceClasses->value(valence)->insert(curKey,
                                                               _trainedValenceClasses->value(valence)->value(curKey)+curVal);
            }else{
                _trainedValenceClasses->value(valence)->insert(curKey,curVal);
            }
        }
    }else{
        _trainedValenceClasses->insert(valence, _valenceSet);
    }
}

void Emotions::storeClassifiers(){
    QFile file(Sbs2Common::getRootAppPath()+"emotionClasses_"+(int)(EMOTION_AROUSAL_ACCURACY)+"ar_"+(int)(EMOTION_VALENCE_ACCURACY)+"val.dat");

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
    QFile file(Sbs2Common::getRootAppPath()+"emotionClasses_"+(int)(EMOTION_AROUSAL_ACCURACY)+"ar_"+(int)(EMOTION_VALENCE_ACCURACY)+"val.dat");
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
