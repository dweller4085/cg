#include "common.hh"
#include "math.hh"
#include <SFML/Graphics.hpp>

static float testVA[] {
    0.f, 0.f, 0.f,
    4.f, 0.f, 0.f,
    0.f, 4.f, 0.f,
    0.f, 0.f, 4.f,
    10.f, 0.f, 1.f,
    -10.f, 0.f, 1.f,
    10.f, 1.f, 0.f,
    -10.f, 1.f, 0.f,
};
static int testIA[] {
    0, 1,
    0, 2,
    0, 3,
    4, 5,
    6, 7,
};

constexpr size_t vertexCount = sizeof(testVA) / (sizeof(float) * 3);
constexpr size_t segmentCount = sizeof(testIA) / (sizeof(float) * 2);

static float * pictureSpace = (float *) malloc(vertexCount * 2 * sizeof(float));
static float * screenSpace = (float *) malloc(vertexCount * 2 * sizeof(float));
static float * viewSpace = (float *) malloc(sizeof(testVA));
static float * finalVA = (float *) malloc(segmentCount * 2 * 3 * sizeof(float));

constexpr Screen screen {800, 600};
static Camera camera {{3, 4, 6}};

static struct CameraController {
    Camera & cam = ::camera;
    bool isDragging = false;
    float dragSpeed = 0.008f;
    struct { int x, y; } prevCurPos;
} camController;

int main() {
    let window = sf::RenderWindow {
        sf::VideoMode {screen.width, screen.height},
        "lab2",
        sf::Style::Default,
        sf::ContextSettings {
            0,
            0,
            0,
            1,
            1,
            sf::ContextSettings::Default,
            false
        }
    };
    let event = sf::Event {};

    enum class Projection : int {
        Perspective,
        Parallel,
    } projection = Projection::Perspective;

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed: {
                    window.close();
                } break;

                case sf::Event::KeyPressed: {
                    switch (event.key.code) {
                        case sf::Keyboard::Escape: {
                            window.close();
                        } break;

                        case sf::Keyboard::P: {
                            projection = Projection ((int) projection ^ 1);
                        } break;
                    }
                } break;

                case sf::Event::MouseButtonPressed: {
                    switch (event.mouseButton.button) {
                        case sf::Mouse::Left: {
                            camController.isDragging = true;
                            camController.prevCurPos = {event.mouseButton.x, event.mouseButton.y};
                        } break;
                    }
                } break;

                case sf::Event::MouseButtonReleased: {
                    switch (event.mouseButton.button) {
                        case sf::Mouse::Left: {
                            camController.isDragging = false;
                        } break;
                    }
                } break;
                
                case sf::Event::MouseMoved: {
                    int dx = event.mouseMove.x - camController.prevCurPos.x;
                    int dy = event.mouseMove.y - camController.prevCurPos.y;

                    camController.prevCurPos = {event.mouseMove.x, event.mouseMove.y};

                    if (camController.isDragging) {
                        Transform {}.rotateZ(dx * camController.dragSpeed).applyTo(camController.cam.pos.a, 1);
                    }
                } break;
            }
        }

        worldToView(camera).applyWith(viewSpace, testVA, vertexCount);

        switch (projection) {
            case Projection::Perspective: {
                float s = Vec3::euclidianDistance(camera.pos, {0.f, 0.f, 0.f});
                perspectiveProj(pictureSpace, viewSpace, s, vertexCount);
            } break;

            case Projection::Parallel: {
                parallelProj(pictureSpace, viewSpace, vertexCount);
            } break;
        }

        pictureToScreen(screenSpace, pictureSpace, vertexCount, screen);

        auto va = flattenIVA(screenSpace, testIA, segmentCount);

        window.clear();
        window.draw(va.data(), va.size(), sf::Lines);
        window.display();
    }
    
    return 0;
}
