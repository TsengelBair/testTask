#include "movemanager.h"
#include <iostream>
#include <algorithm>

MoveManager::MoveManager(const std::vector<Rectangle>& rectangles, int yInterval)
    : originalRectangles(rectangles), interval(yInterval)
{
    // Копируем и сортируем оригинальные прямоугольники, чтобы при сравнении со сдвинутыми соответствовали
    std::vector<Rectangle> sortedOriginalRectangles = originalRectangles;
    std::sort(sortedOriginalRectangles.begin(), sortedOriginalRectangles.end(), [](const Rectangle& a, const Rectangle& b) {
        return a.getCenter().second > b.getCenter().second;
    });

    // Теперь исходные прямоугольники хранятся в отсортированном порядке
    originalRectangles = sortedOriginalRectangles;

    // Резервируем место и заполняем вектор центров
    originalCenters.reserve(originalRectangles.size());
    for (const auto &rec : originalRectangles) {
        originalCenters.emplace_back(rec.getCenter());
    }

    // Инициализируем movedRectangles тем же отсортированным порядком
    movedRectangles = originalRectangles;
}

bool MoveManager::isIntersecting(const Rectangle &rect1, const Rectangle &rect2) const {
    return !(rect1.getRightTop().first < rect2.getLeftTop().first ||   // rect1 находится левее rect2
             rect1.getLeftTop().first > rect2.getRightTop().first ||   // rect1 находится правее rect2
             rect1.getLeftTop().second > rect2.getLeftBottom().second ||  // rect1 находится выше rect2
             rect1.getLeftBottom().second < rect2.getLeftTop().second);  // rect1 находится ниже rect2
}

void MoveManager::moveRectangles() {
    if (movedRectangles.empty()) {
        return;
    }

    // Перемещаем прямоугольники так, чтобы их границы пересекались
    for (size_t i = 0; i < movedRectangles.size() - 1; ++i) {
        for (size_t j = i + 1; j < movedRectangles.size(); ++j) {
            // Проверяем пересечения
            if (isIntersecting(movedRectangles[i], movedRectangles[j])) {
                // Вычисляем новый Y для верхнего прямоугольника, чтобы нижняя граница одного пересекалась с верхней границей другого
                int newY = movedRectangles[i].getLeftTop().second - movedRectangles[j].getHeight();
                movedRectangles[j].setLeftTop({movedRectangles[j].getLeftTop().first, newY});
                movedRectangles[j].setRightTop({movedRectangles[j].getRightTop().first, newY});
                movedRectangles[j].setLeftBottom({movedRectangles[j].getLeftBottom().first, newY + movedRectangles[j].getHeight()});
                movedRectangles[j].setRightBottom({movedRectangles[j].getRightBottom().first, newY + movedRectangles[j].getHeight()});
            }
        }
    }

    // Добавляем интервал между прямоугольниками
    for (size_t i = 1; i < movedRectangles.size(); ++i) {
        int prevBottomY = movedRectangles[i-1].getLeftBottom().second;
        int newY = prevBottomY + interval;

        int height = movedRectangles[i].getHeight();
        movedRectangles[i].setLeftTop({movedRectangles[i].getLeftTop().first, newY});
        movedRectangles[i].setRightTop({movedRectangles[i].getRightTop().first, newY});
        movedRectangles[i].setLeftBottom({movedRectangles[i].getLeftBottom().first, newY + height});
        movedRectangles[i].setRightBottom({movedRectangles[i].getRightBottom().first, newY + height});

        // Обновляем центр после перемещения
        movedRectangles[i].updateCenterFromCorners();
    }

    // Обновляем вектор новых координат центров
    movedCenters.clear();
    movedCenters.reserve(movedRectangles.size());
    for (const auto& rect : movedRectangles) {
        movedCenters.emplace_back(rect.getCenter());
    }

    std::cout << "Original Centers:" << std::endl;
    for (const auto& center : originalCenters) {
        std::cout << "Center: (" << center.first << ", " << center.second << ")" << std::endl;
    }

    std::cout << "New Centers:" << std::endl;
    for (const auto& center : movedCenters) {
        std::cout << "Center: (" << center.first << ", " << center.second << ")" << std::endl;
    }
}



std::vector<Rectangle> MoveManager::getMovedRectangles() {
    return movedRectangles;
}

std::vector<std::pair<int, int>> MoveManager::getMovedCenters()  {
    return movedCenters;
}
