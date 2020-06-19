#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Part
{
private:
    sf::RectangleShape body;
    sf::Vector2u coords;
    int index;
    unsigned int direction, dQueue[324];
    float switchT, totalT;

public:
    bool exists;

    static unsigned int counter;

    Part();

    void setIndex(int,float);
    void setSize(float);
    void setCoords(sf::Vector2u);

    int getIndex() { return index; }
    sf::Vector2u getCoords() { return coords; }
    unsigned int getDirection() { return direction; }

    void turn(unsigned int);
    void update(float, sf::RenderWindow&, Part&);
    void draw(sf::RenderWindow& window) { window.draw(body); }
};

#endif // PART_H
