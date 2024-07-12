#include "graphic.h"

Graphic::Graphic(QWidget* parent) : QWidget(parent)
{
    setWindowTitle("Graph scene");
    // Создаем сцену и вид
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);

    // Создаем вертикальный макет для размещения view и кнопки
    graphicLayout = new QVBoxLayout(this);
    btnMove = new QPushButton("Нажми меня", this);
    connect(btnMove, &QPushButton::clicked, this, &Graphic::onBtnMoveClicked);
    graphicLayout->addWidget(view);
    graphicLayout->addWidget(btnMove);

    // Устанавливаем макет для текущего виджета
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
    RectangleCords data = {QPair<int, int>(left, top),
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
    moveRectangles();
}

void Graphic::moveRectangles()
{
    bool moved = true;
    while (moved) {
        moved = false;
        for (int i = 0; i < rects.size(); ++i) {
            for (int j = i + 1; j < rects.size(); ++j) {
                if (isIntersecting(rects[i], rects[j])) {
                    // Определяем минимальный шаг по оси Y, чтобы прямоугольники перестали пересекаться
                    int minYStep = rects[j].leftTop.second - rects[i].leftBottom.second + 1;

                    // Двигаем rect2 вниз на minYStep
                    rects[j].leftTop.second += minYStep;
                    rects[j].rightTop.second += minYStep;
                    rects[j].leftBottom.second += minYStep;
                    rects[j].rightBottom.second += minYStep;

                    // Обновляем положение фигуры на сцене
                    rects[j].item->setRect(rects[j].leftTop.first, rects[j].leftTop.second,
                                           rects[j].rightTop.first - rects[j].leftTop.first,
                                           rects[j].leftBottom.second - rects[j].leftTop.second);

                    moved = true;  // Помечаем, что было выполнено перемещение
                }
            }
        }
    }
}

Graphic::~Graphic()
{
    delete btnMove;
    delete graphicLayout;
    delete view;
    delete scene;
}
