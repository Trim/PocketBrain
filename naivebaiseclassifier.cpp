#include "naivebaiseclassifier.h"

NaiveBaiseClassifier::NaiveBaiseClassifier(QMap<QString, QMap<double, double> *> *trainedClasses,
                                           QMap<double, double> *totalFeatureOccurrences){
    _trainedClasses=trainedClasses;
    _totalFeatureOccurrences=totalFeatureOccurrences;
}

QString NaiveBaiseClassifier::classify(QMap<double, double> *featureSet){
    QMap<QString, double> * classProbability = new QMap<QString, double>();

    foreach(QString klass, _trainedClasses->keys()){
        classProbability->insert(klass, 0);
    }

    QMapIterator<double, double> featureIt(*featureSet);
    while (featureIt.hasNext()) {
        foreach(QString klass, _trainedClasses->keys()){
            classProbability->insert(klass,
                                     classProbability->value(klass)
                                     + (featureIt.value() * log(probability(featureIt.key(), klass))));
        }
        featureIt.next();
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
