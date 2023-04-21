#pragma once

#include <MKB\aliases.h>
#include <SFML\System\Vector2.hpp>
#include <math.h>

#define self (*this)

constexpr f32 TAU = 6.283185307f;
constexpr f32 E   = 2.718281828f;
using Vec2 = sf::Vector2f;
inline f32 norm (Vec2 const & v) { return hypot (v.x, v.y); }
