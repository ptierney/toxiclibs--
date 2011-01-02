
#include <cinder/AxisAlignedBox.h>

#include <toxi/geom/toxi_geom_AABB.h>

namespace toxi {
namespace geom {

AABB::AABB(const ci::Vec3f &pos, const ci::Vec3f &ext) :
    position(pos), extent(ext) {

    updateBounds();
}

bool AABB::intersectsTriangle(const ci::TriMesh& tri) {
    // tri should only contain one triangle

    ci::Vec3f tri_vert_1, tri_vert_2, tri_vert_3;

    tri.getTriangleVertices(0, &tri_vert_1, &tri_vert_2, &tri_vert_3);
 
    // use separating axis theorem to test overlap between triangle and box
    // need to test for overlap in these directions:
    //
    // 1) the {x,y,z}-directions (actually, since we use the AABB of the
    // triangle
    // we do not even need to test these)
    // 2) normal of the triangle
    // 3) crossproduct(edge from tri, {x,y,z}-directin)
    // this gives 3x3=9 more tests
    ci::Vec3f v0, v1, v2;
    ci::Vec3f normal, e0, e1, e2, f;

    // move everything so that the boxcenter is in (0,0,0)
    v0 = tri_vert_1 - position;
    v1 = tri_vert_2 - position;
    v2 = tri_vert_3 - position;

    // compute triangle edges
    e0 = v1 - v0;
    e1 = v2 - v1;
    e2 = v0 - v2;

    // test the 9 tests first (this was faster)
    f = get_vec_abs(e0);

    if (testAxis(e0.z, -e0.y, f.z, f.y, v0.y, v0.z, v2.y, v2.z, extent.y,
            extent.z)) {
        return false;
    }
    if (testAxis(-e0.z, e0.x, f.z, f.x, v0.x, v0.z, v2.x, v2.z, extent.x,
            extent.z)) {
        return false;
    }
    if (testAxis(e0.y, -e0.x, f.y, f.x, v1.x, v1.y, v2.x, v2.y, extent.x,
            extent.y)) {
        return false;
    }

    f = get_vec_abs(e1);
    if (testAxis(e1.z, -e1.y, f.z, f.y, v0.y, v0.z, v2.y, v2.z, extent.y,
            extent.z)) {
        return false;
    }
    if (testAxis(-e1.z, e1.x, f.z, f.x, v0.x, v0.z, v2.x, v2.z, extent.x,
            extent.z)) {
        return false;
    }
    if (testAxis(e1.y, -e1.x, f.y, f.x, v0.x, v0.y, v1.x, v1.y, extent.x,
            extent.y)) {
        return false;
    }

    f = get_vec_abs(e2);
    if (testAxis(e2.z, -e2.y, f.z, f.y, v0.y, v0.z, v1.y, v1.z, extent.y,
            extent.z)) {
        return false;
    }
    if (testAxis(-e2.z, e2.x, f.z, f.x, v0.x, v0.z, v1.x, v1.z, extent.x,
            extent.z)) {
        return false;
    }
    if (testAxis(e2.y, -e2.x, f.y, f.x, v1.x, v1.y, v2.x, v2.y, extent.x,
            extent.y)) {
        return false;
    }

    // first test overlap in the {x,y,z}-directions
    // find min, max of the triangle each direction, and test for overlap in
    // that direction -- this is equivalent to testing a minimal AABB around
    // the triangle against the AABB

    // test in X-direction
    if (min_3(v0.x, v1.x, v2.x) > extent.x
            || max_3(v0.x, v1.x, v2.x) < -extent.x) {
        return false;
    }

    // test in Y-direction
    if (min_3(v0.y, v1.y, v2.y) > extent.y
            || max_3(v0.y, v1.y, v2.y) < -extent.y) {
        return false;
    }

    // test in Z-direction
    if (min_3(v0.z, v1.z, v2.z) > extent.z
            || max_3(v0.z, v1.z, v2.z) < -extent.z) {
        return false;
    }

    // test if the box intersects the plane of the triangle
    // compute plane equation of triangle: normal*x+d=0
    normal = e0.cross(e1);
    float d = -normal.dot(v0);
    if (!planeBoxOverlap(normal, d, extent)) {
        return false;
    }
    return true;
}

bool AABB::testAxis(float a, float b, float fa, float fb, float va,
        float vb, float wa, float wb, float ea, float eb) {
    float p0 = a * va + b * vb;
    float p2 = a * wa + b * wb;
    float min, max;
    if (p0 < p2) {
        min = p0;
        max = p2;
    } else {
        min = p2;
        max = p0;
    }
    float rad = fa * ea + fb * eb;
    return (min > rad || max < -rad);
}

bool AABB::planeBoxOverlap(ci::Vec3f normal, float d, ci::Vec3f maxbox) {
    ci::Vec3f vmin = ci::Vec3f::zero();
    ci::Vec3f vmax = ci::Vec3f::zero();

    if (normal.x > 0.0f) {
        vmin.x = -maxbox.x;
        vmax.x = maxbox.x;
    } else {
        vmin.x = maxbox.x;
        vmax.x = -maxbox.x;
    }

    if (normal.y > 0.0f) {
        vmin.y = -maxbox.y;
        vmax.y = maxbox.y;
    } else {
        vmin.y = maxbox.y;
        vmax.y = -maxbox.y;
    }

    if (normal.z > 0.0f) {
        vmin.z = -maxbox.z;
        vmax.z = maxbox.z;
    } else {
        vmin.z = maxbox.z;
        vmax.z = -maxbox.z;
    }
    if (normal.dot(vmin) + d > 0.0f) {
        return false;
    }
    if (normal.dot(vmax) + d >= 0.0f) {
        return true;
    }
    return false;
}

void AABB::set(float x, float y, float z) {
    position.x = x;
    position.y = y;
    position.z = z;

    updateBounds();
}

void AABB::updateBounds() {
    min = position - extent;
    max = position + extent;
}

}
}