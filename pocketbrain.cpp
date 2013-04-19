#include "pocketbrain.h"

emoactor::emoactor(QWidget *parent, Qt::WFlags f)
    : QWidget(parent, f)
{
    setupUi(this);
    //create MyCallback object
    MyCallback* myCallback = new MyCallback();
    Sbs2EmotivDataReader* sbs2DataReader = Sbs2EmotivDataReader::New(myCallback,0);
    Emotions emotions = new Emotions();

    QObject::connect(myCallback, SIGNAL(arousalValue(double)), emotions, SLOT(compute_arousal(double)));
    QObject::connect(myCallback, SIGNAL(valenceValue(double)), emotions, SLOT(compute_valence(double)));
    QObject::connect(emotions, SIGNAL(giveEmotion(QString)), uiEmotiveLabel, SLOT(setText(QString)));

    // Required to properly quit
    QObject::connect(app.data(), SIGNAL(aboutToQuit()), sbs2DataReader, SLOT(aboutToQuit()));
}
