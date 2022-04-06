#include "statebinding.h"

StateBinding::StateBinding(const EcuReceiver *receiver, QObject *parent) : QObject(parent), receiver(receiver)
{
    connect(receiver, SIGNAL(errUpdate()), this, SLOT(errUpdate()));
    connect(receiver, SIGNAL(motorUpdate()), this, SLOT(motorUpdate()));
}

void StateBinding::errUpdate()
{

}

void StateBinding::motorUpdate()
{

}
