#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QGraphicsTextItem>
#include <QColorDialog>
#include <QMessageBox>
#include "size.h"
#include <fstream>
#include <string>
#include <vector>

QString srcFileName,destFolderName,srcListName;
QGraphicsScene *scene,*image;
QGraphicsTextItem* text;
size *original_size,*scene_size;
std::vector<std::string> names;
int I=0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void drawSlices(){

}

QImage getImage()
{
    image->clearSelection();
    image->setSceneRect(image->itemsBoundingRect());
    QImage img(image->sceneRect().size().toSize(),QImage::Format_ARGB32);
    img.fill(Qt::transparent);

    QPainter painter(&img);
    image->render(&painter);

    return img;
}

QPixmap getThumbnail()
{
    auto img = getImage();

    QPixmap p;
    p.convertFromImage(img);
    p = p.scaledToHeight(scene_size->height);
    p = p.scaledToWidth(scene_size->width);
    return p;
}

void MainWindow::setText(QString str)
{
    if(text==nullptr)
    {
        if(scene==nullptr or image==nullptr)
            return;
        text = image->addText(str);
    }

    this->ui->Name->setText(str);

    //apply prop to text;
    textProperties prop = getTextProperties();
    text->setPlainText(str);
    text->setFont(prop.font);
    text->setPos(prop.X - prop.offset, prop.Y - prop.offset);
    text->setScale(prop.scale);
    text->setDefaultTextColor(prop.color);

    preview();
}

textProperties MainWindow::getTextProperties()
{
    textProperties p;
    auto &ui = this->ui;
    p.X = ui->Xslider->value();
    p.Y = ui->Yslider->value();
    p.font = ui->fontComboBox->currentFont();
    p.scale = ui->SizeSlider->value();
    p.offset =  ui->OffsetSlider->value();
    p.color = ui->Color->text();
    return p;
}

void MainWindow::preview()
{
    if(scene==nullptr)
        return;
    scene->clear();
    scene->addPixmap(getThumbnail());

    //slices
    auto pen = QPen(Qt::PenStyle::DotLine);
    pen.setWidth(1);
    pen.setColor(QColor(241, 120, 105));

    scene->addLine(QLine(scene_size->width/2,0,scene_size->width/2,scene_size->height),pen);
    scene->addLine(QLine(0,scene_size->height/2,scene_size->width,scene_size->height/2),pen);
    this->ui->graphicsView->setScene(scene);
}

void MainWindow::on_actionLoad_Image_triggered()
{
    // set max values of sliders
    auto &g = this->ui->graphicsView;

    srcFileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "C:\\Users", tr("Image Files (*.png *.jpg *.bmp)"));
    if (not srcFileName.isNull())
    {
        QMessageBox msgBox;
        msgBox.setText(srcFileName);
        msgBox.exec();

        QPixmap picture;
        picture.load(srcFileName);

        original_size = new struct size(picture.size().width(),picture.size().height());
        auto graphics_view_size = new struct size(g->size().width(),g->size().height());

        scene_size = new struct size(picture.size().width(),picture.size().height());
        scene_size->constrainProportions(*graphics_view_size);

        //Actual Image
        if(image==nullptr)
            image = new QGraphicsScene(0, 0,original_size->width,original_size->height);
        image->addPixmap(picture);

        //preview
        if(scene==nullptr)
            scene = new QGraphicsScene(0, 0,scene_size->width,scene_size->height);

        preview();
    }
}


void MainWindow::on_actionSave_Image_triggered()
{
    destFolderName = QFileDialog::getExistingDirectory(this,tr("Save to"),"C:\\Users");
}


void MainWindow::on_actionLoad_Names_triggered()
{
    if(srcFileName.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("Please load image first");
        msgBox.exec();
        return;

    }

    srcListName = QFileDialog::getOpenFileName(this,
        tr("Open File"), "C:\\Users", tr("Text Files (*.txt)"));

    std::fstream file(srcListName.toStdString(),std::ios::in);
    if (file.is_open())
    {
        std::string line;
        while (getline(file, line))
        {
            names.push_back(line);
        }
    }
    //Update Name and text
    this->setWindowTitle("Batch-Certificate " + srcListName);

    if((I < (names.size()-1)) and (not names.empty()))
        qDebug()<<"I : "<<I <<" at names.size() = "<<names.size();
        setText(tr(names[I].c_str()));
}


void MainWindow::on_actionExit_triggered() { exit(EXIT_SUCCESS); }

void MainWindow::on_Xslider_valueChanged(int value) { setText(this->ui->Name->text()); }

void MainWindow::on_Yslider_valueChanged(int value) { setText(this->ui->Name->text()); }

void MainWindow::on_Name_textChanged(const QString &arg1) { setText(this->ui->Name->text()); }

void MainWindow::on_fontComboBox_currentFontChanged(const QFont &f) { setText(this->ui->Name->text()); }

void MainWindow::on_SizeSlider_valueChanged(int value) { setText(this->ui->Name->text()); }

void MainWindow::on_OffsetSlider_valueChanged(int value) { setText(this->ui->Name->text()); }

void MainWindow::on_Picker_released()
{
    QColor color = QColorDialog::getColor(Qt::blue, this );
    if( color.isValid() )
    {
      this->ui->Color->setText(color.name());
    }
    setText(this->ui->Name->text());
}

void MainWindow::on_Centre_released()
{
    if(original_size==nullptr)
        return;
    //    text align
    this->ui->Xslider->setValue(original_size->width/2 - text->sceneBoundingRect().width()/2);
    this->ui->Yslider->setValue(original_size->height/2 - text->sceneBoundingRect().height()/2);

    setText(this->ui->Name->text());
}


void MainWindow::on_pushButton_pressed()
{
    QString uniqueName =  tr(std::to_string(I).c_str())+ "_" + QDateTime::currentDateTime().date().toString();
    uniqueName = uniqueName.replace(' ','_').replace(':','_');

    if(destFolderName.isEmpty())
        on_actionSave_Image_triggered();

    QString path = destFolderName + "/" + uniqueName + ".png";

    auto img = getImage();
    img.save(path);
    this->statusBar()->setWindowTitle("Saved " + uniqueName);

    if((I < (names.size()-1)) and (not names.empty()))
        qDebug()<<"I before increment: "<<I;
        setText(tr(names[++I].c_str()));
}

