#pragma once

#include <SFML/Graphics.hpp>
#include "common.hh"

struct Mat3 {
    f32 elements [9];
    
    static Mat3 identity ();
    static Mat3 zeroes ();
    Mat3 () = default;
    Mat3 (f32 const * elements);
    f32 & operator [] (usize i) { return self.elements [i]; }
    f32 operator [] (usize i) const { return self.elements [i]; }
};

struct Transformable;

struct Transform {
    mutable Mat3 matrix;

    static inline Transform identity () { return Transform {}; }
    Transform ();
    Transform (Mat3 const & matrix);
    Transform & translate (Vec2 offset);
    Transform & scale (Vec2 factors);
    Transform & scale (f32 factor) { return self.scale (Vec2 {factor, factor}); }
    Transform & rotate (f32 turn);
    Transform & clear () const { self.matrix = Mat3::identity (); return const_cast<Transform &> self; }
    Transform const & applyTo (sf::Vertex * vertices, usize vertex_count) const;
    Transform const & applyTo (Transformable & object) const;
};

struct Transformable {
    virtual void transform (Transform const & transform) = 0;
};

Mat3         operator *  (Mat3 const &       lhs, Mat3 const &      rhs);
Mat3 &       operator *= (Mat3 &             lhs, Mat3 const &      rhs);
Transform    operator *  (Transform const &  lhs, Transform const & rhs);
Transform &  operator *= (Transform &        lhs, Transform const & rhs);
Vec2         operator *  (Vec2 const &       lhs, Transform const & rhs);
Vec2 &       operator *= (Vec2 &             lhs, Transform const & rhs);
sf::Vertex   operator *  (sf::Vertex const & lhs, Transform const & rhs);
sf::Vertex & operator *= (sf::Vertex &       lhs, Transform const & rhs);
