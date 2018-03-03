#pragma once
#include <QObject>
#include <QtCore>
#include <QCryptographicHash>

namespace SBC{
class Transaction : public QObject
{
    Q_OBJECT
public:
    Transaction(QString pFrom, QString pTo, double pAmount);
    Transaction(QString pFrom, QString pTo, double pAmount, QByteArray pHash);
    explicit Transaction(QObject *parent=0);    

    QString getFrom() const;
    void setFrom(const QString &value);

    QString getTo() const;
    void setTo(const QString &value);

    double getAmount() const;
    void setAmount(double value);

    QByteArray calculateHash();

    QByteArray getTxHash() const;

    // read from stream only
    void setTxHash(const QByteArray &value);

private:
    QString from;
    QString to;
    double amount;
    QByteArray hash;

    explicit Transaction(const Transaction& rhs) = delete;
    Transaction& operator= (const Transaction& rhs) = delete;
};

}


