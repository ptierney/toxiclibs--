
#include <toxi/volume/toxi_volume_VolumetricBrush.h>

namespace toxi {
namespace volume {

class VolumetricSpace;

class RoundBrush : public VolumetricBrush {
public:
    RoundBrush(VolumetricSpace&, float radius);

    void drawAtGridPos(float cx, float cy, float cz, float density);

    void setSize(float radius);

protected:
    float radius, radSquared;

};

}
}