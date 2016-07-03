#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class MainWindow;
}

struct theta_p {
    double theta1;
    double theta2;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void loadData();

    void on_horizontalSlider_valueChanged(int value);
    void on_timerTimeout();

    void on_pushButtonRun_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    QTimer timer;
    bool validateData();
    void calculatePair();
    double L1;
    double L2;
    double X;
    double Y;
    theta_p theta_pair;
    bool isValid;
    int direction;
};

#endif // MAINWINDOW_HPP
