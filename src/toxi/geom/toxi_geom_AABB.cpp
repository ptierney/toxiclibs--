
#include <toxi/geom/toxi_geom_AABB.h>

namespace toxi {
namespace geom {

AABB::AABB(const ci::Vec3f &aMin, const ci::Vec3f &aMax) :
    ci::AxisAlignedBox3f(aMin, aMax) {
}

bool AABB::intersectsTriangle(const ci::TriMesh& tri) {
    return false;
}

void AABB::set(float x, float y, float z) {

}

void AABB::updateBounds() {

}

}
}