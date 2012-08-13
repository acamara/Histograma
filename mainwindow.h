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
    void create_viewports();
    void initialize_vector();
    void analize_image();
    void calculate_histogram();
    void paint_histogram(int color);
    void paint_components(QImage *image, bool pred, bool pgreen, bool pblue, bool pgray);
    void on_loadimageButton_clicked();
    void on_histogramButton_clicked();

    void on_saveHistogramButton_clicked();

private:
    Ui::MainWindow *ui;

    enum {red=0, green=1, blue=2, gray=3, combined=4, num_colors=4, num_histo=5};

    QLabel *label_[num_histo];
    QImage *image_[num_histo];
    QImage *image_original;

    QVector<int> vector_[num_colors];
};

#endif // MAINWINDOW_H
