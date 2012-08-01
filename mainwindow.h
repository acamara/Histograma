#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void inicializate_vector();
    void analize_image();
    void calculatehistogram();
    void inicializate_image(QImage *image);
    void painthistogram(QVector<int> vector, QString color);
    void on_loadimageButton_clicked();
    void on_histogramButton_clicked();

private:
    Ui::MainWindow *ui;

    QImage image;
    QVector<int> vector_red;
    QVector<int> vector_green;
    QVector<int> vector_blue;
    QVector<int> vector_gray;
    QVector<int> vector_combined;
};

#endif // MAINWINDOW_H
