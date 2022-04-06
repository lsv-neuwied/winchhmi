#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "canreceiver.h"
#include "meteocollector.h"
#include "meteobinding.h"
#include "mqttclient.h"
#include "ecureceiver.h"
#include "errorbinding.h"
#include "iobinding.h"
#include "statebinding.h"

int main(int argc, char *argv[])
{
    if (argc < 5) {
        printf("usage: MeteoHMI <mqttClientId> <mqttHost> <mqttPort> <runwayAngle>\n");
        return 1;
    }

    mosquitto_lib_init();

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QString mqttClientId(argv[1]);
    QString mqttHost(argv[2]);
    int mqttPort = atoi(argv[3]);
    double runwayAngle = atof(argv[4]);

    MqttClient mqtt(mqttClientId);
    MeteoCollector collector(&mqtt);
    MeteoBinding meteo(&collector, runwayAngle);

    CanReceiver receiver;
    EcuReceiver ecuReceiver(&receiver);
    StateBinding stateBinding(&ecuReceiver);
    ErrorBinding errorBinding(&ecuReceiver);
    IOBinding ioBinding(&ecuReceiver);

    mqtt.connectBroker(mqttHost, mqttPort, 60, 15);
    receiver.startup("can0");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("state", &stateBinding);
    engine.rootContext()->setContextProperty("meteo", &meteo);
    engine.rootContext()->setContextProperty("errors", errorBinding.getModel());
    engine.rootContext()->setContextProperty("din", ioBinding.getDigiInModel());
    engine.rootContext()->setContextProperty("dout", ioBinding.getDigiOutModel());
    engine.rootContext()->setContextProperty("vin", ioBinding.getValueInModel());
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    int ret = app.exec();

    mosquitto_lib_cleanup();

    return ret;
}
