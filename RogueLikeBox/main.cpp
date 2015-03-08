
//
// Disclamer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resource, use the helper
// method resourcePath() from ResourcePath.hpp
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
// Here is a small helper for you ! Have a look.
#include "ResourcePath.hpp"
#include "Game.h"

// This is a simple example of building and running a simulation
// using Box2D. Here we create a large ground box and a small dynamic
// box.
// There are no graphics for this example. Box2D is meant to be used
// with your rendering engine in your game engine.
int main(int argc, char** argv)
{
    srand(time(NULL));
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML window");

    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    Game game;
    game.Init();

    sf::Clock deltaClock;

    float accumulatedTime = 0.0f;
    int commandFrame=0;
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
        
        accumulatedTime += deltaClock.getElapsedTime().asSeconds();
        deltaClock.restart();
       
        static double UPDATE_INTERVAL = 1.0f/60.0f;
        static double MAX_CYCLES_PER_FRAME = 10;
        if (accumulatedTime > (MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL)) {
            accumulatedTime = MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL;
        }
        
        while (accumulatedTime>=UPDATE_INTERVAL)
        {
            accumulatedTime -= UPDATE_INTERVAL;
            game.Update(UPDATE_INTERVAL);
            commandFrame++;
        }
        game.Draw(&window);
    }

    return EXIT_SUCCESS;
}
