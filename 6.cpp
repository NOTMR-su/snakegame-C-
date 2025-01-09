#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <windows.h>
using namespace std;

// Определяем константы для избежания конфликтов с Windows API
#define SNAKE_WIDTH 20
#define SNAKE_HEIGHT 20

class SnakeGame {
private:
    bool gameOver;
    const int width;
    const int height;
    int x, y, fruitX, fruitY, score;
    vector<int> tailX;
    vector<int> tailY;
    int nTail;
    enum class Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
    Direction dir;

    char getInput() {
        if (GetAsyncKeyState('A') & 0x8000) return 'a';
        if (GetAsyncKeyState('D') & 0x8000) return 'd';
        if (GetAsyncKeyState('W') & 0x8000) return 'w';
        if (GetAsyncKeyState('S') & 0x8000) return 's';
        if (GetAsyncKeyState('X') & 0x8000) return 'x';
        return 0;
    }

public:
    SnakeGame() : width(SNAKE_WIDTH), height(SNAKE_HEIGHT), 
                  gameOver(false), x(0), y(0), 
                  fruitX(0), fruitY(0), score(0), nTail(0), 
                  dir(Direction::STOP) {
        setup();
    }

    void setup() {
        gameOver = false;
        dir = Direction::STOP;
        x = width / 2;
        y = height / 2;
        fruitX = rand() % width;
        fruitY = rand() % height;
        score = 0;
        nTail = 0;
        tailX.clear();
        tailY.clear();
    }

    void draw() {
        COORD coord;
        coord.X = 0;
        coord.Y = 0;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        for (int i = 0; i < width + 2; i++)
            cout << "#";
        cout << "\n";

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (j == 0)
                    cout << "#";

                if (i == y && j == x)
                    cout << "O";
                else if (i == fruitY && j == fruitX)
                    cout << "F";
                else {
                    bool printTail = false;
                    for (int k = 0; k < nTail; k++) {
                        if (tailX[k] == j && tailY[k] == i) {
                            cout << "o";
                            printTail = true;
                        }
                    }
                    if (!printTail)
                        cout << " ";
                }

                if (j == width - 1)
                    cout << "#";
            }
            cout << "\n";
        }

        for (int i = 0; i < width + 2; i++)
            cout << "#";
        cout << "\nScore: " << score << "\n";
    }

    void input() {
        char key = getInput();
        switch (key) {
            case 'a':
                if (dir != Direction::RIGHT) dir = Direction::LEFT;
                break;
            case 'd':
                if (dir != Direction::LEFT) dir = Direction::RIGHT;
                break;
            case 'w':
                if (dir != Direction::DOWN) dir = Direction::UP;
                break;
            case 's':
                if (dir != Direction::UP) dir = Direction::DOWN;
                break;
            case 'x':
                gameOver = true;
                break;
        }
    }

    void logic() {
        vector<int> prevX = tailX;
        vector<int> prevY = tailY;
        tailX.insert(tailX.begin(), x);
        tailY.insert(tailY.begin(), y);

        switch (dir) {
            case Direction::LEFT:  x--; break;
            case Direction::RIGHT: x++; break;
            case Direction::UP:    y--; break;
            case Direction::DOWN:  y++; break;
            default: break;
        }

        if (x >= width) x = 0;
        else if (x < 0) x = width - 1;
        if (y >= height) y = 0;
        else if (y < 0) y = height - 1;

        for (int i = 0; i < nTail; i++)
            if (tailX[i] == x && tailY[i] == y)
                gameOver = true;

        if (x == fruitX && y == fruitY) {
            score += 10;
            fruitX = rand() % width;
            fruitY = rand() % height;
            nTail++;
        }

        while (tailX.size() > nTail) tailX.pop_back();
        while (tailY.size() > nTail) tailY.pop_back();
    }

    void run() {
        // Скрываем курсор
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = FALSE;
        SetConsoleCursorInfo(consoleHandle, &info);

        while (!gameOver) {
            draw();
            input();
            logic();
            Sleep(100);
        }
    }
};

int main() {
    // Инициализация для Windows
    SetConsoleCP(437);
    SetConsoleOutputCP(437);
    
    // Установка размера консольного окна
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r);
    MoveWindow(console, r.left, r.top, 800, 600, TRUE);
    
    srand(static_cast<unsigned>(time(0)));
    SnakeGame game;
    game.run();
    
    return 0;
}
