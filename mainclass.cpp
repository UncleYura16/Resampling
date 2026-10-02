#include "mainclass.h"
#include "ui_mainclass.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QVector>
#include <QPen>
#include <fstream>
#include <vector>
#include <cstdint>

MainClass::MainClass(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainClass)
{
    ui->setupUi(this);

    ui->Grapt_qcustomplot->setInteractions(
        QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes);

    connect(ui->Grapt_qcustomplot, SIGNAL(mousePress(QMouseEvent*)),
            this, SLOT(mousePress()));
    connect(ui->Grapt_qcustomplot, SIGNAL(mouseWheel(QWheelEvent*)),
            this, SLOT(mouseWheel(QWheelEvent*)));

    ui->Grapt_qcustomplot->xAxis->setLabel("Отсчёт");
    ui->Grapt_qcustomplot->yAxis->setLabel("Амплитуда");
    ui->Grapt_qcustomplot->xAxis->setRange(0, 100);
    ui->Grapt_qcustomplot->yAxis->setRange(-1, 1);
    ui->Grapt_qcustomplot->replot();
}

MainClass::~MainClass()
{
    delete ui;
}
void MainClass::on_OpenSignal_button_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
        this, tr("Выберите файл сигнала"),
        QString(), tr("Data files (*.dat *.bin);;All files (*)"));
    if (filename.isEmpty())
        return;
    
    std::ifstream in(filename.toStdString(), std::ios::binary);
    if (!in) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
        return;
    }

    in.seekg(0, std::ios::end);
    std::streamsize bytes = in.tellg();
    in.seekg(0, std::ios::beg);

    if (bytes <= 0) {
        QMessageBox::warning(this, "Ошибка", "Файл пустой");
        return;
    }

    const std::streamsize HEADER_BYTES = 0;
    if (bytes <= HEADER_BYTES) {
        QMessageBox::warning(this, "Ошибка", "Файл меньше заголовка");
        return;
    }
    in.seekg(HEADER_BYTES, std::ios::beg);

    const std::streamsize dataBytes = bytes - HEADER_BYTES;
    const std::streamsize samples = dataBytes / 2;   

    std::vector<int16_t> data;
    data.reserve(samples);

    int16_t sample;
    while (in.read(reinterpret_cast<char*>(&sample), sizeof(sample))) {
        data.push_back(sample);
    }
    in.close();

    if (data.empty()) {
        QMessageBox::warning(this, "Ошибка",
            "Не удалось прочитать ни одного отсчёта (int16).\n"
            "Возможно, файл имеет другой формат или заголовок.");
        return;
    }

    QVector<double> x(data.size()), y(data.size());
    for (int i = 0; i < (int)data.size(); ++i) {
        x[i] = i;
        y[i] = static_cast<double>(data[i]);
    }

    ui->Grapt_qcustomplot->clearGraphs();
    ui->Grapt_qcustomplot->addGraph();
    ui->Grapt_qcustomplot->graph(0)->setData(x, y);
    ui->Grapt_qcustomplot->graph(0)->setPen(QPen(Qt::red));
    ui->Grapt_qcustomplot->rescaleAxes();
    ui->Grapt_qcustomplot->replot();
}

void MainClass::mousePress()
{
    ui->Grapt_qcustomplot->setSelectionRectMode(QCP::srmZoom);
}

void MainClass::mouseWheel(QWheelEvent *event)
{
    double factor = (event->angleDelta().y() > 0) ? 0.85 : 1.15;

    QCPRange xr = ui->Grapt_qcustomplot->xAxis->range();
    QCPRange yr = ui->Grapt_qcustomplot->yAxis->range();

    ui->Grapt_qcustomplot->xAxis->setRange(
        QCPRange(xr.lower * factor, xr.upper * factor));
    ui->Grapt_qcustomplot->yAxis->setRange(
        QCPRange(yr.lower * factor, yr.upper * factor));
    ui->Grapt_qcustomplot->replot();
}
