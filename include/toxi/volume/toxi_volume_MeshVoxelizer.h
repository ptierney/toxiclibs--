
#include <memory>

#pragma once

namespace toxi {
namespace volume {

class VolumetricSpace;
class VolumetricSpaceVector;

class MeshVoxelizer {
public:
    MeshVoxelizer(int res);
    MeshVoxelizer(int resX, int resY, int resZ);

    std::shared_ptr<VolumetricSpace> voxelizeMesh(ci::TriMesh& mesh);
    std::shared_ptr<VolumetricSpace> voxelizeMesh(ci::TriMesh& mesh, float iso);

    MeshVoxelizer& clear();

    std::shared_ptr<VolumetricSpace> getVolume() { return volume; }
    int getWallThickness() { return wallThickness; }

    MeshVoxelizer& setWallThickness(int w);

protected:
    void setVoxelAt(int x, int y, int z, float iso);
    std::shared_ptr<VolumetricSpace> solidifyVolume(
        std::shared_ptr<VolumetricSpaceVector> volume);

    std::shared_ptr<VolumetricSpace> volume;
    int wallThickness;

private:
    void init();

};

}
}