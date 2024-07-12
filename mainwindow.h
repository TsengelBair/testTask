#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphic.h"

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QWidget* centralWidget;
    QVBoxLayout* mainLayout;
    QComboBox* comboBoxFigures;
    QList<QHBoxLayout*> figureLayouts; // Список слоев для каждой фигуры (у каждой фигуры свой слой)
    QList<QLineEdit*> figureParamsFields; // У каждой фигуры свой список параметров
    QPushButton* btnDraw;
    Graphic* graphic; // Экземпляр класса второго окна (график)

    void setupUi();
    void createFigureInputFields(int numFigures);

private slots:
    void onComboBoxChanged(int num);
    void onBtnDrawClicked();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
