#ifndef NORMALGAUSSGENERATOR_H
#define NORMALGAUSSGENERATOR_H

#include <math.h>
#include <random>
#include <fstream>


class NormalGaussGenerator
{
protected:
    long double omega, samplingTime, disperison,
                pi = 3.14159265359L;
    std::default_random_engine generator;
    std::normal_distribution<> distribution;
public:
    NormalGaussGenerator();
    void setOmega(long double omega);
    long double getOmega(void) { return omega; }
    void setSamplingTime(long double samplingTime);
    long double getSamplingTime(void) { return samplingTime; }
    void setDispersion(long double dispersion);
    long double getDispersion(void) { return disperison; }
    long double generate(void);
};

#endif // NORMALGAUSSGENERATOR_H
