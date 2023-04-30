#include <SFML/Graphics.hpp>
#include "lab3.hh"
#include "math.hh"

namespace {
    struct Prism {
        static constexpr int vertexCount = 6;
        static constexpr int triCount = 8;
        
        float va [vertexCount * 3] {
            2.f, 0.f, -1.5f,
            0.f, -2.f, -1.5f,
            0.f, 2.f, -1.5f,
            2.f, 0.f, 1.5f,
            0.f, -2.f, 1.5f,
            0.f, 2.f, 1.5f,
        };

        int tri [triCount * 3] {
            0, 1, 2,
            3, 5, 4,
            0, 2, 3,
            2, 5, 3,
            1, 0, 4,
            0, 3, 4,
            1, 5, 2,
            1, 4, 5,
        };

        sf::Color triColors[triCount] {
            {255, 0, 0},
            {255, 0, 0},
            {255, 255, 255},
            {255, 255, 255},
            {255, 255, 255},
            {255, 255, 255},
            {255, 255, 255},
            {255, 255, 255},
        };

        float centerPoint[3] {};
        float * W = (float *) malloc (triCount * 4 * sizeof(float));
        
        Prism() {
            enum Var : int { x, y, z };
            
            for (int i = 0; i < vertexCount; i += 1) {
                centerPoint[x] += va[3 * i + x] * (1.f / vertexCount);
                centerPoint[y] += va[3 * i + y] * (1.f / vertexCount);
                centerPoint[z] += va[3 * i + z] * (1.f / vertexCount);
            }

            for (int i = 0; i < triCount; i += 1) {
                auto const static p = [this, &i](Var v, int j) -> float {
                    return va[3 * tri[3 * i + (j - 1)] + v];
                };

                float a {
                    (p(y, 3) - p(y, 1)) * (p(z, 2) - p(z, 1)) -
                    (p(y, 2) - p(y, 1)) * (p(z, 3) - p(z, 1))
                };

                float b {
                    (p(x, 2) - p(x, 1)) * (p(z, 3) - p(z, 1)) -
                    (p(x, 3) - p(x, 1)) * (p(z, 2) - p(z, 1))
                };

                float c {
                    (p(x, 3) - p(x, 1)) * (p(y, 2) - p(y, 1)) -
                    (p(x, 2) - p(x, 1)) * (p(y, 3) - p(y, 1))
                };

                float d {
                    - a * p(x, 1)
                    - b * p(y, 1)
                    - c * p(z, 1)
                };

                if (a * centerPoint[x] + b * centerPoint[y] + c * centerPoint[z] + d < 0.f) {
                    a *= -1.f;
                    b *= -1.f;
                    c *= -1.f;
                    d *= -1.f;
                }

                W[4 * i + 0] = a;
                W[4 * i + 1] = b;
                W[4 * i + 2] = c;
                W[4 * i + 3] = d;
            }
        }

        ~Prism() {
            free(W);
        }

    } prism {};

    float * pictureSpace = (float *) malloc(prism.vertexCount * 2 * sizeof(float));
    float * screenSpace = (float *) malloc(prism.vertexCount * 2 * sizeof(float));
    float * viewSpace = (float *) malloc(prism.vertexCount * 3 * sizeof(float));
    sf::Vertex * finalVA = (sf::Vertex *) malloc(prism.triCount * 3 * sizeof(sf::Vertex));

    Screen screen {800, 600};
    Camera camera {{6, 6, 6}}, visCamera {{6, 6, 6}};
    struct CameraController {
        Camera * cam = &::camera;
        bool isDragging = false;
        float dragSpeed = 0.008f;
        float slideSpeed = 1.25f;
        float zoomSpeed = 0.8f;
        float pitchThreshold = 2.f;
        struct { int x, y; } prevCurPos {0, 0};

        void dragCamera(int dx, int dy) {
            if (!isDragging) return;

            Transform {}.rotateZ(dx * dragSpeed).applyTo(cam->pos.a, 1);

            float d = hypotf(cam->pos.x, cam->pos.y);

            if ((d > pitchThreshold) || (cam->pos.z > 0 && dy < 0) || (cam->pos.z < 0 && dy > 0)) {
                float cosu = cam->pos.y / d;
                float sinu = cam->pos.x / d;
                Transform {}
                .rotateZ(cosu, -sinu)
                .rotateX(-dy * dragSpeed)
                .rotateZ(cosu, sinu)
                .applyTo(cam->pos.a, 1);
            }
        }
    } camController;
    enum class Projection : int {
        Perspective,
        Parallel,
    } projection;
}

