#ifndef TQUATERNION_H
#define TQUATERNION_H
#include "LA.h"


class TQuaternion
{
protected:
double q;
TVector Q;

public:
    TQuaternion();
    TQuaternion(double l0, double l1, double l2, double l3);
    TQuaternion(double phi, const TVector& V);
    TQuaternion(const TQuaternion& rval);

    inline double scal() const { return q; }
    inline TVector vect() const { return Q; };

    TQuaternion& operator = (const TQuaternion& rval);
    TQuaternion operator - (const TQuaternion& rval) const;
    TQuaternion operator + (const TQuaternion& rval) const;
    TQuaternion operator * (const TQuaternion& rval) const;
    TQuaternion operator * (const TVector& rval) const;
    TQuaternion operator ! () const;
    double& operator [] (const int i);

    TQuaternion& norm();

    TQuaternion conj() const;

    TMatrix rotateMatrix() const;

    static TQuaternion KrilAngles(double phi, double psi, double theta);

};

#endif // TQUATERNION_H
