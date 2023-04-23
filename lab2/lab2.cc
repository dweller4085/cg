#include "common.hh"
#include "math.hh"
#include <SFML/Graphics.hpp>

static float testVA[] {
    0.f, 0.f, 0.f,
    4.f, 0.f, 0.f,
    0.f, 4.f, 0.f,
    0.f, 0.f, 4.f,
    //2.f, 0.f, 1.f,
    //-2.f, 0.f, 1.f,
    //2.f, 1.f, 0.f,
    //-2.f, 1.f, 0.f,
};
static int testIA[] {
    0, 1,
    0, 2,
    0, 3,
    //4, 5,
    //6, 7,
};

constexpr size_t vertexCount = sizeof(testVA) / (sizeof(float) * 3);
constexpr size_t segmentCount = sizeof(testIA) / (sizeof(float) * 2);

static float * pictureSpace = (float *) malloc(vertexCount * 2 * sizeof(float));
static float * screenSpace = (float *) malloc(vertexCount * 2 * sizeof(float));
static float * viewSpace = (float *) malloc(sizeof(testVA));

constexpr Screen screen {800, 600};
static Camera camera {{6, 6, 6}};

int main() {
    
    
    
    /*
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
    } projection = Projection::Parallel;


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

                case sf::Event::MouseMoved: {
                    //
                } break;
            }
        }
    }
    */
    return 0;
}
