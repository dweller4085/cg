#include "transform.hh"
#include <cstring>
#include <cmath>

Mat3 Mat3::identity () {
    f32 const identity_matrix [9] = {
        1.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 1.f
    };
    return Mat3 { identity_matrix };
}

Mat3 Mat3::zeroes () {
    f32 const zeroes_matrix [9] = {
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f
    };
    return Mat3 { zeroes_matrix };
}

Mat3::Mat3 (f32 const * elements) {
    memcpy (self.elements, elements, sizeof (f32) * 9);
}

Transform::Transform () :
    matrix {Mat3::identity ()}
{}

Transform::Transform (Mat3 const & matrix) :
    matrix { matrix }
{}

Transform & Transform::translate (Vec2 offset) {
    f32 const translation_matrix [9] = {
        1.f,      0.f,      0.f,
        0.f,      1.f,      0.f,
        offset.x, offset.y, 1.f
    };
    self.matrix *= Mat3 { translation_matrix };
    return self;
}

Transform & Transform::scale (Vec2 factors) {
    f32 const scaling_matrix [9] = {
        factors.x, 0.f,       0.f,
        0.f,       factors.y, 0.f,
        0.f,       0.f,       1.f
    };
    self.matrix *= Mat3 { scaling_matrix };
    return self;
}

Transform & Transform::rotate (f32 turn) {
    let a = TAU * turn;
    f32 const rotation_matrix [9] = {
        cos(a),  sin(a), 0.f,
        -sin(a), cos(a), 0.f,
        0.f,     0.f,    1.f
    };
    self.matrix *= Mat3 { rotation_matrix };
    return self;
}

Transform const & Transform::applyTo (sf::Vertex * vertices, usize vertex_count) const {
    for (usize i = 0; i < vertex_count; i += 1) {
        vertices [i] *= self;
    }

    return self;
}

Transform const & Transform::applyTo (Transformable & object) const {
    object.transform (self);
    return self;
}

Mat3 operator * (Mat3 const & lhs, Mat3 const & rhs) {
    let mat = Mat3::zeroes ();

    for (u32 i = 0; i < 3; i += 1) {
        for (u32 j = 0; j < 3; j += 1) {
            for (u32 k = 0; k < 3; k += 1) {
                mat [3 * i + j] += lhs [3 * i + k] * rhs [3 * k + j];
            }
        }
    }

    return mat;
}

Mat3 & operator *= (Mat3 & lhs, Mat3 const & rhs) {
    lhs = lhs * rhs;
    return lhs;
}

Transform operator * (Transform const & lhs, Transform const & rhs) {
    return Transform { lhs.matrix * rhs.matrix };
}

Transform & operator *= (Transform & lhs, Transform const & rhs) {
    lhs.matrix *= rhs.matrix;
    return lhs;
}

Vec2 operator * (Vec2 const & lhs, Transform const & rhs) {
    return Vec2 {
        lhs.x * rhs.matrix [0] + lhs.y * rhs.matrix [3] + 1.f * rhs.matrix [6],
        lhs.x * rhs.matrix [1] + lhs.y * rhs.matrix [4] + 1.f * rhs.matrix [7]
    };
}

Vec2 & operator *= (Vec2 & lhs, Transform const & rhs) {
    lhs = lhs * rhs;
    return lhs;
}

sf::Vertex operator * (sf::Vertex const & lhs, Transform const & rhs) {
    return sf::Vertex { lhs.position * rhs, lhs.color, lhs.texCoords };
}

sf::Vertex & operator *= (sf::Vertex & lhs, Transform const & rhs) {
    lhs.position *= rhs;
    return lhs;
}
