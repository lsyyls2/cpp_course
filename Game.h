#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Display.h"
#include "Referee.h"
#include "Player.h"
#include "Piece.h"

// 游戏模式枚举
enum GameMode {
    MODE_PVP = 1,  // 玩家对战
    MODE_PVE = 2,  // 人机对战
    MODE_EXIT = 0  // 退出
};

// 游戏状态枚举
enum GameState {
    STATE_MENU,     // 菜单状态
    STATE_PLAYING,  // 游戏进行中
    STATE_OVER      // 游戏结束
};

// Game类 - 游戏调度器/控制器
class Game {
private:
    Board* board;           // 棋盘
    Display* display;       // 显示器
    Referee* referee;       // 裁判
    Player* player1;        // 玩家1 (黑棋)
    Player* player2;        // 玩家2 (白棋)
    Player* currentPlayer;  // 当前玩家
    Piece* lastMove;        // 最后一步棋
    GameState state;        // 游戏状态
    GameMode mode;          // 游戏模式
    int winner;             // 获胜者 (0=平局, 1=黑, 2=白)
    
public:
    // 构造函数
    Game();
    
    // 析构函数
    ~Game();
    
    // 运行游戏
    void run();
    
private:
    // 初始化游戏
    void init();
    
    // 显示菜单并获取模式选择
    bool showMenuAndGetMode();
    
    // 游戏主循环
    void gameLoop();
    
    // 显示玩家信息
    void showPlayersInfo() const;
    
    // 切换当前玩家
    void switchTurn();
    
    // 检查时间是否超时 (保留接口)
    bool checkTime(long long elapsedSeconds);
    
    // 处理一步棋
    // 返回: true=游戏继续, false=游戏结束
    bool processMove();
    
    // 重置游戏
    void reset();
    
    // 清理资源
    void cleanup();
    
    // 询问是否再来一局
    bool askPlayAgain();
};

#endif // GAME_H

