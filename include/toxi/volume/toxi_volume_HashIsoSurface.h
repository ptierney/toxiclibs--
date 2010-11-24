
#pragma once

#include <map>
#include <vector>

#include <cinder/Vector.h>
#include <cinder/TriMesh.h>

#include <toxi/volume/toxi_volume_IsoSurface.h>

namespace toxi {
namespace volume {

class VolumetricSpace;

class HashIsoSurface : public IsoSurface {
public:
    HashIsoSurface(VolumetricSpace&, float density = 0.5f);

    // Override
    virtual std::shared_ptr<ci::TriMesh> computeSurfaceMesh(
        std::shared_ptr<ci::TriMesh> mesh, float iso);

    void createFacesForSlice(std::shared_ptr<ci::TriMesh> mesh, int z);

    int getCellIndex(int x, int y, int z);

    // Override
    virtual void reset();

    void setExpectedDensity(float d);

protected:
    ci::Vec3f cellSize;
    ci::Vec3f centreOffset;
    VolumetricSpace& volume;

    float isoValue;
    int resX, resY, resZ;
    int resX1, resY1, resZ1;

    int sliceRes;
    int nextXY;

    std::map<int, std::shared_ptr<ci::Vec3f>> edgeVertices;
    float density;

    std::vector<short> cellIndexCache, prevCellIndexCache;

};

}
}