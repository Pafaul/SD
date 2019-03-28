#ifndef TVECTOR_H
#define TVECTOR_H

class TMatrix;

class TVector
{
protected:
    int n;
    long double* data;
public:
    TVector();
    TVector(int n);
    TVector(const TVector& rval);

    virtual ~TVector();

    inline int size() const { return n; }

    void resize(int n);

    inline long double& operator[](int i) const { return data[i]; }
    TVector& operator = (const TVector& rval);

    long double length() const;

    TVector operator - () const;
    TVector operator - (const long double rval) const;
    TVector operator - (const TVector& rval) const;

    TVector operator + (const long double rval) const;
    TVector operator + (const TVector& rval) const;

    TVector operator ^ (const TVector& rval) const;

    long double operator * (const TVector& rval) const;
    TVector operator * (const long double rval) const;

};

class TMatrix
{
protected:
    int n, m;

    long double **data;

public:

    TMatrix();

    TMatrix(int n, int m);

    TMatrix(const TMatrix &rval);

    TMatrix& operator = (const TMatrix& rval);

    virtual ~TMatrix();

    inline int rowCount() const { return n; }
    inline int colCount() const { return m; }

    void resize( int n, int m );

    TMatrix& swapRows(int i, int j);

    inline long double& operator() (int i, int j) { return data[i][j]; }
    //inline long double& operator() (int i, int j) const { return data[i][j]; }
    //inline const long double& operator() (int i, int j) { return data[i][j]; }
    inline const long double& operator() (int i, int j) const { return data[i][j]; }

    TMatrix operator - () const;
    TMatrix operator - (const TMatrix& rval) const;
    TMatrix operator + (const TMatrix& rval) const;
    TMatrix operator * (const long double rval) const;
    TMatrix operator * (const TMatrix& rval) const;
    TVector operator * (const TVector& rval) const;
    TMatrix operator !() const;

    TMatrix t() const;

    static TMatrix E(int n);

};



#endif // TVECTOR_H
