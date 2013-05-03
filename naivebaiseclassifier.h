/*
This code was inspired by this article :
http://www.nils-haldenwang.de/computer-science/machine-learning/how-to-apply-naive-bayes-classifiers-to-document-classification-problems
*/
#ifndef NAIVEBAISECLASSIFIER_H
#define NAIVEBAISECLASSIFIER_H

#include <QMap>
#include <QMapIterator>
#include <QString>
#include <math.h>
#include <QObject>

class NaiveBaiseClassifier: public QObject
{
    Q_OBJECT
private:
    QMap<QString,QMap<double, double>*> * _trainedClasses;
    QMap<double, double> * _totalFeatureOccurrences;

    double probability(double feature, QString klass);

public:
    NaiveBaiseClassifier(QMap<QString,QMap<double, double>*>* trainedClasses,   // key : class name, value : value found during training and their number of occurences
                         QMap<double, double>* totalFeatureOccurrences); // key : found feature, value : total of occurences sum of every class
    QString classify(QMap<double, double>* featureSet); // featureSet : set of values to classify with number of occurence
};

#endif // NAIVEBAISECLASSIFIER_H
