#include <SFML/Graphics.hpp>

struct {
    unsigned width;
    unsigned height;
} screen {800, 600};

struct {
    sf::Vector3f pos;
} camera {{0.f, 0.f, -10.f}};

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

    auto texture = sf::RenderTexture {};
    texture.create(screen.width, screen.height, sf::ContextSettings {});
    texture.clear();
    texture.display();

    auto shader = sf::Shader {};
    shader.loadFromFile("frag.glsl", sf::Shader::Fragment);
    shader.setUniform("uScreenRes", sf::Vector2f {(float)screen.width, (float)screen.height});

    auto sprite = sf::Sprite {};
    sprite.setTexture(texture.getTexture());

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
                    }
                } break;
            }
        }

        shader.setUniform("uCameraPos", sf::Vector3f {camera.pos});

        window.clear();
        window.draw(sprite, &shader);
        window.display();
    }

    return EXIT_SUCCESS;
}
