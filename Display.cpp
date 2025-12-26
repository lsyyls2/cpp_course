#include "Display.h"
#include <iostream>
#include <iomanip>
#include <windows.h>

using namespace std;

// 构造函数
Display::Display() {
}

// 析构函数
Display::~Display() {
}

// 设置控制台颜色
void Display::setColor(int color) const {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// 重置控制台颜色
void Display::resetColor() const {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7);
}

// 清屏
void Display::clearScreen() const {
    system("cls");
}

// 绘制棋盘
void Display::drawBoard(const Board& board, const Piece* lastMove) const {
    clearScreen();
    
    cout << endl;
    cout << "  +---------------------------------------------+" << endl;
    cout << "  |              Gomoku (WuZiQi)                |" << endl;
    cout << "  +---------------------------------------------+" << endl << endl;
    
    // 打印上边框
    cout << "     +";
    for (int j = 0; j < BOARD_SIZE; j++) {
        cout << "--+";
    }
    cout << endl;
    
    // 打印棋盘内容 (从上到下: 15, 14, 13, ... 1)
    for (int displayRow = BOARD_SIZE; displayRow >= 1; displayRow--) {
        int i = BOARD_SIZE - displayRow;  // 实际数组索引
        
        cout << "  " << setw(2) << displayRow << " |";
        
        for (int j = 0; j < BOARD_SIZE; j++) {
            int piece = board.getPieceType(i, j);
            bool isLastMove = (lastMove != nullptr && 
                              lastMove->getX() == i && 
                              lastMove->getY() == j);
            
            if (piece == EMPTY) {
                cout << "  |";
            } else if (piece == BLACK) {
                if (isLastMove) {
                    setColor(14);  // 黄色高亮
                    cout << "X ";
                    resetColor();
                    cout << "|";
                } else {
                    cout << "X |";
                }
            } else if (piece == WHITE) {
                if (isLastMove) {
                    setColor(14);  // 黄色高亮
                    cout << "O ";
                    resetColor();
                    cout << "|";
                } else {
                    cout << "O |";
                }
            }
        }
        cout << endl;
        
        // 打印行分隔线
        cout << "     +";
        for (int j = 0; j < BOARD_SIZE; j++) {
            cout << "--+";
        }
        cout << endl;
    }
    
    // 打印列号 (字母a~o) 在底部
    cout << "      ";
    for (int j = 0; j < BOARD_SIZE; j++) {
        cout << " " << (char)('a' + j) << " ";
    }
    cout << endl << endl;
}

// 显示开始菜单
void Display::showMenu() const {
    clearScreen();
    
    cout << endl << endl;
    cout << "  +=============================================+" << endl;
    cout << "  |                                             |" << endl;
    cout << "  |              Gomoku (WuZiQi)                |" << endl;
    cout << "  |                                             |" << endl;
    cout << "  +=============================================+" << endl;
    cout << "  |                                             |" << endl;
    cout << "  |          1. Player vs Player (PvP)          |" << endl;
    cout << "  |                                             |" << endl;
    cout << "  |          2. Player vs Computer (PvE)        |" << endl;
    cout << "  |                                             |" << endl;
    cout << "  |          0. Exit                            |" << endl;
    cout << "  |                                             |" << endl;
    cout << "  +=============================================+" << endl;
    cout << endl;
    cout << "  Please select mode (0-2): ";
}

// 显示游戏结果
void Display::showResult(int winner) const {
    cout << endl;
    cout << "  +=============================================+" << endl;
    
    if (winner == BLACK) {
        setColor(10);
        cout << "  |          Game Over - Black(X) Wins!        |" << endl;
    } else if (winner == WHITE) {
        setColor(10);
        cout << "  |          Game Over - White(O) Wins!        |" << endl;
    } else {
        setColor(11);
        cout << "  |          Game Over - Draw!                 |" << endl;
    }
    
    resetColor();
    cout << "  +=============================================+" << endl << endl;
}

// 显示超时信息 (保留接口但不再使用)
void Display::showTimeout(int color) const {
    cout << endl;
    setColor(12);
    cout << "  +=============================================+" << endl;
    
    if (color == BLACK) {
        cout << "  |          Timeout! Black loses!             |" << endl;
    } else {
        cout << "  |          Timeout! White loses!             |" << endl;
    }
    
    cout << "  +=============================================+" << endl;
    resetColor();
    cout << endl;
}

// 显示禁手信息
void Display::showForbidden() const {
    setColor(12);
    cout << endl << "   Forbidden Move! Black violates rules, White wins! " << endl << endl;
    resetColor();
}

// 显示当前玩家
void Display::showCurrentPlayer(int color) const {
    if (color == BLACK) {
        cout << "  >> Your turn: Black (X)" << endl;
    } else {
        cout << "  >> Your turn: White (O)" << endl;
    }
}

// 显示输入提示
void Display::showInputPrompt() const {
    cout << "  Enter position (col row, e.g. h 8): ";
}

// 显示错误信息
void Display::showError(const char* message) const {
    setColor(12);
    cout << "  Error: " << message << endl;
    resetColor();
}

// 显示剩余时间 (保留接口但不再使用)
void Display::showTimeRemaining(int seconds) const {
    // 不再显示时间
}
