#include "iobinding.h"

const QString IODigiItem::COLOR_OFF = "gray";
const QString IODigiItem::COLOR_ON = "green";

void IODigiItem::update(quint32 ioMask) {
    bool newState = !!(ioMask & mask);
    bool changed = state != newState;
    state = newState;
    if (changed) {
        emit stateChanged();
    }
}

void IOAnaItem::update(double newValue) {
    bool changed = value != newValue;
    value = newValue;
    if (changed) {
        emit valueChanged();
    }
}

void IOEncItem::update(qint32 newValue) {
    bool changed = value != newValue;
    value = newValue;
    if (changed) {
        emit valueChanged();
    }
}

IOBinding::IOBinding(const EcuReceiver *receiver, QObject *parent) : QObject(parent), receiver(receiver)
{
    digiIn.append(new IODigiItem("Not-Aus", EcuReceiver::IN_EMCY_STOP));
    digiIn.append(new IODigiItem("Trommel links", EcuReceiver::IN_SW_REEL_LEFT));
    digiIn.append(new IODigiItem("Trommel rechts", EcuReceiver::IN_SW_REEL_RIGHT));
    digiIn.append(new IODigiItem("Getriebe eingelegt", EcuReceiver::IN_SW_GEAR_ENGAGED));
    digiIn.append(new IODigiItem("Öldruckschalter", EcuReceiver::IN_SW_OIL_PRESS));
    digiIn.append(new IODigiItem("Wartungsschalter", EcuReceiver::IN_SW_SERVICE));
    digiIn.append(new IODigiItem("Rückmeldung Zündung", EcuReceiver::IN_FB_IGN_BAT));
    digiIn.append(new IODigiItem("Taster 'Start'", EcuReceiver::IN_BTN_START));
    digiIn.append(new IODigiItem("Taster 'Stop'", EcuReceiver::IN_BTN_STOP));
    digiIn.append(new IODigiItem("Taster 'Entstören'", EcuReceiver::IN_BTN_ERR_RESET));

    digiOut.append(new IODigiItem("Anlasser", EcuReceiver::OUT_STARTER));
    digiOut.append(new IODigiItem("Lüfter links", EcuReceiver::OUT_FAN1));
    digiOut.append(new IODigiItem("Lüfter rechts", EcuReceiver::OUT_FAN2));
    digiOut.append(new IODigiItem("Steuerspannung", EcuReceiver::OUT_CTRL_VOLTAGE));
    digiOut.append(new IODigiItem("Horn Winde", EcuReceiver::OUT_HORN));
    digiOut.append(new IODigiItem("Hupe Kabine", EcuReceiver::OUT_BUZZER));
    digiOut.append(new IODigiItem("Rundumleuchte", EcuReceiver::OUT_ROTATING_LIGHT));
    digiOut.append(new IODigiItem("Ventil Getriebe", EcuReceiver::OUT_VALVE_TRANSM));
    digiOut.append(new IODigiItem("Lampe 'Fehler'", EcuReceiver::OUT_LAMP_ERROR));
    digiOut.append(new IODigiItem("Lampe 'Start'", EcuReceiver::OUT_LAMP_START));
    digiOut.append(new IODigiItem("Lampe 'Stop'", EcuReceiver::OUT_LAMP_STOP));
    digiOut.append(new IODigiItem("Lampe 'Trommel links'", EcuReceiver::OUT_LAMP_REEL_LEFT));
    digiOut.append(new IODigiItem("Lampe 'Trommel rechts'", EcuReceiver::OUT_LAMP_REEL_RIGHT));
    digiOut.append(new IODigiItem("Kardanbremse", EcuReceiver::OUT_BRAKE));
    digiOut.append(new IODigiItem("Freigabe Zündung", EcuReceiver::OUT_IGNITION));

    valueIn.append(inVoltageBatItem = new IOAnaItem("Batteriespannung (V)", 3));
    valueIn.append(inVoltageGenItem = new IOAnaItem("Generatorspannung (V)", 3));
    valueIn.append(inVoltageTankLevelItem = new IOAnaItem("Tanksensor (V)", 3));

    valueIn.append(inHeadTempLeftOhmsItem = new IOAnaItem("Tempsensor links (Ohm)", 0));
    valueIn.append(inHeadTempRightOhmsItem = new IOAnaItem("Tempsensor rechts (Ohm)", 0));
    valueIn.append(inMotorRpmHzItem = new IOAnaItem("Drehzahlsensor (Hz)", 1));

    valueIn.append(inEncReelLeft = new IOEncItem("Encoder Trommel links"));
    valueIn.append(inEncReelRight = new IOEncItem("Encoder Trommel rechts"));

    connect(receiver, SIGNAL(ioDigiUpdate()), this, SLOT(ioDigiUpdate()));
    connect(receiver, SIGNAL(inAvUpdate()), this, SLOT(inAvUpdate()));
    connect(receiver, SIGNAL(inArUpdate()), this, SLOT(inArUpdate()));
    connect(receiver, SIGNAL(inEncUpdate()), this, SLOT(inEncUpdate()));
}

IOBinding::~IOBinding()
{
    QList<QObject *>::iterator i;
    for (i = digiIn.begin(); i != digiIn.end(); ++i) {
        delete ((IODigiItem *) (*i));
    }
    for (i = digiOut.begin(); i != digiOut.end(); ++i) {
        delete ((IODigiItem *) (*i));
    }
    for (i = valueIn.begin(); i != valueIn.end(); ++i) {
        delete ((IOAnaItem *) (*i));
    }
}

void IOBinding::ioDigiUpdate()
{
    quint32 inMask = receiver->getInDigi();
    quint32 outMask = receiver->getOutDigi();

    QList<QObject *>::iterator i;
    for (i = digiIn.begin(); i != digiIn.end(); ++i) {
        ((IODigiItem *) (*i))->update(inMask);
    }
    for (i = digiOut.begin(); i != digiOut.end(); ++i) {
        ((IODigiItem *) (*i))->update(outMask);
    }
}

void IOBinding::inAvUpdate()
{
    inVoltageBatItem->update(receiver->getInVoltageBat());
    inVoltageGenItem->update(receiver->getInVoltageGen());
    inVoltageTankLevelItem->update(receiver->getInVoltageTankLevel());
}

void IOBinding::inArUpdate()
{
    inHeadTempLeftOhmsItem->update(receiver->getInHeadTempLeftOhms());
    inHeadTempRightOhmsItem->update(receiver->getInHeadTempRightOhms());
    inMotorRpmHzItem->update(receiver->getInMotorRpmHz());
}

void IOBinding::inEncUpdate()
{
    inEncReelLeft->update(receiver->getInEncReelLeft());
    inEncReelRight->update(receiver->getInEncReelRight());
}
