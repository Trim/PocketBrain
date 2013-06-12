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
        emit giveEmotion(QVariant(getEmotion()));
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
    storeClass("calm", _arousalClassifier);
    storeClass("exited", _arousalClassifier);
    storeClass("positive", _valenceClassifier);
    storeClass("negative", _valenceClassifier);
}

void Emotions::storeClass(QString klassName, NaiveBayesClassifier *classifier){
    double acc=0;
    if(klassName.contains("calm") || klassName.contains("exited")){
        acc=EMOTION_AROUSAL_ACCURACY;
    }else if(klassName.contains("positive") || klassName.contains("negative")){
        acc=EMOTION_VALENCE_ACCURACY;
    }else{
        return;
    }

    QFile file(_dataPath+"."+klassName);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Emotions : cannot open file "+file.fileName()+" : "
                 << qPrintable(file.errorString()) << file.fileName()<< endl;
        return;
    }

    QMapIterator<double, double>* clasIt = new QMapIterator<double,double>(*classifier->getTrainedClasses().value(klassName));
    while(clasIt->hasNext()){
        clasIt->next();
        for(int j=0;j<clasIt->value();++j){
            QString* strVal=new QString("");
            strVal->setNum(clasIt->key(),'f', acc);
            file.write(strVal->toAscii());
            file.putChar('\n');
        }
    }
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

QString Emotions::getEmotion(){
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
    return emotion;
}

