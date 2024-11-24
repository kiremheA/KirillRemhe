#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <cmath>
#include <string>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

template <typename T = double>
class SpaceObject {
protected:
    T size;
    sf::Color color;

public:
    SpaceObject(T size = 10, const sf::Color& color = sf::Color::White)
        : size(size), color(color) {}

    virtual ~SpaceObject() = default;

    virtual void updatePosition(T time) = 0;

    virtual void draw(sf::RenderWindow& window) const = 0;

    virtual std::string getName() const { return "SpaceObject"; }
};

template <typename T = double>
class OrbitCalculator {
protected:
    T orbitSpeed;
    T orbitMajorAxis;
    T orbitMinorAxis;
    T centerX, centerY;

public:
    OrbitCalculator(T speed = 0.01, T majorAxis = 50, T minorAxis = 40, T centerX = 0, T centerY = 0)
        : orbitSpeed(speed), orbitMajorAxis(majorAxis), orbitMinorAxis(minorAxis), centerX(centerX), centerY(centerY) {}

    std::pair<T, T> calculatePosition(T time) const {
        T x = centerX + orbitMajorAxis * std::cos(orbitSpeed * time);
        T y = centerY + orbitMinorAxis * std::sin(orbitSpeed * time);
        return {x, y};
    }

    std::vector<sf::Vector2f> calculateOrbitPoints(int numPoints = 80) const {
        std::vector<sf::Vector2f> points;
        for (int i = 0; i <= numPoints; ++i) {
            T angle = i * 2 * M_PI / numPoints;
            T x = centerX + orbitMajorAxis * std::cos(angle);
            T y = centerY + orbitMinorAxis * std::sin(angle);
            points.emplace_back(x, y);
        }
        return points;
    }
};

template <typename T = double>
class Planet : public SpaceObject<T> {
private:
    OrbitCalculator<T> orbitCalculator;
    T positionX, positionY;
    std::string name;
    std::vector<sf::Vector2f> orbitPoints;

public:
    Planet(const std::string& name, T size = 5, const sf::Color& color = sf::Color::Green,
           T orbitSpeed = 0.01, T majorAxis = 50, T minorAxis = 40, T centerX = 0, T centerY = 0)
        : SpaceObject<T>(size, color), orbitCalculator(orbitSpeed, majorAxis, minorAxis, centerX, centerY), name(name) {
        orbitPoints = orbitCalculator.calculateOrbitPoints();
    }

    void updatePosition(T time) override {
        auto [x, y] = orbitCalculator.calculatePosition(time);
        positionX = x;
        positionY = y;
    }

    void drawOrbit(sf::RenderWindow& window) const {
        sf::VertexArray orbit(sf::LineStrip, orbitPoints.size());
        for (size_t i = 0; i < orbitPoints.size(); ++i) {
            orbit[i].position = orbitPoints[i];
            orbit[i].color = sf::Color(150, 150, 150);
        }
        window.draw(orbit);
    }

    void draw(sf::RenderWindow& window) const override {
        drawOrbit(window);

        sf::CircleShape shape(this->size);
        shape.setFillColor(this->color);
        shape.setOrigin(this->size, this->size);
        shape.setPosition(positionX, positionY);
        window.draw(shape);
    }

    std::string getName() const override { return name; }
};

template <typename T = double>
class Star : public SpaceObject<T> {
private:
    T positionX, positionY;
    std::string name;

public:
    Star(const std::string& name, T size = 20, const sf::Color& color = sf::Color::Yellow, T posX = 0, T posY = 0)
        : SpaceObject<T>(size, color), positionX(posX), positionY(posY), name(name) {}

    void updatePosition(T time) override {}

    void draw(sf::RenderWindow& window) const override {
        sf::CircleShape shape(this->size);
        shape.setFillColor(this->color);
        shape.setOrigin(this->size, this->size);
        shape.setPosition(positionX, positionY);
        window.draw(shape);
    }

    std::string getName() const override { return name; }
};

class SolarSystem {
private:
    std::vector<std::unique_ptr<SpaceObject<double>>> objects;

public:
    void addObject(std::unique_ptr<SpaceObject<double>> obj) {
        std::cout << "Добавлен объект: " << obj->getName() << std::endl;
        objects.push_back(std::move(obj));
    }

    void update(double time) {
        for (auto& obj : objects) {
            obj->updatePosition(time);
        }
    }

    void draw(sf::RenderWindow& window) {
        for (const auto& obj : objects) {
            obj->draw(window);
        }
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Realistic Solar System Simulation");

    SolarSystem solarSystem;

    const double centerX = 960.0;
    const double centerY = 540.0;

    solarSystem.addObject(std::make_unique<Star<>>("Солнце", 30.0, sf::Color::Yellow, centerX, centerY));

    solarSystem.addObject(std::make_unique<Planet<>>(
        "Меркурий", 4.0, sf::Color(169, 169, 169), 2 * M_PI / 88, 40.0, 35.0, centerX, centerY));

    solarSystem.addObject(std::make_unique<Planet<>>(
        "Венера", 6.0, sf::Color(218, 165, 32), 2 * M_PI / 225, 70.0, 65.0, centerX, centerY));

    solarSystem.addObject(std::make_unique<Planet<>>(
        "Земля", 8.0, sf::Color::Blue, 2 * M_PI / 365, 100.0, 90.0, centerX, centerY));

    solarSystem.addObject(std::make_unique<Planet<>>(
        "Марс", 6.0, sf::Color::Red, 2 * M_PI / 687, 150.0, 130.0, centerX, centerY));

    solarSystem.addObject(std::make_unique<Planet<>>(
        "Юпитер", 12.0, sf::Color(255, 165, 0), 2 * M_PI / (12 * 365), 200.0, 180.0, centerX, centerY));

    solarSystem.addObject(std::make_unique<Planet<>>(
        "Сатурн", 10.0, sf::Color(210, 180, 140), 2 * M_PI / (29 * 365), 250.0, 220.0, centerX, centerY));

    solarSystem.addObject(std::make_unique<Planet<>>(
        "Уран", 8.0, sf::Color(135, 206, 235), 2 * M_PI / (84 * 365), 300.0, 270.0, centerX, centerY));

    solarSystem.addObject(std::make_unique<Planet<>>(
        "Нептун", 8.0, sf::Color(0, 0, 139), 2 * M_PI / (165 * 365), 350.0, 320.0, centerX, centerY));

    sf::Clock clock;
    double timeScale = 1.0 / 86400.0;
    double totalTime = 0.0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    timeScale *= 2.0;
                }
                if (event.key.code == sf::Keyboard::Down) {
                    timeScale /= 2.0;
                    if (timeScale < 1.0 / 86400.0) {
                        timeScale = 1.0 / 86400.0;
                    }
                }
            }
        }

        double elapsedSeconds = clock.restart().asSeconds();
        totalTime += elapsedSeconds * timeScale;

        solarSystem.update(totalTime);

        window.clear(sf::Color::Black);
        solarSystem.draw(window);
        window.display();
    }

    return 0;
}
