#include <SFML/Graphics.hpp>
#include "lab3.hh"


int main() {
    let window = sf::RenderWindow {
        sf::VideoMode {screen.width, screen.height},
        "lab3",
        sf::Style::Default,
        sf::ContextSettings {
            0, 0, 0, 1, 1,
            sf::ContextSettings::Default,
            false
        }
    };

    let event = sf::Event {};

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
                    }
                }
            }
        }

        window.clear();
        window.display();
    }

    return 0;
}