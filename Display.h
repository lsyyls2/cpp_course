#ifndef DISPLAY_H
#define DISPLAY_H

#include "Board.h"
#include "Piece.h"

// Display类 - 处理所有控制台输出/渲染
class Display {
public:
    // 构造函数
    Display();
    
    // 析构函数
    ~Display();
    
    // 绘制棋盘
    // lastMove: 最后一步棋，用于高亮显示
    void drawBoard(const Board& board, const Piece* lastMove) const;
    
    // 显示开始菜单
    void showMenu() const;
    
    // 显示游戏结果
    // winner: 1=黑棋赢, 2=白棋赢, 0=平局
    void showResult(int winner) const;
    
    // 显示超时信息
    // color: 超时的玩家颜色
    void showTimeout(int color) const;
    
    // 显示禁手信息
    void showForbidden() const;
    
    // 显示当前玩家
    void showCurrentPlayer(int color) const;
    
    // 显示输入提示
    void showInputPrompt() const;
    
    // 显示错误信息
    void showError(const char* message) const;
    
    // 显示剩余时间（倒计时）
    void showTimeRemaining(int seconds) const;
    
    // 清屏
    void clearScreen() const;
    
private:
    // 设置控制台颜色
    void setColor(int color) const;
    
    // 重置控制台颜色
    void resetColor() const;
};

#endif // DISPLAY_H

