#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <time.h>
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

Point curr_block[4];

// to make the pieces. Make the blocks based of the change from the main_point.
int tetromino[7][4][2] = {
   {{0,-1}, {0,0}, {1,0}, {2,0}},    // J block
   {{-1,0}, {0,0}, {0,-1}, {1,0}},   // T block
   {{-1,-1}, {0,0}, {0,-1}, {1,0}},   // Z block
   {{-1,0}, {0,0}, {0,-1}, {1,-1}},   // S block
   {{0,-1}, {0,0}, {1,-1}, {1,0}},    // O block
   {{-1,-1}, {0,-1}, {1,-1}, {2,-1}},   // I block
   {{-1,0}, {0,0}, {1,0}, {1,-1}}    // L block
};

int main()
{
    srand(time(NULL));
    RenderWindow window(VideoMode(COLUMNS * CELL_SIZE * RESIZE, ROWS * CELL_SIZE * RESIZE), "Tetris!");
    // uploading original texture 
    Texture texture;
    texture.loadFromFile("Blocks.png");
    // keep track of what block we're on
    int block_num;
    window.setFramerateLimit(60);
    int field[ROWS][COLUMNS];
    // making the first starter_point
    Point starter_point;
    starter_point.x = 4;
    starter_point.y = 1;
    bool made_block = false;
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
        // populate curr_block based on random block chosen
        if (!made_block) {
            block_num = rand() % 7;
            for (int i = 0; i < 4; i++) {
                curr_block[i].x = starter_point.x + tetromino[block_num][i][0];
                curr_block[i].y = starter_point.y + tetromino[block_num][i][1];
                std::cout << curr_block[i].x << " " << curr_block[i].y << std::endl;
            }
            std::cout << block_num << std::endl;
            made_block = true;
        }

        // draw out curr block
        for (int i = 0; i < 4; i++) {
            Sprite sprite;
            sprite.setTexture(texture);
            sprite.setTextureRect(sf::IntRect(18 * block_num, 0, 18, 18));
            sprite.setScale(RESIZE, RESIZE);
            sprite.setPosition(curr_block[i].x * RESIZE * CELL_SIZE, curr_block[i].y * RESIZE * CELL_SIZE);
            window.draw(sprite);
        }            

    }
    return 0;
}

