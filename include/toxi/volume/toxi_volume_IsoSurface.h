
#include <cinder/TriMesh.h>

namespace toxi {
namespace volume {

class IsoSurface {
public:
    IsoSurface() { }
    virtual ~IsoSurface() { }

    virtual ci::TriMesh computeSurfaceMesh(ci::TriMesh mesh, float iso) = 0;

    virtual void reset() = 0;
};

}
}