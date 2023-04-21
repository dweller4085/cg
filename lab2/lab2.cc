#include "common.hh"
#include "math.hh"
#include <string.h>
#include <stdio.h>

static float testVA[] {
    0.f, 0.f, 0.f,
    4.f, 0.f, 0.f,
    0.f, 4.f, 0.f,
    0.f, 0.f, 4.f,
    2.f, 0.f, 1.f,
    -2.f, 0.f, 1.f,
    2.f, 1.f, 0.f,
    -2.f, 1.f, 0.f,
};
static int testIA[] {
    0, 1,
    0, 2,
    0, 3,
    4, 5,
    6, 7,
};
constexpr size_t vertexCount = sizeof(testVA) / (sizeof(float) * 3);
constexpr size_t segmentCount = sizeof(testIA) / (sizeof(float) * 2);

static Camera camera {{3, 4, 6}};
static float * picturePlane = (float *) malloc(vertexCount * 2 * sizeof(float));
static float * viewVA = (float *) malloc(sizeof(testVA));

int main() {
    if (picturePlane == nullptr || viewVA == nullptr) {
        exit(-1);
    }
    
    float s = Vec3::euclidianDistance(camera.pos, {0.f, 0.f, 0.f});

    worldToView(camera).applyWith(viewVA, testVA, vertexCount);
    perspectiveProj(picturePlane, viewVA, s, vertexCount);
    //parallelProj(picturePlane, va, vertexCount);

    printf("");

    return 0;
}
