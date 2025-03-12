// Copyright 2025 by Mohamed Bouchtout

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <SFML/Graphics.hpp>
#include <boost/test/unit_test.hpp>
#include "Universe.hpp"
#include "CelestialBody.hpp"
using NB::Universe;
using NB::CelestialBody;

static const double G = 6.67430e-11;

BOOST_AUTO_TEST_CASE(CelestialBody_InputStream) {
    std::istringstream input("1 2 3 4 5 earth.gif");

    CelestialBody body(2.50e+11);
    input >> body;

    BOOST_CHECK_CLOSE(body.position().x, 1.0, 1e-6);
    BOOST_CHECK_CLOSE(body.position().y, 2.0, 1e-6);
    BOOST_CHECK_CLOSE(body.velocity().x, 3.0, 1e-6);
    BOOST_CHECK_CLOSE(body.velocity().y, 4.0, 1e-6);
    BOOST_CHECK_CLOSE(body.mass(), 5.0, 1e-6);
}

BOOST_AUTO_TEST_CASE(CelestialBody_OutputStream) {
    std::istringstream input("1 2 3 4 5 earth.gif");

    CelestialBody body(2.50e+11);
    input >> body;

    std::ostringstream output;
    output << body;

    std::string expected = "1 2 3 4 5 earth.gif";
    BOOST_CHECK_EQUAL(output.str(), expected);
}

BOOST_AUTO_TEST_CASE(Universe_InputStream) {
    std::istringstream input("2\n"
                             "100\n"
                             "1 2 3 4 5 earth.gif\n"
                             "6 7 8 9 10 mars.gif\n");

    Universe universe;
    input >> universe;

    BOOST_CHECK_EQUAL(universe.size(), 2);
    BOOST_CHECK_CLOSE(universe.radius(), 100.0, 1e-6);
    BOOST_CHECK_CLOSE(universe[0].position().x, 1.0, 1e-6);
    BOOST_CHECK_CLOSE(universe[1].position().x, 6.0, 1e-6);
}

BOOST_AUTO_TEST_CASE(Universe_OutputStream) {
    Universe universe;
    std::istringstream input("2\n"
                             "100\n"
                             "1 2 3 4 5 earth.gif\n"
                             "6 7 8 9 10 mars.gif\n");
    input >> universe;

    std::ostringstream output;
    output << universe;

    std::string expected = "2\n"
                           "100\n"
                           "1 2 3 4 5 earth.gif\n"
                           "6 7 8 9 10 mars.gif\n";

    BOOST_CHECK_EQUAL(output.str(), expected);
}

BOOST_AUTO_TEST_CASE(Universe_Step_NoAcceleration) {
    std::istringstream is(
"5\n"
"2.50e+11\n"
"1.4960e+11  0.0000e+00  0.0000e+00  2.9800e+04  5.9740e+24    earth.gif\n"
"2.2790e+11  0.0000e+00  0.0000e+00  2.4100e+04  6.4190e+23     mars.gif\n"
"5.7900e+10  0.0000e+00  0.0000e+00  4.7900e+04  3.3020e+23  mercury.gif\n"
"0.0000e+00  0.0000e+00  0.0000e+00  0.0000e+00  1.9890e+30      sun.gif\n"
"1.0820e+11  0.0000e+00  0.0000e+00  3.5000e+04  4.8690e+24    venus.gif\n");

    Universe universe;
    is >> universe;

    std::vector<sf::Vector2f> initialPositions;
    std::vector<sf::Vector2f> initialVelocities;
    for (size_t i = 0; i < universe.size(); i++) {
        initialPositions.push_back(universe[i].position());
        initialVelocities.push_back(universe[i].velocity());
    }

    universe.step(3600.0);

    for (size_t i = 0; i < universe.size(); i++) {
        BOOST_CHECK(universe[i].position() != initialPositions[i]);
        BOOST_CHECK(universe[i].velocity() != initialVelocities[i]);
    }
}

