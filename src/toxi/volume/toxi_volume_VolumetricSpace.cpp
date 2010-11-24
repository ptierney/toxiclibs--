
#include <cinder/CinderMath.h>

#include <toxi/volume/toxi_volume_VolumetricSpace.h>

namespace toxi {
namespace volume {

VolumetricSpace::VolumetricSpace(ci::Vec3f sc, int rX, int rY, int rZ)
    : resX(rX), resY(rY), resZ(rZ) {
    resX1 = resX - 1;
    resY1 = resY - 1;
    resZ1 = resZ - 1;

    sliceRes = resX * resY;
    numCells = sliceRes * resZ;
    setScale(scale);

    // TODO: check that toxi Vec3D init == cinder Vec3f init
    scale = ci::Vec3f();
    halfScale = ci::Vec3f();
    voxelSize = ci::Vec3f();
}

int VolumetricSpace::getIndexFor(int x, int y, int z) {
    x = ci::math<float>::clamp(x, 0, resX1);
    y = ci::math<float>::clamp(y, 0, resY1);
    z = ci::math<float>::clamp(z, 0, resZ1);

    return x + y * resX + z * sliceRes;
}

}
}
