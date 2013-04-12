#include <QApplication>
#include <QtDeclarative>

#include <mycallback.h>
#include <hardware/emotiv/sbs2emotivdatareader.h>

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    //set path where recorded data will be stored
    qDebug() << "catalogPath: "<<Sbs2Common::setDefaultCatalogPath();
    //set path where application data is stored
    qDebug() << "rootAppPath: "<<Sbs2Common::setDefaultRootAppPath();

    //create MyCallback object
    MyCallback* myCallback = new MyCallback();
    Sbs2EmotivDataReader* sbs2DataReader = Sbs2EmotivDataReader::New(myCallback,0);

    //required for proper closing of the app
    QObject::connect(app.data(), SIGNAL(aboutToQuit()), sbs2DataReader, SLOT(aboutToQuit()));
    QObject::connect((QObject*)viewer.engine(), SIGNAL(quit()), app.data(), SLOT(quit()));

    return app->exec();
}
