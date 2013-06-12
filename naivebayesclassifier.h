/*
This code was inspired by this article :
http://www.nils-haldenwang.de/computer-science/machine-learning/how-to-apply-naive-bayes-classifiers-to-document-classification-problems
*/
#ifndef NAIVEBAYESCLASSIFIER_H
#define NAIVEBAYESCLASSIFIER_H

#include <QMap>
#include <QMapIterator>
#include <QString>
#include <math.h>
#include <QObject>
#include <QDataStream>
#include <QDebug>

class NaiveBayesClassifier: public QObject{
    Q_OBJECT

    friend QDataStream& operator <<( QDataStream& stream, const NaiveBayesClassifier& bookmark );
    friend QDataStream& operator >>( QDataStream& stream, NaiveBayesClassifier& bookmark );

private:
    QMap<QString,QMap<double, double>*> * _trainedClasses;
    QMap<double, double> * _totalFeatureOccurrences;

    double probability(double feature, QString klass);
    void incrementNbValueForSet(QMap<double, double> *valueSet, double val);

public:
    NaiveBayesClassifier();
    NaiveBayesClassifier(QMap<QString,QMap<double, double>*>* trainedClasses,   // key : class name, value : value found during training and their number of occurences
                         QMap<double, double>* totalFeatureOccurrences); // key : found feature, value : total of occurences sum of every class
    NaiveBayesClassifier(QMap<QString,QMap<double, double>*>& trainedClasses,
                         QMap<double, double>& totalFeatureOccurrences);
    NaiveBayesClassifier(const NaiveBayesClassifier& naiveBayes);

    NaiveBayesClassifier&  operator=(const NaiveBayesClassifier& naiveBayes);

    void addValueForClass(QString klass, double value);
    QString classify(QMap<double, double>* featureSet); // featureSet : set of values to classify with number of occurence
    QMap<QString,QMap<double, double>*> &getTrainedClasses() const;
    QMap<double, double> &getTotalFeatureOccurences() const;
};

QDataStream &operator<<(QDataStream &out, const NaiveBayesClassifier &naiveBayes);
QDataStream &operator>>(QDataStream &in, NaiveBayesClassifier &naiveBayes);

#endif // NAIVEBAYESCLASSIFIER_H
