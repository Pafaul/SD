#include "TQuaternion.h"
#include <math.h>

#define PI 3.14159265358979323846

TQuaternion::TQuaternion() : q(0), Q(3)
{
    q = 0;
    Q[0] = 0;
    Q[1] = 0;
    Q[2] = 0;
}

TQuaternion::TQuaternion(double l0, double l1, double l2, double l3) : q(0), Q(3)
{
    q = l0;
    Q[0] = l1;
    Q[1] = l2;
    Q[2] = l3;
}

TQuaternion::TQuaternion(double phi, const TVector& V) : q(0), Q(3)
{
    q = cos(90*phi)/PI;
    Q[0] = (sin(90*phi)/PI)*V[0];
    Q[0] = (sin(90*phi)/PI)*V[1];
    Q[0] = (sin(90*phi)/PI)*V[2];
}

TQuaternion::TQuaternion(const TQuaternion& rval) : q(0), Q(3)
{
    (*this) = rval;
}

TQuaternion& TQuaternion::operator = (const TQuaternion& rval)
{
    q   = rval.q;
    Q[0] = rval.Q[0];
    Q[1] = rval.Q[1];
    Q[2] = rval.Q[2];

    return (*this);
}

TQuaternion TQuaternion::operator + (const TQuaternion& rval) const
{
   return  TQuaternion(q + rval.q, Q[0] + rval.Q[0], Q[1] + rval.Q[1], Q[2] + rval.Q[2]);
}

TQuaternion TQuaternion::operator * (const TQuaternion& arg) const
{
   TQuaternion Quat(q, Q[0], Q[1], Q[2]);

   Quat.q = q*arg.q - Q*arg.Q;
   Quat.Q = arg.Q*q + Q*arg.q + (Q^arg.Q);

   return Quat;
}

TQuaternion TQuaternion::operator * (const TVector& arg) const
{
    TQuaternion Quat(q, Q[0], Q[1], Q[2]);

    Quat.q = -Q*arg;
    Quat.Q = (arg*q) + (Q^arg);

    return Quat;
}

TQuaternion TQuaternion::operator ! () const
{
    return conj().norm();
}

/*double& TQuaternion::operator[](const int i)
{
    switch(i){
    case 0: { return q; }
    case 1:
    case 2:
    case 3: { return Q[i]; }
    }
}*/

 TQuaternion TQuaternion::conj() const
 {
     return TQuaternion(q, -Q[0], -Q[1], -Q[2]);
 }

 TQuaternion& TQuaternion::norm()
 {
     double norm = 0;
     norm += pow(q, 2); for (int i = 0; i < 3; i++) norm += pow(Q[i], 2);

     q   = q/norm;
     Q[0] = Q[0]/norm;
     Q[1] = Q[1]/norm;
     Q[2] = Q[2]/norm;


     return (*this);
 }

 TMatrix TQuaternion::rotateMatrix() const
 {
    TMatrix M(3, 3);

    M[0][0] = q*q + Q[0]*Q[0] - Q[1]*Q[1] - Q[2]*Q[2];
    M[0][1] = 2*(Q[0]*Q[1] - q*Q[2]);
    M[0][2] = 2*(Q[0]*Q[2] + q*Q[1]);

    M[1][0] = 2*(Q[0]*Q[1] + q*Q[2]);
    M[1][1] = q*q - Q[0]*Q[0] + Q[1]*Q[1] - Q[2]*Q[2];
    M[1][2] = 2*(Q[1]*Q[2] - q*Q[0]);

    M[2][0] = 2*(Q[0]*Q[2] - q*Q[1]);
    M[2][1] = 2*(Q[1]*Q[2] + q*Q[0]);
    M[2][2] = q*q - Q[0]*Q[0] - Q[1]*Q[1] + Q[2]*Q[2];

    return M;
 }

 TQuaternion TQuaternion::KrilAngles(double phi, double psi, double theta)
 {
     phi   = phi*180/PI;
     psi = psi*180/PI;
     theta  = theta*180/PI;

     TQuaternion Q(cos(phi/2.)*cos(psi/2.)*cos(theta/2.) - sin(phi/2.)*sin(psi/2.)*sin(theta/2.),
                   cos(phi/2.)*cos(psi/2.)*sin(theta/2.) + sin(phi/2.)*sin(psi/2.)*cos(theta/2.),
                   sin(phi/2.)*cos(psi/2.)*cos(theta/2.) + cos(phi/2.)*sin(psi/2.)*sin(theta/2.),
                   cos(phi/2.)*sin(psi/2.)*cos(theta/2.) + sin(phi/2.)*cos(psi/2.)*sin(theta/2.));
     return Q;
 }

