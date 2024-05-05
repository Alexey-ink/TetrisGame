#include "mainwindow.h"
#include "gamewindow.h"

#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow mainWindow;
    TetrisWidget tetrisWidget;
    MainMenuWindow mainMenuWindow;

    QPushButton *mainMenuButton = new QPushButton("Главное меню");
    QPushButton *restartButton = new QPushButton("Перезапустить");

    // Установка стилей для кнопок
    QString buttonStyle = "QPushButton {"
                          "background-color: #4CAF50;" // Зеленый цвет фона
                          "border: none;" // Убираем границы
                          "color: white;" // Белый цвет текста
                          "padding: 10px 24px;" // Отступы внутри кнопки
                          "text-align: center;" // Выравниваем текст по центру
                          "text-decoration: none;" // Убираем подчеркивание текста
                          "font-size: 16px;" // Размер шрифта
                          "}"
                          "QPushButton:hover {"
                          "background-color: #45a049;" // Цвет фона при наведении
                          "}";

    mainMenuButton->setStyleSheet(buttonStyle);
    restartButton->setStyleSheet(buttonStyle);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(mainMenuButton);
    buttonLayout->addWidget(restartButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(buttonLayout); // Добавляем строку с кнопками
    mainLayout->addWidget(&tetrisWidget); // Добавляем TetrisWidget
    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(mainLayout);
    mainWindow.setCentralWidget(centralWidget);

    mainWindow.move(770, 150);

    // Настройка подключения кнопок к слотам

    QObject::connect(restartButton, &QPushButton::clicked, &tetrisWidget, &TetrisWidget::restartGame);

    QObject::connect(mainMenuButton, &QPushButton::clicked, &mainWindow, &QMainWindow::hide);
    QObject::connect(mainMenuButton, &QPushButton::clicked, &mainMenuWindow, &MainMenuWindow::show);
    QObject::connect(mainMenuButton, &QPushButton::clicked, &tetrisWidget, &TetrisWidget::stopTimer);

    QObject::connect(&mainMenuWindow, &MainMenuWindow::startGame, &mainWindow, &QMainWindow::show);
    QObject::connect(&mainMenuWindow, &MainMenuWindow::startGame, &tetrisWidget, &TetrisWidget::restartGame);

    tetrisWidget.setFocus();

    mainMenuWindow.show();
    return a.exec();
}
