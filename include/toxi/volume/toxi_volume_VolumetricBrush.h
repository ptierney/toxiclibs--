
#pragma once

#include <cinder/Vector.h>

namespace toxi {
namespace volume {

class VolumetricSpace;

class VolumetricBrush {
public:
    VolumetricBrush(VolumetricSpace&);

    enum Mode {
        ADDITIVE = 1,
        MULTIPLY = 2,
        REPLACE = 3
    };

    void drawAtAbsolutePos(ci::Vec3f pos, float density);

    virtual void drawAtGridPos(float cx, float cy, float cz, float density) = 0;

    void setMode(Mode mode) { brushMode = mode; }

    virtual void setSize(float radius) = 0;

    void updateVoxel(int x, int y, int z, float cellVal);
    
protected:
    VolumetricSpace& volume;
    int cellRadiusX, cellRadiusY, cellRadiusZ;
    float stretchY, stretchZ;

    Mode brushMode;

};

}
}
