#include <wrarmadillo.h>

using namespace std;
using namespace arma;

arma::mat armaHelper::readMatrFromFile(QString path)
{
    mat A;
    A.load(path.toStdString(), raw_ascii);
    return A;
}

QString armaHelper::MatrToStr(const arma::mat &mat)
{
    stringstream ss;
    ss << mat;
    const string s = ss.str();
    return QString::fromStdString(s);
}

MatrArm::MatrArm(const MatrArm &rv):data_(rv.data_){}

MatrArm &MatrArm::operator =(MatrArm rv)
{
    if (this != &rv) {
        this->data_ = rv.data_;
    }
    return *this;
}

void MatrArm::assignMatr(MatrArm rv)
{
    data_ = rv.data_;
}

void MatrArm::readMatrFromFile(QString path)
{
    data_ = armaHelper::readMatrFromFile(path);
}

QObject *MatrArm::readMatrFromFile2(QString path)
{
    MatrArm* a= new MatrArm();
    a->readMatrFromFile(path);
    return (QObject*)a;
}

MatrArm MatrArm::create(QString path)
{
    MatrArm res;
    res.readMatrFromFile(path);
    return res;
}

QString MatrArm::toStr()const
{
    return armaHelper::MatrToStr(data_);
}

QString MatrArm::print(QString header) const
{
    return QString("%1\n%2").arg(header).arg(toStr());
}

MatrArm MatrArm::inv() const
{
    return MatrArm(arma::inv(data_));
}

QObject *MatrArm::inv2() const
{
    MatrArm* a= new MatrArm(inv());
    return (QObject*)a;
}

void MatrArm::inv(MatrArm rv) const
{
    rv = rv.inv();
}

QVariantList MatrArm::getDelta(MatrArm* rv) const
{
    auto pair = armaHelper::getMinMax(data_-rv->data_);
    //
    QVariantList res;
    res << pair.first;
    res << pair.second;
    //
    return res;
}

MatrArm *MatrArm::solve(MatrArm *rvec) const
{
    Q_UNUSED(rvec)
    arma::mat x = arma::solve(this->data_, rvec->data_);
    MatrArm * res = new MatrArm(x);
    return res;
}

void MatrArm::lu(MatrArm *l, MatrArm *u) const
{
    arma::mat dl, du;
    arma::lu(dl,du, this->data_);
    l->data_ = dl;
    u->data_ = du;
}

void MatrArm::setElem(int r, int col, double val)
{
    this->data_(r, col) = val;
}

QString mathHelper::strRelErr(double cv, double ov)
{
    auto del = fabs(cv-ov)/fabs(ov);
    return QString("%1").arg(del);
}

std::pair<double, double> armaHelper::getMinMax(const arma::mat &mat)
{
    arma::mat tmp(mat);
    tmp.for_each(
        [](mat::elem_type& val) { val = fabs(val); }
    );

    std::pair<double, double> res{tmp.min(), tmp.max()};
    return res;
}

MatrArm *armaHlpr::mkVec_e(int size, int index)
{
    arma::Col<double> data(size, fill::zeros);
    data(index) = 1;
    MatrArm * res = new MatrArm(data);
    //
    return res;
}

MatrArm *armaHlpr::mkMatr(int r, int c)
{
    arma::Mat<double> data(r, c);
    data.zeros();
    MatrArm * res = new MatrArm(data);
    //
    return res;
}

MatrArm *armaHlpr::mkVecByDig(int size, double val)
{
    arma::Col<double> data(size);
    data.fill(val);
    MatrArm * res = new MatrArm(data);
    //
    return res;
}
