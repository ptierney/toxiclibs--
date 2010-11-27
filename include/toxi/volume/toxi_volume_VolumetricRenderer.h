
#pragma once

#include <cinder/Color.h>

namespace toxi {
namespace volume {

class VolumetricSpace;

class VolumetricRenderer {
public:
    VolumetricRenderer();

    void drawVolume(VolumetricSpace& volume);

private:
    // colors, line weight,
    ci::Color side_color_;
    ci::Color line_color_;

};

}
}