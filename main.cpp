// Copyright 2025 by Mohamed Bouchtout

#include <iostream>
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Universe.hpp"
#include "CelestialBody.hpp"
using NB::Universe;
using NB::CelestialBody;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "No file name found\n";
        return 1;
    }

    double time = std::stod(argv[1]);
    double deltaTime = std::stod(argv[2]);

    Universe universe;
    std::cin >> universe;

    double currentTime = 0.0;
    sf::Clock clock;

    sf::Font font;
    if (!font.loadFromFile("font.ttf")) {
        std::cerr << "Error: Failed to load font.\n";
        return 1;
    }

    sf::Text timeText;
    timeText.setFont(font);
    timeText.setCharacterSize(24);
    timeText.setFillColor(sf::Color::White);
    timeText.setPosition(-200, -380);

    sf::RenderWindow window(sf::VideoMode(800, 800), "The Solar System!");
    sf::View view;
    view.setSize(800, 800);
    view.setCenter(0, 0);
    window.setView(view);

    while (window.isOpen() && currentTime < time) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        universe.step(deltaTime);
        currentTime += deltaTime;

        std::ostringstream timeStream;
        timeStream.precision(2);
        timeStream << std::fixed << currentTime;
        timeText.setString("Elapsed Time: " + timeStream.str() + " s");

        window.clear();
        window.draw(universe);
        window.draw(timeText);
        window.display();
    }

    std::cout << universe << std::endl;
    return 0;
}
