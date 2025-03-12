// Copyright 2025 by Mohamed Bouchtout

#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <cmath>
#include <SFML/Graphics.hpp>

namespace NB {
class CelestialBody: public sf::Drawable {
 public:
    explicit CelestialBody();
    explicit CelestialBody(float mass);
    explicit CelestialBody(float mass, sf::Vector2f position,
               sf::Vector2f velocity, std::string fileName);  // Required

    sf::Vector2f position() const;  // Optional
    sf::Vector2f velocity() const;  // Optional
    float mass() const;  // Optional
    sf::Sprite& sprite();
    std::string filename() const;
    sf::Vector2<double> force() const;
    sf::Vector2<double> acceleration() const;

    void setPosition(sf::Vector2f position);
    void setVelocity(sf::Vector2f velocity);
    void setFileName(std::string filename);
    void setMass(float mass);
    void setTexture(std::shared_ptr<sf::Texture> texture);
    void setSprite(sf::Sprite sprite);
    void updateSpritePosition(float scale);
    void setForce(sf::Vector2<double> force);
    void setAcceleration(sf::Vector2<double> acceleration);

 protected:
    void draw(sf::RenderTarget& window,
      sf::RenderStates states) const override;  // From sf::Drawable

 private:
    float _mass;
    sf::Vector2f _position;
    sf::Vector2f _velocity;
    sf::Vector2<double> _force;
    sf::Vector2<double> _acceleration;
    std::string _fileName;
    std::shared_ptr<sf::Texture> _texture;
    sf::Sprite _sprite;
    // Fields and helper methods go here
};

std::istream& operator>>(std::istream& is, CelestialBody& uni);
std::ostream& operator<<(std::ostream& os, const CelestialBody& uni);
}  // namespace NB
