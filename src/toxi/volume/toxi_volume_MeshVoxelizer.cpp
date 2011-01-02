
#include <cinder/TriMesh.h>
#include <cinder/AxisAlignedBox.h>

#include <toxi/volume/toxi_volume_MeshVoxelizer.h>
#include <toxi/volume/toxi_volume_VolumetricSpace.h>
#include <toxi/volume/toxi_volume_VolumetricSpaceVector.h>
#include <toxi/math/toxi_math_ScaleMap.h>
#include <toxi/geom/toxi_geom_AABB.h>

namespace toxi {
namespace volume {

MeshVoxelizer::MeshVoxelizer(int res) {
    volume = std::shared_ptr<VolumetricSpace>(new VolumetricSpaceVector(
        ci::Vec3f::one() * 100.0f, res, res, res));

    init();
}

MeshVoxelizer::MeshVoxelizer(int resX, int resY, int resZ) {
    volume = std::shared_ptr<VolumetricSpace>(new VolumetricSpaceVector(
        ci::Vec3f::one() * 100.0f, resX, resY, resZ));

    init();
}

void MeshVoxelizer::init() {
    wallThickness = 0;
}

MeshVoxelizer& MeshVoxelizer::clear() {
    volume->clear();
    return *this;
}

void MeshVoxelizer::setVoxelAt(int x, int y, int z, float iso) {
    int mix = ci::math<int>::max(x - wallThickness, 0);
    int miy = ci::math<int>::max(y - wallThickness, 0);
    int miz = ci::math<int>::max(z - wallThickness, 0);
    int max = ci::math<int>::max(x + wallThickness, volume->resX1);
    int may = ci::math<int>::max(y + wallThickness, volume->resY1);
    int maz = ci::math<int>::max(z + wallThickness, volume->resZ1);
    for (z = miz; z <= maz; z++) {
        for (y = miy; y <= may; y++) {
            for (x = mix; x <= max; x++) {
                volume->setVoxelAt(x, y, z, iso);
            }
        }
    }
}

MeshVoxelizer& MeshVoxelizer::setWallThickness(int w) {
    wallThickness = w;
    return *this;
}

std::shared_ptr<VolumetricSpace> MeshVoxelizer::solidifyVolume(
    std::shared_ptr<VolumetricSpaceVector> volume) {
    for (int z = 0; z < volume->resZ; z++) {
        for (int y = 0; y < volume->resY; y++) {
            bool isFilled = false;
            int startX = 0;
            for (int x = 0; x < volume->resX; x++) {
                float val = volume->getVoxelAt(x, y, z);
                if (val > 0) {
                    if (!isFilled) {
                        startX = x;
                        isFilled = true;
                    } else {
                        for (int i = startX; i <= x; i++) {
                            volume->setVoxelAt(i, y, z, 1);
                        }
                        isFilled = false;
                    }
                }
            }
        }
    }
    return volume;
}

std::shared_ptr<VolumetricSpace> MeshVoxelizer::voxelizeMesh(ci::TriMesh& mesh) {
    return voxelizeMesh(mesh, 1.f);
}

std::shared_ptr<VolumetricSpace> MeshVoxelizer::voxelizeMesh(ci::TriMesh& mesh, float iso) {
    ci::AxisAlignedBox3f box = mesh.calcBoundingBox(); 
    ci::Vec3f bmin = box.getMin();
    ci::Vec3f bmax = box.getMax();
    toxi::math::ScaleMap wx(bmin.x, bmax.x, 1, volume->resX - 2);
    toxi::math::ScaleMap wy(bmin.y, bmax.y, 1, volume->resY - 2);
    toxi::math::ScaleMap wz(bmin.z, bmax.z, 1, volume->resZ - 2);
    toxi::math::ScaleMap gx(1, volume->resX - 2, bmin.x, bmax.x);
    toxi::math::ScaleMap gy(1, volume->resY - 2, bmin.y, bmax.y);
    toxi::math::ScaleMap gz(1, volume->resZ - 2, bmin.z, bmax.z);

    // don't need to scale by 2 since cinder aabb returns full extent
    volume->setScale(box.getSize());

    ci::TriMesh tri = ci::TriMesh();
    ci::Vec3f a, b, c;

    // set the position and the extent
    geom::AABB voxel = geom::AABB(ci::Vec3f::zero(), 
        volume->voxelSize * 0.5f);

    for (int i = 0; i < mesh.getNumTriangles(); ++i) {
        mesh.getTriangleVertices(i, &a, &b, &c);

        tri.clear();
        tri.appendVertex(a);
        tri.appendVertex(b);
        tri.appendVertex(c);
        tri.appendTriangle(0, 1, 2);

        ci::AxisAlignedBox3f bounds = tri.calcBoundingBox();

        ci::Vec3f min = bounds.getMin();
        ci::Vec3f max = bounds.getMax();

        min = ci::Vec3f((int) wx.getClippedValueFor(min.x),
                        (int) wy.getClippedValueFor(min.y),
                        (int) wz.getClippedValueFor(min.z));
        max = ci::Vec3f((int) wx.getClippedValueFor(max.x),
                        (int) wy.getClippedValueFor(max.y),
                        (int) wz.getClippedValueFor(max.z));

        for (int z = (int) min.z; z <= max.z; z++) {
            for (int y = (int) min.y; y <= max.y; y++) {
                for (int x = (int) min.x; x <= max.x; x++) {
                    if (x < volume->resX1 && y < volume->resY1
                            && z < volume->resZ1) {
                        voxel.set((float) gx.getClippedValueFor(x),
                                (float) gy.getClippedValueFor(y),
                                (float) gz.getClippedValueFor(z));
                        if (voxel.intersectsTriangle(tri)) {
                            setVoxelAt(x, y, z, iso);
                        }
                    }
                }
            }
        }
    }
    return volume;
}

}
}