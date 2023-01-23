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
   {{-1, -1}, {0,0}, {-1,0}, {1,0}},    // J block
   {{-1,0}, {0,0}, {0,-1}, {1,0}},   // T block
   {{-1,-1}, {0,0}, {0,-1}, {1,0}},   // Z block
   {{-1,0}, {0,0}, {0,-1}, {1,-1}},   // S block
   {{-1,-1}, {0,0}, {-1,0}, {0,-1}},    // O block
   {{-2,-1}, {0,-1}, {-1,-1}, {1,-1}},   // I block
   {{-1,0}, {0,0}, {1,0}, {1,-1}}    // L block
};

// function prototypes
bool checkBounds(Point curr_block[], int x_change, int y_change, bool &has_collided, int field[][COLUMNS]);
void moveBlock(Point curr_block[], int x_change, int y_change);
void rotateBlock(Point curr_block[]);
bool checkCanRotate(Point curr_block[], int field[][COLUMNS]);
int updateField(int field[][COLUMNS]);
bool checkGameover(int field[][COLUMNS]);
void drawField(int field[][COLUMNS], RenderWindow &window, Texture &texture);

int main()
{
    srand(time(NULL));
    RenderWindow window(VideoMode(COLUMNS * CELL_SIZE * RESIZE + 6 * CELL_SIZE * RESIZE, ROWS * CELL_SIZE * RESIZE), "Tetris!");
    // uploading original texture 
    Texture texture;
    texture.loadFromFile("Blocks.png");
    // uploading font
    Font font;
    font.loadFromFile("VCR_OSD_MONO_1.001.ttf");
    // keep track of what block we're on
    int block_num;
    // keep track of what block is next
    int next_block_num = rand() % 7;;
    window.setFramerateLimit(60);
    int field[ROWS][COLUMNS];
    // set all the values of the field to 9. 9 means that the spot is empty.
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLUMNS; c++) {
            field[r][c] = 9;
        }
    }

    // counting frames, so that on certain frame we can do stuff
    int frame {0};

    // hold the curr block
    Point curr_block[4];
    // making the first starter_point
    Point starter_point;
    starter_point.x = 4;
    starter_point.y = 1;

    unsigned long score = 0;

    // made block is to keep track of whether we need to make a new block. 
    // has_collided keeps track of whether or not the current block has collided with the bottom or field
    bool made_block = false;
    bool has_collided = false;
    bool pressed_space = false;
    bool stop_game = false;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == (sf::Event::KeyPressed)) {
                // move right if user presses the right arrow
                if (event.key.code == sf::Keyboard::Right && checkBounds(curr_block, 1, 0, has_collided, field)) {
                    moveBlock(curr_block, 1, 0);
                // move right if the user presses the left arrow
                } else if (event.key.code == sf::Keyboard::Left && checkBounds(curr_block, -1, 0, has_collided, field)) {
                    moveBlock(curr_block, -1, 0);
                }
                // rotating the block
                if (event.key.code == sf::Keyboard::Up && checkCanRotate(curr_block, field)) {
                    rotateBlock(curr_block);
                }
                // want to go down faster
                if (event.key.code == sf::Keyboard::Down) {
                    window.setFramerateLimit(800);
                }
                // down for instant
                if (event.key.code == sf::Keyboard::Space) {
                    pressed_space = true;
                    window.setFramerateLimit(800);
                }
            }
            if (event.type == Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Down) {
                    window.setFramerateLimit(60);
                }
                if (event.key.code == sf::Keyboard::Space) {
                    pressed_space = false;
                    window.setFramerateLimit(60);
                }
            }
        }
        if (!stop_game) {
            window.clear();
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
                // replace the current block with the one that's in queue to be next, then find a new next block
                block_num = next_block_num;
                do {
                    next_block_num = rand() % 7;
                } while (next_block_num == block_num);
                for (int i = 0; i < 4; i++) {
                    curr_block[i].x = starter_point.x + tetromino[block_num][i][0];
                    curr_block[i].y = starter_point.y + tetromino[block_num][i][1];
                }
                made_block = true;
                has_collided = false;
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

            // draw out the next block area and next block
            sf::RectangleShape next_block_area(sf::Vector2f(CELL_SIZE * RESIZE * 4, CELL_SIZE * RESIZE * 4));
            next_block_area.setPosition(CELL_SIZE * RESIZE * 11, CELL_SIZE * RESIZE);
            next_block_area.setFillColor(Color::White);
            window.draw(next_block_area);

            // draw out the next block
            Point next_block_mainpt;
            next_block_mainpt.x = 13;
            next_block_mainpt.y = 3;
            for (int i=0; i < 4; i++) {
                cout << "coolio" << endl;
                cout << next_block_num << endl;
                Sprite next_block;
                next_block.setTexture(texture);
                next_block.setTextureRect(sf::IntRect(18 * next_block_num, 0, 18, 18));
                next_block.setScale(RESIZE, RESIZE);
                next_block.setPosition(RESIZE * CELL_SIZE * (next_block_mainpt.x + tetromino[next_block_num][i][0]),RESIZE * CELL_SIZE * (next_block_mainpt.y + tetromino[next_block_num][i][1]));
                window.draw(next_block);
            }

            // draw out the points display
            Text text;
            text.setFont(font);
            text.setString("Score: \n" + to_string(score));
            text.setFillColor(sf::Color::White);
            text.setPosition(11 * RESIZE * CELL_SIZE, 6 * RESIZE * CELL_SIZE);
            text.setCharacterSize(30);
            window.draw(text);

            // count frame to make the block drop
            if (pressed_space) {
                frame += 30;
            } else {
                frame++;
            }
            // to drop the block make sure that it hasn't collided with anything yet 
            if (frame >= 30 && checkBounds(curr_block, 0, 1, has_collided, field) & !has_collided) {
                moveBlock(curr_block, 0, 1);
                frame = 0;
            }

            // the block has either collided with the bottom or another block
            if (has_collided) {
                made_block = false;
                // add the block to the field
                for (int i = 0; i < 4; i++) {
                        field[curr_block[i].y][curr_block[i].x] = block_num;
                }
                int to_add = updateField(field);
                score += to_add * 100;
                pressed_space = false;
            }
            drawField(field, window, texture);
            window.display();
        }
        // check to see if it's game over by checking if the field has something in its top row
        if (checkGameover(field)) {
            stop_game = true;
            drawField(field, window, texture);
            window.display();
        }
    }
    return 0;
}

