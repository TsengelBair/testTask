#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <QPair>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>

// Структура прямоугольника для отслеживания координат точек
struct RectangleCords {
    QPair<int, int> center; // Добавил коор-ты центральной точки
    QPair<int, int> leftTop;
    QPair<int, int> rightTop;
    QPair<int, int> rightBottom;
    QPair<int, int> leftBottom;
    QGraphicsRectItem* item;
};

class Graphic : public QWidget {
    Q_OBJECT

public:
    Graphic(QWidget* parent = nullptr);
    ~Graphic();

    void addRectangle(int centerX, int centerY, int width, int height);

private:
    QVector<RectangleCords> rects;
    QGraphicsScene* scene;
    QGraphicsView* view;
    QVBoxLayout* graphicLayout; // доп слой для корректного расположения кнопки
    QHBoxLayout* btnLayout; // доп слой, куда поместим кнопку и lineEdit, в которой вводят расстояние
    QPushButton* btnMove;
    QLineEdit* yParam;

    bool isIntersecting(const RectangleCords& rect1, const RectangleCords& rect2) const;
    void moveRectangles();

private slots:
    void onBtnMoveClicked();
};

#endif // GRAPHIC_H
