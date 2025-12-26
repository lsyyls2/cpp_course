#include "Game.h"
#include <iostream>
#include <windows.h>
#include <cctype>

using namespace std;

// 构造函数
Game::Game() {
    board = new Board();
    display = new Display();
    referee = new Referee();
    player1 = nullptr;
    player2 = nullptr;
    currentPlayer = nullptr;
    lastMove = nullptr;
    state = STATE_MENU;
    mode = MODE_EXIT;
    winner = 0;
}

// 析构函数
Game::~Game() {
    cleanup();
    
    delete board;
    delete display;
    delete referee;
}

// 清理资源
void Game::cleanup() {
    if (player1 != nullptr) {
        delete player1;
        player1 = nullptr;
    }
    if (player2 != nullptr) {
        delete player2;
        player2 = nullptr;
    }
    if (lastMove != nullptr) {
        delete lastMove;
        lastMove = nullptr;
    }
    currentPlayer = nullptr;
}

// 重置游戏
void Game::reset() {
    cleanup();
    board->clear();
    winner = 0;
    state = STATE_MENU;
}

// 运行游戏
void Game::run() {
    while (true) {
        if (!showMenuAndGetMode()) {
            break;  // 用户选择退出
        }
        
        init();
        gameLoop();
        
        if (!askPlayAgain()) {
            break;
        }
        
        reset();
    }
    
    display->clearScreen();
    cout << endl << "  Thanks for playing! Goodbye!" << endl << endl;
}

// 显示菜单并获取模式选择
bool Game::showMenuAndGetMode() {
    display->showMenu();
    
    int choice;
    while (true) {
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "  Invalid input, please enter 0-2: ";
            continue;
        }
        
        if (choice == 0) {
            return false;  // 退出游戏
        } else if (choice == 1) {
            mode = MODE_PVP;
            return true;
        } else if (choice == 2) {
            mode = MODE_PVE;
            return true;
        } else {
            cout << "  Invalid choice, please enter 0, 1 or 2: ";
        }
    }
}

// 初始化游戏
void Game::init() {
    // 创建玩家1 (黑棋, 总是人类)
    player1 = new HumanPlayer(BLACK);
    
    // 根据模式创建玩家2
    if (mode == MODE_PVP) {
        player2 = new HumanPlayer(WHITE);
    } else {
        player2 = new AIPlayer(WHITE);
    }
    
    // 黑棋先手
    currentPlayer = player1;
    state = STATE_PLAYING;
    
    // 清空棋盘
    board->clear();
    
    // 删除之前的lastMove
    if (lastMove != nullptr) {
        delete lastMove;
        lastMove = nullptr;
    }
}

// 显示玩家信息
void Game::showPlayersInfo() const {
    if (mode == MODE_PVP) {
        cout << "  Player 1: Black (X)  vs  Player 2: White (O)" << endl;
    } else {
        cout << "  Player: Black (X)  vs  Computer: White (O)" << endl;
    }
    cout << endl;
}

// 游戏主循环
void Game::gameLoop() {
    while (state == STATE_PLAYING) {
        // 绘制棋盘
        display->drawBoard(*board, lastMove);
        
        // 显示玩家信息
        showPlayersInfo();
        
        // 显示当前玩家
        display->showCurrentPlayer(currentPlayer->getColor());
        
        // 处理这一步
        if (!processMove()) {
            state = STATE_OVER;
        }
    }
    
    // 显示最终棋盘
    display->drawBoard(*board, lastMove);
    
    // 显示玩家信息
    showPlayersInfo();
    
    // 显示结果
    display->showResult(winner);
}

// 处理一步棋 (无超时限制)
bool Game::processMove() {
    Piece* move = nullptr;
    
    if (currentPlayer->isAI()) {
        // AI玩家
        cout << "  Computer is thinking..." << endl;
        move = currentPlayer->getMove(*board);
        
        // 显示AI的落子位置 (转换为显示坐标: 行从1-15, 从下往上)
        char colChar = 'a' + move->getY();
        int displayRow = BOARD_SIZE - move->getX();
        cout << "  Computer plays: " << colChar << " " << displayRow << endl;
    } else {
        // 人类玩家 - 直接获取输入
        display->showInputPrompt();
        
        char colChar;
        int row = -1, col = -1;
        bool validInput = false;
        
        while (!validInput) {
            cin >> colChar >> row;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "  Invalid input, please enter (col row, e.g. h 8): ";
                continue;
            }
            
            // 转换字母为列号
            colChar = tolower(colChar);
            if (colChar >= 'a' && colChar <= 'o') {
                col = colChar - 'a';
            } else {
                cout << "  Column must be a-o, please re-enter: ";
                continue;
            }
            
            // 验证行号范围 (1-15)
            if (row < 1 || row > 15) {
                cout << "  Row out of range (1-15), please re-enter: ";
                continue;
            }
            
            // 转换显示行号为数组索引 (显示1对应数组14, 显示15对应数组0)
            int arrayRow = BOARD_SIZE - row;
            
            if (!board->isEmpty(arrayRow, col)) {
                cout << "  Position occupied, please re-enter: ";
                continue;
            }
            
            row = arrayRow;  // 使用转换后的数组索引
            validInput = true;
        }
        
        move = new Piece(currentPlayer->getColor(), row, col);
    }
    
    // 检查禁手 (仅对黑棋)
    if (currentPlayer->getColor() == BLACK) {
        if (referee->checkForbidden(*board, move->getX(), move->getY())) {
            display->showForbidden();
            winner = WHITE;  // 黑棋禁手, 白棋获胜
            delete move;
            return false;
        }
    }
    
    // 放置棋子
    board->placePiece(move->getX(), move->getY(), currentPlayer->getColor());
    
    // 更新最后一步
    if (lastMove != nullptr) {
        delete lastMove;
    }
    lastMove = move;
    
    // 检查获胜
    if (referee->checkWin(*board, lastMove->getX(), lastMove->getY(), currentPlayer->getColor())) {
        winner = currentPlayer->getColor();
        return false;
    }
    
    // 检查平局
    if (board->isFull()) {
        winner = 0;  // 平局
        return false;
    }
    
    // 切换玩家
    switchTurn();
    
    return true;
}

// 切换当前玩家
void Game::switchTurn() {
    if (currentPlayer == player1) {
        currentPlayer = player2;
    } else {
        currentPlayer = player1;
    }
}

// 检查时间是否超时 (保留接口但不再使用)
bool Game::checkTime(long long elapsedSeconds) {
    return false;
}

// 询问是否再来一局
bool Game::askPlayAgain() {
    cout << endl << "  Play again? (1=Yes, 0=No): ";
    
    int choice;
    while (true) {
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "  Please enter 0 or 1: ";
            continue;
        }
        
        if (choice == 0 || choice == 1) {
            break;
        }
        
        cout << "  Please enter 0 or 1: ";
    }
    
    return choice == 1;
}
