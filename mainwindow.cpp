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
    create_viewports();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::initialize_vector(){
    for(int i = 0; i < colors; i++){
        vector_[i] = QVector<int>(256, 0);
    }
}

void MainWindow::create_viewports(){
    QString nom("HISTO %1");

    for (int k=0;k<num_histo;k++){
            label_[k] = new QLabel(tr(nom.arg(k + 1).toAscii()));
            label_[k]->setStyleSheet("background-color: rgb(0, 0, 0); border: 1px solid gray;");
            label_[k]->setMinimumSize(255, 255);
            label_[k]->setMaximumSize(255,255);

            if(k == combined){
                ui->gridLayout->addWidget(label_[k],1,3);
            }else{
                ui->gridLayout->addWidget(label_[k],5,k);
            }
    }
}

void MainWindow::on_loadimageButton_clicked(){
    create_viewports();
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
    calculate_histogram();
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

void MainWindow::calculate_histogram(){
    QVector<int> maxs_of_colors(colors,0);

    for(int i=0; i<colors; i++){
        maxs_of_colors[i] = (max_of_vector(vector_[i]));
        normalize_vector(&vector_[i],maxs_of_colors.at(i));
        paint_histogram(i);
    }
    maxs_of_colors.remove(3);
    int max = max_of_vector(maxs_of_colors);
    //qDebug()<<maxs_of_colors;
    for(int i=0; i<colors-1; i++){
        double factor_escala = (double)max/maxs_of_colors[i];
        normalize_vector(&vector_[i],factor_escala*255);
    }

    paint_histogram(combined);
}

void MainWindow::paint_components(QImage *image, bool pred, bool pgreen, bool pblue, bool pgray ){
    QVector<int> vred, vgreen, vblue;
    vred = vector_[red];
    vgreen = vector_[green];
    vblue = vector_[blue];

    for(int i=0; i< image->width(); i++){
        for(int j=image->height()-1; j>0; j--){
            int r = 0, g = 0, b = 0;
            if(vred[i]>0 && pred){
                r=255;
                vred[i]--;
            }
            if(vgreen[i]>0 && pgreen){
                g=255;
                vgreen[i]--;
            }
            if(vblue[i]>0 && pblue){
                b=255;
                vblue[i]--;
            }
            if(vector_[gray][i]>0 && pgray){
                r=g=b=128;
                vector_[gray][i]--;
            }
        image->setPixel(i,j,qRgb(r,g,b));
        }
    }
}

void MainWindow::paint_histogram(int color){
    QImage  *image = new QImage(256,256,QImage::Format_RGB888);

    if(color == red){
        paint_components(image, true, false, false, false);
    }
    if(color == green){
        paint_components(image, false, true, false, false);
    }
    if(color == blue){
        paint_components(image, false, false, true, false);
    }
    if(color == gray){
        paint_components(image, false, false, false, true);
    }
    if(color == combined){
        paint_components(image, true, true, true, false);
    }

    label_[color]->setPixmap(QPixmap::fromImage(*image));
}
