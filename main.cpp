#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <time.h>
#include <algorithm> 

using namespace sf;
using namespace std;
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
   {{0,-1}, {0,0}, {1,0}, {2,0}},    // J block
   {{-1,0}, {0,0}, {0,-1}, {1,0}},   // T block
   {{-1,-1}, {0,0}, {0,-1}, {1,0}},   // Z block
   {{-1,0}, {0,0}, {0,-1}, {1,-1}},   // S block
   {{0,-1}, {0,0}, {1,-1}, {1,0}},    // O block
   {{-1,-1}, {0,-1}, {1,-1}, {2,-1}},   // I block
   {{-1,0}, {0,0}, {1,0}, {1,-1}}    // L block
};

// function prototypes
bool checkRightBounds(Point curr_block[], int x_change, int y_change);
void moveBlock(Point curr_block[], int x_change, int y_change);

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

    // counting frames, so that on certain frame we can do stuff
    int frame {0};

    // hold the curr block
    Point curr_block[4];
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
            // move right if user presses the right arrow
            if (event.type == (sf::Event::KeyPressed)) {
                if (event.key.code == sf::Keyboard::Right && checkRightBounds(curr_block, 1, 0)) {
                    moveBlock(curr_block, 1, 0);
                } else if (event.key.code == sf::Keyboard::Left && checkRightBounds(curr_block, -1, 0)) {
                    moveBlock(curr_block, -1, 0);
                }
            }
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
            checkRightBounds(curr_block, 1, 1);
            for (int i = 0; i < 4; i++) {
                cout << '\n' << curr_block[i].x  << " " << curr_block[i].y << endl;
            }
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
        // count frame to make the block drop
        frame++;
        if (frame % 30 == 0) {
            moveBlock(curr_block, 0, 1);
            frame = 0;
        }
    }
    return 0;
}

// check right bounds
bool checkRightBounds(Point curr_block[], int x_change, int y_change) {
    std::cout << &curr_block;
    Point new_block[4];
    copy(curr_block, curr_block + 4, new_block);
    for (int i = 0; i < 4; i++) {
        cout << new_block[i].x  << " " << new_block[i].y << endl;
        new_block[i].x += x_change;
        new_block[i].y += y_change;
        // for the left and right edges
        if (new_block[i].x < 0 || new_block[i].x > 9) {
            return false;
        } else if (new_block[i].y < 0 || new_block[i].y > 19) {
            return false;
        }
    }
    return true;
}

// move the curr_block
void moveBlock(Point curr_block[], int x_change, int y_change) {
    for (int i = 0; i < 4; i++) {
        curr_block[i].x += x_change;
        curr_block[i].y += y_change;
        cout << curr_block[i].x << ' ' << curr_block[i].y << endl;;
    }
}

