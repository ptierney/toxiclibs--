
#include <cinder/CinderMath.h>

#include <toxi/volume/toxi_volume_VolumetricBrush.h>
#include <toxi/volume/toxi_volume_VolumetricSpace.h>

namespace toxi {
namespace volume {

VolumetricBrush::VolumetricBrush(VolumetricSpace& vol) 
    : volume(vol) { 
    brushMode = ADDITIVE;
}

void VolumetricBrush::drawAtAbsolutePos(ci::Vec3f pos, float density) {
    float cx = ci::math<float>::clamp((pos.x + volume.halfScale.x) / volume.scale.x
                    * volume.resX1, 0, volume.resX1);
    float cy = ci::math<float>::clamp((pos.y + volume.halfScale.y) / volume.scale.y
                    * volume.resY1, 0, volume.resY1);
    float cz = ci::math<float>::clamp((pos.z + volume.halfScale.z) / volume.scale.z
                    * volume.resZ1, 0, volume.resZ1);

    drawAtGridPos(cx, cy, cz, density);
}

void VolumetricBrush::updateVoxel(int x, int y, int z, float cellVal) {
    int idx = volume.getIndexFor(x, y, z);
    switch (brushMode) {
        case ADDITIVE:
        default:
            volume.setVoxelAt(idx, volume.getVoxelAt(idx) + cellVal);
            break;
        case MULTIPLY:
            volume.setVoxelAt(idx, volume.getVoxelAt(idx) * cellVal);
            break;
        case REPLACE:
            volume.setVoxelAt(idx, cellVal);
            break;
    }
}

}
}
