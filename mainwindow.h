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
    void initialize_vector();
    void analize_image();
    void calculatehistogram();
    void initialize_image(QImage *image);
    void painthistogram(int color);
    void paint_color(QVector<int> vector, QImage *image, QRgb rgb);
    void on_loadimageButton_clicked();
    void on_histogramButton_clicked();

private:

    enum {red=0, green=1, blue=2, gray=3, combined=4, colors=4, num_histo=5};

    Ui::MainWindow *ui;
    QLabel *label_[num_histo];
    QImage *image_original;
    QVector<int> vector_[colors];
};

#endif // MAINWINDOW_H
