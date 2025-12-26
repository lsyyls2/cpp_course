#include "Board.h"

// 构造函数
Board::Board() : pieceCount(0) {
    clear();
}

// 析构函数
Board::~Board() {
}

// 放置棋子
bool Board::placePiece(int x, int y, PieceColor color) {
    if (!isValidPosition(x, y)) {
        return false;
    }
    if (!isEmpty(x, y)) {
        return false;
    }
    grid[x][y] = static_cast<int>(color);
    pieceCount++;
    return true;
}

// 获取指定位置的棋子类型
int Board::getPieceType(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return -1;
    }
    return grid[x][y];
}

// 检查位置是否为空
bool Board::isEmpty(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return false;
    }
    return grid[x][y] == EMPTY;
}

// 清空棋盘
void Board::clear() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            grid[i][j] = EMPTY;
        }
    }
    pieceCount = 0;
}

// 检查棋盘是否已满
bool Board::isFull() const {
    return pieceCount >= BOARD_SIZE * BOARD_SIZE;
}

// 检查坐标是否在棋盘范围内
bool Board::isValidPosition(int x, int y) const {
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

// 获取已落子数量
int Board::getPieceCount() const {
    return pieceCount;
}

// 临时放置棋子
void Board::setTemp(int x, int y, int color) {
    if (isValidPosition(x, y)) {
        grid[x][y] = color;
    }
}

// 移除棋子
void Board::removeTemp(int x, int y) {
    if (isValidPosition(x, y)) {
        grid[x][y] = EMPTY;
        // 注意：不减少pieceCount，因为这是临时操作
    }
}

