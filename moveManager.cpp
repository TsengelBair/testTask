#include "movemanager.h"

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

// bool MoveManager::isIntersecting(const Rectangle &rect1, const Rectangle &rect2) const
// {
//     // Проверка пересечения прямоугольников
//     return !(rect1.getRightTop().first < rect2.getLeftTop().first ||   // rect1 находится левее rect2
//              rect1.getLeftTop().first > rect2.getRightTop().first ||   // rect1 находится правее rect2
//              rect1.getLeftTop().second > rect2.getLeftBottom().second ||  // rect1 находится выше rect2
//              rect1.getLeftBottom().second < rect2.getLeftTop().second);  // rect1 находится ниже rect2
// }

void MoveManager::moveRectangles() {
    if (movedRectangles.empty()) {
        return;
    }

    // Перемещаем фигуры так, чтобы их границы не пересекались
    for (size_t i = 0; i < movedRectangles.size() - 1; ++i) {
        // Проверяем, нужно ли переместить следующий прямоугольник
        int minYStep = movedRectangles[i].getBottom() - movedRectangles[i + 1].getTop();
        if (minYStep > 0) {
            // Перемещаем следующий прямоугольник вниз
            movedRectangles[i + 1].setLeftTop({movedRectangles[i + 1].getLeftTop().first, movedRectangles[i + 1].getLeftTop().second + minYStep + interval});
            movedRectangles[i + 1].setRightTop({movedRectangles[i + 1].getRightTop().first, movedRectangles[i + 1].getRightTop().second + minYStep + interval});
            movedRectangles[i + 1].setLeftBottom({movedRectangles[i + 1].getLeftBottom().first, movedRectangles[i + 1].getLeftBottom().second + minYStep + interval});
            movedRectangles[i + 1].setRightBottom({movedRectangles[i + 1].getRightBottom().first, movedRectangles[i + 1].getRightBottom().second + minYStep + interval});

            // Обновляем центр после перемещения
            movedRectangles[i + 1].updateCenterFromCorners();
        }
    }

    // Перемещаем все прямоугольники на добавленный интервал
    for (size_t i = 0; i < movedRectangles.size(); ++i) {
        int currentInterval = interval * (i + 1);
        movedRectangles[i].setLeftTop({movedRectangles[i].getLeftTop().first, movedRectangles[i].getLeftTop().second + currentInterval});
        movedRectangles[i].setRightTop({movedRectangles[i].getRightTop().first, movedRectangles[i].getRightTop().second + currentInterval});
        movedRectangles[i].setLeftBottom({movedRectangles[i].getLeftBottom().first, movedRectangles[i].getLeftBottom().second + currentInterval});
        movedRectangles[i].setRightBottom({movedRectangles[i].getRightBottom().first, movedRectangles[i].getRightBottom().second + currentInterval});

        // Обновляем центр после сдвига
        movedRectangles[i].updateCenterFromCorners();
    }

    // Обновляем вектор новых координат центров
    movedCenters.clear();
    movedCenters.reserve(movedRectangles.size());
    for (const auto& rect : movedRectangles) {
        movedCenters.emplace_back(rect.getCenter());
    }
}


std::vector<Rectangle> MoveManager::getMovedRectangles()
{
    return movedRectangles;
}
