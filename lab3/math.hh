#pragma once
#include "common.hh"
#include <math.h>
#include <vector>
#include <SFML/Graphics.hpp>

constexpr float TAU = 6.28318530718f;
constexpr float EPSILON = 0.0000001f;

struct alignas(4) vec3 {
    union {
        struct {
            float x;
            float y;
            float z;
        };

        float a[3];
    };

    vec3 operator - () {
        return vec3 {-x, -y, -z};
    }

    vec3 operator + (vec3 v) {
        return {x + v.x, y + v.y, z + v.z};
    }

    vec3 operator - (vec3 v) {
        return {x - v.x, y - v.y, z - v.z};
    }

    void operator += (vec3 v) {
        self = self + v;
    }
    void operator -= (vec3 v) {
        self = self - v;
    }


    static inline float euclidianDistance (vec3 a, vec3 b) {
        return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
    }

    float norm() {
        return vec3::euclidianDistance(self, {0, 0, 0});
    }

    friend vec3 operator * (float a, vec3 v);

    vec3& normalize() {
        self = (1.f / self.norm()) * self;
        return self;
    }

    vec3 normalized() {
        return (1.f / self.norm()) * self;
    }
};

inline vec3 operator * (float a, vec3 v) {
    return {a * v.x, a * v.y, a * v.z};
}

struct Transform {
    float mat [16];
    Transform();
    Transform (Transform const &) = default;
    Transform& translate (vec3);
    Transform& scale (vec3);
    Transform& rotateX (float, float);
    Transform& rotateY (float, float);
    Transform& rotateZ (float, float);
    Transform& rotateX (float a) { return self.rotateX(cosf(a), sinf(a)); };
    Transform& rotateY (float a) { return self.rotateY(cosf(a), sinf(a)); };
    Transform& rotateZ (float a) { return self.rotateZ(cosf(a), sinf(a)); };
    Transform& operator *= (Transform const &);
    void applyTo (float *, size_t);
    void applyWith (float * __restrict, float const * __restrict, size_t);
};

struct Camera {
    vec3 pos;
    float zoom = 50.f;
    //cam always points to world origin
    //vec3 dir;
};

struct Screen {
    unsigned width;
    unsigned height;
};

Transform worldToView (Camera);
void perspectiveProj (float * __restrict, float * __restrict, Camera, size_t);
void parallelProj (float * __restrict, float * __restrict, size_t);
void pictureToScreen (float * __restrict, float * __restrict, size_t, Screen, float);
