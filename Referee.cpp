#include "Referee.h"

// 四个方向的增量
const int Referee::dx[4] = {0, 1, 1, 1};   // 水平, 垂直, 主对角线, 副对角线
const int Referee::dy[4] = {1, 0, 1, -1};

// 构造函数
Referee::Referee() {
}

// 析构函数
Referee::~Referee() {
}

// 计算某一方向上的连续棋子数 (单向)
int Referee::countDirection(const Board& board, int x, int y, int color, int dirX, int dirY) const {
    int count = 0;
    int nx = x + dirX;
    int ny = y + dirY;
    
    while (board.isValidPosition(nx, ny) && board.getPieceType(nx, ny) == color) {
        count++;
        nx += dirX;
        ny += dirY;
    }
    
    return count;
}

// 计算某一方向上的总连续棋子数 (两边)
int Referee::countLine(const Board& board, int x, int y, int color, int direction) const {
    int count = 1;  // 包含当前棋子
    count += countDirection(board, x, y, color, dx[direction], dy[direction]);
    count += countDirection(board, x, y, color, -dx[direction], -dy[direction]);
    return count;
}

// 检查是否获胜
bool Referee::checkWin(const Board& board, int x, int y, int color) const {
    // 检查四个方向
    for (int dir = 0; dir < 4; dir++) {
        if (countLine(board, x, y, color, dir) >= 5) {
            return true;
        }
    }
    return false;
}

