#ifndef GAMEOVERWINDOW_H
#define GAMEOVERWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QApplication>

class MainMenuWindow : public QWidget {
    Q_OBJECT
public:
    MainMenuWindow(QWidget *parent = nullptr) : QWidget(parent) {
        setFixedSize(300, 600);

        QPushButton *startButton = new QPushButton("Start", this);
        QPushButton *exitButton = new QPushButton("Exit", this);

        // Устанавливаем стиль кнопок
        startButton->setStyleSheet("QPushButton {"
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
                                    "}");

        exitButton->setStyleSheet("QPushButton {"
                                   "background-color: #f44336;" // Красный цвет фона
                                   "border: none;"
                                   "color: white;"
                                   "padding: 10px 24px;"
                                   "text-align: center;"
                                   "text-decoration: none;"
                                   "font-size: 16px;"
                                   "}"
                                   "QPushButton:hover {"
                                   "background-color: #da190b;" // Цвет фона при наведении
                                   "}");

        connect(startButton, &QPushButton::clicked, this, &MainMenuWindow::onStartButtonClicked);
        connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(startButton);
        layout->addWidget(exitButton);

        move(770, 150);

    }

signals:
    void startGame();

private slots:
    void onStartButtonClicked() {
        hide(); // Скрываем окно MainMenuWindow
        emit startGame(); // Вызываем сигнал startGame()
    }
};


#endif // GAMEOVERWINDOW_H
