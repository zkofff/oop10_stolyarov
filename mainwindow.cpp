#include "mainwindow.h"
#include <QPainter>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFormLayout>
#include <QFrame>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), useGraphicalMethod(false) {
    setupUI();
    setWindowTitle("Векторный редактор");
    resize(800, 600);
}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    QFrame* inputFrame = new QFrame();
    inputFrame->setFrameShape(QFrame::StyledPanel);
    QFormLayout* inputLayout = new QFormLayout(inputFrame);

    vec1XEdit = new QLineEdit();
    vec1YEdit = new QLineEdit();
    vec2XEdit = new QLineEdit();
    vec2YEdit = new QLineEdit();

    inputLayout->addRow("Вектор 1 X:", vec1XEdit);
    inputLayout->addRow("Вектор 1 Y:", vec1YEdit);
    inputLayout->addRow("Вектор 2 X:", vec2XEdit);
    inputLayout->addRow("Вектор 2 Y:", vec2YEdit);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    btnComponents = new QPushButton("Сложить по компонентам");
    btnGraphical = new QPushButton("Сложить графически");
    buttonLayout->addWidget(btnComponents);
    buttonLayout->addWidget(btnGraphical);

    resultLabel = new QLabel("Результат: не вычислен");
    resultLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #2E8B57;");
    resultLabel->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(inputFrame);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(resultLabel);
    mainLayout->addStretch();

    connect(btnComponents, &QPushButton::clicked, this, &MainWindow::addComponents);
    connect(btnGraphical, &QPushButton::clicked, this, &MainWindow::addGraphically);

    setCentralWidget(centralWidget);
}

void MainWindow::addComponents() {
    calculateVectors(false);
}

void MainWindow::addGraphically() {
    calculateVectors(true);
}

void MainWindow::calculateVectors(bool graphical) {
    useGraphicalMethod = graphical;

    bool ok1, ok2, ok3, ok4;
    double x1 = vec1XEdit->text().toDouble(&ok1);
    double y1 = vec1YEdit->text().toDouble(&ok2);
    double x2 = vec2XEdit->text().toDouble(&ok3);
    double y2 = vec2YEdit->text().toDouble(&ok4);

    if (!ok1 || !ok2 || !ok3 || !ok4) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, введите корректные числовые значения!");
        return;
    }

    vec1.reset(new Vector(x1, y1));
    vec2.reset(new Vector(x2, y2));
    result.reset(new Vector(Vector::add(*vec1, *vec2)));

    updateResultDisplay();
    update();
}

void MainWindow::updateResultDisplay() {
    if (!result) return;

    QString method = useGraphicalMethod ? "графический" : "компонентный";
    resultLabel->setText(
        QString("Результат (%1 метод): (%2, %3)")
            .arg(method)
            .arg(result->x, 0, 'f', 2)
            .arg(result->y, 0, 'f', 2)
    );
}

void MainWindow::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    if (!vec1 || !vec2 || !result) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect drawingArea(50, 250, width()-100, height()-300);
    painter.fillRect(drawingArea, QColor(240, 240, 240));
    painter.setPen(Qt::darkGray);
    painter.drawRect(drawingArea);

    painter.translate(drawingArea.center().x(), drawingArea.center().y());

    double maxVal = qMax(
        qMax(qAbs(vec1->x), qAbs(vec1->y)),
        qMax(qAbs(vec2->x), qAbs(vec2->y))
    );
    double scale = qMin(
        drawingArea.width() / (2.2 * maxVal),
        drawingArea.height() / (2.2 * maxVal)
    );

    painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));
    painter.drawLine(-drawingArea.width()/2, 0, drawingArea.width()/2, 0);
    painter.drawLine(0, -drawingArea.height()/2, 0, drawingArea.height()/2);

    painter.setPen(QPen(Qt::blue, 2));
    QPointF v1End(vec1->x * scale, -vec1->y * scale);
    painter.drawLine(QPointF(0, 0), v1End);

    painter.setPen(QPen(Qt::red, 2));
    QPointF v2End(vec2->x * scale, -vec2->y * scale);
    if (useGraphicalMethod) {
        painter.drawLine(v1End, v1End + v2End);
    } else {
        painter.drawLine(QPointF(0, 0), v2End);
    }

    painter.setPen(QPen(Qt::darkGreen, 3));
    QPointF resEnd(result->x * scale, -result->y * scale);
    painter.drawLine(QPointF(0, 0), resEnd);

    painter.setPen(Qt::black);
    QFont font = painter.font();
    font.setPointSize(10);
    painter.setFont(font);

    painter.drawText(v1End + QPointF(5, -5), "V1");
    painter.drawText(v2End + QPointF(5, -5), "V2");
    painter.drawText(resEnd + QPointF(5, -5), "Result");

    if (useGraphicalMethod) {
        painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));
        painter.drawLine(v2End, resEnd);
        painter.drawLine(v1End, resEnd);
    }
}
