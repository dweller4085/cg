#include "math.hh"
#include "common.hh"
#include <string.h>

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

void Transform::applyTo (float * va, size_t n) {
    for (size_t i = 0; i < n * 3; i += 3) {
        Vec3 v {va[i], va[i + 1], va[i + 2]};
        va[i] = v.x * mat[0] + v.y * mat[4] + v.z * mat[8] + 1.f * mat[12];
        va[i+1] = v.x * mat[1] + v.y * mat[5] + v.z * mat[9] + 1.f * mat[13];
        va[i+2] = v.x * mat[2] + v.y * mat[6] + v.z * mat[10] + 1.f * mat[14];
    }
}

void Transform::applyWith (float * to, float * with, size_t n) {
    for (size_t i = 0; i < n * 3; i += 3) {
        Vec3 v {with[i], with[i + 1], with[i + 2]};
        to[i] = v.x * mat[0] + v.y * mat[4] + v.z * mat[8] + 1.f * mat[12];
        to[i + 1] = v.x * mat[1] + v.y * mat[5] + v.z * mat[9] + 1.f * mat[13];
        to[i + 2] = v.x * mat[2] + v.y * mat[6] + v.z * mat[10] + 1.f * mat[14];
    }
}

Transform worldToView (Camera cam) {
    float d;

    /* 1..3 */
    let viewTransform = Transform {}
        .translate(-cam.pos)
        .scale({-1.f, 1.f, 1.f})
        .rotateX(.25f * TAU)
    ;

    /* 4 */
    {
        Transform R {};
        d = hypotf(cam.pos.x, cam.pos.y);
        float cosu;
        float sinu;

        if (d < EPSILON) {
            cosu = 1.f;
            sinu = 0.f;
        }
        else {
            cosu = cam.pos.y / d;
            sinu = cam.pos.x / d;
        }

        R.mat[0] = cosu;
        R.mat[10] = cosu;
        R.mat[2] = sinu;
        R.mat[8] = -sinu;

        viewTransform *= R;
    }

    /* 5 */
    {
        Transform R {};
        float s = hypotf(d, cam.pos.z);
        float cosw;
        float sinw;

        if (s < EPSILON) {
            cosw = 1.f;
            sinw = 0.f;
        }
        else {
            cosw = d / s;
            sinw = cam.pos.z / s;
        }

        R.mat[5] = cosw;
        R.mat[10] = cosw;
        R.mat[6] = -sinw;
        R.mat[9] = sinw;

        viewTransform *= R;
    }

    return viewTransform;
}

void perspectiveProj (float * pp, float * va, float s, size_t n) {
    memset(pp, 0, n * 2 * sizeof(float));

    for (size_t i = 0; i < n; i += 1) {
        pp[2 * i + 0] = va[3 * i + 0] * (s / va[3 * i + 2]);
        pp[2 * i + 1] = va[3 * i + 1] * (s / va[3 * i + 2]);
    }
}

void parallelProj (float * pp, float * va, size_t n) {
    memset(pp, 0, n * 2 * sizeof(float));
    
    for (size_t i = 0; i < n; i += 1) {
        pp[2 * i + 0] = va[3 * i + 0];
        pp[2 * i + 1] = va[3 * i + 1];
    }
}
