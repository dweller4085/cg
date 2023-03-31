#include <cmath>
#include "scene.hh"
#include "transform.hh"
#include "lab1.hh"

Line::Line (Vec2 a, Vec2 b, sf::Color color) {
    self.vertices [0] = sf::Vertex { a, color };
    self.vertices [1] = sf::Vertex { b, color };
}

void Line::draw (sf::RenderTarget & target, sf::RenderStates states) const {
    target.draw (self.vertices, self.vertex_count, self.primitive_type, states);
}

Cross::Cross (Vec2 position, f32 size, sf::Color color) :
    position {position},
    size {size}
{
    vertices [0] = sf::Vertex { position + Vec2 { -size, 0.f }, color };
    vertices [1] = sf::Vertex { position + Vec2 { +size, 0.f }, color };
    vertices [2] = sf::Vertex { position + Vec2 { 0.f, -size }, color };
    vertices [3] = sf::Vertex { position + Vec2 { 0.f, +size }, color };
}

void Cross::draw (sf::RenderTarget & target, sf::RenderStates states) const {
    target.draw (self.vertices, self.vertex_count, self.primitive_type, states);
}

void Cross::transform (Transform const & transform) {
    transform.applyTo (self.vertices, self.vertex_count);
    self.position *= transform;
}

SnowBall::SnowBall (Vec2 position, f32 radius, usize vertex_count, sf::Color color) :
    vertices { new sf::Vertex [vertex_count + 1] },
    vertex_count { vertex_count + 1 },
    position { position },
    radius { radius }
{
    let angle = 0.f;
    let increment = 1.f / (self.vertex_count - 1);
    for (usize i = 0; i < self.vertex_count; i += 1) {
        self.vertices [i] = sf::Vertex {
            self.position + Vec2 {
                self.radius * cos (TAU * angle),
                self.radius * sin (TAU * angle)
            },
            color
        };

        angle += increment;
    }
}

SnowBall::~SnowBall () {
    delete [] self.vertices;
}

void SnowBall::draw (sf::RenderTarget & target, sf::RenderStates states) const {
    target.draw (self.vertices, self.vertex_count, self.primitive_type, states);
}

void SnowBall::transform (Transform const & transform) {
    transform.applyTo (self.vertices, self.vertex_count);
    self.position *= transform;
}

Scene::Scene () :
    cross { settings::ball.position + Vec2 { settings::ball.radius, 0.f }, settings::cross.size, settings::cross.color },
    ball { settings::ball.position, settings::ball.radius, settings::ball.vertex_count, sf::Color::White },
    slope { settings::slope.a, settings::slope.b, sf::Color::White },
    animation { 0.f, settings::animation.speed, false }
{
    let w = settings::slope.b - settings::slope.a;
    w /= norm (w);
    w = Vec2 {w.y, -w.x};

    self.velocity = (settings::slope.b + w * self.ball.radius * 2.f) - (settings::slope.a + w * self.ball.radius);
    self.velocity_ab = settings::slope.b - settings::slope.a;
    self.angular_velocity = norm(self.velocity_ab) / self.ball.radius / TAU;
    self.scaling_velocity = 2.f;
    self.ln_scaling_vel = log (scaling_velocity);
}

void Scene::update (f32 dt) {
    if (!self.animation.is_playing) {
        return;
    }

    if (self.animation.progress > 1.f) {
        if (!settings::animation.loop) {
            self.animation.is_playing = false;
            return;
        }
        else {
            self.resetAnimation ();
            self.animation.is_playing = true;
        }
    }

    if (settings::animation.stepping_mode) {
        self.animation.is_playing = false;
    }

    let const t = self.animation.speed * dt;
    self.animation.progress += t;



    let const scaling_factor = exp (self.ln_scaling_vel * t);
    self.ball.radius *= scaling_factor;
    self.angular_velocity = norm (self.velocity_ab) / self.ball.radius / TAU;

    Transform {}
        .translate (-self.ball.position)
        .scale (scaling_factor)
        .translate (self.ball.position)
        .applyTo (self.ball)
    ;

    Transform {}
        .translate (-self.ball.position)
        .rotate (self.angular_velocity * t)
        .translate (self.ball.position)
        .translate (self.velocity * t)
        .applyTo (self.ball)
        .applyTo (self.cross)
    ;

    // correct cross's rotation
    Transform {}
        .translate (-self.cross.position)
        .rotate (-self.angular_velocity * t)
        .translate (self.cross.position)
        .applyTo (self.cross)
    ;
}

void Scene::resetAnimation () {
    self.animation.progress = 0.f;
    self.animation.is_playing = false;


    self.ball.position = settings::ball.position;
    self.ball.radius = settings::ball.radius;

    let angle = 0.f;
    let increment = 1.f / (self.ball.vertex_count - 1);
    for (usize i = 0; i < self.ball.vertex_count; i += 1) {
        self.ball.vertices[i].position =
            self.ball.position + Vec2 {
            self.ball.radius * cos (TAU * angle),
            self.ball.radius * sin (TAU * angle)
        };

        angle += increment;
    }


    self.cross.position = self.ball.position + Vec2 { settings::ball.radius, 0.f};
    self.cross.vertices [0].position = self.cross.position + Vec2 { -self.cross.size, 0.f };
    self.cross.vertices [1].position = self.cross.position + Vec2 { +self.cross.size, 0.f };
    self.cross.vertices [2].position = self.cross.position + Vec2 { 0.f, -self.cross.size };
    self.cross.vertices [3].position = self.cross.position + Vec2 { 0.f, +self.cross.size };
}

void Scene::draw (sf::RenderTarget & target, sf::RenderStates states) const {
    target.draw (self.slope, states);
    target.draw (self.ball, states);
    target.draw (self.cross, states);
}
