#ifndef WRARMADILLO_H
#define WRARMADILLO_H

#include <QObject>
#include <QString>
#include <QVariant>

#include <../MathLib/Armadiilo/armadillo-7.800.2/include/armadillo>

namespace armaHelper {
    arma::mat readMatrFromFile(QString path);
    QString MatrToStr(const arma::mat& mat);
    std::pair<double, double> getMinMax(const arma::mat& mat);
}

class MatrArm;

class armaHlpr : public QObject
{
    Q_OBJECT
public:
    armaHlpr(QObject *obj=nullptr):QObject(obj){}
    armaHlpr(const armaHlpr&){}
    ~armaHlpr(){}
    Q_INVOKABLE MatrArm* mkVec_e(int size, int index);
    Q_INVOKABLE MatrArm* mkMatr(int r, int c);
    Q_INVOKABLE MatrArm* mkVecByDig(int size, double val);
};

namespace mathHelper {
    inline double relErr(double cv, double ov){return fabs(cv-ov)/ov;}
    inline QString strRelErr(double cv, double ov);
}

class MatrArm : public QObject
{
    Q_OBJECT
public:
    MatrArm(QObject *obj=nullptr):QObject(obj){}
    //MatrArm(const arma::mat& data):QObject(nullptr), data_(data){}
    MatrArm(const MatrArm&rv);
    explicit MatrArm(const arma::mat&rv):data_(rv){}
    MatrArm& operator =(MatrArm rv);
    ~MatrArm(){}
    Q_INVOKABLE void assignMatr(MatrArm rv);
    Q_INVOKABLE void readMatrFromFile(QString path);
    Q_INVOKABLE static QObject* readMatrFromFile2(QString path);
    Q_INVOKABLE static MatrArm create(QString path);
    //
    Q_INVOKABLE QString toStr()const;
    Q_INVOKABLE QString print(QString header)const;
    Q_INVOKABLE double det()const{return arma::det(data_);}
    Q_INVOKABLE MatrArm inv()const;
    Q_INVOKABLE QObject* inv2()const;
    Q_INVOKABLE void inv(MatrArm rv)const;
    //
    Q_INVOKABLE QVariantList getDelta(MatrArm* rv)const;
    Q_INVOKABLE MatrArm* solve(MatrArm* rvec)const;
    Q_INVOKABLE void lu(MatrArm* l, MatrArm* u)const;
    Q_INVOKABLE void setElem(int r, int col, double val);
    Q_INVOKABLE double getElem(int i, int j){return data_(i,j);}
    Q_INVOKABLE int rows()const{return data_.n_rows;}
    Q_INVOKABLE void zeros(int r, int c){ data_.zeros(r, c);}
private:
    arma::mat data_;
};

//Q_DECLARE_METATYPE(MatrArm)
//Q_DECLARE_METATYPE(arma::mat)

#endif // WRARMADILLO_H


