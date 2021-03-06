#include "normalgaussgenerator.h"

NormalGaussGenerator::NormalGaussGenerator(): omega(0), samplingTime(0), disperison(0) {}

void NormalGaussGenerator::setOmega(long double omega)
{
    this->disperison = omega/(2*pi);
    this->samplingTime = (2*pi)/omega;
    this->omega = omega;
    std::normal_distribution<long double> distribution(0.0L, 1.0);
}

void NormalGaussGenerator::setDispersion(long double dispersion)
{
    this->omega = disperison/(2*omega);
    this->samplingTime = 1/dispersion;
    this->disperison = dispersion;
    std::normal_distribution<long double> distribution(0.0L, 1.0);
}

void NormalGaussGenerator::setSamplingTime(long double samplingTime)
{
    this->omega = (2*pi)/samplingTime;
    this->disperison = 1/samplingTime;
    this->samplingTime = samplingTime;
    std::normal_distribution<long double> distribution(0.0L, 1.0);
}

long double NormalGaussGenerator::generate()
{
    return (long double) distribution(generator)*sqrt(disperison);
}
