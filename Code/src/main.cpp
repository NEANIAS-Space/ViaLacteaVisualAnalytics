#include "mainwindow.h"
#include <QApplication>
#include "singleton.h"
#include <QSplashScreen>
#include <QBitmap>
#include <QDebug>
//#include "vosamp.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow *w = &Singleton<MainWindow>::Instance();
    a.setApplicationName("Vialactea - Visual Analytics client");


 //  vosamp *samp = &Singleton<vosamp>::Instance();

   // w->show();


    return a.exec();

}
