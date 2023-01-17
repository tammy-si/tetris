#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
using namespace sf;
// clang++ main.cpp -I/opt/homebrew/Cellar/sfml/2.5.1_2/include/ -L/opt/homebrew/Cellar/sfml/2.5.1_2/lib  -lsfml-graphics -lsfml-window -lsfml-system -std=c++20

const unsigned int CELL_SIZE = 18;
const unsigned int COLUMNS = 10;
const unsigned int ROWS = 20;
const unsigned int RESIZE = 2;

struct Point {
    int x;
    int y;
};

// to make the pieces. Make the blocks based of the change from the main_point.
int tetromino[7][4][2] = {
   {{0,1}, {0,0}, {1,0}, {2,0}},    // J block
   {{-1,0}, {0,0}, {0,1}, {1,0}},   // T block
   {{-1,1}, {0,0}, {0,1}, {1,0}},   // Z block
   {{-1,0}, {0,0}, {0,1}, {1,1}},   // S block
   {{0,1}, {0,0}, {1,1}, {1,0}},    // O block
   {{-1,0}, {0,0}, {1,0}, {2,0}},   // I block
   {{-1,0}, {0,0}, {1,0}, {1,1}}    // L block
};



int main()
{
    RenderWindow window(VideoMode(COLUMNS * CELL_SIZE * RESIZE, ROWS * CELL_SIZE * RESIZE), "Tetris!");
    int field[ROWS][COLUMNS];
    Point main_point;
    main_point.x = 4;
    main_point.y = 1;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        window.display();

        // draw the grid out
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 10; j++) {
                sf::RectangleShape block(sf::Vector2f(CELL_SIZE * RESIZE - 1, CELL_SIZE * RESIZE - 1));
                block.setFillColor(Color::White);
                block.setPosition(j * 36, i * 36);
                window.draw(block);
            }
        }
        Texture texture;
        texture.loadFromFile("Blocks.png");
        Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0, 0, 18, 18));
        sprite.setScale(RESIZE, RESIZE);
        sprite.setPosition(main_point.x * RESIZE * CELL_SIZE, main_point.y * RESIZE * CELL_SIZE);
        window.draw(sprite);

    }
    return 0;
}

