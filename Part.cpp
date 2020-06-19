#include "Part.h"

Part::Part()
{

}

void Part::setIndex(int index, float switchT)
{
    exists = false;
    direction = 6;
    for (int i=0; i<324; i++) dQueue[i] = 0;

    this -> switchT = switchT;
    this->index = index;
    body.setFillColor(sf::Color(255, std::min(255,index), std::min(255,index)));
    if (index<3)
    {
        coords = sf::Vector2u(3-index, 8);
        exists = true;
    }


}

void Part::setSize(float width)
{
    body.setSize(sf::Vector2f(0.05f*width-5.f, 0.05f*width-5.f));
}

void Part::setCoords(sf::Vector2u c)
{
    coords = c;
}

void Part::turn(unsigned int dir)
{
    for (int i=0; i<324; i++)
    {
        if (dQueue[i] == 0)
        {
            if (i == 0)
            {
                if (abs(dir-direction) != 2 && dir != direction) dQueue[i] = dir;
            }
            else
            {
                if (abs(dir-dQueue[i-1]) != 2 && dir != dQueue[i-1]) dQueue[i] = dir;
            }
            break;
        }
    }

    //if (abs(dir-direction) != 2) direction = dir;
}

void Part::update(float deltaT, sf::RenderWindow& window, Part& next)
{
    totalT += deltaT;
    if (totalT >= switchT)
    {
        totalT -= switchT;

        if (dQueue[0] != direction && dQueue[0] != 0)
        {
            direction = dQueue[0];
            for (int i=0; i<323; i++)
            {
                dQueue[i] = dQueue[i+1];
            }
            dQueue[323] = 0;
        }

        switch (direction)
        {
        case 1: coords.y--; break;
        case 2: coords.x++; break;
        case 3: coords.y++; break;
        case 4: coords.x--; break;
        }

        next.turn(direction);
    }

    body.setPosition(sf::Vector2f((coords.x+1)*window.getSize().x*0.05f + 2.5f, (coords.y+1)*window.getSize().x*0.05f + window.getSize().y-window.getSize().x + 2.5f));
}

