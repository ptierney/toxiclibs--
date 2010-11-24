
#pragma once

#include <cinder/TriMesh.h>

namespace toxi {
namespace volume {

class IsoSurface {
public:
    IsoSurface() { }
    virtual ~IsoSurface() { }

    virtual std::shared_ptr<ci::TriMesh> computeSurfaceMesh(
        std::shared_ptr<ci::TriMesh>, float iso) = 0;

    virtual void reset() = 0;
};

}
}