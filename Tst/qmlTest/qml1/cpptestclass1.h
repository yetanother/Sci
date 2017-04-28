#ifndef CPPTESTCLASS1_H
#define CPPTESTCLASS1_H

#include <QObject>

class cppTestClass1 : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
public:
    Q_INVOKABLE QString readMatr1(QString path);
    //
    cppTestClass1();
    QString type()const{return type_;}
    void setType(QString val);
signals:
    void typeChanged();

private:
    QString type_;

};

#endif // CPPTESTCLASS1_H
