#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <deque>

using namespace std;

// Game constants
const int WINDOW_WIDTH = 350;
const int WINDOW_HEIGHT = 350;
const int CELL_SIZE = 10;
const int INITIAL_LENGTH = 2;
const int GAME_SPEED = 150; 

// Game state
struct GameState {
    int score;
    bool isPlaying;
    char direction;
    char lastDirection;
    
    // Snake properties
    int headX, headY;
    int length;
    deque<pair<int, int>> snakeBody;
    
    // Fruit properties
    int fruitX, fruitY;
    
    GameState() : 
        score(0), 
        isPlaying(true),
        direction('-'),
        lastDirection('-'),
        headX(30),
        headY(170),
        length(INITIAL_LENGTH) {}
} game;

// Function declarations
void initializeGame();
void handleInput();
void updateGame();
void drawGame();
void spawnFruit();
bool checkCollision();
void gameOver();

int main() {
    initializeGame();
    
    while (game.isPlaying) {
        handleInput();
        updateGame();
        drawGame();
        delay(GAME_SPEED);
        
        if (checkCollision()) {
            game.isPlaying = false;
        }
    }
    
    gameOver();
    closegraph();
    return 0;
}

void initializeGame() {
    // Initialize graphics
    initwindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Snake Game");
    
    // Set random seed
    srand(static_cast<unsigned>(time(nullptr)));
    
    // Initialize snake's initial position
    game.snakeBody.push_front({game.headX, game.headY});
    
    // Spawn first fruit
    spawnFruit();
}

void handleInput() {
    if (kbhit()) {
        char input = getch();
        
        // Prevent 180-degree turns
        bool isValidMove = true;
        if ((input == 'w' && game.lastDirection == 's') ||
            (input == 's' && game.lastDirection == 'w') ||
            (input == 'a' && game.lastDirection == 'd') ||
            (input == 'd' && game.lastDirection == 'a')) {
            isValidMove = false;
        }
        
        if (isValidMove && (input == 'w' || input == 'a' || input == 's' || input == 'd')) {
            game.direction = input;
        }
    }
}

void updateGame() {
    // Store previous position
    int prevX = game.headX;
    int prevY = game.headY;
    
    // Update head position based on direction
    switch (game.direction) {
        case 'w': game.headY -= CELL_SIZE; break;
        case 's': game.headY += CELL_SIZE; break;
        case 'a': game.headX -= CELL_SIZE; break;
        case 'd': game.headX += CELL_SIZE; break;
    }
    
    game.lastDirection = game.direction;
    
    // Update snake body
    game.snakeBody.push_front({game.headX, game.headY});
    
    // Check if fruit is eaten
    if (game.headX == game.fruitX && game.headY == game.fruitY) {
        game.score++;
        game.length++;
        spawnFruit();
    } else {
        // Remove tail if no fruit was eaten
        if (game.snakeBody.size() > game.length) {
            game.snakeBody.pop_back();
        }
    }
}

void drawGame() {
    cleardevice();
    
    // Draw score
    char scoreText[50];
    sprintf(scoreText, "SCORE: %d", game.score);
    setcolor(GREEN);
    setfillstyle(SOLID_FILL, GREEN);
    rectangle(0, 0, WINDOW_WIDTH, CELL_SIZE);
    floodfill(5, 5, GREEN);
    setcolor(WHITE);
    outtextxy(10, 0, scoreText);
    
    // Draw fruit
    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    rectangle(game.fruitX, game.fruitY, game.fruitX + CELL_SIZE, game.fruitY + CELL_SIZE);
    floodfill(game.fruitX + 5, game.fruitY + 5, RED);
    
    // Draw snake
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    for (const auto& segment : game.snakeBody) {
        rectangle(segment.first, segment.second, 
                 segment.first + CELL_SIZE, segment.second + CELL_SIZE);
        floodfill(segment.first + 5, segment.second + 5, WHITE);
    }
}

void spawnFruit() {
    bool validPosition;
    do {
        validPosition = true;
        game.fruitX = (rand() % (WINDOW_WIDTH / CELL_SIZE)) * CELL_SIZE;
        game.fruitY = ((rand() % (WINDOW_HEIGHT / CELL_SIZE - 1)) * CELL_SIZE) + CELL_SIZE;
        
        // Check if fruit spawned on snake
        for (const auto& segment : game.snakeBody) {
            if (game.fruitX == segment.first && game.fruitY == segment.second) {
                validPosition = false;
                break;
            }
        }
    } while (!validPosition);
}

bool checkCollision() {
    // Check wall collision
    if (game.headX >= WINDOW_WIDTH || game.headX < 0 || 
        game.headY >= WINDOW_HEIGHT || game.headY < CELL_SIZE) {
        return true;
    }
    
    // Check self collision
    auto it = game.snakeBody.begin();
    ++it;  // Skip head
    for (; it != game.snakeBody.end(); ++it) {
        if (game.headX == it->first && game.headY == it->second) {
            return true;
        }
    }
    
    return false;
}

void gameOver() {
    cleardevice();
    char finalScore[50];
    sprintf(finalScore, "SCORE: %d", game.score);
    outtextxy(70, 170, "GAME OVER! YOUR ");
    outtextxy(188, 170, finalScore);
    getch();
}