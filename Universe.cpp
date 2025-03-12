// Copyright 2025 by Mohamed Bouchtout

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Universe.hpp"
#include "CelestialBody.hpp"
using NB::Universe;
using NB::CelestialBody;

static const double G = 6.67430e-11;

Universe::Universe(): _size(0), _radius(0.0),
_fileName(""), _windowSize({800, 800}), _list() {}

Universe::Universe(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    _fileName = filename;
    _windowSize = {800, 800};

    if (!file.is_open()) {
        std::cout << "file did not open.\n";
        exit(1);
    }

    if (std::getline(file, line)) {
        std::istringstream iss(line);
        iss >> _size;
    } else {
        std::cout << "Error: File is empty or missing size value.\n";
        exit(1);
    }

    if (std::getline(file, line)) {
        std::istringstream iss(line);
        iss >> _radius;
    } else {
        std::cout << "Error: Missing radius value.\n";
        exit(1);
    }

    float scale = (_windowSize.x / 2) / _radius;

    for (size_t i = 0; i < _size; i++) {
        if (std::getline(file, line)) {
            std::istringstream iss(line);
            float posX, posY, velX, velY, mass;
            std::string fileName;

            if (!(iss >> posX >> posY >> velX >> velY >> mass >> fileName)) {
                std::cout << "Error parsing line: " << line << "\n";
                exit(1);
            }

            sf::Vector2f position = sf::Vector2f(posX, posY);
            sf::Vector2f velocity = sf::Vector2f(velX, velY);

            auto obj = std::make_shared<NB::CelestialBody>
            (mass, position, velocity, fileName);

            obj->updateSpritePosition(scale);

            _list.push_back(obj);

        } else {
            std::cout << "Error: Missing data\n";
            exit(1);
        }
    }
}

size_t Universe::size() const { return _size; }

double Universe::radius() const { return _radius; }

const std::vector<std::shared_ptr<NB::CelestialBody>>&
Universe::list() const { return _list; }

void Universe::setSize(size_t size) { _size = size; }

void Universe::setRadius(double radius) { _radius = radius; }

void Universe::addToList(std::shared_ptr<NB::CelestialBody> ptr) {
    _list.push_back(ptr);
}

void Universe::clearList() { _list.clear(); }

const CelestialBody& Universe::operator[](size_t i) const {
    return *_list[i];
}

void Universe::step(double dt) {
    float scale = (_windowSize.x / 2) / _radius;
    std::vector<sf::Vector2<double>> forces(_size);
    std::vector<sf::Vector2<double>> newPositions(_size);
    std::vector<sf::Vector2<double>> newVelocities(_size);

    for (size_t i = 0; i < _size; i++) {
        for (size_t j = 0; j < _size; j++) {
            if (i != j) {
                sf::Vector2f diff = _list[j]->position() - _list[i]->position();
                double distanceSquared = diff.x * diff.x + diff.y * diff.y;
                double distance = std::sqrt(distanceSquared);
                double forceMagnitude = G * _list[i]->mass()
                * _list[j]->mass() / distanceSquared;

                sf::Vector2<double> direction = sf::Vector2<double>
                (diff.x / distance, diff.y / distance);

                sf::Vector2<double> force = sf::Vector2<double>
                (direction.x * forceMagnitude, direction.y * forceMagnitude);
                forces[i] += force;
            }
        }

        double mass = _list[i]->mass();
        _list[i]->setForce(forces[i]);
        _list[i]->setAcceleration(forces[i] / mass);
    }

    for (size_t i = 0; i < _size; i++) {
        sf::Vector2<double> velocity(_list[i]->velocity());
        sf::Vector2<double> position(_list[i]->position());

        sf::Vector2<double> newVelocity = velocity
        + _list[i]->acceleration() * dt;

        sf::Vector2<double> newPosition = position + newVelocity * dt;

        newVelocities[i] = newVelocity;
        newPositions[i] = newPosition;
    }

    for (size_t i = 0; i < _size; i++) {
        sf::Vector2f velocity(newVelocities[i]);
        sf::Vector2f position(newPositions[i]);

        _list[i]->setVelocity(velocity);
        _list[i]->setPosition(position);
        _list[i]->updateSpritePosition(scale);
    }
}

void Universe::draw(sf::RenderTarget& window, sf::RenderStates states) const {
    for (const auto& obj : _list) {
        window.draw(obj->sprite(), states);
    }
}

namespace NB {
std::istream& operator>>(std::istream& is, Universe& uni) {
    float size;
    double radius;

    is >> size >> radius;
    uni.setSize(size);
    uni.setRadius(radius);
    uni.clearList();

    float scale = (800 / 2) / radius;

    for (size_t i = 0; i < uni.size(); i++) {
        auto obj = std::make_shared<NB::CelestialBody>();

        is >> *obj;
        obj->updateSpritePosition(scale);
        uni.addToList(obj);
    }

    return is;
}

std::ostream& operator<<(std::ostream& os, const Universe& uni) {
    os << uni.size() << std::endl << uni.radius() << std::endl;
    for (size_t i = 0; i < uni.size(); i++) {
        os << uni[i];
    }

    return os;
}
}   // namespace NB
