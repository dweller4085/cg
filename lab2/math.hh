#pragma once

constexpr float TAU = 6.28318530718f;
constexpr float EPSILON = 0.0000001f;

struct alignas(4) Vec3 {
    float x;
    float y;
    float z;

    Vec3 operator - () {
        return Vec3 {-x, -y, -z};
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
    void apply (float *, size_t);
};

