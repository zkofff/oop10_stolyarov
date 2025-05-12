#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "vector.h"
#include "smartptr.h"

class QLineEdit;
class QPushButton;
class QLabel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() = default;

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void addComponents();
    void addGraphically();

private:
    void setupUI();
    void calculateVectors(bool graphical);
    void updateResultDisplay();

    QLineEdit *vec1XEdit, *vec1YEdit, *vec2XEdit, *vec2YEdit;
    QPushButton *btnComponents, *btnGraphical;
    QLabel *resultLabel;
    smart_ptr<Vector> vec1, vec2, result;
    bool useGraphicalMethod;
};

#endif // MAINWINDOW_H
