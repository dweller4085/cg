#pragma once

#include <SFML/Graphics.hpp>
#include "common.hh"

namespace settings {
    extern inline char const * const window_name = "snowball";
    extern inline const bool antialiasing = false;
    extern inline const struct {
        u32 width = 800;
        u32 height = 600;
    } screen = {};
    extern inline const struct {
        f32 size = 6;
        sf::Color color = sf::Color::Red;
    } cross = {};
    extern inline const struct {
        f32 speed = 1.f / 3250000.f; // cycles per microsecond
        bool loop = false;
        bool play_on_startup = false;
        bool stepping_mode = false;
    } animation = {};
    extern inline const struct {
        Vec2 a = Vec2 {
            (1.f - 0.75f) * screen.width / 2.f,
            (1.f - 0.2f + 0.33f) * screen.height / 2.f
        };
        Vec2 b = Vec2 {
            (1.f + 0.75f) * screen.width / 2.f,
            (1.f + 0.2f + 0.33f) * screen.height / 2.f
        };
    } slope = {};
    extern inline const struct {
        f32 radius = 40.f;
        usize vertex_count = 32;
        Vec2 v = slope.b - slope.a;
        Vec2 position = slope.a + radius / hypot (v.x, v.y) * Vec2 {
            v.y,
            -v.x
        };
    } ball = {};
}
