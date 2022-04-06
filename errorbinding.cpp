#include "errorbinding.h"

const QString ErrorBindingItem::COLOR_OFF = "gray";
const QString ErrorBindingItem::COLOR_ACTIVE = "red";
const QString ErrorBindingItem::COLOR_NACK = "yellow";

void ErrorBindingItem::update(quint16 activeMask, quint16 nackMask) {
    bool newActive = !!(activeMask & mask);
    bool newNack = !!(nackMask & mask);
    bool changed = active != newActive || nack != newNack;
    active = newActive;
    nack = newNack;
    if (changed) {
        emit stateChanged();
    }
}

ErrorBinding::ErrorBinding(const EcuReceiver *receiver, QObject *parent) : QObject(parent), receiver(receiver)
{
    errors.append(new ErrorBindingItem("Not-Aus betätigt", EcuReceiver::ERROR_MASK_EMCY_STOP));
    errors.append(new ErrorBindingItem("Öldruck fehlerhaft", EcuReceiver::ERROR_MASK_OIL_PRESS));
    errors.append(new ErrorBindingItem("Fehler Batteriespannung", EcuReceiver::ERROR_MASK_VOLT_BAT));
    errors.append(new ErrorBindingItem("Fehler Generatorspannung", EcuReceiver::ERROR_MASK_VOLT_GEN));
    errors.append(new ErrorBindingItem("Fehler Tanksensor", EcuReceiver::ERROR_MASK_TANK));
    errors.append(new ErrorBindingItem("Fehler Motortemperatur", EcuReceiver::ERROR_MASK_TEMP));
    errors.append(new ErrorBindingItem("Getriebe eingelegt", EcuReceiver::ERROR_MASK_GEAR_ENG));
    errors.append(new ErrorBindingItem("Wartungsmodus aktiv", EcuReceiver::ERROR_MASK_SERVICE));

    connect(receiver, SIGNAL(errUpdate()), this, SLOT(errUpdate()));
}


ErrorBinding::~ErrorBinding()
{
    QList<QObject *>::iterator i;
    for (i = errors.begin(); i != errors.end(); ++i) {
        delete ((ErrorBindingItem *) (*i));
    }
}

void ErrorBinding::errUpdate() {
    quint16 activeMask = receiver->getErrMaskActive();
    quint16 nackMask = receiver->getErrMaskNack();

    QList<QObject *>::iterator i;
    for (i = errors.begin(); i != errors.end(); ++i) {
        ((ErrorBindingItem *) (*i))->update(activeMask, nackMask);
    }
}
