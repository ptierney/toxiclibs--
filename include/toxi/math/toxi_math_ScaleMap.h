
/**
 * NOTE: THIS SHOULD USE TEMPLATES!!!
 */

#pragma once

#include <toxi/util/datatypes/toxi_util_datatypes_DoubleRange.h>
#include <toxi/math/toxi_math_InterpolateStrategy.h>
#include <toxi/math/toxi_math_LinearInterpolation.h>

namespace toxi {
namespace math {

class ScaleMap {
public:
    ScaleMap(double minIn, double maxIn, double minOut, double maxOut);

    double getClippedValueFor(double val);
    double getInputMedian();
    double getMappedMedian();
    double getMappedValueFor(double val);
    double getOutputMedian();

    util::datatypes::DoubleRange getInputRange() { return in; }
    util::datatypes::DoubleRange getOutputRange() { return out; }

    void setInputRange(double min, double max);
    void setOutputRange(double min, double max);
    void setMapFunction(std::shared_ptr<InterpolateStrategy> func);

protected:
    double interval;
    double mapRange;

    util::datatypes::DoubleRange in, out;

    std::shared_ptr<InterpolateStrategy> mapFunction;
};

}
}