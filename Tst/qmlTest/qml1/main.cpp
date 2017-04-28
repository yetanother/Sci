#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "wrarmadillo.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<MatrArm>("armadillo", 1, 0, "MatrArm");
    qRegisterMetaType<MatrArm>("MatrArm");
    qmlRegisterType<armaHlpr>("armadillo", 1, 0, "ArmaHlpr");
    qRegisterMetaType<armaHlpr>("ArmaHlpr");
    //
    //qRegisterMetaType<arma::mat>("Arm");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
