#include "emotions.h"

Emotions::Emotions()
{
    _arousalClassifier=NULL;
    _valenceClassifier=NULL;
    _saveCalm=false;
    _saveJoy=false;
    _saveSad=false;
    _saveFear=false;
    _nbInitDone=0;
    _sizeSet=0;
    _arousalSet = new QMap<double,double>();
    _valenceSet = new QMap<double,double>();
    _totalArousalOccurrences = new QMap<double, double>();
    _totalValenceOccurrences = new QMap<double, double>();

    _trainedArousalClasses = new QMap<QString, QMap<double, double>*>();
    _trainedValenceClasses = new QMap<QString, QMap<double, double>*>();
}

void Emotions::arousalValence(double arousal, double valence){
    insertValueAndTotal(_arousalSet, _totalArousalOccurrences, arousal);
    insertValueAndTotal(_valenceSet, _totalValenceOccurrences, valence);
    _sizeSet++;

    if(_saveCalm){
        if(_sizeSet >= NB_AROUSAL_VALENCE_BEFORE_CLASS){
            updateTrainedClass("calm", "positive");
            _saveCalm=false;
            _nbInitDone++;
            qDebug()<<"initCalm done";
        }
    }
    if(_saveJoy){
        if(_sizeSet >= NB_AROUSAL_VALENCE_BEFORE_CLASS){
            updateTrainedClass("exited", "positive");
            _saveJoy=false;
            _nbInitDone++;
            qDebug()<<"initJoy done";
        }
    }
    if(_saveSad){
        if(_sizeSet >= NB_AROUSAL_VALENCE_BEFORE_CLASS){
            updateTrainedClass("calm", "negative");
            _saveSad=false;
            _nbInitDone++;
            qDebug()<<"initSad done";
        }
    }
    if(_saveFear){
        if(_sizeSet >= NB_AROUSAL_VALENCE_BEFORE_CLASS){
            updateTrainedClass("exited", "negative");
            _saveFear=false;
            _nbInitDone++;
            qDebug()<<"initFear done";
        }
    }

    if(_nbInitDone==4 && _sizeSet>=NB_AROUSAL_VALENCE_BEFORE_CLASS){
        if(_arousalClassifier==NULL){
            _arousalClassifier = new NaiveBaiseClassifier(_trainedArousalClasses, _totalArousalOccurrences);
        }
        if(_valenceClassifier==NULL){
            _valenceClassifier = new NaiveBaiseClassifier(_trainedValenceClasses, _totalValenceOccurrences);
        }

        curArousal = _arousalClassifier->classify(_arousalSet);
        curValence = _valenceClassifier->classify(_valenceSet);
        emit giveEmotion(QVariant(curArousal+" "+curValence));
    }
}

void Emotions::initCalm(){
    qDebug()<<"initCalm begin";
    _saveCalm = true;
}

void Emotions::initJoy(){
    qDebug()<<"initJoy begin";
    _saveJoy = true;
}

void Emotions::initSad(){
    qDebug()<<"initSad begin";
    _saveSad=true;
}

void Emotions::initFear(){
    qDebug()<<"initFear begin";
    _saveFear=true;
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
            double curKey = arousalIt.key();
            double curVal = arousalIt.value();
            if(_trainedArousalClasses->value(arousal)->contains(curKey)){
                _trainedArousalClasses->value(arousal)->insert(curKey,
                                               _trainedArousalClasses->value(arousal)->value(curKey)+curVal);
            }else{
                _trainedArousalClasses->value(arousal)->insert(curKey,curVal);
            }
            arousalIt.next();
        }
    }else{
        _trainedArousalClasses->insert(arousal, _arousalSet);
    }

    if(_trainedValenceClasses->contains(valence)){
        QMapIterator<double, double> valenceIt(*_valenceSet);
        while (valenceIt.hasNext()) {
            double curKey = valenceIt.key();
            double curVal = valenceIt.value();
            if(_trainedValenceClasses->value(valence)->contains(curKey)){
                _trainedValenceClasses->value(valence)->insert(curKey,
                                               _trainedValenceClasses->value(valence)->value(curKey)+curVal);
            }else{
                _trainedValenceClasses->value(valence)->insert(curKey,curVal);
            }
            valenceIt.next();
        }
    }else{
        _trainedValenceClasses->insert(valence, _valenceSet);
    }
    _arousalSet->clear();
    _valenceSet->clear();
    _sizeSet = 0;
}
