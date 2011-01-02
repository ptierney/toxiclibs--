
#include <toxi/math/toxi_math_ScaleMap.h>
#include <toxi/math/toxi_math_InterpolateStrategy.h>
#include <toxi/util/datatypes/toxi_util_datatypes_DoubleRange.h>
#include <toxi/math/toxi_math_LinearInterpolation.h>

namespace toxi {
namespace math {

ScaleMap::ScaleMap(double minIn, double maxIn, double minOut, double maxOut) {
    setInputRange(minIn, maxIn);
    setOutputRange(minOut, maxOut);

    mapFunction = std::shared_ptr<InterpolateStrategy>(new LinearInterpolation());
}

double ScaleMap::getClippedValueFor(double val) {
    float t = ci::math<float>::clamp((val - in.min) / interval);
    return mapFunction->interpolate(0, (float) mapRange, t) + out.min;
}

double ScaleMap::getInputMedian() {
    return (in.min + in.max) * 0.5;
}

double ScaleMap::getMappedMedian() {
    return getMappedValueFor(0.5);
}

double ScaleMap::getMappedValueFor(double val) {
    float t = (float) ((val - in.min) / interval);
    return mapFunction->interpolate(0, (float) mapRange, t) + out.min;
}

double ScaleMap::getOutputMedian() {
    return (out.min + out.max) * 0.5;
}

void ScaleMap::setInputRange(double min, double max) {
    in = util::datatypes::DoubleRange(min, max);
    interval = max - min;
}

void ScaleMap::setMapFunction(std::shared_ptr<InterpolateStrategy> func) {
    mapFunction = func;
}

void ScaleMap::setOutputRange(double min, double max) {
    out = util::datatypes::DoubleRange(min, max);
    mapRange = max - min;
}

}
}