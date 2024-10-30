#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>

// Абстрактный класс SpaceObject
class SpaceObject {
protected:
    double size;
    sf::Color color;
    double positionX, positionY;

public:
    SpaceObject(double size, const sf::Color& color, double posX, double posY)
        : size(size), color(color), positionX(posX), positionY(posY) {}

    virtual ~SpaceObject() {}

    virtual void updatePosition(double time) = 0; // Чисто виртуальная функция

    virtual void draw(sf::RenderWindow& window) const {
        sf::CircleShape shape(size);
        shape.setFillColor(color);
        shape.setOrigin(size, size); // Центрируем объект
        shape.setPosition(positionX, positionY);
        window.draw(shape);
    }

    double getPositionX() const { return positionX; }
    double getPositionY() const { return positionY; }
};

// Класс Planet, наследник SpaceObject
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

// Класс Star, наследник SpaceObject
class Star : public SpaceObject {
public:
    Star(double size, const sf::Color& color, double posX, double posY)
        : SpaceObject(size, color, posX, posY) {}

    void updatePosition(double time) override {
        // Звезда остается на месте, не двигается
    }
};

// Класс SolarSystem для управления объектами
class SolarSystem {
private:
    std::vector<SpaceObject*> objects; // Массив указателей на космические объекты

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

// Основная программа
int main() {
    // Создание окна
    sf::RenderWindow window(sf::VideoMode(1000, 800), "Solar System Simulation");

    SolarSystem solarSystem;

    // Центр орбит
    const double centerX = 500.0;
    const double centerY = 400.0;

    // Создаем Солнце и все планеты
    Star* sun = new Star(30.0, sf::Color::Yellow, centerX, centerY);
    Planet* mercury = new Planet(4.0, sf::Color(169, 169, 169), centerX + 40.0, centerY, 0.04, 40.0, 35.0, centerX, centerY);
    Planet* venus = new Planet(6.0, sf::Color(255, 223, 0), centerX + 70.0, centerY, 0.015, 70.0, 60.0, centerX, centerY);
    Planet* earth = new Planet(8.0, sf::Color::Blue, centerX + 100.0, centerY, 0.01, 100.0, 90.0, centerX, centerY);
    Planet* mars = new Planet(6.0, sf::Color::Red, centerX + 150.0, centerY, 0.008, 150.0, 130.0, centerX, centerY);
    Planet* jupiter = new Planet(12.0, sf::Color(255, 165, 0), centerX + 200.0, centerY, 0.005, 200.0, 180.0, centerX, centerY);
    Planet* saturn = new Planet(10.0, sf::Color(210, 180, 140), centerX + 250.0, centerY, 0.003, 250.0, 220.0, centerX, centerY);
    Planet* uranus = new Planet(8.0, sf::Color(135, 206, 235), centerX + 300.0, centerY, 0.002, 300.0, 270.0, centerX, centerY);
    Planet* neptune = new Planet(8.0, sf::Color(0, 0, 139), centerX + 350.0, centerY, 0.0015, 350.0, 320.0, centerX, centerY);

    // Добавляем объекты в солнечную систему
    solarSystem.addObject(sun);
    solarSystem.addObject(mercury);
    solarSystem.addObject(venus);
    solarSystem.addObject(earth);
    solarSystem.addObject(mars);
    solarSystem.addObject(jupiter);
    solarSystem.addObject(saturn);
    solarSystem.addObject(uranus);
    solarSystem.addObject(neptune);

    double time = 0.0;

    // Главный цикл программы
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Обновляем позиции планет
        time += 0.01;
        solarSystem.update(time);

        // Очищаем окно
        window.clear(sf::Color::Black);

        // Рисуем объекты
        solarSystem.draw(window);

        // Отображаем окно
        window.display();
    }

    return 0;
}
