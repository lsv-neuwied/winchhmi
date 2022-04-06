#ifndef METEOCOLLECTOR_H
#define METEOCOLLECTOR_H

#include <QObject>

#include "mqttclient.h"

class MeteoCollector : public QObject
{
    Q_OBJECT
public:
    enum AirPressTrend { Steady, Unsteady, Rising, Falling };

    explicit MeteoCollector(MqttClient *mqtt, QObject *parent = 0);

    double getWindVelo() { return windVelo; }
    double getWindVeloPeak() { return windVeloPeak; }
    double getWindDir() { return windDir; }
    double getWindDirAvg() { return windDirAvg; }
    double getAirTemp() { return airTemp; }
    double getAirPress() { return airPress; }
    enum AirPressTrend getAirPressTrend() { return airPressTrend; }

    qint64 currentTimestamp();

    qint64 getWindTimestamp() { return windTimestamp; }
    qint64 getAirTempTimestamp() { return airTempTimestamp; }
    qint64 getAirPressTimestamp() { return airPressTimestamp; }

private:
    MqttClient *mqtt;

    double windVelo;
    double windVeloPeak;
    double windDir;
    double windDirAvg;
    double airTemp;
    double airPress;
    enum AirPressTrend airPressTrend;

    qint64 windTimestamp;
    qint64 airTempTimestamp;
    qint64 airPressTimestamp;

    bool parseDoubleArray(const QString &s, int count, double *out);

signals:
    void windUpdate();
    void airTempUpdate();
    void airPressUpdate();

private slots:
    void onConnect(int rc);
    void onMessage(int mid, const QString &topic, const QByteArray &payload, int qos, bool retain);

};

#endif // METEOCOLLECTOR_H
