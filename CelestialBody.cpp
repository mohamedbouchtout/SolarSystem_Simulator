// Copyright 2025 by Mohamed Bouchtout

#include <iostream>
#include <string>
#include <memory>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Universe.hpp"
#include "CelestialBody.hpp"
using NB::Universe;
using NB::CelestialBody;

CelestialBody::CelestialBody(): _mass(0), _position({0, 0}),
_velocity({0, 0}), _fileName(""), _texture(nullptr), _sprite() {}

CelestialBody::CelestialBody(float mass): _mass(mass) {}

CelestialBody::CelestialBody(float mass, sf::Vector2f position,
                            sf::Vector2f velocity, std::string fileName) {
    _mass = mass;
    _position = position;
    _velocity = velocity;
    _force = {0.0, 0.0};
    _acceleration = {0.0, 0.0};
    _fileName = fileName;
    _texture = std::make_shared<sf::Texture>();

    if (!_texture->loadFromFile(_fileName)) {
        std::cout << "Error loading image to texture." << std::endl;
        exit(1);
    } else {
        _sprite.setTexture(*_texture);
    }
}

sf::Vector2f CelestialBody::position() const { return _position; }

sf::Vector2f CelestialBody::velocity() const { return _velocity; }

float CelestialBody::mass() const { return _mass; }

sf::Sprite& CelestialBody::sprite() { return _sprite; }

std::string CelestialBody::filename() const { return _fileName; }

sf::Vector2<double> CelestialBody::force() const { return _force; }

sf::Vector2<double> CelestialBody::acceleration()
const { return _acceleration; }



void CelestialBody::setPosition(sf::Vector2f position) { _position = position; }

void CelestialBody::setVelocity(sf::Vector2f velocity) { _velocity = velocity; }

void CelestialBody::setMass(float mass) { _mass = mass; }

void CelestialBody::setFileName(std::string filename) { _fileName = filename; }

void CelestialBody::setTexture(std::shared_ptr<sf::Texture> texture)
{ _texture = texture; }

void CelestialBody::setSprite(sf::Sprite sprite) { _sprite = sprite; }

void CelestialBody::updateSpritePosition(float scale) {
    sf::Vector2f scaledPosition = _position * scale;
    _sprite.setPosition(scaledPosition);
}

void CelestialBody::setForce(sf::Vector2<double> force) { _force = force; }

void CelestialBody::setAcceleration(sf::Vector2<double> acceleration)
{ _acceleration = acceleration; }

void CelestialBody::draw(sf::RenderTarget& window,
                        sf::RenderStates states) const {
    window.draw(_sprite, states);
}

namespace NB {
std::istream& operator>>(std::istream& is, CelestialBody& uni) {
    float mass = 0;
    sf::Vector2f position = {0, 0};
    sf::Vector2f velocity = {0, 0};
    std::string filename;

    is >> position.x >> position.y >>
    velocity.x >> velocity.y >> mass >> filename;

    uni.setMass(mass);
    uni.setPosition(position);
    uni.setVelocity(velocity);
    uni.setFileName(filename);

    std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();

    if (!texture->loadFromFile(filename)) {
        std::cout << "Error loading image to texture." << std::endl;
        exit(1);
    }

    uni.setTexture(texture);
    sf::Sprite sprite;
    sprite.setTexture(*texture);
    sprite.setPosition(position);
    uni.setSprite(sprite);

    return is;
}

std::ostream& operator<<(std::ostream& os, const CelestialBody& uni) {
    os << uni.position().x << " " << uni.position().y << " " <<
    uni.velocity().x << " " << uni.velocity().y << " " << uni.mass()
    << " " << uni.filename() << std::endl;

    return os;
}
}  // namespace NB
