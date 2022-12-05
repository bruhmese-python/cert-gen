#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "textproperties.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setText(QString);
    textProperties getTextProperties();
    void preview();

private slots:
    void on_actionLoad_Image_triggered();

    void on_actionSave_Image_triggered();

    void on_actionLoad_Names_triggered();

    void on_actionExit_triggered();

    void on_Name_textChanged(const QString &arg1);

    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_Yslider_valueChanged(int value);

    void on_Xslider_valueChanged(int value);

    void on_Picker_released();

    void on_OffsetSlider_valueChanged(int value);

    void on_SizeSlider_valueChanged(int value);

    void on_Centre_released();

    void on_pushButton_pressed();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
