 #include "mainwindow.h"

TetrisWidget::TetrisWidget(QWidget *parent) : QWidget(parent), accelerated(false), score(0) {
    // Установка размера окна
    setFixedSize(300, 700);

    // Создание таймера для обновления падающих фигур
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TetrisWidget::updateTetris);

    // Добавление стартовых позиций
    startPositions.append(QPoint(0, 50));
    startPositions.append(QPoint(50, 50));
    startPositions.append(QPoint(100, 50));
    startPositions.append(QPoint(150, 50));
    startPositions.append(QPoint(200, 50));
    startPositions.append(QPoint(250, 50));

    availableColors.append(Qt::red);
    availableColors.append(Qt::green);
    availableColors.append(Qt::blue);
    //availableColors.append(Qt::yellow); 

    QString labelStyle = "QLabel {"
                         "color: #333;" // Цвет текста
                         "font-size: 20px;" // Размер шрифта
                         "font-weight: bold;" // Жирный шрифт
                         "}";

    statusLabel = new QLabel("Score: " + QString::number(score), this);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet(labelStyle); // Установка стиля

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(statusLabel);

    QVBoxLayout* mainLayout = new QVBoxLayout(); // Вертикальное

    mainLayout->addLayout(layout);
    mainLayout->setAlignment(layout, Qt::AlignTop);

    setLayout(mainLayout);
    setFocus();

}

void TetrisWidget::paintEvent(QPaintEvent *event) {
       QPainter painter(this);
       painter.setBrush(Qt::NoBrush);

       // Рисование падающей фигуры
       painter.fillRect(currentRect, currentColor);
       painter.setPen(Qt::black); // Черная рамка
       painter.drawRect(currentRect);

       // Рисование фигур, уже находящихся в столбике
       for (const auto& rectColorPair : fallenRects) {
           painter.fillRect(rectColorPair.first, rectColorPair.second);
           painter.setPen(Qt::black); // Черная рамка
           painter.drawRect(rectColorPair.first);
       }
}

void TetrisWidget::updateTetris() {

    // Проверка столкновения с уже упавшими квадратами
    for (const auto& rectColorPair : fallenRects) {
        if (currentRect.translated(0, 50).intersects(rectColorPair.first)) {
            // Добавление падающей фигуры в столбик
            fallenRects.append(qMakePair(currentRect, currentColor));

            // Проверка, достиг ли хоть один столбик верхнего края окна
            for (const auto& rectColorPair : fallenRects) {
                if (rectColorPair.first.top() <= 50) {
                    stopAdding = true;
                    qDebug() << "Останавливаем";
                    QMessageBox::information(this, "Game Over", "Игра окончена! Score: " + QString::number(score)); // Отображение диалогового окна с сообщением о завершении игры
                    timer->stop(); // Остановка таймера
                    return;
                }
            }

            // Генерация случайной стартовой позиции для следующей фигуры
            QPoint startPos = startPositions.at(rand() % startPositions.size());

            currentRect = QRect(startPos, QSize(50, 50));

            // Генерация случайного цвета для следующей фигуры
            colorIndex = QRandomGenerator::global()->bounded(availableColors.size());
            currentColor = availableColors[colorIndex];

            // Проверка и удаление заполненных строк
            checkAndRemoveFilledRows();
            checkAndRemoveFilledColumns();

            accelerated = false;
            update();
            return;

        }
    }

        // Установка интервала таймера в зависимости от того, активировано ли ускорение
        int interval = accelerated ? 50 : 500;
        timer->setInterval(interval);

        // Проверка достижения нижней границы
        if (currentRect.y() < height() - 50) {
            currentRect.translate(0, 50); // Сдвиг фигуры вниз
        } else {
            // Добавление падающей фигуры в столбик
            fallenRects.append(qMakePair(currentRect, currentColor));

            // Проверка и удаление заполненных строк
            checkAndRemoveFilledRows();

            // Генерация случайной стартовой позиции для следующей фигуры
            QPoint startPos = startPositions.at(rand() % startPositions.size());

            // Установка случайной стартовой позиции для следующей фигуры
            currentRect = QRect(startPos, QSize(50, 50));

            // Генерация случайного цвета для следующей фигуры
            colorIndex = QRandomGenerator::global()->bounded(availableColors.size());
            currentColor = availableColors[colorIndex];

            accelerated = false;
        }

        if(stopAdding == 0) { // Перерисовка виджета
           update();
           return;
        }

    }

void TetrisWidget::moveLeft() {
    // Проверяем, есть ли препятствие слева от текущего квадрата
    bool obstacle = false;
    for (const auto& rectColorPair : fallenRects) {
        if (currentRect.translated(-50, 0).intersects(rectColorPair.first)) {
            obstacle = true;
            break;
        }
    }

    if (!obstacle && currentRect.x() > 0)
        currentRect.translate(-50, 0);
    update();
}

void TetrisWidget::moveRight() {
    // Проверяем, есть ли препятствие справа от текущего квадрата
    bool obstacle = false;
    for (const auto& rectColorPair : fallenRects) {
        if (currentRect.translated(50, 0).intersects(rectColorPair.first)) {
            obstacle = true;
            break;
        }
    }

    if (!obstacle && currentRect.x() < width() - 50)
        currentRect.translate(50, 0);
    update();
}

void TetrisWidget::accelerate() {
    accelerated = true;
    update();
}

