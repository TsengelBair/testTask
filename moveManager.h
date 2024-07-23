#ifndef MOVEMANAGER_H
#define MOVEMANAGER_H

#include "rectangle.h"

#include <vector>

class MoveManager
{
public:
    MoveManager(const std::vector<Rectangle>&rectangles, int yInterval);

    void moveRectangles();
    bool isIntersecting(const Rectangle& rect1, const Rectangle& rect2) const;

    std::vector<Rectangle>getMovedRectangles();
    std::vector<std::pair<int, int>>getMovedCenters();

private:
    std::vector<Rectangle>originalRectangles;
    std::vector<Rectangle>movedRectangles;

    std::vector<std::pair<int, int>>originalCenters;
    std::vector<std::pair<int, int>>movedCenters;
    int interval;
};

#endif // MOVEMANAGER_H