// check right bounds
bool checkBounds(Point curr_block[], int x_change, int y_change, bool &has_collided, int field[][COLUMNS]) {
    Point new_block[4];
    copy(curr_block, curr_block + 4, new_block);
    for (int i = 0; i < 4; i++) {
        new_block[i].x += x_change;
        new_block[i].y += y_change;
        // for the left and right edges
        if (new_block[i].x < 0 || new_block[i].x > 9 || (new_block[i].y < 0)) {
            return false;
        // hits the bottom fo the field or another block in the field
        } else if (new_block[i].y > 19 || field[new_block[i].y][new_block[i].x] != 9) {
            // make sure it's due to natural drop and not the moving side to side
            if (x_change == 0) {
                has_collided = true;
            }
            return false;
        }
    }
    return true;
}

// check if can rotate without hitting the field
bool checkCanRotate(Point curr_block[], int field[][COLUMNS]) {
    Point new_block[4];
    copy(curr_block, curr_block + 4, new_block);
    Point point_of_rot = new_block[1];
    for (int i = 0; i < 4; i++) {
        // rotate around the main point of the block, which should be the point at new_block[1]
        // subtract 
        new_block[i].x = new_block[i].x - point_of_rot.x;
        new_block[i].y = new_block[i].y - point_of_rot.y;
        // rotate as if around the origin
        int temp = new_block[i].x;
        new_block[i].x = -new_block[i].y;
        new_block[i].y = temp;
        // add point fo rotation back to 
        new_block[i].x += point_of_rot.x;
        new_block[i].y += point_of_rot.y;
        // for the left and right edges
        if (new_block[i].x < 0 || new_block[i].x > 9 || (new_block[i].y < 0)) {
            return false;
        } else if (new_block[i].y > 19 || field[new_block[i].y][new_block[i].x] != 9) {
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
    }
}

// rotate block
void rotateBlock(Point curr_block[]) {
    Point point_of_rot = curr_block[1];
    for (int i = 0; i < 4; i++) {
        // rotate around the main point of the block, which should be the point at new_block[1]
        // subtract 
        curr_block[i].x = curr_block[i].x - point_of_rot.x;
        curr_block[i].y = curr_block[i].y - point_of_rot.y;
        // rotate as if around the origin
        int temp = curr_block[i].x;
        curr_block[i].x = -curr_block[i].y;
        curr_block[i].y = temp;
        // add point fo rotation back to 
        curr_block[i].x += point_of_rot.x;
        curr_block[i].y += point_of_rot.y;
    }
}

// update field
// delete full rows and move the blocks down also keep track of the amount of rows deleted and return it so we can add points
int updateField(int field[][COLUMNS]) {
    int rows_cleared {0};
    // an array to keep track of whether the row has been cleared. order is top down the same as the field
    // 1 is that the row is cleared (1 is true). 0 is that the row is not cleared
    int cleared[20];
    bool to_be_cleared = false;
    for (int row = 0; row < 20; row++) {
        to_be_cleared = true;
        for (int column = 0; column < 10; column++) {
            // if there an empty area, you can skip this row
            if (field[row][column] == 9) {
                to_be_cleared = false;
                cleared[row] = 0;
                break;
            }
        }
        if (to_be_cleared) {
            // if there isn't a break that means the row is full. That means clear it
            for (int b = 0; b < COLUMNS; b++) {
                field[row][b] = 9;
            }
            cleared[row] = 1;
            rows_cleared++;
        }
    }

    // go bottom to top and swap an empty rows with the first row with blocks. 
    for (int i = 19; i >= 0; i--) {
        // found a empty row
        if (cleared[i] == 1) {
            // now look for the next non-empty row
            int curr = i - 1;
            bool swapped = false;
            while (curr >= 0 && !swapped){
                if (cleared[curr] == 0) {
                    // swap the empty row with the closest non-empty row
                    for (int col = 0; col < 10; col++) {
                        int temp = field[i][col];
                        field[i][col] = field[curr][col];
                        field[curr][col] = temp;
                    }
                    // change the swaped status and the status of the rows in cleared
                    cleared[i] = 0;
                    cleared[curr] = 1;
                    swapped = true;
                }
                curr--;
            }
        }
    }
    return rows_cleared;
}

// check for gameover
bool checkGameover(int field[][COLUMNS]) {
    for (int i = 0; i < COLUMNS; i++) {
        // there's something in the top row, so not 9 (which means empty)
        if (field[0][i] != 9) {
            return true;
        }
    }
    return false;
}

void drawField(int field[][COLUMNS], RenderWindow &window, Texture &texture) {
    // draw out the field of blocks that have been placed
    for (int row = 0; row < ROWS; row++) {
        for (int column = 0; column < COLUMNS; column++) {
            // 9 in the field is an empty spot, so we skip
            if (field[row][column] == 9) {
                continue;
            }
            Sprite block;
            block.setTexture(texture);
            block.setTextureRect(sf::IntRect(18 * field[row][column], 0, 18, 18));
            block.setScale(RESIZE, RESIZE);
            block.setPosition(column * RESIZE * CELL_SIZE, row * RESIZE * CELL_SIZE);
            window.draw(block);          
        }
    }
}