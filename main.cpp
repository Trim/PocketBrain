#include <QApplication>
#include "qmlapplicationviewer.h"
#include <QtDeclarative>

#include <hardware/emotiv/sbs2emotivdatareader.h>
#include <mycallback.h>

#include <QVariant>
#include <QDir>
#include "emotions.h"


Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    //set path where recorded data will be stored
    qDebug() << "catalogPath: "<<Sbs2Common::setDefaultCatalogPath();
    //set path where application data is stored
    qDebug() << "rootAppPath: "<<Sbs2Common::setDefaultRootAppPath();
    QDir* dir = new QDir();
    dir->mkpath(Sbs2Common::getCatalogPath());
    dir->mkpath(Sbs2Common::getRootAppPath());

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/PocketBrain/main.qml"));
    viewer.showExpanded();
    //get main qml object (from main.qml)
    QObject *rootObject = dynamic_cast<QObject*>(viewer.rootObject());

    //create mycallback object and link it well
    MyCallback* myCallback = new MyCallback();
    Sbs2EmotivDataReader* sbs2DataReader = Sbs2EmotivDataReader::New(myCallback,0);
    Emotions *emotions = new Emotions();

    QObject::connect(myCallback, SIGNAL(arousalValence(double, double)), emotions, SLOT(arousalValence(double,double)));
    QObject::connect(rootObject, SIGNAL(toggleSaveSad(bool)), emotions, SLOT(toggleSaveSad(bool)));
    QObject::connect(rootObject, SIGNAL(toggleSaveCalm(bool)), emotions, SLOT(toggleSaveCalm(bool)));
    QObject::connect(rootObject, SIGNAL(toggleSaveFear(bool)), emotions, SLOT(toggleSaveFear(bool)));
    QObject::connect(rootObject, SIGNAL(toggleSaveJoy(bool)), emotions, SLOT(toggleSaveJoy(bool)));
    QObject::connect(rootObject, SIGNAL(storeClassifiers()), emotions, SLOT(storeClassifiers()));
    QObject::connect(rootObject, SIGNAL(guessEmotion()), emotions, SLOT(guessEmotion()));
    QObject::connect(emotions, SIGNAL(giveEmotion(QVariant)), rootObject, SLOT(updateEmotion(QVariant)));
    QObject::connect(myCallback,SIGNAL(timeTick8()),rootObject,SLOT(timeTick()));

    //required for proper closing of the app
    QObject::connect((QObject*)viewer.engine(), SIGNAL(quit()), app.data(), SLOT(quit()));
    QObject::connect(app.data(), SIGNAL(aboutToQuit()), sbs2DataReader, SLOT(aboutToQuit()));

    return app->exec();
}
