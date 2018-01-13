#pragma once
#include <QObject>

namespace SBC{
class Transaction : public QObject
{
    Q_OBJECT
public:
    explicit Transaction(QObject *parent=0);

    Transaction(Transaction *data);

    QString getFrom() const;
    void setFrom(const QString &value);

    QString getTo() const;
    void setTo(const QString &value);

    double getAmount() const;
    void setAmount(double value);

private:
    QString from;
    QString to;
    double amount;

    explicit Transaction(const Transaction& rhs) = delete;
    Transaction& operator= (const Transaction& rhs) = delete;
};
}


