/*
  Start: 29/4/2024
  End: DD/MM/YYYY
*/

#include "raylib.h"
#include <ctime>
#include <stdlib.h>

class Player {
public:
    int x;
    int y;
    int w = 50;
    int h = 50;
    int speed = 10;

    Player(int x, int y) {
        this->x = x;
        this->y = y;
    }

    void Draw() {
        DrawRectangle(this->x, this->y, this->w, this->h, BLUE);
    }

    void Move(int screenWidth) {
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            this->speed = 15;
        }
        else {
            this->speed = 10;
        }

        if (IsKeyDown(KEY_A) && this->x > 0) {
            this->x -= this->speed;
        }
        if (IsKeyDown(KEY_D) && this->x < (screenWidth - this->w)) {
            this->x += this->speed;
        }
    }
};

class Bullet {
public:
    int x;
    int y;
    int w;
    int h;
    int speed = 8;
    bool fall = false;
    bool ex = false;

    Bullet(int x, int y, int w, int h) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }

    void Draw() {
        DrawRectangle(this->x, this->y, this->w, this->h, RED);
    }

    bool Fall(int screenHeight) {
        if (this->fall) {
            this->y += this->speed;
            return false;
        }

        if (this->y > screenHeight) {
            this->y = -this->h;
            return true;
        }

        return false;
    }

    void Check(int screenHeight) {
        if (this->y < 0 || this->y > screenHeight) { // This doesn't make sense but it works
            switch (rand() % 9) { // Make some bullets bigger than others
            case 0:
                this->h = 200; // Laser
                break;
            case 1:
                this->h = 150;
                break;
            case 2:
                this->h = 100;
                break;
            case 3:
                this->h = 50;
                break;
            default:
                this->h = 25; // Normal
                break;
            }


            switch (rand() % 3) { // Distribute the bullets
            case 0:
                this->fall = true;
                break;
            default:
                this->fall = false;
                break;
            }
        }
    }

    void updateEx(int screenHeight) {
        if (this->y < 0 || this->y > screenHeight) {
            this->ex = false;
        } else {
            this->ex = true;
        }
    }
};

bool checkCollision(const Player rect1, const Bullet rect2);

int main(void)
{
    srand(time(NULL));

    const int screenWidth = 900;
    const int screenHeight = 500;

    bool isGameOver = false;
    bool isGamePaused = false;

    unsigned int timesAvoided = 0;
    unsigned int points = 0;

    Player player(450, 450);
    Bullet bullets[] = {
        Bullet(0, -75, 25, 25), // [0]
        Bullet(70, -75, 25, 25), // [1]
        Bullet(140, -75, 25, 25), // [2]
        Bullet(210, -75, 25, 25), // [3]
        Bullet(280, -75, 25, 25), // [6]
        Bullet(350, -75, 25, 25), // [7]
        Bullet(420, -75, 25, 25), // [8]
        Bullet(490, -75, 25, 25), // [9]
        Bullet(560, -75, 25, 25), // [10]
        Bullet(630, -75, 25, 25), // [11]
        Bullet(700, -75, 25, 25), // [12]
        Bullet(770, -75, 25, 25), // [13]
        Bullet(840, -75, 25, 25), // [14]
    };

    InitWindow(screenWidth, screenHeight, "Chaos Cascade");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(LIGHTGRAY);

        if (!isGameOver) {
            if (isGamePaused) { // Pause Menu
                DrawText(TextFormat("Bullets Avoided: %02i", timesAvoided), 10, 10, 20, BLACK);
                DrawText(TextFormat("Points: %02i", points), 10, 40, 20, BLACK);
                DrawText("Game Paused", screenWidth / 2 - 100, screenHeight / 2, 50, BLACK);

                // Resume the game if space key is pressed
                if (IsKeyPressed(KEY_F)) {
                    isGamePaused = false;
                }
            } else { // Main Game Loop
                points = timesAvoided / 12;

                DrawText(TextFormat("Bullets Avoided: %02i", timesAvoided), 10, 10, 20, BLACK);
                DrawText(TextFormat("Points: %02i", points), 10, 40, 20, BLACK);

                // Player
                player.Draw();
                player.Move(screenWidth);

                // Bullets
                for (int i = 0; i < sizeof(bullets) / sizeof(Bullet); i++) {
                    bullets[i].updateEx(screenHeight);

                    if (bullets[i].ex) {
                        bullets[i].Draw();
                    }
                    bullets[i].Check(screenHeight);

                    if (bullets[i].Fall(screenHeight)) {
                        timesAvoided++;
                    }

                    // Check for collision with player
                    if (bullets[i].ex && checkCollision(player, bullets[i])) {
                        isGameOver = true;
                    }
                }

                if (IsKeyPressed(KEY_SPACE)) {
                    if (points >= 3) {
                        points -= 3;
                        for (int i = 0; i < sizeof(bullets) / sizeof(bullets[0]); i++) {
                            bullets[i].y = screenHeight + 500; // Adjust the reset position as needed
                        }
                    }
                }

                // Pause the game if space key is pressed
                if (IsKeyPressed(KEY_F)) {
                    isGamePaused = true;
                }
            }
        }
        else {
            DrawText(TextFormat("Bullets Avoided: %02i", timesAvoided), 10, 10, 20, BLACK);
            DrawText(TextFormat("Points: %02i", points), 10, 40, 20, BLACK);
            DrawText("Game Over", screenWidth / 2 - 100, screenHeight / 2, 50, BLACK);
            DrawText("Press F to reset", screenWidth / 2 - 80, screenHeight / 2 + 40, 30, BLACK);

            // Reset game state if space key is pressed
            if (IsKeyPressed(KEY_F)) {
                // Reset stuff
                bullets[0] = Bullet(0, -75, 25, 25);
                bullets[1] = Bullet(70, -75, 25, 25);
                bullets[2] = Bullet(140, -75, 25, 25);
                bullets[3] = Bullet(210, -75, 25, 25);
                bullets[4] = Bullet(280, -75, 25, 25);
                bullets[5] = Bullet(350, -75, 25, 25);
                bullets[6] = Bullet(420, -75, 25, 25);
                bullets[7] = Bullet(490, -75, 25, 25);
                bullets[8] = Bullet(560, -75, 25, 25);
                bullets[9] = Bullet(630, -75, 25, 25);
                bullets[10] = Bullet(700, -75, 25, 25);
                bullets[11] = Bullet(770, -75, 25, 25);
                bullets[12] = Bullet(840, -75, 25, 25);

                timesAvoided = 0;
                points = 0;

                isGameOver = false;
                isGamePaused = false;
            }
        }

        EndDrawing();
    }


    CloseWindow();

    return 0;
}

bool checkCollision(const Player rect1, const Bullet rect2) {
    // Check if rect1 is to the left of rect2
    if (rect1.x + rect1.w < rect2.x) {
        return false;
    }

    // Check if rect1 is to the right of rect2
    if (rect1.x > rect2.x + rect2.w) {
        return false;
    }

    // Check if rect1 is above rect2
    if (rect1.y + rect1.h < rect2.y) {
        return false;
    }

    // Check if rect1 is below rect2
    if (rect1.y > rect2.y + rect2.h) {
        return false;
    }

    // If none of the above conditions are true, rectangles overlap and there is a
    // collision
    return true;
}