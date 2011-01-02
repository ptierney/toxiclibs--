
#pragma once

#include <cinder/AxisAlignedBox.h>
#include <cinder/TriMesh.h>

namespace toxi {
namespace geom {

class AABB {
public:
    AABB(const ci::Vec3f &position, const ci::Vec3f &extent);

    bool intersectsTriangle(const ci::TriMesh& tri);

    // Updates the position of the box in space and updates bounds
    void set(float x, float y, float z);
    void updateBounds();

private:
    bool testAxis(float a, float b, float fa, float fb, float va,
        float vb, float wa, float wb, float ea, float eb);
    bool planeBoxOverlap(ci::Vec3f normal, float d, ci::Vec3f maxbox);

    ci::Vec3f get_vec_abs(const ci::Vec3f& v) {
        return ci::Vec3f(ci::math<float>::abs(v.x), 
            ci::math<float>::abs(v.y),
            ci::math<float>::abs(v.z));
    }

    float min_3(float a, float b, float c) {
        float m = ci::math<float>::min(a, b);
        return ci::math<float>::min(m, c);
    }

    float max_3(float a, float b, float c) {
        float m = ci::math<float>::max(a, b);
        return ci::math<float>::max(m, c);
    }

    ci::Vec3f position;
    ci::Vec3f extent;
    ci::Vec3f min, max;
    float x, y, z;
};

}
}