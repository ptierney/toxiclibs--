
#include <toxi/volume/toxi_volume_VolumetricSpaceVector.h>

namespace toxi {
namespace volume {

VolumetricSpaceVector::VolumetricSpaceVector(ci::Vec3f scl, int rX_, int rY_, int rZ_)
    : VolumetricSpace(scl, rX_, rY_, rZ_), rX(rX_), rY(rY_), rZ(rZ_) {
    data.resize(rX * rY * rZ);
}

void VolumetricSpaceVector::clear() {
    data.clear();
    data.resize(rX * rY * rZ);
}

void VolumetricSpaceVector::setVolumeSidesTo(float density) {
    // close left/right & top/bottom
    for (int z = resZ1; z > 0; z--) {
        int sliceIdx = z * sliceRes;
        int sliceIdxAlt = sliceIdx + resY1 * resX;
        for (int x = 0; x < resX; x++) {
            data[sliceIdx + x] = density;
            data[sliceIdxAlt + x] = density;
        }
        for (int y = 1, rowIndex = sliceIdx + resX; y < resY1; y++) {
            data[rowIndex] = density;
            data[rowIndex + resX1] = density;
            rowIndex += resX;
        }
    }
    // front & back
    for (int i = 0, front = 0, back = resZ1 * sliceRes; i < sliceRes; i++) {
        data[front++] = density;
        data[back++] = density;
    }
}

void VolumetricSpaceVector::setVoxelAt(int index, float value) {
    if (index >= 0 && index < data.size()) {
        data[index] = value;
    }
}

void VolumetricSpaceVector::setVoxelAt(int x, int y, int z, float value) {
    int idx = x + y * resX + z * sliceRes;
    if (idx >= 0 && idx < data.size()) {
        data[idx] = value;
    }
}

}
}