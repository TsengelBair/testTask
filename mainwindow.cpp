#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), graphic(nullptr) // инициализируем graphic(второе окно) как nullptr
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

    // Добавляем кнопку после вызова setupUi, чтобы кнопка была внизу
    btnDraw = new QPushButton("Построить", this);
    connect(btnDraw, &QPushButton::clicked, this, &MainWindow::onBtnDrawClicked);
    mainLayout->addWidget(btnDraw);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUi()
{
    createFigureInputFields(1); // По дефолту одно поле
}

// Метод для создания полей для выбранного кол-ва фигур
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
    // Удаление старых полей ввода
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

    // Создание новых полей ввода
    int numFigures = comboBoxFigures->itemText(num).toInt();
    createFigureInputFields(numFigures);
    mainLayout->addWidget(btnDraw);
    // Подгонка размера окна под новое содержимое
    adjustSize();
}

void MainWindow::onBtnDrawClicked()
{
    // Создаем новое окно Graphic
    Graphic *graphicWindow = new Graphic();

    // Передаем параметры фигур из MainWindow в Graphic и строим их
    for (int i = 0; i < figureParamsFields.size(); i += 4) {
        int x = figureParamsFields.at(i)->text().toInt();
        int y = figureParamsFields.at(i + 1)->text().toInt();
        int width = figureParamsFields.at(i + 2)->text().toInt();
        int height = figureParamsFields.at(i + 3)->text().toInt();
        graphicWindow->addRectangle(x, y, width, height);
    }

    graphicWindow->show();
}

