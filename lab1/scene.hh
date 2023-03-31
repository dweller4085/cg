#pragma once

#include <SFML/Graphics.hpp>
#include "common.hh"
#include "transform.hh"

struct Line : sf::Drawable {
    sf::Vertex vertices [2];

    static inline usize const vertex_count = 2;
    static inline sf::PrimitiveType const primitive_type = sf::PrimitiveType::Lines;
    Line (Vec2 a, Vec2 b, sf::Color color);

private:
    virtual void draw (sf::RenderTarget & target, sf::RenderStates states) const override;
};

struct Cross : sf::Drawable, Transformable {
    sf::Vertex vertices [4];
    Vec2 position;
    f32 const size;

    static inline usize const vertex_count = 4;
    static inline sf::PrimitiveType const primitive_type = sf::PrimitiveType::Lines;
    Cross (Vec2 position, f32 size, sf::Color color);

    virtual void transform (Transform const & transform) override;

private:
    virtual void draw (sf::RenderTarget & target, sf::RenderStates states) const override;
};

struct SnowBall : sf::Drawable, Transformable {
    sf::Vertex * const vertices;
    usize const vertex_count;
    Vec2 position;
    f32 radius;

    static inline sf::PrimitiveType const primitive_type = sf::PrimitiveType::LineStrip;
    SnowBall (Vec2 position, f32 radius, usize vertex_count, sf::Color color);
    ~SnowBall ();

    virtual void transform (Transform const & transform) override;

private:
    virtual void draw (sf::RenderTarget & target, sf::RenderStates states) const override;
};

struct Scene : sf::Drawable {
    Line slope;
    SnowBall ball;
    Cross cross;
    struct {
        f32 progress;
        f32 speed;
        bool is_playing;
    } animation;

    Vec2 velocity;
    Vec2 velocity_ab;
    f32 scaling_velocity;
    f32 ln_scaling_vel;
    f32 angular_velocity;

    Scene ();
    void update (f32 dt);
    void resetAnimation ();
    void startAnimation () { self.animation.is_playing = true; }
    void stopAnimation () { self.animation.is_playing = false; }

private:
    virtual void draw (sf::RenderTarget & target, sf::RenderStates states) const override;
};
