#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>

#define WIDTH 80
#define HEIGHT 25

#define PADDLE_HEIGHT 5
#define PADDLE_WIDTH 2
#define BALL_SIZE 1

#define PADDLE1_STARTX 2
#define PADDLE2_STARTX WIDTH-3
#define PADDLE_STARTY (HEIGHT-PADDLE_HEIGHT)/2

#define BALL_STARTX WIDTH/2
#define BALL_STARTY HEIGHT/2

#define LEFT_GOAL 0
#define RIGHT_GOAL 1

enum Direction {
    STOP = 0,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    UP_LEFT,
    UP_RIGHT,
    DOWN_LEFT,
    DOWN_RIGHT
};

struct Paddle {
    int x;
    int y;
    int score;
};

struct Ball {
    int x;
    int y;
    enum Direction direction;
};

bool quit = false;

void setCursorPosition(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

void clearScreen() {
    printf("\033[2J");
}

void drawPaddle(struct Paddle* paddle) {
    for (int i = 0; i < PADDLE_HEIGHT; i++) {
        setCursorPosition(paddle->x, paddle->y+i);
        printf("|");
    }
}

void erasePaddle(struct Paddle* paddle) {
    for (int i = 0; i < PADDLE_HEIGHT; i++) {
        setCursorPosition(paddle->x, paddle->y+i);
        printf(" ");
    }
}

void movePaddle(struct Paddle* paddle, enum Direction direction) {
    erasePaddle(paddle);

    switch(direction) {
        case UP:
            if (paddle->y > 3) {
                paddle->y--;
            }
            break;
        case DOWN:
            if (paddle->y+PADDLE_HEIGHT < HEIGHT-1) {
                paddle->y++;
            }
            break;
    }

    drawPaddle(paddle);
}

void drawBall(struct Ball* ball) {
    setCursorPosition(ball->x, ball->y);
    printf("*");
}

void eraseBall(struct Ball* ball) {
    setCursorPosition(ball->x, ball->y);
    printf(" ");
}

void moveBall(struct Ball* ball) {
    eraseBall(ball);

    switch(ball->direction) {
    case UP:
        if (ball->y > 0) {
            ball->y--;
        } else {
            ball->direction = DOWN;
        }
        break;
    case DOWN:
        if (ball->y < HEIGHT-1) {
            ball->y++;
        } else {
            ball->direction = UP;
        }
        break;
    case LEFT:
        if (ball->x > 0) {
            ball->x--;
        } else {
            ball->direction = RIGHT;
        }
        break;
    case RIGHT:
        if (ball->x < WIDTH-1) {
            ball->x++;
        } else {
            ball->direction = LEFT;
        }
        break;
    case UP_LEFT:
        if (ball->y > 0 && ball->x > 0) {
            ball->y--;
            ball->x--;
        } else if (ball->y == 0 && ball->x > 0) {
            ball->x--;
            ball->direction = DOWN_RIGHT;
        } else if (ball->y > 0 && ball->x == 0) {
            ball->y--;
            ball->direction = UP_RIGHT;
        } else {
            ball->direction = DOWN_RIGHT;
        }
        break;
    case UP_RIGHT:
        if (ball->y > 0 && ball->x < WIDTH-1) {
            ball->y--;
            ball->x++;
        } else if (ball->y == 0 && ball->x < WIDTH-1)
        {
            ball->x++;
            ball->direction = DOWN_LEFT;
        } else if (ball->y > 0 && ball->x == WIDTH-1) {
            ball->y--;
            ball->direction = UP_LEFT;
        } else {
            ball->direction = DOWN_LEFT;
        }
        break;
    case DOWN_LEFT:
        if (ball->y < HEIGHT-1 && ball->x > 0) {
            ball->y++;
            ball->x--;
        } else if (ball->y == HEIGHT-1 && ball->x > 0) {
            ball->x--;
            ball->direction = UP_RIGHT;
        } else if (ball->y < HEIGHT-1 && ball->x == 0) {
            ball->y++;
            ball->direction = DOWN_RIGHT;
        } else {
            ball->direction = UP_RIGHT;
        }
        break;
    case DOWN_RIGHT:
        if (ball->y < HEIGHT-1 && ball->x < WIDTH-1) {
            ball->y++;
            ball->x++;
        } else if (ball->y == HEIGHT-1 && ball->x < WIDTH-1) {
            ball->x++;
            ball->direction = UP_LEFT;
        } else if (ball->y < HEIGHT-1 && ball->x == WIDTH-1) {
            ball->y++;
            ball->direction = DOWN_LEFT;
        } else {
            ball->direction = UP_LEFT;
        }
        break;
    }

    drawBall(ball);
}

void checkBallCollision(struct Ball * ball, struct Paddle * paddle1, struct Paddle * paddle2) {
  // проверка столкновения с левой ракеткой
  if (ball -> x == paddle1 -> x + PADDLE_WIDTH &&
    ball -> y >= paddle1 -> y &&
    ball -> y < paddle1 -> y + PADDLE_HEIGHT) {
    ball -> direction = (ball -> direction == UP_RIGHT) ? UP_LEFT : DOWN_LEFT;
    // генерация случайного числа для определения вертикального направления мяча
    int rand_num = rand() % 3 - 1;
    if (ball -> direction == UP_LEFT) {
      ball -> direction = (rand_num == -1) ? UP_RIGHT : (rand_num == 0) ? RIGHT : DOWN_RIGHT;
    } else {
      ball -> direction = (rand_num == -1) ? DOWN_RIGHT : (rand_num == 0) ? RIGHT : UP_RIGHT;
    }
  }
  // проверка столкновения с правой ракеткой
  else if (ball -> x == paddle2 -> x - PADDLE_WIDTH &&
    ball -> y >= paddle2 -> y &&
    ball -> y < paddle2 -> y + PADDLE_HEIGHT) {
    ball -> direction = (ball -> direction == UP_LEFT) ? UP_RIGHT : DOWN_RIGHT;
    // генерация случайного числа для определения вертикального направления мяча
    int rand_num = rand() % 3 - 1;
    if (ball -> direction == UP_RIGHT) {
      ball -> direction = (rand_num == -1) ? UP_LEFT : (rand_num == 0) ? LEFT : DOWN_LEFT;
    } else {
      ball -> direction = (rand_num == -1) ? DOWN_LEFT : (rand_num == 0) ? LEFT : UP_LEFT;
    }
  }
  // проверка столкновения с верхней стенкой
  else if (ball -> y == 3) {
    switch (ball -> direction) {
    case UP_LEFT:
      ball -> direction = DOWN_LEFT;
      break;
    case UP_RIGHT:
      ball -> direction = DOWN_RIGHT;
      break;
    default:
      break;
    }
  }
  // проверка столкновения с нижней стенкой
  else if (ball -> y == HEIGHT - 2) {
    switch (ball -> direction) {
    case DOWN_LEFT:
      ball -> direction = UP_LEFT;
      break;
    case DOWN_RIGHT:
      ball -> direction = UP_RIGHT;
      break;
    default:
      break;
    }
  }
  if (ball->x == 0) {
        paddle2->score++;
        setCursorPosition(WIDTH/2-5, HEIGHT/2);
        printf("GOAL for RIGHT!");
        ball->x = BALL_STARTX;
        ball->y = BALL_STARTY;
        ball->direction = RIGHT;
        paddle1->y = paddle2->y = PADDLE_STARTY;
        _getch();
        clearScreen();
    } else if (ball->x == WIDTH-1) {
        paddle1->score++;
        setCursorPosition(WIDTH/2-5, HEIGHT/2);
        printf("GOAL for LEFT!");
        ball->x = BALL_STARTX;
        ball->y = BALL_STARTY;
        ball->direction = LEFT;
        paddle1->y = paddle2->y = PADDLE_STARTY;
        _getch();
        clearScreen();
    }
}

void displayScore(struct Paddle* paddle1, struct Paddle* paddle2) {
setCursorPosition(WIDTH/2-10-20, 0);
printf("LEFT SCORE: %d\t\t\t\tRIGHT SCORE: %d\n", paddle1->score, paddle2->score);
}
void drawField() {
    // draw top border
    for (int i = 0; i < WIDTH; i++) {
        setCursorPosition(i, 2);
        printf("-");
    }

    // draw bottom border
    for (int i = 0; i < WIDTH; i++) {
        setCursorPosition(i, HEIGHT-1);
        printf("-");
    }

    // draw middle line
    for (int i = 1; i < HEIGHT-3; i++) {
        setCursorPosition(WIDTH/2, i+2);
        printf("|");
    }
}

int main() {
struct Paddle paddle1 = {PADDLE1_STARTX, PADDLE_STARTY, 0};
struct Paddle paddle2 = {PADDLE2_STARTX, PADDLE_STARTY, 0};
struct Ball ball = {BALL_STARTX, BALL_STARTY, LEFT};
// Hide cursor
printf("\033[?25l");

while(!quit) {
    // Move paddles
    drawField();
    if (_kbhit()) {
        char input = _getch();
        switch(input) {
            case 'w':
                movePaddle(&paddle1, UP);
                break;
            case 's':
                movePaddle(&paddle1, DOWN);
                break;
            case 'i':
                movePaddle(&paddle2, UP);
                break;
            case 'k':
                movePaddle(&paddle2, DOWN);
                break;
            case 'q':
                quit = true;
                break;
        }
    }

    // Move ball
    moveBall(&ball);

    // Check for collision
    checkBallCollision(&ball, &paddle1, &paddle2);

    // Display score
    displayScore(&paddle1, &paddle2);

    // Sleep for a bit to slow down the game
    usleep(50000);
}

// Show cursor
printf("\033[?25h");

return 0;
}