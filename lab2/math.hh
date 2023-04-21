#pragma once
#include <math.h>

constexpr float TAU = 6.28318530718f;
constexpr float EPSILON = 0.0000001f;

struct alignas(4) Vec3 {
    float x;
    float y;
    float z;

    Vec3 operator - () {
        return Vec3 {-x, -y, -z};
    }

    static inline float euclidianDistance (Vec3 a, Vec3 b) {
        return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
    }
};

struct Transform {
    float mat [16];
    Transform();
    Transform (Transform const &) = default;
    Transform& translate (Vec3);
    Transform& scale (Vec3);
    Transform& rotateX (float);
    Transform& rotateY (float);
    Transform& rotateZ (float);
    Transform& operator *= (Transform const &);
    void applyTo (float *, size_t);
    void applyWith (float *, float *, size_t);
};

struct Camera {
    Vec3 pos;
    //cam always points to world origin
    //Vec3 dir;
};

Transform worldToView (Camera);
void perspectiveProj (float *, float *, float, size_t);
void parallelProj (float *, float *, size_t);
