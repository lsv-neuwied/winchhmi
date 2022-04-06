#ifndef ERRORBINDING_H
#define ERRORBINDING_H

#include <QObject>
#include <QVariant>
#include <QList>

#include "ecureceiver.h"

class ErrorBindingItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString caption READ getCaption NOTIFY captionChanged)
    Q_PROPERTY(QString color READ getColor NOTIFY stateChanged)

public:
    explicit ErrorBindingItem(const QString &caption, quint16 mask, QObject *parent = 0):
        QObject(parent),
        caption(caption),
        mask(mask),
        active(false),
        nack(false)
    {
        emit captionChanged();
    }

    void update(quint16 activeMask, quint16 nackMask);

    const QString &getCaption() { return caption; }
    const QString &getColor() { return active ? COLOR_ACTIVE : (nack ? COLOR_NACK : COLOR_OFF); }

private:
    static const QString COLOR_OFF;
    static const QString COLOR_ACTIVE;
    static const QString COLOR_NACK;

    QString caption;
    quint16 mask;
    bool active;
    bool nack;

signals:
    void captionChanged();
    void stateChanged();
};

class ErrorBinding : public QObject
{
    Q_OBJECT
public:
    explicit ErrorBinding(const EcuReceiver *receiver, QObject *parent = 0);
    ~ErrorBinding();

    QVariant getModel() { return QVariant::fromValue(errors); }

private:
    const EcuReceiver *receiver;
    QList<QObject*> errors;

private slots:
    void errUpdate();

};

#endif // ERRORBINDING_H
