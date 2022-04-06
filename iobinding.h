#ifndef IOBINDING_H
#define IOBINDING_H

#include <QObject>
#include <QList>
#include <QVariant>

#include "ecureceiver.h"

class IOGenericItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString caption READ getCaption NOTIFY captionChanged)
public:
    explicit IOGenericItem(const QString &caption, QObject *parent = 0):
        QObject(parent), caption(caption) { emit captionChanged(); }
    const QString &getCaption() { return caption; }
private:
    QString caption;
signals:
    void captionChanged();
};

class IODigiItem : public IOGenericItem
{
    Q_OBJECT
    Q_PROPERTY(QString color READ getColor NOTIFY stateChanged)
public:
    explicit IODigiItem(const QString &caption, quint32 mask, QObject *parent = 0):
        IOGenericItem(caption, parent), mask(mask), state(false) { }
    void update(quint32 ioMask);
    const QString &getColor() { return state ? COLOR_ON : COLOR_OFF; }

private:
    static const QString COLOR_OFF;
    static const QString COLOR_ON;
    quint32 mask;
    bool state;
signals:
    void stateChanged();
};

class IOAnaItem : public IOGenericItem
{
    Q_OBJECT
    Q_PROPERTY(QString value READ getValue NOTIFY valueChanged)
public:
    explicit IOAnaItem(const QString &caption, int prec, QObject *parent = 0):
        IOGenericItem(caption, parent), prec(prec), value(0.0) { }
    void update(double newValue);
    QString getValue() { return QString::number(value, 'f', prec); }
private:
    int prec;
    double value;
signals:
    void valueChanged();
};

class IOEncItem : public IOGenericItem
{
    Q_OBJECT
    Q_PROPERTY(QString value READ getValue NOTIFY valueChanged)
public:
    explicit IOEncItem(const QString &caption, QObject *parent = 0):
        IOGenericItem(caption, parent), value(0) { }
    void update(qint32 newValue);
    QString getValue() { return QString::number(value); }
private:
    qint32 value;
signals:
    void valueChanged();
};


class IOBinding : public QObject
{
    Q_OBJECT
public:
    explicit IOBinding(const EcuReceiver *receiver, QObject *parent = 0);
    ~IOBinding();

    QVariant getDigiInModel() { return QVariant::fromValue(digiIn); }
    QVariant getDigiOutModel() { return QVariant::fromValue(digiOut); }
    QVariant getValueInModel() { return QVariant::fromValue(valueIn); }

private:
    IOAnaItem *inVoltageBatItem;
    IOAnaItem *inVoltageGenItem;
    IOAnaItem *inVoltageTankLevelItem;

    IOAnaItem *inHeadTempLeftOhmsItem;
    IOAnaItem *inHeadTempRightOhmsItem;
    IOAnaItem *inMotorRpmHzItem;

    IOEncItem *inEncReelLeft;
    IOEncItem *inEncReelRight;

    const EcuReceiver *receiver;
    QList<QObject*> digiIn;
    QList<QObject*> digiOut;
    QList<QObject*> valueIn;

private slots:
    void ioDigiUpdate();
    void inAvUpdate();
    void inArUpdate();
    void inEncUpdate();

};

#endif // IOBINDING_H
