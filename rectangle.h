#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <utility>  // для std::pair

// Вся логика описана прямо в хэдере, без cpp
class Rectangle
{
public:
    Rectangle(int x, int y, int w, int h)
        : center(x, y), width(w), height(h) {
        updateCorners();
    }

    void updateCorners() {
        leftTop = {center.first - width / 2, center.second - height / 2};
        rightTop = {center.first + width / 2, center.second - height / 2};
        leftBottom = {center.first - width / 2, center.second + height / 2};
        rightBottom = {center.first + width / 2, center.second + height / 2};
    }

    void updateCenterFromCorners() {
        center = {leftTop.first + width / 2, leftTop.second + height / 2};
    }

    // Геттеры для получения углов прямоугольника
    std::pair<int, int> getLeftTop() const { return leftTop; }
    std::pair<int, int> getRightTop() const { return rightTop; }
    std::pair<int, int> getLeftBottom() const { return leftBottom; }
    std::pair<int, int> getRightBottom() const { return rightBottom; }

    // Геттеры для получения крайних координат по оси Y
    int getTop() const { return leftTop.second; }
    int getBottom() const { return leftBottom.second; }

    // Геттеры для ширины, высоты и центра прямоугольника
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    std::pair<int, int> getCenter() const { return center; }

    // Сеттеры для обновления координат углов
    void setLeftTop(std::pair<int, int> lt) { leftTop = lt; }
    void setRightTop(std::pair<int, int> rt) { rightTop = rt; }
    void setLeftBottom(std::pair<int, int> lb) { leftBottom = lb; }
    void setRightBottom(std::pair<int, int> rb) { rightBottom = rb; }

private:
    std::pair<int, int> center;
    int width, height;

    // Точки, образующие прямоугольник
    std::pair<int, int> leftTop;
    std::pair<int, int> rightTop;
    std::pair<int, int> leftBottom;
    std::pair<int, int> rightBottom;
};

#endif // RECTANGLE_H