void flattenIVA (sf::Vertex * finalVA, float const * screenSpace, int const * tri, int triCount) {
    static int * orderedTri = (int *) malloc (sizeof(int) * triCount);
    auto static const isCCW { [=] (int i) -> bool {
        float x [3] {
            screenSpace[2 * tri[3 * i + 0] + 0],
            screenSpace[2 * tri[3 * i + 1] + 0],
            screenSpace[2 * tri[3 * i + 2] + 0],
        };

        float y [3] {
            screenSpace[2 * tri[3 * i + 0] + 1],
            screenSpace[2 * tri[3 * i + 1] + 1],
            screenSpace[2 * tri[3 * i + 2] + 1],
        };

        // some fucking determinant magic shit
        return x[0]*y[1] + x[1]*y[2] + x[2]*y[0] < y[0]*x[1] + y[1]*x[2] + y[2]*x[0];
    }};

    {
        int i = 0;
        int j = triCount - 1;
        for (int k = 0; k < triCount; k += 1) {
            if (isCCW(k)) {
                orderedTri[j--] = k;
            } else {
                orderedTri[i++] = k;
            }
        }
    }

    for (int i = 0; i < triCount; i += 1) {
        for (int j = 0; j < 3; j += 1) {
            finalVA[3 * i + j] = sf::Vertex {
                sf::Vector2f {
                    screenSpace[2 * tri[3 * orderedTri[i] + j] + 0],
                    screenSpace[2 * tri[3 * orderedTri[i] + j] + 1]
                },
                prism.triColors[orderedTri[i]]
            };
        }
    }
}

void markVisibleTris (Projection projection, Prism& prism, Camera& cam) {
    auto static const isVisible = [&](int i) -> bool {
        bool r = false;
        switch (projection) {
            case Projection::Perspective: {
                r = (
                    prism.W[4 * i + 0] * cam.pos.x +
                    prism.W[4 * i + 1] * cam.pos.y +
                    prism.W[4 * i + 2] * cam.pos.z +
                    prism.W[4 * i + 3]
                ) < 0.f;
            } break;

            case Projection::Parallel: {
                r = (
                    prism.W[4 * i + 0] * cam.pos.x +
                    prism.W[4 * i + 1] * cam.pos.y +
                    prism.W[4 * i + 2] * cam.pos.z
                ) < 0.f;
            } break;
        }

        return r;
    };
    
    for (int i = 0; i < prism.triCount; i += 1) {
        if (isVisible(i)) {
            prism.triColors[i] = sf::Color::White;
        } else {
            prism.triColors[i] = sf::Color::Red;
        }
    }
}

int main() {
    if (!(pictureSpace && viewSpace && screenSpace && finalVA)) {
        exit(-1);
    }

    auto window = sf::RenderWindow {
        sf::VideoMode {screen.width, screen.height},
        "lab3",
        sf::Style::Default,
        sf::ContextSettings {
            0, 0, 0, 1, 1,
            sf::ContextSettings::Default,
            false
        }
    };

    while (window.isOpen()) {
        sf::Event static event;
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

                        case sf::Keyboard::Q: {
                            camController.cam->zoom -= camController.zoomSpeed;
                        } break;

                        case sf::Keyboard::E: {
                            camController.cam->zoom += camController.zoomSpeed;
                        } break;

                        case sf::Keyboard::LShift: {
                            camController.cam->pos += camController.slideSpeed * camController.cam->pos.normalized();
                        } break;

                        case sf::Keyboard::LControl: {
                            camController.cam->pos += -camController.slideSpeed * camController.cam->pos.normalized();
                        } break;

                        case sf::Keyboard::Num1: {
                            camController.cam = &camera;
                        } break;

                        case sf::Keyboard::Num2: {
                            camController.cam = &visCamera;
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
                    camController.dragCamera(dx, dy);
                } break;
            }
        }

        markVisibleTris(projection, prism, visCamera);
        worldToView(*camController.cam).applyWith(viewSpace, prism.va, prism.vertexCount);
        switch (projection) {
            case Projection::Perspective: {
                perspectiveProj(pictureSpace, viewSpace, camera, prism.vertexCount);
            } break;

            case Projection::Parallel: {
                parallelProj(pictureSpace, viewSpace, prism.vertexCount);
            } break;
        }
        pictureToScreen(screenSpace, pictureSpace, prism.vertexCount, screen, camController.cam->zoom);
        flattenIVA(finalVA, screenSpace, prism.tri, prism.triCount);

        window.clear();
        window.draw(finalVA, prism.triCount * 3, sf::Triangles);
        window.display();
    }
    
    return 0;
}
