#include "Piece.h"

// 默认构造函数
Piece::Piece() : color(EMPTY), x(-1), y(-1) {
}

// 带参数构造函数
Piece::Piece(PieceColor c, int posX, int posY) : color(c), x(posX), y(posY) {
}

// 拷贝构造函数
Piece::Piece(const Piece& other) : color(other.color), x(other.x), y(other.y) {
}

// 析构函数
Piece::~Piece() {
}

// 获取颜色
PieceColor Piece::getColor() const {
    return color;
}

// 获取X坐标
int Piece::getX() const {
    return x;
}

// 获取Y坐标
int Piece::getY() const {
    return y;
}

// 设置颜色
void Piece::setColor(PieceColor c) {
    color = c;
}

// 设置位置
void Piece::setPosition(int posX, int posY) {
    x = posX;
    y = posY;
}

