#ifndef STATUSRECEIVER_H
#define STATUSRECEIVER_H

#include <QObject>

class EcuReceiver : public QObject
{
    Q_OBJECT
public:
    enum MotorMode {
        MTMO_STOPPED = 0,
        MTMO_IDLE,
        MTMO_RUNNING
    };

    static const quint16 ERROR_MASK_EMCY_STOP = (1 << 0);
    static const quint16 ERROR_MASK_SERVICE = (1 << 1);
    static const quint16 ERROR_MASK_OIL_PRESS = (1 << 2);
    static const quint16 ERROR_MASK_VOLT_BAT = (1 << 3);
    static const quint16 ERROR_MASK_VOLT_GEN = (1 << 4);
    static const quint16 ERROR_MASK_TANK = (1 << 5);
    static const quint16 ERROR_MASK_TEMP = (1 << 6);
    static const quint16 ERROR_MASK_GEAR_ENG = (1 << 7);

    static const quint16 ERROR_STATE_STOP_REQ = (1 << 1);
    static const quint16 ERROR_STATE_WARMUP_REQ = (1 << 2);
    static const quint16 ERROR_STATE_MOT_WARM = (1 << 3);

    static const quint32 IN_EMCY_STOP = (1 << 0);
    static const quint32 IN_SW_REEL_LEFT = (1 << 1);
    static const quint32 IN_SW_REEL_RIGHT = (1 << 2);
    static const quint32 IN_SW_GEAR_ENGAGED = (1 << 3);
    static const quint32 IN_SW_OIL_PRESS = (1 << 4);
    static const quint32 IN_SW_SERVICE = (1 << 5);
    static const quint32 IN_FB_IGN_BAT = (1 << 6);
    static const quint32 IN_BTN_START = (1 << 7);
    static const quint32 IN_BTN_STOP = (1 << 8);
    static const quint32 IN_BTN_ERR_RESET = (1 << 9);

    static const quint32 OUT_STARTER = (1 << 0);
    static const quint32 OUT_FAN1 = (1 << 1);
    static const quint32 OUT_FAN2 = (1 << 2);
    static const quint32 OUT_CTRL_VOLTAGE = (1 << 3);
    static const quint32 OUT_HORN = (1 << 4);
    static const quint32 OUT_BUZZER = (1 << 5);
    static const quint32 OUT_ROTATING_LIGHT = (1 << 6);
    static const quint32 OUT_VALVE_TRANSM = (1 << 7);
    static const quint32 OUT_LAMP_ERROR = (1 << 8);
    static const quint32 OUT_LAMP_START = (1 << 9);
    static const quint32 OUT_LAMP_STOP = (1 << 10);
    static const quint32 OUT_LAMP_REEL_LEFT = (1 << 11);
    static const quint32 OUT_LAMP_REEL_RIGHT = (1 << 12);
    static const quint32 OUT_BRAKE = (1 << 13);
    static const quint32 OUT_IGNITION = (1 << 14);

    explicit EcuReceiver(const QObject *receiver, QObject *parent = 0);

    qint64 currentTimestamp();

    qint64 getErrTimestamp() const { return errTimestamp; }
    quint16 getErrMaskActive() const { return errMaskActive; }
    quint16 getErrMaskNack() const { return errMaskNack; }
    quint16 getStateMask() const { return stateMask; }
    enum MotorMode getMotorMode() const { return motorMode; }

    qint64 getIODigiTimestamp() const { return ioDigiTimestamp; }
    quint32 getInDigi() const { return inDigi; }
    quint32 getOutDigi() const { return outDigi; }


    qint64 getInAvTimestamp() const { return inAvTimestamp; }
    double getInVoltageBat() const { return inVoltageBat; }
    double getInVoltageGen() const { return inVoltageGen; }
    double getInVoltageTankLevel() const { return inVoltageTankLevel; }

    qint64 getInArTimestamp() const { return inArTimestamp; }
    double getInHeadTempLeftOhms() const { return inHeadTempLeftOhms; }
    double getInHeadTempRightOhms() const { return inHeadTempRightOhms; }
    double getInMotorRpmHz() const { return inMotorRpmHz; }

    qint64 getInEncTimestamp() const { return inEncTimestamp; }
    qint32 getInEncReelLeft() const { return inEncReelLeft; }
    qint32 getInEncReelRight() const { return inEncReelRight; }

    qint64 getMotorTimestamp() const { return motorTimestamp; }
    double getHeadTempLeft() const { return headTempLeft; }
    double getHeadTempRight() const { return headTempRight; }
    double getTankLevelPct() const { return tankLevelPct; }
    double getMotorRpm() const { return motorRpm; }

private:
    qint64 errTimestamp;
    quint16 errMaskActive;
    quint16 errMaskNack;
    quint16 stateMask;
    enum MotorMode motorMode;

    qint64 ioDigiTimestamp;
    quint32 inDigi;
    quint32 outDigi;

    qint64 inAvTimestamp;
    double inVoltageBat;
    double inVoltageGen;
    double inVoltageTankLevel;

    qint64 inArTimestamp;
    double inHeadTempLeftOhms;
    double inHeadTempRightOhms;
    double inMotorRpmHz;

    qint64 inEncTimestamp;
    qint32 inEncReelLeft;
    qint32 inEncReelRight;

    qint64 motorTimestamp;
    double headTempLeft;
    double headTempRight;
    double tankLevelPct;
    double motorRpm;

signals:
    void errUpdate();
    void ioDigiUpdate();
    void inAvUpdate();
    void inArUpdate();
    void inEncUpdate();
    void motorUpdate();

private slots:
    void canReceived(bool isEff, bool isRtr, bool isErr, quint32 canId, const QByteArray &data);
};

#endif // STATUSRECEIVER_H
