// Copyright 2025 by Mohamed Bouchtout

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "CelestialBody.hpp"

namespace NB {
class Universe: public sf::Drawable {
 public:
    Universe();
    explicit Universe(const std::string& filename);   // Optional

    size_t size() const;  // Optional
    double radius() const;  // Optional
    const std::vector<std::shared_ptr<NB::CelestialBody>>& list() const;

    void setSize(size_t size);
    void setRadius(double radius);
    void addToList(std::shared_ptr<NB::CelestialBody> ptr);
    void clearList();

    const CelestialBody& operator[](size_t i) const;  // Optional

    void step(double dt);  // Implemented in part b,
                           // behavior for part a is undefined
 protected:
    void draw(sf::RenderTarget& window, sf::RenderStates states)
    const override;  // From sf::Drawable

 private:
    size_t _size;
    double _radius;
    std::string _fileName;
    sf::Vector2f _windowSize;
    std::vector<std::shared_ptr<NB::CelestialBody>> _list;
    // Fields and helper functions go here
};

std::istream& operator>>(std::istream& is, Universe& uni);
std::ostream& operator<<(std::ostream& os, const Universe& uni);
}  // namespace NB
