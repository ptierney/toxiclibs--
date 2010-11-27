
#include <cinder/gl/gl.h>
#include <cinder/Color.h>

#include <toxi/volume/toxi_volume_VolumetricRenderer.h>
#include <toxi/volume/toxi_volume_VolumetricSpace.h>

namespace toxi {
namespace volume {

VolumetricRenderer::VolumetricRenderer() {
    side_color_ = ci::Color(1.0f, 0.0f, 0.0f);
    line_color_ = ci::Color::white();
}

void VolumetricRenderer::drawVolume(VolumetricSpace& volume) {
    // draw all the sectors as cubes
    ci::Vec3<int> resolution = volume.getResolution();
    ci::Vec3f scale = volume.getScale();

    int resX = volume.resX;
    int resY = volume.resY;
    int resZ = volume.resZ;
    int sliceRes = volume.sliceRes;

    float alpha; 

   ci::Vec3f size = ci::Vec3f(scale.x / volume.resX1, scale.y / volume.resY1,
        scale.z / volume.resZ1);
    ci::Vec3f start_center = -volume.halfScale;
    ci::Vec3f center = start_center;

    for (int i = 0; i < resX; ++i) {
        center.y = start_center.y;
        for (int j = 0; j < resY; ++j) {
            center.z = start_center.z;
            for (int k = 0; k < resZ; ++k) {
                alpha = volume.getVoxelAt(volume.getIndexFor(i, j, k));

                if (alpha > 0.001) {
                    glColor4f(side_color_.r, side_color_.g, side_color_.b, alpha);
                    ci::gl::drawCube(center, size);
                }

                center += ci::Vec3f(0.f, 0.f, size.z);
            }
            center += ci::Vec3f(0.f, size.y, 0.f);
        }
        center += ci::Vec3f(size.x, 0.f, 0.f);
    }
}

}
}