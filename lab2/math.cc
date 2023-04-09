#include "math.hh"
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <math.h>

Transform::Transform() : mat {
    1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f
}{}

Transform& Transform::translate(Vec3 v) {
    auto shift = Transform {};
    shift.mat[12] = v.x;
    shift.mat[13] = v.y;
    shift.mat[14] = v.z;

    return (*this) *= shift;
}

Transform& Transform::scale(Vec3 v) {
    auto scale = Transform {};
    scale.mat[0] = v.x;
    scale.mat[5] = v.y;
    scale.mat[10] = v.z;

    return (*this) *= scale;
}

Transform& Transform::rotateX(float a) {
    auto rotateX = Transform {};
    rotateX.mat[5] = cos(a);
    rotateX.mat[10] = cos(a);
    rotateX.mat[6] = -sin(a);
    rotateX.mat[9] = sin(a);

    return (*this) *= rotateX;
}

Transform& Transform::rotateY(float a) {
    auto rotateX = Transform {};
    rotateX.mat[0] = cos(a);
    rotateX.mat[10] = cos(a);
    rotateX.mat[8] = -sin(a);
    rotateX.mat[2] = sin(a);

    return (*this) *= rotateX;
}

Transform& Transform::rotateZ(float a) {
    auto rotateX = Transform {};
    rotateX.mat[0] = cos(a);
    rotateX.mat[5] = cos(a);
    rotateX.mat[1] = -sin(a);
    rotateX.mat[4] = sin(a);

    return (*this) *= rotateX;
}

Transform& Transform::operator *= (Transform const & b) {
    Transform a {(*this)};

    for (int i = 0; i < 4; i += 1) {
        for (int j = 0; j < 4; j += 1) {
            for (int k = 0; k < 4; k += 1) {
                mat[4 * i + j] += a.mat[4 * i + k] * b.mat[4 * k + j];
            }
        }
    }

    return *this;
}

void Transform::apply (float * va, size_t n) {
    for (size_t i = 0; i < n; i += 3) {
        Vec3 v {va[i], va[i + 1], va[i + 2]};
        va[i] = v.x * mat[0] + v.y * mat[4] + v.z * mat[8] + 1.f * mat[12];
        va[i+1] = v.x * mat[1] + v.y * mat[5] + v.z * mat[9] + 1.f * mat[13];
        va[i+2] = v.x * mat[2] + v.y * mat[6] + v.z * mat[10] + 1.f * mat[14];
    }
}
