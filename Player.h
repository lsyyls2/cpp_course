#ifndef PLAYER_H
#define PLAYER_H

#include "Board.h"
#include "Piece.h"

// Player抽象基类
class Player {
protected:
    PieceColor color;  // 玩家颜色
    
public:
    // 构造函数
    Player(PieceColor c);
    
    // 虚析构函数
    virtual ~Player();
    
    // 纯虚函数：获取下一步棋
    // 返回: 新创建的Piece对象，调用者负责释放
    virtual Piece* getMove(const Board& board) = 0;
    
    // 获取玩家颜色
    PieceColor getColor() const;
    
    // 是否是AI
    virtual bool isAI() const = 0;
};

// 人类玩家类
class HumanPlayer : public Player {
public:
    // 构造函数
    HumanPlayer(PieceColor c);
    
    // 析构函数
    virtual ~HumanPlayer();
    
    // 获取玩家输入的下一步棋
    virtual Piece* getMove(const Board& board) override;
    
    // 是否是AI
    virtual bool isAI() const override;
};

// AI玩家类
class AIPlayer : public Player {
public:
    // 构造函数
    AIPlayer(PieceColor c);
    
    // 析构函数
    virtual ~AIPlayer();
    
    // AI计算下一步棋
    virtual Piece* getMove(const Board& board) override;
    
    // 是否是AI
    virtual bool isAI() const override;
    
private:
    // 评估某个位置的分数
    int evaluatePosition(const Board& board, int x, int y, int color) const;
    
    // 计算某方向上的得分
    int evaluateDirection(const Board& board, int x, int y, int color, int dirX, int dirY) const;
    
    // 获取某条线上的连子情况
    int getLineScore(int count, int openEnds) const;
};

#endif // PLAYER_H