void TetrisWidget::keyPressEvent(QKeyEvent *event) {
        if (!stopAdding) {
            switch (event->key()) {
                case Qt::Key_Left:
                case Qt::Key_A:
                    moveLeft();
                    break;
                case Qt::Key_Right:
                case Qt::Key_D:
                    moveRight();
                    break;
                case Qt::Key_Down:
                case Qt::Key_S:
                    accelerate();
                    break;
                default:
                    QWidget::keyPressEvent(event);
            }
        }
}

void TetrisWidget::checkAndRemoveFilledRows() {
    QMap<int, int> rowCounts; // Подсчитываем количество квадратов в каждой строке по цветам
    bool flag = 0;  // QMap<numberOfRow, amountOfRects>

    for (const auto& rect : fallenRects) {
        int row = rect.first.y() / 50;
        rowCounts[row]++;
    }

     qDebug() << "rowCounts:";
        for (auto it = rowCounts.begin(); it != rowCounts.end(); ++it) {
            qDebug() << "Row:" << it.key() << "Count:" << it.value();
        }


    QList<int> filledRows;

    // Проверяем, какие строки полностью заполнены квадратами одного цвета
    for (auto it = rowCounts.begin(); it != rowCounts.end(); ++it) {
        int row = it.key();
        int count = it.value();

        if (count == width() / 50) { // Если строка полностью
            QColor rowColor = QColor(); // Цвет текущей строки

            // Проверяем, все ли квадраты в строке имеют один и тот же цвет
            for (const auto& rect : fallenRects) {
                if (rect.first.y() / 50 == row) {
                    if (!rowColor.isValid()) {
                        rowColor = rect.second;
                    } else if (rowColor != rect.second) {
                        rowColor = QColor();
                        break;
                    }
                }
            }
            if (rowColor.isValid()) {
                filledRows.append(row);
                flag = 1;
            }
        }
    }

    // Удаляем строку
    for (int row : filledRows) {
        auto it = fallenRects.begin();
        while (it != fallenRects.end()) {
            if (it->first.y() / 50 == row)
                it = fallenRects.erase(it);
            else
                ++it;
        }

        // Сдвигаем квадратики выше на одну строку вниз
        for (auto& rectColorPair : fallenRects) {
            if (rectColorPair.first.y() / 50 < row)
                rectColorPair.first.translate(0, 50);
        }
    }

    if(flag == 1) {
         score += 10;
         updateStatusLabel();
    }
}

void TetrisWidget::checkAndRemoveFilledColumns() {

    // Определим размер матрицы
    int rows = (height() / 50);
    int columns = width() / 50;

    // Создаем матрицу, заполненную цветом по умолчанию
    QVector<QVector<QColor>> colorMatrix(rows, QVector<QColor>(columns, QColor()));

    // Заполняем матрицу цветами из QList<QPair<QRect, QColor>>
    for (const auto& rectColorPair : fallenRects) {
        int row = rectColorPair.first.y() / 50; // Номер строки
        int column = rectColorPair.first.x() / 50; // Номер столбца
        colorMatrix[row][column] = rectColorPair.second; // Присваиваем цвет квадрата в матрице
    }

    QList<QPoint> squaresToRemove; // Список для хранения координат квадратов, которые нужно удалить

    // Проходимся по каждому столбцу
    for (int col = 0; col < columns; ++col) {
        int consecutiveCount = 1; // Счетчик подряд идущих квадратов одного цвета
        QColor previousColor; // Цвет предыдущего квадрата

        for (int row = 0; row < rows; ++row) {
            QColor currentColor = colorMatrix[row][col]; // Цвет текущего квадрата

            // Если текущий цвет совпадает с предыдущим и не является пустым
            if (currentColor.isValid() && currentColor == previousColor) {
                ++consecutiveCount; // Увеличиваем счетчик подряд идущих квадратов
            } else {
                consecutiveCount = 1; // Сбрасываем счетчик, если цвет не совпадает или является пустым
                previousColor = currentColor; // Обновляем предыдущий цвет
            }

            // Если нашли подряд стоящие пять квадратов одного цвета
            if (consecutiveCount == 5) {
                // Добавляем координаты этих квадратов в список для удаления
                for (int i = row - 4; i <= row; ++i) {
                    squaresToRemove.append(QPoint(col * 50, i * 50));
                }
                // Сбрасываем счетчик для дальнейшего поиска
                consecutiveCount = 1;
            }
        }
    }

    // Удаление квадратов из списка fallenRects
    for (const auto& point : squaresToRemove) {
        auto it = std::remove_if(fallenRects.begin(), fallenRects.end(), [&](const QPair<QRect, QColor>& rectColorPair) {
            return rectColorPair.first.contains(point);
        });
        fallenRects.erase(it, fallenRects.end());
    }

    if(!squaresToRemove.isEmpty()) {
        score += 10;
        updateStatusLabel();
    }
}


void TetrisWidget::restartGame() {

    qDebug() << "Restarting game...";
    if (!isVisible()) {
        qDebug() << "TetrisWidget is hidden";
        show();
    }
    else
        qDebug() << "TetrisWidget is visible";

    // Очистка списка упавших квадратов и текущей фигуры
    fallenRects.clear();
    currentRect = QRect(startPositions.at(QRandomGenerator::global()->bounded(startPositions.size())), QSize(50, 50));
    colorIndex = QRandomGenerator::global()->bounded(availableColors.size());
    currentColor = availableColors[colorIndex];

    // Сброс флагов
    stopAdding = false;
    accelerated = false;

    // Перезапуск таймера
    timer->start(500);

    // Обновление метрики
    score = 0;
    updateStatusLabel();
    setFocus();

    // Перерисовка виджета
    update();
}



