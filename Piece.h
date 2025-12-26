#ifndef PIECE_H
#define PIECE_H

// 棋子颜色枚举
enum PieceColor {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

// Piece类 - 表示单个棋子
class Piece {
private:
    PieceColor color;  // 棋子颜色
    int x;             // x坐标 (行)
    int y;             // y坐标 (列)

public:
    // 构造函数
    Piece();
    Piece(PieceColor c, int posX, int posY);
    
    // 拷贝构造函数
    Piece(const Piece& other);
    
    // 析构函数
    ~Piece();
    
    // Getter方法
    PieceColor getColor() const;
    int getX() const;
    int getY() const;
    
    // Setter方法
    void setColor(PieceColor c);
    void setPosition(int posX, int posY);
};

#endif // PIECE_H

