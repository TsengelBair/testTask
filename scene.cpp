#include "scene.h"
#include <QDebug>
#include <QMessageBox>

Scene::Scene(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Graph scene");
    // Сцена и вид для отображения фигур
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);

    // Основной слой, на который добавляем view (отображение фигур) и btnLayout (в нем кнопка "Переместить" и lineEdit)
    sceneLayout = new QVBoxLayout(this);
    sceneLayout->addWidget(view);

    // Доп горизонтальный слой для расположения кнопки и вводимого в lineEdit параметра Y
    btnLayout = new QHBoxLayout;
    yParam = new QLineEdit(this);
    moveBtn = new QPushButton("Переместить", this);
    yParam->setPlaceholderText("y");
    btnLayout->addWidget(yParam);
    btnLayout->addWidget(moveBtn);
    connect(moveBtn, &QPushButton::clicked, this, &Scene::onMoveBtnClicked);

    sceneLayout->addLayout(btnLayout);
    setLayout(sceneLayout);
}

void Scene::drawRectangles(const std::vector<Rectangle>& rectangles) {
    scene->clear(); // Очистка предыдущих прямоугольников
    for (const Rectangle& rect : rectangles) {
        int leftTopX = rect.getLeftTop().first;
        int leftTopY = rect.getLeftTop().second;
        int width = rect.getWidth();
        int height = rect.getHeight();

        QGraphicsRectItem* rectItem = scene->addRect(leftTopX, leftTopY, width, height);

        QBrush brush(Qt::transparent); // без заливки (т.е. прозрачная фигура)
        QPen pen(Qt::red); // контур красный
        rectItem->setBrush(brush);
        rectItem->setPen(pen);
    }
}

void Scene::setRectangles(const std::vector<Rectangle>& rectangles) {
    this->rectangles = rectangles;
}

void Scene::onMoveBtnClicked() {
    bool ok;
    int yInterval = yParam->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Ошибка", "Неверный ввод интервала!");
        return;
    }

    MoveManager moveManager(rectangles, yInterval);
    moveManager.moveRectangles();

    // Перерисовываем прямоугольники после сдвига
    drawRectangles(moveManager.getMovedRectangles());
}

Scene::~Scene() {
    // Деструктор пуст, т.к. у виджетов указал родителя
}
