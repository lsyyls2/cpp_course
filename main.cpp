#define _CRT_SECURE_NO_WARNINGS
#include "Game.h"
#include <cstdlib>
#include <windows.h>

int main() {
    // 设置控制台UTF-8编码
    system("chcp 65001");
    
    // 设置控制台窗口标题
    SetConsoleTitle(TEXT("五子棋 - Gomoku"));
    
    // 创建游戏实例并运行
    Game* game = new Game();
    game->run();
    
    // 清理
    delete game;
    
    return 0;
}

