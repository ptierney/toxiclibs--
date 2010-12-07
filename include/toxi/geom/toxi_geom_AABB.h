
#pragma once

#include <cinder/AxisAlignedBox.h>
#include <cinder/TriMesh.h>

namespace toxi {
namespace geom {

class AABB : public ci::AxisAlignedBox3f {
public:
    AABB(const ci::Vec3f &aMin, const ci::Vec3f &aMax);

    bool intersectsTriangle(const ci::TriMesh& tri);

    // Updates the position of the box in space and updates bounds
    void set(float x, float y, float z);
    void updateBounds();
};

}
}