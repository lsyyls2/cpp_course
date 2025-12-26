#define _CRT_SECURE_NO_WARNINGS
#include "Player.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// ==================== Player 基类实现 ====================

Player::Player(PieceColor c) : color(c) {
}

Player::~Player() {
}

PieceColor Player::getColor() const {
    return color;
}

// ==================== HumanPlayer 实现 ====================

HumanPlayer::HumanPlayer(PieceColor c) : Player(c) {
}

HumanPlayer::~HumanPlayer() {
}

Piece* HumanPlayer::getMove(const Board& board) {
    int x, y;
    
    while (true) {
        cin >> x >> y;
        
        // 检查输入是否有效
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "    输入无效，请输入两个数字 (行 列): ";
            continue;
        }
        
        // 检查坐标范围
        if (!board.isValidPosition(x, y)) {
            cout << "    坐标超出范围 (0-14)，请重新输入: ";
            continue;
        }
        
        // 检查位置是否为空
        if (!board.isEmpty(x, y)) {
            cout << "    该位置已有棋子，请重新输入: ";
            continue;
        }
        
        break;
    }
    
    return new Piece(color, x, y);
}

bool HumanPlayer::isAI() const {
    return false;
}

// ==================== AIPlayer 实现 ====================

AIPlayer::AIPlayer(PieceColor c) : Player(c) {
    srand(static_cast<unsigned int>(time(nullptr)));
}

AIPlayer::~AIPlayer() {
}

// 获取某条线上的得分
int AIPlayer::getLineScore(int count, int openEnds) const {
    // openEnds: 0=两端都被堵, 1=一端开放, 2=两端开放
    if (openEnds == 0 && count < 5) {
        return 0;  // 两端都被堵且不足5子，无价值
    }
    
    switch (count) {
        case 5:
            return 100000;  // 五连 - 获胜
        case 4:
            if (openEnds == 2) return 10000;   // 活四
            if (openEnds == 1) return 1000;    // 冲四
            break;
        case 3:
            if (openEnds == 2) return 1000;    // 活三
            if (openEnds == 1) return 100;     // 眠三
            break;
        case 2:
            if (openEnds == 2) return 100;     // 活二
            if (openEnds == 1) return 10;      // 眠二
            break;
        case 1:
            if (openEnds == 2) return 10;
            if (openEnds == 1) return 1;
            break;
    }
    return 0;
}

// 计算某方向上的得分
int AIPlayer::evaluateDirection(const Board& board, int x, int y, int evalColor, int dirX, int dirY) const {
    int count = 1;
    int openEnds = 0;
    
    // 正方向计数
    int nx = x + dirX;
    int ny = y + dirY;
    while (board.isValidPosition(nx, ny) && board.getPieceType(nx, ny) == evalColor) {
        count++;
        nx += dirX;
        ny += dirY;
    }
    if (board.isValidPosition(nx, ny) && board.getPieceType(nx, ny) == EMPTY) {
        openEnds++;
    }
    
    // 负方向计数
    nx = x - dirX;
    ny = y - dirY;
    while (board.isValidPosition(nx, ny) && board.getPieceType(nx, ny) == evalColor) {
        count++;
        nx -= dirX;
        ny -= dirY;
    }
    if (board.isValidPosition(nx, ny) && board.getPieceType(nx, ny) == EMPTY) {
        openEnds++;
    }
    
    return getLineScore(count, openEnds);
}

// 评估某个位置的总分数
int AIPlayer::evaluatePosition(const Board& board, int x, int y, int evalColor) const {
    int score = 0;
    
    // 四个方向
    int dx[] = {1, 0, 1, 1};
    int dy[] = {0, 1, 1, -1};
    
    for (int dir = 0; dir < 4; dir++) {
        score += evaluateDirection(board, x, y, evalColor, dx[dir], dy[dir]);
    }
    
    return score;
}

Piece* AIPlayer::getMove(const Board& board) {
    int bestX = -1, bestY = -1;
    int bestScore = -1;
    
    // 对手颜色
    int opponentColor = (color == BLACK) ? WHITE : BLACK;
    
    // 如果是第一步棋，下在中心
    if (board.getPieceCount() == 0) {
        return new Piece(color, BOARD_SIZE / 2, BOARD_SIZE / 2);
    }
    
    // 遍历所有空位
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (!board.isEmpty(i, j)) {
                continue;
            }
            
            // 只考虑周围有棋子的位置（优化）
            bool hasNeighbor = false;
            for (int di = -2; di <= 2 && !hasNeighbor; di++) {
                for (int dj = -2; dj <= 2 && !hasNeighbor; dj++) {
                    if (di == 0 && dj == 0) continue;
                    int ni = i + di;
                    int nj = j + dj;
                    if (board.isValidPosition(ni, nj) && !board.isEmpty(ni, nj)) {
                        hasNeighbor = true;
                    }
                }
            }
            
            if (!hasNeighbor && board.getPieceCount() > 0) {
                continue;
            }
            
            // 临时放置棋子计算分数
            Board& mutableBoard = const_cast<Board&>(board);
            mutableBoard.setTemp(i, j, color);
            
            // 进攻分数
            int attackScore = evaluatePosition(board, i, j, color);
            
            mutableBoard.removeTemp(i, j);
            mutableBoard.setTemp(i, j, opponentColor);
            
            // 防守分数 (阻止对手)
            int defenseScore = evaluatePosition(board, i, j, opponentColor);
            
            mutableBoard.removeTemp(i, j);
            
            // 总分 = 进攻 + 防守 * 0.9 (略微倾向进攻)
            int totalScore = attackScore + static_cast<int>(defenseScore * 0.9);
            
            // 添加一点随机性避免固定走法
            totalScore += rand() % 10;
            
            if (totalScore > bestScore) {
                bestScore = totalScore;
                bestX = i;
                bestY = j;
            }
        }
    }
    
    // 如果没找到合适位置，下在中心附近
    if (bestX == -1) {
        bestX = BOARD_SIZE / 2;
        bestY = BOARD_SIZE / 2;
        while (!board.isEmpty(bestX, bestY)) {
            bestX = rand() % BOARD_SIZE;
            bestY = rand() % BOARD_SIZE;
        }
    }
    
    return new Piece(color, bestX, bestY);
}

bool AIPlayer::isAI() const {
    return true;
}

