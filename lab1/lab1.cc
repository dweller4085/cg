#include <SFML/System/Clock.hpp>
#include <SFML/Graphics.hpp>
#include "common.hh"
#include "scene.hh"
#include "lab1.hh"

int main (int argc, char ** argv) {
    
    let window = sf::RenderWindow {
        sf::VideoMode { settings::screen.width, settings::screen.height },
        settings::window_name,
        sf::Style::Default,
        sf::ContextSettings {
            0,
            0,
            settings::antialiasing,
            1,
            1,
            sf::ContextSettings::Default,
            false
        }
    };

    let event = sf::Event {};
    let clock = sf::Clock {};
    let dt = 1.f / 60.f * 1000000.f;

    let scene = Scene {};
    scene.animation.is_playing = settings::animation.play_on_startup;


    while (window.isOpen ()) {
        while (window.pollEvent (event)) {
            if (event.type == sf::Event::Closed) {
                window.close ();
            }

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Escape: {
                        window.close ();
                    } break;

                    case sf::Keyboard::P: {
                        if (scene.animation.is_playing) {
                            scene.stopAnimation ();
                        }
                        else {
                            if (scene.animation.progress > 1.f) {
                                scene.resetAnimation ();
                            }

                            scene.startAnimation ();
                        }
                    } break;

                    case sf::Keyboard::R: {
                        scene.resetAnimation ();
                    } break;
                }
            }
        }

        scene.update (dt);

        window.clear ();
        window.draw (scene);
        window.display ();

        dt = clock.restart().asMicroseconds();
    }

    return EXIT_SUCCESS;
}
