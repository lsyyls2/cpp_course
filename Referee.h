#ifndef REFEREE_H
#define REFEREE_H

#include "Board.h"
#include "Piece.h"

// Referee类 - 裁判，负责规则判定
class Referee {
public:
    // 构造函数
    Referee();
    
    // 析构函数
    ~Referee();
    
    // 检查是否获胜 (五连珠)
    // 返回: true=获胜, false=未获胜
    bool checkWin(const Board& board, int x, int y, int color) const;
    
    // 检查禁手 (仅对黑棋有效)
    // 返回: true=禁手, false=非禁手
    // 规则: 三三禁手、四四禁手、长连禁手
    // 优先级: 五连 > 禁手 (如果同时形成五连和禁手，视为获胜)
    bool checkForbidden(Board& board, int x, int y) const;
    
private:
    // 四个方向的增量: 水平、垂直、左上到右下、右上到左下
    static const int dx[4];
    static const int dy[4];
    
    // 计算某一方向上的连续棋子数
    int countDirection(const Board& board, int x, int y, int color, int dirX, int dirY) const;
    
    // 计算某一方向上的总连续棋子数(两边)
    int countLine(const Board& board, int x, int y, int color, int direction) const;
    
    // 检查是否形成活三 (两端都是空的三连)
    // 活三: _XXX_ 形式，两端各有至少一个空位
    bool isLiveThree(Board& board, int x, int y, int color, int direction) const;
    
    // 检查是否形成四 (四连，包括活四和冲四)
    bool isFour(Board& board, int x, int y, int color, int direction) const;
    
    // 统计活三数量
    int countLiveThrees(Board& board, int x, int y, int color) const;
    
    // 统计四的数量
    int countFours(Board& board, int x, int y, int color) const;
    
    // 检查长连 (六子或以上)
    bool isOverline(const Board& board, int x, int y, int color) const;
};

#endif // REFEREE_H

