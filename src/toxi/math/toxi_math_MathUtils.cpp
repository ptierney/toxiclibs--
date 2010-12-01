
#include <toxi/math/toxi_math_MathUtils.h>

namespace toxi {
namespace math {

float MathUtils::clipNormalized(float a) {
    if (a < 0) {
        return 0;
    } else if (a > 1) {
        return 1;
    }
    return a;
}

}
}