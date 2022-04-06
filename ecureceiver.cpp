#include "ecureceiver.h"

#include <time.h>

#define CAN_BASE_ID 0x600

class EcuBuffer
{
public:
    EcuBuffer(const QByteArray &data) : data(data), pos(0) {}

    int length() {
        return data.length();
    }

    quint8 getByte() {
        return (quint8) data[pos++];
    }

    qint8 getSignedByte() {
        return getByte();
    }

    quint16 getShort() {
        return ((quint16) getByte()) | (((quint16) getByte()) << 8);
    }

    qint16 getSignedShort() {
        return getShort();
    }

    quint32 getLong() {
        return ((quint32) getByte()) | (((quint32) getByte()) << 8) | (((quint32) getByte()) << 16) | (((quint32) getByte()) << 24);
    }

    qint32 getSignedLong() {
        return getLong();
    }

private:
    const QByteArray data;
    int pos;

};

EcuReceiver::EcuReceiver(const QObject *receiver, QObject *parent) :
    QObject(parent),
    errTimestamp(0),
    errMaskActive(0),
    errMaskNack(0),
    stateMask(0),
    motorMode(MTMO_STOPPED),
    ioDigiTimestamp(0),
    inDigi(0),
    outDigi(0),
    inAvTimestamp(0),
    inVoltageBat(0.0),
    inVoltageGen(0.0),
    inVoltageTankLevel(0.0),
    inArTimestamp(0),
    inHeadTempLeftOhms(0.0),
    inHeadTempRightOhms(0.0),
    inMotorRpmHz(0.0),
    inEncTimestamp(0),
    inEncReelLeft(0),
    inEncReelRight(0),
    motorTimestamp(0),
    headTempLeft(0.0),
    headTempRight(0.0),
    tankLevelPct(0.0),
    motorRpm(0.0)
{
    connect(receiver, SIGNAL(received(bool, bool, bool, quint32, const QByteArray &)), this, SLOT(canReceived(bool, bool, bool, quint32, const QByteArray &)));
}

qint64 EcuReceiver::currentTimestamp() {
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    return (qint64) tp.tv_sec * 1000LL + ((qint64) tp.tv_nsec / 1000000LL);
}

void EcuReceiver::canReceived(bool isEff, bool isRtr, bool isErr, quint32 canId, const QByteArray &data) {
    Q_UNUSED(isRtr);

    // ignore error frames
    if (isErr) {
        return;
    }

    // need standard frame
    if (isEff) {
        return;
    }

    EcuBuffer buf(data);
    switch (canId) {
    case CAN_BASE_ID + 0:
        if (buf.length() >= 7) {
            errTimestamp = currentTimestamp();
            errMaskActive = buf.getShort();
            errMaskNack = buf.getShort();
            stateMask = buf.getShort();
            motorMode = (enum MotorMode) buf.getByte();
            emit errUpdate();
        }
        break;
    case CAN_BASE_ID + 1:
        if (buf.length() >= 8) {
            ioDigiTimestamp = currentTimestamp();
            inDigi = buf.getLong();
            outDigi = buf.getLong();
            emit ioDigiUpdate();
        }
        break;
    case CAN_BASE_ID + 2:
        if (buf.length() >= 6) {
            inAvTimestamp = currentTimestamp();
            inVoltageBat = ((double) buf.getSignedShort()) * 0.001;
            inVoltageGen = ((double) buf.getSignedShort()) * 0.001;
            inVoltageTankLevel = ((double) buf.getSignedShort()) * 0.001;
            emit inAvUpdate();
        }
        break;
    case CAN_BASE_ID + 3:
        if (buf.length() >= 6) {
            inArTimestamp = currentTimestamp();
            inHeadTempLeftOhms = ((double) buf.getSignedShort()) * 1.0;
            inHeadTempRightOhms = ((double) buf.getSignedShort()) * 1.0;
            inMotorRpmHz = ((double) buf.getSignedShort()) * 0.1;
            emit inArUpdate();
        }
        break;
    case CAN_BASE_ID + 4:
        if (buf.length() >= 8) {
            inEncTimestamp = currentTimestamp();
            inEncReelLeft = buf.getSignedLong();
            inEncReelRight = buf.getSignedLong();
            emit inEncUpdate();
        }
        break;
    case CAN_BASE_ID + 5:
        if (buf.length() >= 8) {
            motorTimestamp = currentTimestamp();
            headTempLeft = ((double)buf.getSignedShort()) * 0.1;
            headTempRight = ((double)buf.getSignedShort()) * 0.1;
            tankLevelPct = ((double) buf.getSignedShort()) * 0.1;
            motorRpm = ((double) buf.getSignedShort()) * 1.0;
            emit motorUpdate();
        }
        break;
    }
}
