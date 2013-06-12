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
    _saveAngry=false;

    /* These sets are used to record current to find later the emotion*/
    _arousalSet = new QMap<double,double>();
    _valenceSet = new QMap<double,double>();

    _lastPlot=QDateTime::currentDateTime();
    getClassifiers();
}

void Emotions::arousalValence(double arousal, double valence){
    arousal=(int)(arousal*qPow(10,EMOTION_AROUSAL_ACCURACY));
    valence=(int)(valence*qPow(10,EMOTION_VALENCE_ACCURACY));

    if(_record){
        insertValue(_arousalSet, arousal);
        insertValue(_valenceSet, valence);

        QString* movie = new QString(EMOTION_MOVIE);
        if(!movie->isEmpty()){
            QFile file(_dataPath+"."+EMOTION_MOVIE);
            if (!file.open(QIODevice::WriteOnly|QIODevice::Append)) {
                qDebug() << "Emotions : cannot open file "+file.fileName()+" : "
                         << qPrintable(file.errorString()) << file.fileName()<< endl;
                return;
            }

            QString* arousalStr=new QString("");
            arousalStr->setNum(arousal,'f', EMOTION_AROUSAL_ACCURACY);

            QString* valenceStr=new QString("");
            valenceStr->setNum(valence,'f', EMOTION_VALENCE_ACCURACY);

            QString emotion = getEmotion();
            QDateTime time = QDateTime::currentDateTime();

            QString* line = new QString(*arousalStr+" "+*valenceStr+" "+emotion+" "+time.toString("hh:mm:ss:zzz")+"\n");
            file.write(line->toAscii());
            file.close();

            if(_lastPlot.msecsTo(time)>EMOTION_PLOT_TIME){
                // QProcess to call R script
                /*
                 * This script will make a boxplot of valence and arousal every
                 * EMOTION_PLOT_TIME seconds and save them in a pdf.
                 *
                 * WARNING : be aware that your computer will make a lot of job using this script
                 * and it will certainly _overload_ your CPU. You can reduce it by increasing
                 * EMOTION_PLOT_TIME.
                */
                /*QString program = "plot_movie.R";
                QStringList arguments;
                arguments << file.fileName()
                          << _dataPath+".calm"
                          << _dataPath+".exited"
                          << _dataPath+".positive"
                          << _dataPath+".negative";

                QProcess *plotProcess = new QProcess(this);
                plotProcess->startDetached(program, arguments);
                _lastPlot=QDateTime::currentDateTime();
                */
            }
        }
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
    if(_saveAngry){
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

void Emotions::toggleSaveAngry(bool save){
    _saveAngry=save;
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
        qDebug() << "Emotions : cannot open file "<< file.fileName() <<" : "
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

    if(classifier->getTrainedClasses().contains(klassName)){
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
}


void Emotions::getClassifiers(){
    QFile file(_dataPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Emotions : cannot open file "<< file.fileName() <<" : "
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
        emotion="angry";
    }else{
        emotion="try again later";
    }
    return emotion;
}
