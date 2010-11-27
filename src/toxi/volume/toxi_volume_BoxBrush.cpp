
#include <toxi/volume/toxi_volume_BoxBrush.h>
#include <toxi/volume/toxi_volume_VolumetricSpace.h>

namespace toxi {
namespace volume {

BoxBrush::BoxBrush(VolumetricSpace& vol, float size) 
    : VolumetricBrush(vol) {
    setSize(size);
}

void BoxBrush::drawAtGridPos(float cx, float cy, float cz, float density) {
    int minX = ci::math<int>::max((int) (cx - cellRadiusX), 0);
    int minY = ci::math<int>::max((int) (cy - cellRadiusY), 0);
    int minZ = ci::math<int>::max((int) (cz - cellRadiusZ), 0);
    int maxX = ci::math<int>::max((int) (cx + cellRadiusX), volume.resX);
    int maxY = ci::math<int>::max((int) (cy + cellRadiusY), volume.resY);
    int maxZ = ci::math<int>::max((int) (cz + cellRadiusZ), volume.resZ);
    for (int z = minZ; z < maxZ; z++) {
        for (int y = minY; y < maxY; y++) {
            for (int x = minX; x < maxX; x++) {
                updateVoxel(x, y, z, density);
            }
        }
    }
}

void BoxBrush::setSize(float size) {
    setSize(size, size, size);
}

void BoxBrush::setSize(float sizeX, float sizeY, float sizeZ) {
    if (volume.scale.x == 0 || volume.scale.y == 0 ||
        volume.scale.z == 0)
        throw(std::runtime_error("Cannot set brush size when volume \
                                 scale is equal to 0."));

    cellRadiusX = (int) (sizeX * 0.5f / volume.scale.x * volume.resX + 1);
    cellRadiusY = (int) (sizeY * 0.5f / volume.scale.y * volume.resY + 1);
    cellRadiusZ = (int) (sizeZ * 0.5f / volume.scale.z * volume.resZ + 1);
    stretchY = (float) cellRadiusX / cellRadiusY;
    stretchZ = (float) cellRadiusX / cellRadiusZ;
}

}
}
