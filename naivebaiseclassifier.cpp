#include "naivebaiseclassifier.h"

NaiveBaiseClassifier::NaiveBaiseClassifier(){
    _trainedClasses=new QMap<QString,QMap<double, double>*>();
    _totalFeatureOccurrences = new QMap<double,double>();
}

NaiveBaiseClassifier::NaiveBaiseClassifier(QMap<QString, QMap<double, double> *> *trainedClasses,
                                           QMap<double, double> *totalFeatureOccurrences){
    _trainedClasses = new QMap<QString, QMap<double,double>*>(*trainedClasses);
    foreach(QString klass, _trainedClasses->keys()){
        _trainedClasses->insert(klass,new QMap<double,double>(*_trainedClasses->value(klass)));
    }

    _totalFeatureOccurrences=new QMap<double,double>(*totalFeatureOccurrences);
}

NaiveBaiseClassifier::NaiveBaiseClassifier(QMap<QString, QMap<double, double> *> &trainedClasses,
                                           QMap<double, double> &totalFeatureOccurrences){
    _trainedClasses = new QMap<QString, QMap<double,double>*>(trainedClasses);
    foreach(QString klass, _trainedClasses->keys()){
        _trainedClasses->insert(klass,new QMap<double,double>(*_trainedClasses->value(klass)));
    }

    _totalFeatureOccurrences=new QMap<double,double>(totalFeatureOccurrences);
}

QString NaiveBaiseClassifier::classify(QMap<double, double> *featureSet){
    QMap<QString, double> * classProbability = new QMap<QString, double>();

    foreach(QString klass, _trainedClasses->keys()){
        classProbability->insert(klass, 0);
    }

    QMapIterator<double, double> featureIt(*featureSet);
    while (featureIt.hasNext()) {
        foreach(QString klass, _trainedClasses->keys()){
            if(classProbability->value(klass)!=0)
            classProbability->insert(klass,
                                     classProbability->value(klass)
                                     + (featureIt.value() * log(probability(featureIt.key(), klass))));
        }
        featureIt.next();
    }

    foreach(QString klass, classProbability->keys()){
        qDebug()<<klass<<" has prob. "<<classProbability->value(klass)<< " and size "<<_trainedClasses->value(klass)->size();
    }

    QString resultClass="";

    foreach(QString klass, classProbability->keys()){
        if(resultClass.isEmpty()){
            resultClass=klass;
        }

        if(classProbability->value(klass)>classProbability->value(resultClass)){
            resultClass=klass;
        }
    }

    return resultClass;
}

double NaiveBaiseClassifier::probability(double feature, QString klass){
    double num = _trainedClasses->value(klass)->value(feature)+1.0;
    double den = _totalFeatureOccurrences->value(feature)+_trainedClasses->size();
    return num/den;
}

QMap<QString,QMap<double, double>*>& NaiveBaiseClassifier::getTrainedClasses() const{
    return *_trainedClasses;
}

QMap<double, double>& NaiveBaiseClassifier::getTotalFeatureOccurences() const{
    return *_totalFeatureOccurrences;
}

QDataStream &operator<<(QDataStream &out, const NaiveBaiseClassifier &naiveBaise){
    QMap<QString, QMap<double,double>*>* trainedClasses =
            new QMap<QString, QMap<double,double>*>(naiveBaise.getTrainedClasses());
    qDebug()<<"size  : "<<naiveBaise.getTrainedClasses().size();
    out<<naiveBaise.getTrainedClasses().size();

    foreach(QString klass, trainedClasses->keys()){
        out<<klass;
        out<<*(trainedClasses->value(klass));
    }
    out << naiveBaise.getTotalFeatureOccurences();
    return out;
}

QDataStream &operator>>(QDataStream &in, NaiveBaiseClassifier &naiveBaise){
    int sizeMap;
    naiveBaise._trainedClasses=new QMap<QString,QMap<double, double>*>();

    QMap<double, double> totalFeatureOccurences;

    in >> sizeMap;
    for(int i=0;i<sizeMap;++i){
        QMap<double, double> trainedValue;
        QString trainedKey;
        in >> trainedKey >> trainedValue;
        naiveBaise._trainedClasses->insert(trainedKey, &trainedValue);
    }
    in >> *(naiveBaise._totalFeatureOccurrences);
    return in;
}
