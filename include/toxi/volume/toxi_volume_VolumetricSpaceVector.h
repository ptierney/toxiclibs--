
// this is comparable to VolumetricSpaceArray in the original toxilibs

#include <vector>

#include <cinder/Vector.h>

#include <toxi/volume/toxi_volume_VolumetricSpace.h>

namespace toxi {
namespace volume {

class VolumetricSpaceVector : public VolumetricSpace {
public:
    VolumetricSpaceVector(ci::Vec3f scl, int rX, int rY, int rZ);

    void clear();

    const std::vector<float>& getData() const { return data; }

    float getVoxelAt(int index) { return data[index]; }

    float getVoxelAt(int x, int y, int z) { return data[x + y * resX + z * sliceRes]; }

    void setVolumeSidesTo(float density);

    void setVoxelAt(int index, float value);

    void setVoxelAt(int x, int y, int z, float value);

protected:
    std::vector<float> data;

};

}
}
