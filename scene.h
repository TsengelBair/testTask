#ifndef SCENE_H
#define SCENE_H

#include "rectangle.h"
#include "movemanager.h"

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QGraphicsRectItem>

class Scene : public QWidget {
    Q_OBJECT
public:
    Scene(QWidget* parent = nullptr);
    ~Scene();

    void drawRectangles(const std::vector<Rectangle>& rectangles); // Отрисовка прямоугольников
    void setRectangles(const std::vector<Rectangle>& rectangles); // Установка прямоугольников

private:
    QGraphicsScene* scene;
    QGraphicsView* view;
    QVBoxLayout* sceneLayout; // Главный слой
    QHBoxLayout* btnLayout; // Слой, куда поместим кнопку с lineEdit и этот слой внутрь sceneLayout
    QLineEdit* yParam;
    QPushButton* moveBtn;

    std::vector<Rectangle> rectangles; // Хранение прямоугольников

private slots:
    void onMoveBtnClicked();
};

#endif // SCENE_H
