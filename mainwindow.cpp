#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QDebug>

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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString nom("HISTO %1");

    for (int k=0;k<colors;k++){
            label_[k] = new QLabel(tr(nom.arg(k + 1).toAscii()));
            label_[k]->setStyleSheet("background-color: rgb(0, 0, 0)");
            label_[k]->setMinimumSize(255, 255);
            label_[k]->setMaximumSize(255,255);
            ui->gridLayout->addWidget(label_[k],5,k);
    }

    QLabel *label_combined = new QLabel(tr(nom.arg("combined").toAscii()));
    label_combined->setStyleSheet("background-color: rgb(0, 0, 0)");
    label_combined->setMinimumSize(255, 255);
    label_combined->setMaximumSize(255,255);
    ui->gridLayout->addWidget(label_combined,1,3);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::inicializate_vector(){
    for(int i = 0; i < colors; i++){
        vector_[i] = QVector<int>(256, 0);
    }
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
    imageresized=image.scaled((image.width()*255)/image.height(),255,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->label_image->setMaximumWidth((image.width()*255)/image.height());
    ui->label_image->setPixmap(QPixmap::fromImage(imageresized));
    ui->label_image->setStyleSheet("background-color");
    ui->histogramButton->setEnabled(true);
}

void MainWindow::on_histogramButton_clicked(){
    analize_image();
    calculatehistogram();
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
        vector_[red][r]++;
        vector_[green][g]++;
        vector_[blue][b]++;
        vector_[gray][qRound((r+g+b)/3)]++;
       }
    }
    //qDebug()<<vector_red;
}

void MainWindow::calculatehistogram(){
    for(int i=0; i<colors; i++){
        normalize_vector(&vector_[i],max_of_vector(vector_[i]));
        painthistogram(vector_[i], i);
    }
}

void MainWindow::painthistogram(QVector<int> vector, int color){
    QImage  image(256,256,QImage::Format_RGB888);

    inicializate_image(&image);

    for(int i=0; i< vector.size(); i++){
        int j = 0;
        while(vector.at(i) >= 0){
            if(color == red){
                image.setPixel(i,image.height()-j-1,qRgb(255,0,0));
            }
            if(color == green){
                image.setPixel(i,image.height()-j-1,qRgb(0,255,0));
            }
            if(color == blue){
                image.setPixel(i,image.height()-j-1,qRgb(0,0,255));
            }
            if(color == gray){
                image.setPixel(i,image.height()-j-1,qRgb(128,128,128));
            }
            vector[i]--;
            j++;
        }
    }

    label_[color]->setPixmap(QPixmap::fromImage(image));

}
