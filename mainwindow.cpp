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

    for (int k=0;k<num_histo;k++){
            label_[k] = new QLabel(tr(nom.arg(k + 1).toAscii()));
            label_[k]->setStyleSheet("background-color: rgb(0, 0, 0)");
            label_[k]->setMinimumSize(255, 255);
            label_[k]->setMaximumSize(255,255);

            if(k == combined){
                ui->gridLayout->addWidget(label_[k],1,3);
            }else{
                ui->gridLayout->addWidget(label_[k],5,k);
            }
    }   
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::initialize_vector(){
    for(int i = 0; i < colors; i++){
        vector_[i] = QVector<int>(256, 0);
    }
}

void MainWindow::initialize_image(QImage *image){
    for(int i =0; i < image->width(); ++i){
       for(int j =0; j < image->height(); ++j) {
          image->setPixel(i,j,qRgb(0,0,0));
       }
    }
}

void MainWindow::on_loadimageButton_clicked(){
    QString imagefile = QFileDialog::getOpenFileName( this,tr("Selecciona una Imatge"),QDir::currentPath(),"Imatges (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)");
    QImage  imageresized;
    image_original->load (imagefile);
    imageresized=image_original->scaled((image_original->width()*255)/image_original->height(),255,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->label_image->setMaximumWidth((image_original->width()*255)/image_original->height());
    ui->label_image->setPixmap(QPixmap::fromImage(imageresized));
    ui->label_image->setStyleSheet("background-color");
    ui->histogramButton->setEnabled(true);
}

void MainWindow::on_histogramButton_clicked(){
    analize_image();
    calculatehistogram();
}

void MainWindow::analize_image(){
   initialize_vector();

   for(int i =0; i < image_original->width(); ++i){
    for(int j =0; j < image_original->height(); ++j) {
        QRgb c = image_original->pixel(i,j);
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
    QVector<int> maxs_of_colors(colors,0);

    for(int i=0; i<colors; i++){
        maxs_of_colors[i] = (max_of_vector(vector_[i]));
        normalize_vector(&vector_[i],maxs_of_colors.at(i));
        painthistogram(i);
    }
    maxs_of_colors.remove(3);
    int max = max_of_vector(maxs_of_colors);
    qDebug()<<maxs_of_colors;
    for(int i=0; i<colors-1; i++){
        double factor_escala = (double)max/maxs_of_colors[i];
        normalize_vector(&vector_[i],factor_escala*255);
    }

    painthistogram(combined);
}


void MainWindow::paint_color(QVector<int> vector, QImage *image, QRgb rgb){
   for(int i=0; i< vector.size(); i++){
        int j = 0;
        while(vector.at(i) >= 0){
            image->setPixel(i,image->height()-j-1,rgb);
            vector[i]--;
            j++;
         }
    }
}

void MainWindow::paint_components(QImage *image){
    qDebug()<<blue;
    for(int i=0; i< image->width(); i++){
        for(int j=image->height()-1; j>0; j--){
            int r = 0, g = 0, b = 0;
            if(vector_[red][i]>0){
                r=255;
                vector_[red][i]--;
            }
            if(vector_[green][i]>0){
                g=255;
                vector_[green][i]--;
            }
            if(vector_[blue][i]>0){
                b=255;
                vector_[blue][i]--;
            }

        image->setPixel(i,j,qRgb(r,g,b));
        }
    }
}

void MainWindow::painthistogram(int color){
    QImage  *image = new QImage(256,256,QImage::Format_RGB888);

    initialize_image(image);

    if(color == red){
        paint_color(vector_[color],image,qRgb(255,0,0));
    }
    if(color == green){
        paint_color(vector_[color],image,qRgb(0,255,0));
    }
    if(color == blue){
        paint_color(vector_[color],image,qRgb(0,0,255));
    }
    if(color == gray){
        paint_color(vector_[color],image,qRgb(128,128,128));
    }
    if(color == combined){
        paint_components(image);
    }

    label_[color]->setPixmap(QPixmap::fromImage(*image));

}
