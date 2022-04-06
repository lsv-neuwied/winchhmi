#include "meteocollector.h"

#include <time.h>
#include <math.h>

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>

const QString WIND_TOPIC("meteo/wind");
const QString AIR_PRESS_TOPIC("meteo/air/press");
const QString AIR_TEMP_TOPIC("meteo/air/temp");

MeteoCollector::MeteoCollector(MqttClient *mqtt, QObject *parent): QObject(parent), mqtt(mqtt)
{
    connect(mqtt, SIGNAL(onConnect(int)), this, SLOT(onConnect(int)), Qt::QueuedConnection);
    connect(mqtt, SIGNAL(onMessage(int, const QString &, const QByteArray &, int, bool)), this, SLOT(onMessage(int, const QString &, const QByteArray &, int, bool)), Qt::QueuedConnection);

    windVelo = 0.0;
    windVeloPeak = 0.0;
    windDir = 0.0;
    windDirAvg = 0.0;
    windTimestamp = 0;

    airTemp = 0.0;
    airTempTimestamp = 0;

    airPress = 0.0;
    airPressTrend = Steady;
    airPressTimestamp = 0;
}

qint64 MeteoCollector::currentTimestamp() {
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    return (qint64) tp.tv_sec * 1000LL + ((qint64) tp.tv_nsec / 1000000LL);
}

void MeteoCollector::onConnect(int rc)
{
    Q_UNUSED(rc)

    mqtt->subscribe(WIND_TOPIC, 0);
    mqtt->subscribe(AIR_PRESS_TOPIC, 0);
    mqtt->subscribe(AIR_TEMP_TOPIC, 0);
}

bool MeteoCollector::parseDoubleArray(const QString &s, int count, double *out)
{
    QStringList list = s.split(';');
    if (list.length() < count) {
        return false;
    }

    bool ok;
    for (int i = 0; i < count; i++) {
        out[i] = list[i].toDouble(&ok);
        if (!ok) {
            return false;
        }
    }

    return true;
}

void MeteoCollector::onMessage(int mid, const QString &topic, const QByteArray &payload, int qos, bool retain)
{
    Q_UNUSED(mid)
    Q_UNUSED(qos)
    Q_UNUSED(retain)

    if (topic == WIND_TOPIC) {
        QJsonParseError err;
        const QJsonDocument &json = QJsonDocument::fromJson(payload, &err);
        if (err.error == QJsonParseError::NoError) {
            const QJsonObject &obj = json.object();
            const QJsonValue &dir = obj["d"];
            const QJsonValue &dirAvg = obj["da"];
            const QJsonValue &velo = obj["s"];
            const QJsonValue &veloPeak = obj["sp"];

            if (dir.isDouble() && dirAvg.isDouble() && velo.isDouble() && veloPeak.isDouble()) {
                windDir = dir.toDouble();
                windDirAvg = dirAvg.toDouble();
                windVelo = velo.toDouble();
                windVeloPeak = veloPeak.toDouble();
                windTimestamp = currentTimestamp();
                emit windUpdate();
            }
        }
        return;
    }

    if (topic == AIR_PRESS_TOPIC) {
        QJsonParseError err;
        const QJsonDocument &json = QJsonDocument::fromJson(payload, &err);
        if (err.error == QJsonParseError::NoError) {
            const QJsonObject &obj = json.object();
            const QJsonValue &press = obj["p"];
            const QJsonValue &trend = obj["t"];

            if (press.isDouble() && trend.isString()) {
                airPress = press.toDouble();
                if (trend == "u") {
                    airPressTrend = Unsteady;
                } else if (trend == "r") {
                    airPressTrend = Rising;
                } else if (trend == "f") {
                    airPressTrend = Falling;
                } else {
                    airPressTrend = Steady;
                }
                airPressTimestamp = currentTimestamp();
                emit airPressUpdate();
            }
        }
        return;
    }

    if (topic == AIR_TEMP_TOPIC) {
        const QString &s = QString::fromUtf8(payload);
        bool ok;
        double val = s.toDouble(&ok);
        if (ok) {
            airTemp = val;
            airTempTimestamp = currentTimestamp();
            emit airTempUpdate();
        }
        return;
    }
}
