#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::inicializate_vector(){
    vector_red = QVector<int>(256, 0);
    vector_green = QVector<int>(256, 0);
    vector_blue = QVector<int>(256, 0);
    vector_gray = QVector<int>(256, 0);
}

void MainWindow::inicializate_image(QImage *image){
    for(int i =0; i < image->width(); ++i){
       for(int j =0; j < image->height(); ++j) {
          image->setPixel(i,j,qRgb(0,0,0));
       }
    }
}

void MainWindow::on_loadimageButton_clicked(){
    QString imagefile = QFileDialog::getOpenFileName( this,tr("Selecciona una Imatge"),QDir::currentPath(),"Imatges (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)");
    QImage  imageresized;
    image.load (imagefile);
    imageresized=image.scaled(ui->label_image->width(),ui->label_image->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->label_image->setPixmap(QPixmap::fromImage(imageresized));
}

int max_of_vector(QVector<int> vector){
    int max = 0;
    for(int i = 0; i < vector.size(); i++){
        if (vector.at(i)>max) max =vector.at(i);
    }

    return max;
}

void normalize_vector(QVector<int> *vector, int max){
    for(int i = 0; i < vector->size(); i++){
        vector->replace(i, qRound((vector->at(i)*255)/max));
    }
}

void MainWindow::analize_image(){
   inicializate_vector();

   for(int i =0; i < image.width(); ++i){
    for(int j =0; j < image.height(); ++j) {
        QRgb c = image.pixel(i,j);
        int r = qRed(c);
        int g = qGreen(c);
        int b = qBlue(c);
        //qDebug()<<"Red: "<<r<<" Green: "<<g<<" Blue: "<<b;
        vector_red[r]++;
        vector_green[g]++;
        vector_blue[b]++;
        vector_gray[qRound((r+g+b)/3)]++;
       }
    }
    //qDebug()<<vector_red;
}

void MainWindow::on_histogramButton_clicked(){
    analize_image();
    calculatehistogram();
}

void MainWindow::calculatehistogram(){
    normalize_vector(&vector_red,max_of_vector(vector_red));
    normalize_vector(&vector_green,max_of_vector(vector_green));
    normalize_vector(&vector_blue,max_of_vector(vector_blue));
    normalize_vector(&vector_gray,max_of_vector(vector_gray));
    //qDebug()<<vector_red;

    painthistogram(vector_red, "red");
    painthistogram(vector_green, "green");
    painthistogram(vector_blue, "blue");
    painthistogram(vector_gray, "gray");
}

void MainWindow::painthistogram(QVector<int> vector, QString color){
    QImage  image(256,256,QImage::Format_RGB888);

    inicializate_image(&image);

    for(int i=0; i< vector.size(); i++){
        int j = 0;
        while(vector.at(i) >= 0){
            if(color == "red"){
                image.setPixel(i,image.height()-j-1,qRgb(255,0,0));
            }
            if(color == "green"){
                image.setPixel(i,image.height()-j-1,qRgb(0,255,0));
            }
            if(color == "blue"){
                image.setPixel(i,image.height()-j-1,qRgb(0,0,255));
            }
            if(color == "gray"){
                image.setPixel(i,image.height()-j-1,qRgb(128,128,128));
            }
            vector[i]--;
            j++;
        }
    }

    if(color == "red"){
        ui->label_red->setPixmap(QPixmap::fromImage(image));
    }
    if(color == "green"){
        ui->label_green->setPixmap(QPixmap::fromImage(image));
    }
    if(color == "blue"){
       ui->label_blue->setPixmap(QPixmap::fromImage(image));
    }
    if(color == "gray"){
       ui->label_gray->setPixmap(QPixmap::fromImage(image));
    }

}
