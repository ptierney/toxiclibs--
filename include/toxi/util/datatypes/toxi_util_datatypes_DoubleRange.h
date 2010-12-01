
/**
 * NOTE: THIS SHOULD USE TEMPLATES!!!
 */

#pragma once

#include <vector>
#include <memory>

#include <cinder/Rand.h>

namespace toxi {
namespace util {
namespace datatypes {

class DoubleRange {
public:
    static std::shared_ptr<DoubleRange> fromSamples(const std::vector<double>& 
        samples);

    DoubleRange();
    DoubleRange(double min, double max);

    double setCurrent(double newCurrent);
    double getCurrent();

    double adjustCurrentBy(double val);
    double getMedian();
    bool isValueInRange(float val);

    double pickRandom();

    void seed();

    std::vector<double> toVector(double step);

    double min, max;
    double currValue;

    DoubleRange& operator=(const DoubleRange& r);

protected:
    ci::Rand random;
};

}
}
}