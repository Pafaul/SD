
#pragma once
// Подключение заголовочного файла библиотеки векторных и матричных операций

#include "tvector.h"
#include "tmatrix.h"
//---------------------------------------------------------------------------

// Опережающие декларации
class TModel;

class TIntegrator
{
    protected:
        // Максимальная погрешность на шаге
        long double Eps;

    public:
		// Базовый конструктор
        TIntegrator() : Eps( 1e-16 ) {}
        inline void setPrecision( long double Eps ) { this->Eps = Eps; }
        inline long double getPrecision() const { return Eps; }
		// Абстрактный метод, реализующий процедуру численного интегрирования и возвращающий глобальную погрешность вычислений
        virtual long double Run(TModel* Model) = 0;
};

//---------------------------------------------------------------------------

class TDormandPrinceIntegrator : public TIntegrator
{
    private:
        // Коэффициенты a,b,c
        static const long double c[7], a[7][6], b1[7], b2[7];
        // Коэффициенты K[i]
        TVector K[7];
		// Машинный нуль
		long double u;
    public:
        TDormandPrinceIntegrator();
        long double Run(TModel* Model);
};

//---------------------------------------------------------------------------


