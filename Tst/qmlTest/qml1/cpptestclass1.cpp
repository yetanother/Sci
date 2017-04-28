#include "cpptestclass1.h"
#include "wrarmadillo.h"

using namespace std;

QString cppTestClass1::readMatr1(QString path)
{
    auto m = armaHelper::readMatrFromFile(path);
    return armaHelper::MatrToStr(m);
}

cppTestClass1::cppTestClass1()
    : type_("initialValue")
{

}

void cppTestClass1::setType(QString val)
{
    if(val!=type_) {
        type_ = val;
        emit typeChanged();
    }
}