BOOST_AUTO_TEST_CASE(Universe_Step_AntiGravity) {
    std::istringstream is(
"5\n"
"2.50e+11\n"
"1.4960e+11  0.0000e+00  0.0000e+00  2.9800e+04  5.9740e+24    earth.gif\n"
"2.2790e+11  0.0000e+00  0.0000e+00  2.4100e+04  6.4190e+23     mars.gif\n"
"5.7900e+10  0.0000e+00  0.0000e+00  4.7900e+04  3.3020e+23  mercury.gif\n"
"0.0000e+00  0.0000e+00  0.0000e+00  0.0000e+00  1.9890e+30      sun.gif\n"
"1.0820e+11  0.0000e+00  0.0000e+00  3.5000e+04  4.8690e+24    venus.gif\n");

    Universe universe;
    is >> universe;

    for (size_t i = 0; i < universe.size(); i++) {
        for (size_t j = 0; j < universe.size(); j++) {
            if (i != j) {
                sf::Vector2f diff = universe[j].position()
                - universe[i].position();

                float distanceSquared = diff.x * diff.x + diff.y * diff.y;
                float forceMagnitude = G * universe[i].mass()
                * universe[j].mass() / distanceSquared;

                sf::Vector2f direction = diff / std::sqrt(distanceSquared);
                sf::Vector2f force = direction * forceMagnitude;

                BOOST_CHECK(force.x * diff.x + force.y * diff.y > 0);
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(Universe_Step_InvertedAxis) {
    std::istringstream is(
"5\n"
"2.50e+11\n"
"1.4960e+11  0.0000e+00  0.0000e+00  2.9800e+04  5.9740e+24    earth.gif\n"
"2.2790e+11  0.0000e+00  0.0000e+00  2.4100e+04  6.4190e+23     mars.gif\n"
"5.7900e+10  0.0000e+00  0.0000e+00  4.7900e+04  3.3020e+23  mercury.gif\n"
"0.0000e+00  0.0000e+00  0.0000e+00  0.0000e+00  1.9890e+30      sun.gif\n"
"1.0820e+11  0.0000e+00  0.0000e+00  3.5000e+04  4.8690e+24    venus.gif\n");

    Universe universe;
    is >> universe;

    for (size_t i = 0; i < universe.size(); i++) {
        BOOST_CHECK(universe[i].position().x >= 0);
        BOOST_CHECK(universe[i].position().y >= 0);
    }

    universe.step(3600.0);

    for (size_t i = 0; i < universe.size(); i++) {
        BOOST_CHECK(universe[i].position().x >= 0);
        BOOST_CHECK(universe[i].position().y >= 0);
    }
}

BOOST_AUTO_TEST_CASE(Universe_Step_FixedDeltas) {
    std::istringstream is(
"5\n"
"2.50e+11\n"
"1.4960e+11  0.0000e+00  0.0000e+00  2.9800e+04  5.9740e+24    earth.gif\n"
"2.2790e+11  0.0000e+00  0.0000e+00  2.4100e+04  6.4190e+23     mars.gif\n"
"5.7900e+10  0.0000e+00  0.0000e+00  4.7900e+04  3.3020e+23  mercury.gif\n"
"0.0000e+00  0.0000e+00  0.0000e+00  0.0000e+00  1.9890e+30      sun.gif\n"
"1.0820e+11  0.0000e+00  0.0000e+00  3.5000e+04  4.8690e+24    venus.gif\n");

    Universe universe;
    is >> universe;

    std::vector<sf::Vector2f> initialPositions;
    for (size_t i = 0; i < universe.size(); i++) {
        initialPositions.push_back(universe[i].position());
    }

    universe.step(3600.0);

    for (size_t i = 0; i < universe.size(); i++) {
        BOOST_CHECK(universe[i].position() != initialPositions[i]);
    }
}

BOOST_AUTO_TEST_CASE(Universe_Step_Leapfrog) {
    std::istringstream is(
"5\n"
"2.50e+11\n"
"1.4960e+11  0.0000e+00  0.0000e+00  2.9800e+04  5.9740e+24    earth.gif\n"
"2.2790e+11  0.0000e+00  0.0000e+00  2.4100e+04  6.4190e+23     mars.gif\n"
"5.7900e+10  0.0000e+00  0.0000e+00  4.7900e+04  3.3020e+23  mercury.gif\n"
"0.0000e+00  0.0000e+00  0.0000e+00  0.0000e+00  1.9890e+30      sun.gif\n"
"1.0820e+11  0.0000e+00  0.0000e+00  3.5000e+04  4.8690e+24    venus.gif\n");

    Universe universe;
    is >> universe;

    for (size_t i = 0; i < universe.size(); i++) {
        for (size_t j = 0; j < universe.size(); j++) {
            if (i != j) {
                BOOST_CHECK(universe[i].position() != universe[j].position());
            }
        }
    }

    universe.step(3600.0);

    for (size_t i = 0; i < universe.size(); i++) {
        for (size_t j = 0; j < universe.size(); j++) {
            if (i != j) {
                BOOST_CHECK(universe[i].position() != universe[j].position());
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(Universe_Step_ExtraCredit) {
    std::istringstream is(
"5\n"
"2.50e+11\n"
"1.4960e+11  0.0000e+00  0.0000e+00  2.9800e+04  5.9740e+24    earth.gif\n"
"2.2790e+11  0.0000e+00  0.0000e+00  2.4100e+04  6.4190e+23     mars.gif\n"
"5.7900e+10  0.0000e+00  0.0000e+00  4.7900e+04  3.3020e+23  mercury.gif\n"
"0.0000e+00  0.0000e+00  0.0000e+00  0.0000e+00  1.9890e+30      sun.gif\n"
"1.0820e+11  0.0000e+00  0.0000e+00  3.5000e+04  4.8690e+24    venus.gif\n");

    Universe universe;
    is >> universe;

    std::vector<sf::Vector2f> initialPositions;
    for (size_t i = 0; i < universe.size(); i++) {
        initialPositions.push_back(universe[i].position());
    }

    universe.step(3600.0);

    for (size_t i = 0; i < universe.size(); i++) {
        BOOST_CHECK(universe[i].position() != initialPositions[i]);
    }
}
