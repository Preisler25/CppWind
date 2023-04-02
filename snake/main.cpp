#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

const int TILE_SIZE = 16;
const int MAP_WIDTH = 40;
const int MAP_HEIGHT = 30;
const int INITIAL_SNAKE_LENGTH = 3;

class Snake
{
public:
    Snake()
    {
        for (int i = 0; i < INITIAL_SNAKE_LENGTH; i++)
        {
            body.push_back(sf::RectangleShape(sf::Vector2f(TILE_SIZE, TILE_SIZE)));
            body[i].setPosition((MAP_WIDTH / 2) * TILE_SIZE, (MAP_HEIGHT / 2 + i) * TILE_SIZE);
        }
    }

    void move(int dx, int dy)
    {
        for (int i = body.size() - 1; i > 0; i--)
        {
            body[i].setPosition(body[i - 1].getPosition());
        }
        body[0].move(dx * TILE_SIZE, dy * TILE_SIZE);
    }

    void grow()
    {
        body.push_back(sf::RectangleShape(sf::Vector2f(TILE_SIZE, TILE_SIZE)));
        body[body.size() - 1].setPosition(body[body.size() - 2].getPosition());
    }

    bool isColliding(sf::RectangleShape object)
    {
        for (int i = 0; i < body.size(); i++)
        {
            if (body[i].getGlobalBounds().intersects(object.getGlobalBounds()))
            {
                return true;
            }
        }
        return false;
    }


    void draw(sf::RenderWindow& window)
    {
        for (int i = 0; i < body.size(); i++)
        {
            sf::RectangleShape rect = body[i];
            rect.setPosition(rect.getPosition().x * TILE_SIZE, rect.getPosition().y * TILE_SIZE);
            window.draw(rect);
        }
    }


    sf::RectangleShape getPosition()
    {
        return body[0];
    }

private:
    std::vector<sf::RectangleShape> body;
};

class Food
{
public:
    Food()
    {
        food.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
        spawn();
    }

    void spawn()
    {
        int x = std::rand() % MAP_WIDTH;
        int y = std::rand() % MAP_HEIGHT;
        food.setPosition(x * TILE_SIZE, y * TILE_SIZE);
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(food);
    }

    sf::RectangleShape getPosition()
    {
        return food;
    }

private:
    sf::RectangleShape food;
};

int main()
{
    std::srand(std::time(0));
    
    sf::RenderWindow window(sf::VideoMode(MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE), "Snake Game");
    
    Snake snake;
    Food food;
    
    int dx = 0;
    int dy = 1;
    
    int score = 0;
    
    sf::Font font;
    font.loadFromFile("arial.ttf");
    
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setPosition(10, 10);
    scoreText.setFillColor(sf::Color::White);
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Left && dx == 0)
                {
                    dx = -1;
                    dy = 0;
                }
                else if (event.key.code == sf::Keyboard::Right && dx == 0)
                {
                    dx = 1;
                    dy = 0;
                }
                else if (event.key.code == sf::Keyboard::Up && dy == 0)
                {
                    dx = 0;
                    dy = -1;
                }
                else if (event.key.code == sf::Keyboard::Down && dy == 0)
                {
                    dx = 0;
                    dy = 1;
                }
            }
        }
        
        if (snake.isColliding(food.getPosition()))
        {
            food.spawn();
            snake.grow();
            score++;
        }
        
        snake.move(dx, dy);
        
        if (snake.getPosition().x < 0 || snake.getPosition().x >= MAP_WIDTH * TILE_SIZE
            || snake.getPosition().y < 0 || snake.getPosition().y >= MAP_HEIGHT * TILE_SIZE
            || snake.isColliding(snake.getPosition()))
        {
            std::cout << "Game Over!" << std::endl;
            std::cout << "Your score: " << score << std::endl;
            window.close();
        }
        
        window.clear();
        
        snake.draw(window);
        food.draw(window);
        
        scoreText.setString("Score: " + std::to_string(score));
        window.draw(scoreText);
        
        window.display();
    }
    
    return 0;
}
