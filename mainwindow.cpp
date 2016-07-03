#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QUdpSocket>

#include <cmath>

const int steps = 999;
constexpr double rtod = 180 / (std::atan(1) * 4);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&timer, SIGNAL(timeout()),
            this, SLOT(on_timerTimeout()));
    ui->horizontalSlider->setRange(0, steps);
    loadData();
    direction = 1;
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::validateData()
{
    double hipo = std::sqrt(std::pow(X, 2) + std::pow(Y, 2));
    double min = (L1 + L2) * 0.27;
    return min <= hipo && hipo <= L1 + L2;
}

void MainWindow::calculatePair()
{
    double k1 = std::pow(X, 2) + std::pow(Y, 2) -
                std::pow(L1, 2) - std::pow(L2, 2);
    double k2 = 2 * L1 * L2;
    double t2 = std::atan2(
                    std::sqrt(1 - std::pow(k1 / k2, 2)),
                    k1 / k2);
    double t1 = std::atan2(Y, X) -
                    std::atan2(L2 * std::sin(t2),
                               L1 + L2 * std::cos(t2));
    theta_pair.theta1 = t1;
    theta_pair.theta2 = t2;
}

void MainWindow::loadData()
{
    L1 = ui->spinBoxL1->value();
    L2 = ui->spinBoxL2->value();
    X = ui->spinBoxX->value();
    Y = ui->spinBoxY->value();
    isValid = validateData();
    if (!isValid) {
        ui->lineEditTheta1->setText("Error");
        ui->lineEditTheta2->setText("Error");
    } else {
        calculatePair();
        ui->lineEditTheta1->setText(
                    QString::number(rtod * theta_pair.theta1)
                        + "º");
        ui->lineEditTheta2->setText(
                    QString::number(rtod * theta_pair.theta2)
                        + "º");
    }
    ui->horizontalSlider->setValue(0);
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    double d = (double)value / (double)steps;
    theta_p dtheta = {d * theta_pair.theta1,
                      d * theta_pair.theta2};
    ui->lineEditITheta1->setText(
                QString::number(rtod * dtheta.theta1) + "º");
    ui->lineEditITheta2->setText(
                QString::number(rtod * dtheta.theta2) + "º");
    QUdpSocket sock;
    sock.connectToHost(ui->lineEditHost->text(),
                       ui->spinBoxPort->value());
    sock.write(QByteArray::fromRawData((char*)&dtheta,
                                       sizeof(theta_p)));
    sock.close();
}

void MainWindow::on_timerTimeout()
{
    int step = ui->horizontalSlider->value();
    step += direction;
    ui->horizontalSlider->setValue(step);
    if (step >= steps)
        direction = -1;
    if (step <= 0)
        direction = 1;
}

void MainWindow::on_pushButtonRun_toggled(bool checked)
{
    ui->spinBoxL1->setEnabled(!checked);
    ui->spinBoxL2->setEnabled(!checked);
    ui->spinBoxX->setEnabled(!checked);
    ui->spinBoxY->setEnabled(!checked);
    ui->spinBoxT->setEnabled(!checked);
    double t = ui->spinBoxT->value();
    if (checked)
        timer.start((t * 1000) / steps);
    else
        timer.stop();
}
