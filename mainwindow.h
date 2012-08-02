#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QLabel>

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
    void painthistogram(QVector<int> vector, int color);
    void on_loadimageButton_clicked();
    void on_histogramButton_clicked();

private:

    enum {red=0, green=1, blue=2, gray=3, colors=4};

    Ui::MainWindow *ui;
    QLabel *label_[colors];
    QImage image;
    QVector<int> vector_[colors];
};

#endif // MAINWINDOW_H
