#include "mainwindow.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), sceneWindow(nullptr)
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    mainLayout = new QVBoxLayout(centralWidget);

    QLabel* figuresLabel = new QLabel("Количество фигур:");
    comboBoxFigures = new QComboBox(this);
    for (int i = 1; i <= 10; ++i) {
        comboBoxFigures->addItem(QString::number(i));
    }
    connect(comboBoxFigures, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxChanged(int)));
    mainLayout->addWidget(figuresLabel);
    mainLayout->addWidget(comboBoxFigures);

    QLabel* figuresParams = new QLabel("Параметры фигур:");
    mainLayout->addWidget(figuresParams);

    setupUi();

    btnDraw = new QPushButton("Построить", this);
    connect(btnDraw, &QPushButton::clicked, this, &MainWindow::onBtnDrawClicked);
    mainLayout->addWidget(btnDraw);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUi()
{
    createFigureInputFields(1); // По дефолту одно поле
}

void MainWindow::createFigureInputFields(int numFigures)
{
    for (int i = 0; i < numFigures; ++i) {
        QHBoxLayout* figureLayout = new QHBoxLayout();
        figureLayouts.append(figureLayout);

        QLineEdit* lineEditX = new QLineEdit(this);
        lineEditX->setPlaceholderText("X");
        figureParamsFields.append(lineEditX);
        figureLayout->addWidget(lineEditX);

        QLineEdit* lineEditY = new QLineEdit(this);
        lineEditY->setPlaceholderText("Y");
        figureParamsFields.append(lineEditY);
        figureLayout->addWidget(lineEditY);

        QLineEdit* lineEditW = new QLineEdit(this);
        lineEditW->setPlaceholderText("Width");
        figureParamsFields.append(lineEditW);
        figureLayout->addWidget(lineEditW);

        QLineEdit* lineEditH = new QLineEdit(this);
        lineEditH->setPlaceholderText("Height");
        figureParamsFields.append(lineEditH);
        figureLayout->addWidget(lineEditH);

        mainLayout->addLayout(figureLayout);
    }
}

void MainWindow::onComboBoxChanged(int num)
{
    for (QHBoxLayout* layout : figureLayouts) {
        QLayoutItem* item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        mainLayout->removeItem(layout);
        delete layout;
    }
    figureLayouts.clear();
    figureParamsFields.clear();

    int numFigures = comboBoxFigures->itemText(num).toInt();
    createFigureInputFields(numFigures);
    mainLayout->addWidget(btnDraw);
    adjustSize();
}

void MainWindow::onBtnDrawClicked()
{
    rectangles.clear(); // Очистка предыдущих прямоугольников
    rectanglesCenters.clear(); // Очистка предыдущих координат

    for (int i = 0; i < figureParamsFields.size(); i += 4) {
        bool allOk = true;
        int x = figureParamsFields[i]->text().toInt(&allOk);
        int y = figureParamsFields[i + 1]->text().toInt(&allOk);
        int w = figureParamsFields[i + 2]->text().toInt(&allOk);
        int h = figureParamsFields[i + 3]->text().toInt(&allOk);

        if (!allOk) {
            QMessageBox::warning(this, "Ошибка", "Неверный ввод параметров фигуры!");
            return;
        }

        Rectangle rect(x, y, w, h);
        rectangles.push_back(rect);
        rectanglesCenters.push_back(rect.getCenter());
    }

    if (sceneWindow != nullptr) {
        delete sceneWindow;
    }

    sceneWindow = new Scene();
    sceneWindow->setRectangles(rectangles);
    sceneWindow->drawRectangles(rectangles);

    sceneWindow->show();
}


