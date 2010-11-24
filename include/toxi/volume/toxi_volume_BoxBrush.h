
#pragma once

#include <toxi/volume/toxi_volume_VolumetricBrush.h>

namespace toxi {
namespace volume {

class VolumetricSpace;

class BoxBrush : public VolumetricBrush {
public:
    BoxBrush(VolumetricSpace&, float size);

    void drawAtGridPos(float cx, float cy, float cz, float density);

    void setSize(float size);

    void setSize(float sizeX, float sizeY, float sizeZ);
};

}
}