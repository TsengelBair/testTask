#include "graphic.h"

Graphic::Graphic(QWidget* parent) : QWidget(parent)
{
    setWindowTitle("Graph scene");
    // сцена и вид для отображения фигур
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);

    // вертикальный слой для размещения view и слоя btnLayout (в нем кнопка и lineEdit)
    graphicLayout = new QVBoxLayout;
    graphicLayout->addWidget(view);

    // Доп горизонтальный слой для расположения кнопки и вводимого в lineEdit параметра Y
    btnLayout = new QHBoxLayout;
    yParam = new QLineEdit(this);
    yParam->setPlaceholderText("y");
    btnMove = new QPushButton("Переместить", this);
    connect(btnMove, &QPushButton::clicked, this, &Graphic::onBtnMoveClicked);
    btnLayout->addWidget(yParam);
    btnLayout->addWidget(btnMove);
    graphicLayout->addLayout(btnLayout);

    setLayout(graphicLayout);
}

void Graphic::addRectangle(int centerX, int centerY, int width, int height)
{
    // Вычисляем координаты углов прямоугольника
    int left = centerX - width / 2;
    int top = centerY - height / 2;  // т.к. в Qt ось oY направлена вниз, для верхней точки вычитаем
    int right = centerX + width / 2;
    int bottom = centerY + height / 2;  // для нижней прибавляем

    // addRect ожидает коор-ту верхнего левого для создания прямоугольника
    QGraphicsRectItem* rect = scene->addRect(left, top, width, height);

    // Сохраняем данные прямоугольника
    RectangleCords data = {QPair<int, int>(centerX, centerY),
                           QPair<int, int>(left, top),
                           QPair<int, int>(right, top),
                           QPair<int, int>(left, bottom),
                           QPair<int, int>(right, bottom),
                           rect};
    rects.push_back(data);
}

bool Graphic::isIntersecting(const RectangleCords& rect1, const RectangleCords& rect2) const
{
    // Проверяем условия, при которых прямоугольники не пересекаются
    if (rect1.rightTop.first < rect2.leftTop.first ||   // rect1 справа от rect2
        rect1.leftTop.first > rect2.rightTop.first ||   // rect1 слева от rect2
        rect1.leftTop.second > rect2.leftBottom.second ||  // rect1 выше rect2
        rect1.leftBottom.second < rect2.leftTop.second) {   // rect1 ниже rect2
        return false;
    }

    return true;
}

void Graphic::onBtnMoveClicked()
{
    // Перемещаем фигуры, так чтобы их границы пересекались
    moveRectangles();

    QString yIntervalStr = yParam->text();
    int yInterval = yIntervalStr.toInt();

    // Добавляем интервалы
    for (int i = 0; i < rects.size(); ++i) {
        int currentInterval = yInterval * (i + 1);
        rects[i].leftTop.second += currentInterval;
        rects[i].rightTop.second += currentInterval;
        rects[i].leftBottom.second += currentInterval;
        rects[i].rightBottom.second += currentInterval;

        rects[i].item->setRect(rects[i].leftTop.first, rects[i].leftTop.second,
                               rects[i].rightTop.first - rects[i].leftTop.first,
                               rects[i].leftBottom.second - rects[i].leftTop.second);
    }
}



void Graphic::moveRectangles()
{
    // Сортируем rects по координате центра по оси Y (от самой нижней до самой верхней)
    std::sort(rects.begin(), rects.end(), [](const RectangleCords& a, const RectangleCords& b) {
        return a.center.second > b.center.second; // сортируем по убыванию y, самая нижняя фигура будет иметь максимальное значение по y
    });

    // Перемещаем фигуры
    for (int i = 0; i < rects.size() - 1; ++i) {
        // Определяем минимальный шаг по оси Y, чтобы прямоугольники пересеклись
        int minYStep = rects[i].leftBottom.second - rects[i + 1].leftTop.second;
        if (minYStep > 0) {
            // Двигаем следующую фигуру вниз на minYStep
            rects[i + 1].leftTop.second += minYStep;
            rects[i + 1].rightTop.second += minYStep;
            rects[i + 1].leftBottom.second += minYStep;
            rects[i + 1].rightBottom.second += minYStep;

            // Обновляем положение фигуры на сцене
            rects[i + 1].item->setRect(rects[i + 1].leftTop.first, rects[i + 1].leftTop.second,
                                       rects[i + 1].rightTop.first - rects[i + 1].leftTop.first,
                                       rects[i + 1].leftBottom.second - rects[i + 1].leftTop.second);
        }
    }
}




Graphic::~Graphic()
{
    // Указал родителя для виджетов, поэтому деструктор пуст
}
