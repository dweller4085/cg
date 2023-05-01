#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <functional>
#include "math.hh"

Screen screen {800, 600};
Camera camera {{0.f, 0.f, -100.f}};

constexpr auto wave = [](float x, float z) -> float {
    return std::cosf(std::sqrtf(x * x + z * z));
};

struct Surface {
    static constexpr u32 n = 40;
    static constexpr u32 vertexCount = n * n;
    vec3 * sva;
    vec3 * dva;
    vec2 * pva;;

    Surface(std::function<float(float, float)> f, float minX, float minZ, float maxX, float maxZ) {
        dva = (vec3 *) malloc(vertexCount * sizeof(vec3));
        sva = (vec3 *) malloc(vertexCount * sizeof(vec3));
        pva = (vec2 *) malloc(vertexCount * sizeof(vec2));
        
        if (!(sva && dva && pva)) {
            exit(EXIT_FAILURE);
        }
        
        float const incX = (maxX - minX) / ((float) n - 1.f);
        float const incZ = (maxZ - minZ) / ((float) n - 1.f);

        for (int i = 0; i < n; i += 1) {
            float x = minX + incX * (float) i;
            for (int j = 0; j < n; j += 1) {
                float z = minZ + incZ * (float) j ;
                sva[n * i + j] = {x, f(x, z), z};
                dva[n * i + j] = sva[n * i + j];
            }
        }
    }

    ~Surface() {
        free(sva);
        free(dva);
        free(pva);
    }

} surface {wave, -PI, -PI, PI, PI};
/*
void floatingHorizon (sf::RenderWindow& window, Screen screen, Surface& surface) {
    static int * upperHorizon = (int *) malloc(screen.width * sizeof(int));
    static int * lowerHorizon = (int *) malloc(screen.width * sizeof(int));
    static int * upperHorizonSwap = (int *) malloc(screen.width * sizeof(int));
    static int * lowerHorizonSwap = (int *) malloc(screen.width * sizeof(int));

    memset(upperHorizon, screen.height - 1, screen.width);
    memset(lowerHorizon, 0, screen.width);

    int x0, y0, x1, y1;
    bool flag_p, flag;

    // processing
    uint32_t v = 0;

    for (int32_t z = 0; z < Surface::n; z++) {
        for (int32_t x = 0; x < Surface::n; x++) {
            // current point
            x1 = round(vertices[v][0]);
            y1 = round(vertices[v][1]);

            flag = y1 < upper[x1] || y1 > lower[x1];

            // correct
            if (x != 0 && (flag_p || flag)) {
                int32_t dx = abs(x1 - x0); // delta x
                int32_t xi = x0 < x1 ? 1 : -1; // x increment

                int32_t dy = -abs(y1 - y0);
                int32_t yi = y0 < y1 ? 1 : -1;

                int32_t de = dx + dy; // delta error

                while (true) {
                    uint8_t flag = 0b00;

                    if (upper[x0] > y0) {
                        upper_n[x0] = y0;
                        flag |= 0b01;
                    }

                    if (lower[x0] < y0) {
                        lower_n[x0] = y0;
                        flag |= 0b10;
                    }

                    if (flag == 0b11) {
                        g.SetPixel(x0, y0, RGB(255, 255, 255)); // section
                    }
                    else if (flag & 0b01) {
                        g.SetPixel(x0, y0, RGB(255, 255, 255)); // upper
                    }
                    else if (flag & 0b10) {
                        g.SetPixel(x0, y0, RGB(200, 0, 0)); // lower
                    }

                    if (x0 == x1 && y0 == y1) {
                        break;
                    }

                    int32_t de2 = 2 * de;

                    if (de2 >= dy) {
                        de += dy;
                        x0 += xi;
                    }

                    if (de2 <= dx) {
                        de += dx;
                        y0 += yi;
                    }
                }
            }
            else {
                x0 = x1;
                y0 = y1;
            }

            flag_p = flag;
            v++;
        }

        upper = upper_n;
        lower = lower_n;
    }
}
*/

vec2 * tva = (vec2 *) malloc(Surface::vertexCount * sizeof(vec2));
sf::Vertex * fva = (sf::Vertex *) malloc(Surface::vertexCount * sizeof(sf::Vertex));

int main() {
    auto window = sf::RenderWindow {
        sf::VideoMode {screen.width, screen.height},
        "lab4",
        sf::Style::Default,
        sf::ContextSettings {
            0, 0, 0, 1, 1,
            sf::ContextSettings::Default,
            false
        }
    };

    auto angle = 0.16f * PI;
    auto const incSpeed = -0.0000002f;
    float dt = EPSILON;

    while (window.isOpen()) {
        sf::Event static event {};
        sf::Clock static clock {};
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

                        case sf::Keyboard::R: {
                            angle = 0.16f * PI;
                        } break;
                    }
                } break;
            }
        }

        if (angle > -0.16f * PI) {
            angle += incSpeed * dt;
        }

        (Transform {}.rotateX(angle) * worldToView(camera))
        .applyWith((float *) surface.dva, (float const *) surface.sva, Surface::vertexCount);
        parallelProj((float *) surface.pva, (float *) surface.dva, Surface::vertexCount);
        pictureToScreen((float *) tva, (float *) surface.pva, Surface::vertexCount, screen, camera.zoom);

        //for (int i = 0; i < Surface::vertexCount; i += 1) {
        //    fva[i] = sf::Vertex {{tva[i].x, tva[i].y}, sf::Color::White};
        //}

        window.clear();
        //floatingHorizon(window, screen, surface);
        //window.draw(fva, Surface::vertexCount, sf::Points);
        window.display();

        dt = clock.restart().asMicroseconds();
    }

    return EXIT_SUCCESS;
}
