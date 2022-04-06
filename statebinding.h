#ifndef STATEBINDING_H
#define STATEBINDING_H

#include <QObject>

#include "ecureceiver.h"

class StateBinding : public QObject
{
    Q_OBJECT
public:
    explicit StateBinding(const EcuReceiver *receiver, QObject *parent = 0);

private:
    const EcuReceiver *receiver;

private slots:
    void errUpdate();
    void motorUpdate();

};

#endif // STATEBINDING_H
