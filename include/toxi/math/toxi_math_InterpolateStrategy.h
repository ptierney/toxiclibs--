
#pragma once

namespace toxi {
namespace math {

class InterpolateStrategy {
public:
    virtual ~InterpolateStrategy() { }

    virtual float interpolate(float a, float b, float f) = 0;
};

}
}