#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"

// 棋盘大小常量
const int BOARD_SIZE = 15;

// Board类 - 管理棋盘数据状态
class Board {
private:
    int grid[BOARD_SIZE][BOARD_SIZE];  // 棋盘网格 (0=空, 1=黑, 2=白)
    int pieceCount;                     // 已落子数量

public:
    // 构造函数
    Board();
    
    // 析构函数
    ~Board();
    
    // 放置棋子
    bool placePiece(int x, int y, PieceColor color);
    
    // 获取指定位置的棋子类型
    int getPieceType(int x, int y) const;
    
    // 检查位置是否为空
    bool isEmpty(int x, int y) const;
    
    // 清空棋盘
    void clear();
    
    // 检查棋盘是否已满
    bool isFull() const;
    
    // 检查坐标是否在棋盘范围内
    bool isValidPosition(int x, int y) const;
    
    // 获取已落子数量
    int getPieceCount() const;
    
    // 临时放置棋子(用于AI计算和禁手检测)
    void setTemp(int x, int y, int color);
    
    // 移除棋子(用于AI计算和禁手检测)
    void removeTemp(int x, int y);
};

#endif // BOARD_H

