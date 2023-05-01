#include <SFML/Graphics.hpp>

struct {
    unsigned width;
    unsigned height;
} screen {800, 600};

struct {
    sf::Vector3f pos;
} camera {{0.f, 0.f, -10.f}};

struct surface {

};



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

    while (window.isOpen()) {
        sf::Event static event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed: {
                    window.close();
                } break;
            }
        }

        window.clear();
        window.display();
    }

    return EXIT_SUCCESS;
}
