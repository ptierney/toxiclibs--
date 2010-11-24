
#include <cinder/Vector.h>

#include <toxi/volume/toxi_volume_HashIsoSurface.h>
#include <toxi/volume/toxi_volume_VolumetricSpace.h>
#include <toxi/volume/toxi_volume_MarchingCubesIndex.h>

namespace toxi {
namespace volume {

HashIsoSurface::HashIsoSurface(VolumetricSpace& vol, float d) 
    : volume(vol), density(d) {

    cellSize = ci::Vec3f(volume.scale.x / volume.resX1,
        volume.scale.y / volume.resY1, volume.scale.z / volume.resZ1);

    resX = volume.resX;
    resY = volume.resY;
    resZ = volume.resZ;
    resX1 = volume.resX1;
    resY1 = volume.resY1;
    resZ1 = volume.resZ1;

    sliceRes = volume.sliceRes;
    nextXY = resX + sliceRes;

    cellIndexCache.resize(sliceRes);
    prevCellIndexCache.resize(sliceRes);

    centreOffset = volume.halfScale.inverse();
    reset();
}

std::shared_ptr<ci::TriMesh> HashIsoSurface::computeSurfaceMesh(std::shared_ptr<ci::TriMesh> 
    mesh, float iso) {

    if (mesh.get() == NULL) {
        mesh = std::shared_ptr<ci::TriMesh>(new ci::TriMesh());
    } else {
        mesh->clear();
    }

    isoValue = iso;
    float offsetZ = centreOffset.z;
    for (int z = 0; z < resZ1; z++) {
        int sliceOffset = sliceRes * z;
        float offsetY = centreOffset.y;
        for (int y = 0; y < resY1; y++) {
            float offsetX = centreOffset.x;
            int sliceIndex = resX * y;
            int offset = sliceIndex + sliceOffset;
            for (int x = 0; x < resX1; x++) {
                int cellIndex = getCellIndex(x, y, z);
                cellIndexCache[sliceIndex + x] = (short) cellIndex;
                if (cellIndex > 0 && cellIndex < 255) {
                    int edgeFlags = MarchingCubesIndex::edgesToCompute[cellIndex];
                    if (edgeFlags > 0 && edgeFlags < 255) {
                        int edgeOffsetIndex = offset * 3;
                        float offsetData = volume.getVoxelAt(offset);
                        float isoDiff = isoValue - offsetData;
                        if ((edgeFlags & 1) > 0) {
                            float t = isoDiff / (volume.getVoxelAt(offset + 1) - 
                                offsetData);
                            edgeVertices[edgeOffsetIndex] = std::shared_ptr<ci::Vec3f>(
                                new ci::Vec3f(offsetX + t * cellSize.x, y * cellSize.y + 
                                    centreOffset.y, z * cellSize.z + centreOffset.z));
                        }
                        if ((edgeFlags & 2) > 0) {
                            float t = isoDiff / (volume.getVoxelAt(offset 
                                + resX) - offsetData);
                            edgeVertices[edgeOffsetIndex + 1] = std::shared_ptr<ci::Vec3f>(
                                new ci::Vec3f(x * cellSize.x + centreOffset.x, offsetY + t
                                * cellSize.y, z * cellSize.z + centreOffset.z));
                        }
                        if ((edgeFlags & 4) > 0) {
                            float t = isoDiff / (volume.getVoxelAt(offset 
                                + sliceRes) - offsetData);
                            edgeVertices[edgeOffsetIndex + 2] = std::shared_ptr<ci::Vec3f>(
                                new ci::Vec3f(x * cellSize.x + centreOffset.x, y * 
                                cellSize.y + centreOffset.y, offsetZ + t * cellSize.z));
                        }
                    }
                }
                offsetX += cellSize.x;
                offset++;
            }
            offsetY += cellSize.y;
        }
        if (z > 0) {
            createFacesForSlice(mesh, z - 1);
        }
        std::vector<short> tmp = prevCellIndexCache;
        prevCellIndexCache = cellIndexCache;
        cellIndexCache = tmp;
        offsetZ += cellSize.z;
    }
    createFacesForSlice(mesh, resZ1 - 1);
    return mesh;
}

void HashIsoSurface::createFacesForSlice(std::shared_ptr<ci::TriMesh> mesh, int z) {
    int* face = new int[16];
    int sliceOffset = sliceRes * z;
    for (int y = 0; y < resY1; y++) {
        int offset = resX * y;
        for (int x = 0; x < resX1; x++) {
            int cellIndex = prevCellIndexCache[offset];
            if (cellIndex > 0 && cellIndex < 255) {
                int n = 0;
                int edgeIndex;
                const int* cellTriangles = MarchingCubesIndex::cellTriangles[cellIndex];
                while ((edgeIndex = cellTriangles[n]) != -1) {
                    const int* edgeOffsetInfo = MarchingCubesIndex::edgeOffsets[edgeIndex];
                    face[n] =((x + edgeOffsetInfo[0]) + resX
                        * (y + edgeOffsetInfo[1]) + sliceRes
                        * (z + edgeOffsetInfo[2]))
                        * 3 + edgeOffsetInfo[3];
                    n++;
                }
                for (int i = 0; i < n; i += 3) {
                    // find out if there is a vertice at this location
                    std::shared_ptr<ci::Vec3f> va = edgeVertices[face[i + 1]];
                    if (va.get() != NULL) {
                        std::shared_ptr<ci::Vec3f> vb = edgeVertices[face[i + 2]];
                        if (vb.get() != NULL) {
                            std::shared_ptr<ci::Vec3f> vc = edgeVertices[face[i]];
                            if (vc.get() != NULL) {
                                mesh->appendVertex(*(va.get()));
                                mesh->appendVertex(*(vb.get()));
                                mesh->appendVertex(*(vc.get()));
                                int nt = mesh->getNumTriangles();
                                mesh->appendTriangle(nt - 3, nt - 2, nt - 1);
                            }
                        }
                    }
                }
            }
            offset++;
        }
    }
    int minIndex = sliceOffset * 3;
    bool all_removed = false;

    while (!all_removed) {
        for(std::map<int, std::shared_ptr<ci::Vec3f>>::iterator it = edgeVertices.begin();
            ; ++it) {
            if (it == edgeVertices.end()) {
                all_removed = true;
                break; // break out of for
            }

            if (it->first < minIndex) {
                edgeVertices.erase(it); // this invalidates the iterator
                break; // break out of for
            }
        }
    }
}

int HashIsoSurface::getCellIndex(int x, int y, int z) {
    int cellIndex = 0;
    int idx = x + y * resX + z * sliceRes;
    if (volume.getVoxelAt(idx) < isoValue) {
        cellIndex |= 0x01;
    }
    if (volume.getVoxelAt(idx + sliceRes) < isoValue) {
        cellIndex |= 0x08;
    }
    if (volume.getVoxelAt(idx + resX) < isoValue) {
        cellIndex |= 0x10;
    }
    if (volume.getVoxelAt(idx + resX + sliceRes) < isoValue) {
        cellIndex |= 0x80;
    }
    idx++;
    if (volume.getVoxelAt(idx) < isoValue) {
        cellIndex |= 0x02;
    }
    if (volume.getVoxelAt(idx + sliceRes) < isoValue) {
        cellIndex |= 0x04;
    }
    if (volume.getVoxelAt(idx + resX) < isoValue) {
        cellIndex |= 0x20;
    }
    if (volume.getVoxelAt(idx + resX + sliceRes) < isoValue) {
        cellIndex |= 0x40;
    }
    return cellIndex;
}

void HashIsoSurface::reset() {
    edgeVertices.clear();
}

void HashIsoSurface::setExpectedDensity(float d) {
    density = d;
}

}
}
