
#include <cinder/CinderMath.h>

#include <toxi/volume/toxi_volume_RoundBrush.h>
#include <toxi/volume/toxi_volume_VolumetricSpace.h>

namespace toxi {
namespace volume {

RoundBrush::RoundBrush(VolumetricSpace& vol, float r)
    : VolumetricBrush(vol) {
    setSize(r);
}

void RoundBrush::drawAtGridPos(float cx, float cy, float cz, float density) {
    int minX = ci::math<int>::max((int) (cx - cellRadiusX), 0);
    int minY = ci::math<int>::max((int) (cy - cellRadiusY), 0);
    int minZ = ci::math<int>::max((int) (cz - cellRadiusZ), 0);
    int maxX = ci::math<int>::max((int) (cx + cellRadiusX), volume.resX);
    int maxY = ci::math<int>::max((int) (cy + cellRadiusY), volume.resY);
    int maxZ = ci::math<int>::max((int) (cz + cellRadiusZ), volume.resZ);
    for (int z = minZ; z < maxZ; z++) {
        float dz = (z - cz) * stretchZ;
        dz *= dz;
        for (int y = minY; y < maxY; y++) {
            float dyz = (y - cy) * stretchY;
            dyz = dyz * dyz + dz;
            for (int x = minX; x < maxX; x++) {
                float dx = x - cx;
                float d = (float) ci::math<float>::sqrt(dx * dx + dyz);
                if (d <= cellRadiusX) {
                    float cellVal = (1 - d / cellRadiusX) * density;
                    updateVoxel(x, y, z, cellVal);
                }
            }
        }
    }
}

void RoundBrush::setSize(float r) {
    radius = r;
    cellRadiusX = (int) (radius / volume.scale.x * volume.resX + 1);
    cellRadiusY = (int) (radius / volume.scale.y * volume.resY + 1);
    cellRadiusZ = (int) (radius / volume.scale.z * volume.resZ + 1);
    stretchY = (float) cellRadiusX / cellRadiusY;
    stretchZ = (float) cellRadiusX / cellRadiusZ;
}

}
}