

namespace toxi {
namespace volume {

class MarchingCubesIndex {
public:
    static const int edgeOffsets[][4];

    static const int cellTriangles[][16];

    static const int edgesToCompute[];
};

}
}