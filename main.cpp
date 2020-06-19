#include <SFML/Graphics.hpp>
#include <string>
#include <time.h>

#include "Part.h"


unsigned int Part::counter = 0;

int main()
{
    srand(time(NULL));

    //constants
    unsigned int
    SCRWIDTH = 600,
    SCRHEIGHT = 640;
    float
    SPEED = 0.15f;


    bool game = false;
    bool failed = false;
    bool feedColliding = false;

    sf::Vector2u newCoords;
    Part* next;

    float deltaT;
    sf::Clock clock;

    unsigned int turnD = 0;

    sf::RenderWindow window(sf::VideoMode(SCRWIDTH, SCRHEIGHT), "Snake", sf::Style::Close | sf::Style::Titlebar);

    sf::RectangleShape board;
    board.setFillColor(sf::Color(255, 238, 168));
    board.setOutlineColor(sf::Color(255, 200, 0));
    board.setOutlineThickness(5.f);
    board.setSize(sf::Vector2f(SCRWIDTH * 0.9f, SCRWIDTH * 0.9f));
    board.setPosition(sf::Vector2f(0.05*SCRWIDTH, SCRHEIGHT-SCRWIDTH+0.05*SCRWIDTH));

    sf::Font font;
    font.loadFromFile("fonts/font.ttf");

    sf::Text title;
    title.setFont(font);
    title.setString("SNAKE");
    title.setCharacterSize(50);
    title.setOrigin(title.getLocalBounds().left + title.getLocalBounds().width / 2.f, title.getLocalBounds().top);
    title.setPosition(sf::Vector2f(SCRWIDTH/2.f, 10.f));

    sf::Text* gameOver = new sf::Text;
    gameOver->setFont(font);
    gameOver->setString("GAME OVER");
    gameOver->setCharacterSize(70);
    gameOver->setOrigin(gameOver->getLocalBounds().left + gameOver->getLocalBounds().width / 2.f, gameOver->getLocalBounds().top);
    gameOver->setPosition(SCRWIDTH / 2.f, SCRWIDTH / 2.f + SCRHEIGHT - SCRWIDTH - 65.f);
    gameOver->setFillColor(sf::Color::Black);

    sf::Text* gameOver2 = new sf::Text;
    gameOver2->setFont(font);
    gameOver2->setString("PRESS ENTER TO PLAY AGAIN");
    gameOver2->setCharacterSize(27);
    gameOver2->setOrigin(gameOver2->getLocalBounds().left + gameOver2->getLocalBounds().width / 2.f, gameOver2->getLocalBounds().top);
    gameOver2->setPosition(SCRWIDTH / 2.f, SCRWIDTH / 2.f + SCRHEIGHT - SCRWIDTH);
    gameOver2->setFillColor(sf::Color::Black);

    Part snake[324];
    Part* pointer = snake;
    for (int i=0; i<324; i++)
    {
        pointer->setIndex(i, SPEED);
        pointer->setSize(SCRWIDTH);
        if (i<3) Part::counter++;
        pointer++;
    }

    sf::Vector2u feedCoords(10,8);
    sf::RectangleShape feed;
    feed.setSize(sf::Vector2f(0.05f * SCRWIDTH - 5.f, 0.05f * SCRWIDTH - 5.f));
    feed.setFillColor(sf::Color(0,192,0));

    sf::Text score;
    score.setFont(font);
    score.setCharacterSize(27);
    score.setPosition(sf::Vector2f(5.f, 2.f));
    score.setString("Score: "+std::to_string(Part::counter - 3));


    //main loop
    while (window.isOpen())
    {

        deltaT = clock.restart().asSeconds();

        //event loop
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            switch (ev.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (ev.key.code == sf::Keyboard::Enter)
                {
                    if (failed)
                    {
                    Part::counter = 0;
                    pointer = snake;
                    for (int i=0; i<324; i++)
                    {
                        pointer->setIndex(i, SPEED);
                        if (i<3) Part::counter++;
                        pointer++;
                    }
                    feedCoords = sf::Vector2u(10, 8);
                    }
                    failed = false;
                }
                else
                {

                if (ev.key.code == sf::Keyboard::D || ev.key.code == sf::Keyboard::Right) turnD = 2;
                else if (ev.key.code == sf::Keyboard::A || ev.key.code == sf::Keyboard::Left) turnD = 4;
                else if (ev.key.code == sf::Keyboard::W || ev.key.code == sf::Keyboard::Up) turnD = 1;
                else if (ev.key.code == sf::Keyboard::S || ev.key.code == sf::Keyboard::Down) turnD = 3;
                if (turnD != 0)
                {
                    snake[0].turn(turnD);
                    if (!game && !failed)
                    {
                        game = true;
                        snake[1].turn(2);
                        snake[2].turn(2);
                    }
                }

                }
                break;
            default:
                break;
            }
        } //event loop


        window.clear(sf::Color(255, 217, 31));
        window.draw(board);
        window.draw(title);

        if (!failed)
        {

        pointer = &snake[323];
        for (int i=323; i>=0; i--)
        {
            pointer->update(deltaT, window, *(pointer+1));
            if (pointer->exists)
            {
                pointer->draw(window);
            }
            pointer--;
        }

        pointer = &snake[1];
        for (int i=1; i<324; i++)
        {
            if (pointer->getCoords() == snake->getCoords() && pointer->exists)
            {
                game = false;
                failed = true;
            }
            pointer++;
        }

        if (snake[0].getCoords().x > 17 || snake[0].getCoords().y > 17)
        {
            game = false;
            failed = true;
        }

        if (snake[0].getCoords() == feedCoords)
        {
            Part::counter++;

            pointer = snake;
            for (int i=0; i<324; i++)
            {
                if (!pointer->exists)
                {
                    pointer->exists = true;
                    next = pointer - 1;
                    switch (next->getDirection())
                    {
                        case 1: newCoords = sf::Vector2u(next->getCoords().x, next->getCoords().y + 1); break;
                        case 2: newCoords = sf::Vector2u(next->getCoords().x - 1, next->getCoords().y); break;
                        case 3: newCoords = sf::Vector2u(next->getCoords().x, next->getCoords().y - 1); break;
                        case 4: newCoords = sf::Vector2u(next->getCoords().x + 1, next->getCoords().y); break;
                    }
                    pointer->setCoords(newCoords);
                    break;
                }
                pointer++;
            }


            do
            {
                feedCoords.x = rand()%18;
                feedCoords.y = rand()%18;

                pointer = snake;
                feedColliding = false;
                for (int i=0; i<324; i++)
                {
                    if (pointer->exists && pointer->getCoords() == feedCoords)
                    {
                        feedColliding = true;
                        break;
                    }
                    pointer++;
                }
            } while (feedColliding);
        }

        feed.setPosition(sf::Vector2f((feedCoords.x+1)*SCRWIDTH*0.05f + 2.5f, (feedCoords.y+1)*SCRWIDTH*0.05f + SCRHEIGHT-SCRWIDTH + 2.5f));
        window.draw(feed);

        }

        score.setString("Score: "+std::to_string(Part::counter - 3));
        window.draw(score);
        if (failed)
        {
            window.draw(*gameOver);
            window.draw(*gameOver2);
        }

        window.display();

    } //main loop
    
    
    delete gameOver;
    delete gameOver2;

    return 0;
}
