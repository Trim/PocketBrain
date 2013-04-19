#include <QApplication>
//#include "qmlapplicationviewer.h"
//#include <QtDeclarative>
#include <QScopedPointer>

#include <hardware/emotiv/sbs2emotivdatareader.h>
#include <mycallback.h>


Q_DECL_EXPORT int main(int argc, char *argv[])
{
    //QScopedPointer<QApplication> app(createApplication(argc, argv));
    QScopedPointer<QApplication> app(new QApplication(argc, argv));

    //set path where recorded data will be stored
    qDebug() << "catalogPath: "<<Sbs2Common::setDefaultCatalogPath();
    //set path where application data is stored
    qDebug() << "rootAppPath: "<<Sbs2Common::setDefaultRootAppPath();

    //required for proper closing of the app
    //QObject::connect((QObject*)viewer.engine(), SIGNAL(quit()), app.data(), SLOT(quit()));

    return app->exec();
}