// 检查是否形成活三
// 活三的定义：在这条线上，再走一步就能形成活四的三
bool Referee::isLiveThree(Board& board, int x, int y, int color, int direction) const {
    // 首先计算这条线上的连续棋子
    int count = 1;
    int left = 0, right = 0;
    
    // 向正方向数
    int nx = x + dx[direction];
    int ny = y + dy[direction];
    while (board.isValidPosition(nx, ny) && board.getPieceType(nx, ny) == color) {
        right++;
        nx += dx[direction];
        ny += dy[direction];
    }
    // 正方向末端位置
    int rightEndX = nx;
    int rightEndY = ny;
    bool rightOpen = board.isValidPosition(rightEndX, rightEndY) && board.getPieceType(rightEndX, rightEndY) == EMPTY;
    
    // 向负方向数
    nx = x - dx[direction];
    ny = y - dy[direction];
    while (board.isValidPosition(nx, ny) && board.getPieceType(nx, ny) == color) {
        left++;
        nx -= dx[direction];
        ny -= dy[direction];
    }
    // 负方向末端位置
    int leftEndX = nx;
    int leftEndY = ny;
    bool leftOpen = board.isValidPosition(leftEndX, leftEndY) && board.getPieceType(leftEndX, leftEndY) == EMPTY;
    
    count = 1 + left + right;
    
    // 活三：恰好3子，两端都开放
    if (count == 3 && leftOpen && rightOpen) {
        // 还需要检查两端外侧是否还有空间形成活四
        // 检查是否是真正的活三：_XXX_ 且两端外侧至少一边有空位
        return true;
    }
    
    // 跳活三：如 X_XX 或 XX_X 的情况
    // 检查 _X_XX_ 或 _XX_X_ 模式
    if (count == 2) {
        // 检查是否有跳的情况
        // 正方向：检查空一格后是否有同色子
        if (rightOpen) {
            int jumpX = rightEndX + dx[direction];
            int jumpY = rightEndY + dy[direction];
            if (board.isValidPosition(jumpX, jumpY) && board.getPieceType(jumpX, jumpY) == color) {
                // 检查跳过去的子后面是否开放
                int afterJumpX = jumpX + dx[direction];
                int afterJumpY = jumpY + dy[direction];
                bool afterJumpOpen = board.isValidPosition(afterJumpX, afterJumpY) && 
                                    board.getPieceType(afterJumpX, afterJumpY) == EMPTY;
                if (leftOpen && afterJumpOpen) {
                    return true;
                }
            }
        }
        // 负方向：检查空一格后是否有同色子
        if (leftOpen) {
            int jumpX = leftEndX - dx[direction];
            int jumpY = leftEndY - dy[direction];
            if (board.isValidPosition(jumpX, jumpY) && board.getPieceType(jumpX, jumpY) == color) {
                int afterJumpX = jumpX - dx[direction];
                int afterJumpY = jumpY - dy[direction];
                bool afterJumpOpen = board.isValidPosition(afterJumpX, afterJumpY) && 
                                    board.getPieceType(afterJumpX, afterJumpY) == EMPTY;
                if (rightOpen && afterJumpOpen) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

// 检查是否形成四
bool Referee::isFour(Board& board, int x, int y, int color, int direction) const {
    int count = countLine(board, x, y, color, direction);
    
    // 直接四连
    if (count == 4) {
        return true;
    }
    
    // 跳四：如 X_XXX 或 XXX_X 或 XX_XX
    if (count == 3) {
        // 向正方向找
        int nx = x + dx[direction];
        int ny = y + dy[direction];
        while (board.isValidPosition(nx, ny) && board.getPieceType(nx, ny) == color) {
            nx += dx[direction];
            ny += dy[direction];
        }
        // 检查是否空一格后有同色子
        if (board.isValidPosition(nx, ny) && board.getPieceType(nx, ny) == EMPTY) {
            int jumpX = nx + dx[direction];
            int jumpY = ny + dy[direction];
            if (board.isValidPosition(jumpX, jumpY) && board.getPieceType(jumpX, jumpY) == color) {
                return true;
            }
        }
        
        // 向负方向找
        nx = x - dx[direction];
        ny = y - dy[direction];
        while (board.isValidPosition(nx, ny) && board.getPieceType(nx, ny) == color) {
            nx -= dx[direction];
            ny -= dy[direction];
        }
        if (board.isValidPosition(nx, ny) && board.getPieceType(nx, ny) == EMPTY) {
            int jumpX = nx - dx[direction];
            int jumpY = ny - dy[direction];
            if (board.isValidPosition(jumpX, jumpY) && board.getPieceType(jumpX, jumpY) == color) {
                return true;
            }
        }
    }
    
    return false;
}

// 统计活三数量
int Referee::countLiveThrees(Board& board, int x, int y, int color) const {
    int count = 0;
    for (int dir = 0; dir < 4; dir++) {
        if (isLiveThree(board, x, y, color, dir)) {
            count++;
        }
    }
    return count;
}

// 统计四的数量
int Referee::countFours(Board& board, int x, int y, int color) const {
    int count = 0;
    for (int dir = 0; dir < 4; dir++) {
        if (isFour(board, x, y, color, dir)) {
            count++;
        }
    }
    return count;
}

// 检查长连
bool Referee::isOverline(const Board& board, int x, int y, int color) const {
    for (int dir = 0; dir < 4; dir++) {
        if (countLine(board, x, y, color, dir) >= 6) {
            return true;
        }
    }
    return false;
}

// 检查禁手 (仅对黑棋)
bool Referee::checkForbidden(Board& board, int x, int y) const {
    // 临时放置黑棋
    board.setTemp(x, y, BLACK);
    
    // 优先检查是否获胜 (五连优先于禁手)
    if (checkWin(board, x, y, BLACK)) {
        board.removeTemp(x, y);
        return false;  // 形成五连，不是禁手
    }
    
    // 检查长连禁手 (六子或以上)
    if (isOverline(board, x, y, BLACK)) {
        board.removeTemp(x, y);
        return true;  // 长连禁手
    }
    
    // 检查三三禁手
    int liveThrees = countLiveThrees(board, x, y, BLACK);
    if (liveThrees >= 2) {
        board.removeTemp(x, y);
        return true;  // 三三禁手
    }
    
    // 检查四四禁手
    int fours = countFours(board, x, y, BLACK);
    if (fours >= 2) {
        board.removeTemp(x, y);
        return true;  // 四四禁手
    }
    
    board.removeTemp(x, y);
    return false;  // 非禁手
}

