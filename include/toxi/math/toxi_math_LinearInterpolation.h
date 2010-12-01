
#pragma once

#include <toxi/math/toxi_math_InterpolateStrategy.h>

namespace toxi {
namespace math {

class LinearInterpolation : public InterpolateStrategy {
public:
    virtual ~LinearInterpolation() { }

    // Hello Internet! Any thoughts on whether this can be inlined?
    // I'm placing it here for code readability, but can inlined virtual methods cause 
    // problems? 
    // http://www.parashift.com/c%2B%2B-faq-lite/value-vs-ref-semantics.html#faq-31.6

    virtual float interpolate(float a, float b, float f) { return a + (b - a) * f; }
};

}
}