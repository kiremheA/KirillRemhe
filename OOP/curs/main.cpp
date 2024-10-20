#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>

class SpaceObject {
protected:
    double size;
    sf::Color color;
    double positionX, positionY;

public:
    SpaceObject(double size, const sf::Color& color, double posX, double posY)
        : size(size), color(color), positionX(posX), positionY(posY) {}

    virtual ~SpaceObject() {}

    virtual void updatePosition(double time) = 0;

    virtual void draw(sf::RenderWindow& window) const {
        sf::CircleShape shape(size);
        shape.setFillColor(color);
        shape.setPosition(positionX, positionY);
        window.draw(shape);
    }

    double getPositionX() const { return positionX; }
    double getPositionY() const { return positionY; }
};

class Planet : public SpaceObject {
private:
    double orbitSpeed;
    double orbitMajorAxis;
    double orbitMinorAxis;
    double centerX, centerY;

public:
    Planet(double size, const sf::Color& color, double posX, double posY,
           double speed, double majorAxis, double minorAxis, double centerX, double centerY)
        : SpaceObject(size, color, posX, posY), orbitSpeed(speed),
          orbitMajorAxis(majorAxis), orbitMinorAxis(minorAxis),
          centerX(centerX), centerY(centerY) {}

    void updatePosition(double time) override {
        positionX = centerX + orbitMajorAxis * std::cos(orbitSpeed * time);
        positionY = centerY + orbitMinorAxis * std::sin(orbitSpeed * time);
    }
};

class Star : public SpaceObject {
public:
    Star(double size, const sf::Color& color, double posX, double posY)
        : SpaceObject(size, color, posX, posY) {}

    void updatePosition(double time) override {
    }
};

class SolarSystem {
private:
    std::vector<SpaceObject*> objects;

public:
    ~SolarSystem() {
        for (auto obj : objects) {
            delete obj;
        }
    }

    void addObject(SpaceObject* obj) {
        objects.push_back(obj);
    }

    void update(double time) {
        for (auto obj : objects) {
            obj->updatePosition(time);
        }
    }

    void draw(sf::RenderWindow& window) {
        for (auto obj : objects) {
            obj->draw(window);
        }
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Solar System Simulation");

    SolarSystem solarSystem;

    Star* sun = new Star(30.0, sf::Color::Yellow, 400.0, 300.0);
    Planet* earth = new Planet(10.0, sf::Color::Blue, 500.0, 300.0, 0.01, 150.0, 100.0, 400.0, 300.0);
    Planet* mars = new Planet(8.0, sf::Color::Red, 550.0, 300.0, 0.008, 200.0, 150.0, 400.0, 300.0);
    Planet* venus = new Planet(12.0, sf::Color::Green, 450.0, 300.0, 0.015, 100.0, 70.0, 400.0, 300.0);

    solarSystem.addObject(sun);
    solarSystem.addObject(earth);
    solarSystem.addObject(mars);
    solarSystem.addObject(venus);

    double time = 0.0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        time += 0.01;
        solarSystem.update(time);

        window.clear(sf::Color::Black);

        solarSystem.draw(window);

        window.display();
    }

    return 0;
}
