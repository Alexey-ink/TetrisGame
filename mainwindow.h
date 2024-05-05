#include <QApplication>
#include <QMessageBox>
#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QColor>
#include <QRect>
#include <QList>
#include <cstdlib>
#include <QDebug>
#include <QRandomGenerator>
#include "gamewindow.h"
#include <QLabel>

class TetrisWidget : public QWidget {
    Q_OBJECT
public:
    TetrisWidget(QWidget *parent = nullptr);
     QLabel *statusLabel;

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event) override;
    void checkAndRemoveFilledRows();
    void checkAndRemoveFilledColumns();

public slots:
    void updateTetris();
    void restartGame();
    void stopTimer() {
        timer->stop(); // Останавливаем таймер
    }

private:
    QTimer *timer;
    //int x, y;
    int score;
    QColor currentColor;
    QRect currentRect;
    QList<QPair<QRect, QColor>> fallenRects; // Список для хранения падающих фигур и их цветов
    QList<QPoint> startPositions; // Список стартовых позиций для падающих фигур
    QList<QColor> availableColors; // Список доступных цветов
    int colorIndex; // Индекс текущего цвета
    bool stopAdding; // Флаг для остановки добавления новых квадратов
    bool accelerated; // Флаг для указания, что ускорение активировано

    void moveLeft();
    void moveRight();
    void accelerate();
    void updateStatusLabel() {
            statusLabel->setText("Score: " + QString::number(score));
        }
};


