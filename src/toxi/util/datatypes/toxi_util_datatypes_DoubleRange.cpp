
#include <cinder/Rand.h>

#include <toxi/util/datatypes/toxi_util_datatypes_DoubleRange.h>

namespace toxi {
namespace util {
namespace datatypes {

std::shared_ptr<DoubleRange> DoubleRange::fromSamples(const std::vector<double>& 
    samples) {
    if (samples.empty())
        return std::shared_ptr<DoubleRange>(new DoubleRange());

    double min = samples[0];
    double max = samples[0];

    std::for_each(samples.begin(), samples.end(), [&] (double d) {
        min = ci::math<double>::min(min, d);
        max = ci::math<double>::max(max, d);
    } );

    return std::shared_ptr<DoubleRange>(new DoubleRange(min, max));
}


DoubleRange::DoubleRange() {
    min = 0.0;
    max = 1.0;
    currValue = min;
}

DoubleRange::DoubleRange(double inmin, double inmax) {
    min = inmin;
    max = inmax;
    currValue = min;
}

double DoubleRange::adjustCurrentBy(double val) {
    return setCurrent(currValue + val);
}

double DoubleRange::setCurrent(double val) {
    currValue = ci::math<double>::clamp(val, min, max);
    return currValue;
}

double DoubleRange::getCurrent() {
    return currValue;
}

double DoubleRange::getMedian() {
    return (min + max) * 0.5f;
}

bool DoubleRange::isValueInRange(float val) {
    return val >= min && val <= max;
}

double DoubleRange::pickRandom() {
    currValue = random.randFloat(min, max);
    return currValue;
}

void DoubleRange::seed() {
    random.randomize();
}

std::vector<double> DoubleRange::toVector(double step) {
    std::vector<double> range;
    double v = min;
    while (v < max) {
        range.push_back(v);
        v += step;
    }
    return range;
}

// err, this might want to be rethought. Maybe force people to use ptrs
// that would be more java-like
DoubleRange& DoubleRange::operator=(const DoubleRange& r)  {
    min = r.min;
    max = r.max;
    currValue = r.currValue;

    return *this;
}

}
}
}