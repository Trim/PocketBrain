#include "naivebayesclassifier.h"

NaiveBayesClassifier::NaiveBayesClassifier(){
    _trainedClasses=new QMap<QString,QMap<double, double>*>();
    _totalFeatureOccurrences = new QMap<double,double>();
}

NaiveBayesClassifier::NaiveBayesClassifier(QMap<QString, QMap<double, double> *> *trainedClasses,
                                           QMap<double, double> *totalFeatureOccurrences){
    _trainedClasses = new QMap<QString, QMap<double,double>*>(*trainedClasses);
    foreach(QString klass, _trainedClasses->keys()){
        _trainedClasses->insert(klass,new QMap<double,double>(*_trainedClasses->value(klass)));
    }

    _totalFeatureOccurrences=new QMap<double,double>(*totalFeatureOccurrences);
}

NaiveBayesClassifier::NaiveBayesClassifier(QMap<QString, QMap<double, double> *> &trainedClasses,
                                           QMap<double, double> &totalFeatureOccurrences){
    _trainedClasses = new QMap<QString, QMap<double,double>*>(trainedClasses);
    foreach(QString klass, _trainedClasses->keys()){
        _trainedClasses->insert(klass,new QMap<double,double>(*_trainedClasses->value(klass)));
    }

    _totalFeatureOccurrences=new QMap<double,double>(totalFeatureOccurrences);
}

QString NaiveBayesClassifier::classify(QMap<double, double> *featureSet){
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

double NaiveBayesClassifier::probability(double feature, QString klass){
    double num = _trainedClasses->value(klass)->value(feature)+1.0;
    double den = _totalFeatureOccurrences->value(feature)+_trainedClasses->size();
    return num/den;
}

QMap<QString,QMap<double, double>*>& NaiveBayesClassifier::getTrainedClasses() const{
    return *_trainedClasses;
}

QMap<double, double>& NaiveBayesClassifier::getTotalFeatureOccurences() const{
    return *_totalFeatureOccurrences;
}

QDataStream &operator<<(QDataStream &out, const NaiveBayesClassifier &naiveBayes){
    qDebug()<<"size  : "<<naiveBayes._trainedClasses->size();

    out<<naiveBayes._trainedClasses->size();
    foreach(QString klass, naiveBayes._trainedClasses->keys()){
        out<<klass;
        out<<naiveBayes._trainedClasses->value(klass)->keys().size();
        foreach(double feature, (naiveBayes._trainedClasses->value(klass)->keys())){
            out<<feature<<naiveBayes._trainedClasses->value(klass)->value(feature);
        }
    }
    out << *(naiveBayes._totalFeatureOccurrences);
    return out;
}

QDataStream &operator>>(QDataStream &in, NaiveBayesClassifier &naiveBayes){
    int nbTrainedClasses;

    in >> nbTrainedClasses;
    for(int i=0;i<nbTrainedClasses;++i){
        QString klass;
        QMap<double,double>* features = new QMap<double,double>();
        int nbFeatures;

        in >> klass;
        in>>nbFeatures;
        naiveBayes._trainedClasses->insert(klass, features);

        for(int j=0;j<nbFeatures;++j){
            double feature;
            double occFeature;
            in >> feature >> occFeature;
            features->insert(feature, occFeature);
        }
    }
    in >> *(naiveBayes._totalFeatureOccurrences);
    return in;
}
