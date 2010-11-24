
#pragma once

#include <cinder/Vector.h>

namespace toxi {
namespace volume {

class VolumetricSpace {
public:

    VolumetricSpace(ci::Vec3f sc, int rX, int rY, int rZ);

    virtual void clear() = 0;

    virtual void closeSides() { 
        throw(std::runtime_error(
            "This VolumetricSpace implementation does not support closeSides()"));
    }

    int getIndexFor(int x, int y, int z);

    ci::Vec3f getResolution() { return ci::Vec3f(resX, resY, resZ); }
    
    ci::Vec3f getScale() { return scale; }

    virtual float getVoxelAt(int index) = 0;

    void setScale(ci::Vec3f scl) {
        scale = scl;
        halfScale = scl * 0.5f;
        voxelSize = ci::Vec3f(scl.x / resX, scl.y / resY, scl.z / resZ);
    }

    virtual void setVoxelAt(int index, float value) {
        throw(std::runtime_error(
            "This VolumetricSpace implementation does not support setVoxelAt()"));
    }

    virtual void setVoxelAt(int x, int y, int z, float value) {
        throw(std::runtime_error(
            "This VolumetricSpace implementation does not support setVoxelAt()"));
    }

    int resX, resY, resZ;

    int resX1, resY1, resZ1;

    int sliceRes;

    ci::Vec3f scale;
    ci::Vec3f halfScale;
    ci::Vec3f voxelSize;

    int numCells;
};

}
}
