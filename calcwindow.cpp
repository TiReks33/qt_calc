#include "calcwindow.h"
#include "ui_calcwindow.h"
#include <QtMath>

CalcWindow::CalcWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CalcWindow)
    , buffer_(Buffer::instance().data()) //singleton instance initialization
{
    buffer_=0.0;
    //this->setFixedSize(QSize(375, 375));
    ui->setupUi(this);
    this->move(this->geometry().center());
    this->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0,0,0);");
    ui->result->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0,0,0);border: 1px solid black; border-width: 1px;");

    //this->setStyleSheet("border: 1px solid black; border-width: 1px; background-color: rgb(1, 153, 26); ");
    //this->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,    stop: 0 #77b9ea, stop: 1 #1593ee);");
    //ui->result->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,    stop: 0 #2d7ab2, stop: 1 #77c3fa);"
    //                            "color: white;");
    //this->setStyleSheet("* {color: qlineargradient(spread:pad, x1:0 y1:0, x2:1 y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));"
    //                       "background: qlineargradient( x1:0 y1:0, x2:1 y2:0, stop:0 cyan, stop:1 blue);}");
    //border-color: beige;border-width: 2px;border: solid black; background-color: rgb(1, 153, 26);");

    connect(ui->button_1,SIGNAL(clicked()),this,SLOT(digits()));
    connect(ui->button_2,SIGNAL(clicked()),this,SLOT(digits()));
    connect(ui->button_3,SIGNAL(clicked()),this,SLOT(digits()));
    connect(ui->button_4,SIGNAL(clicked()),this,SLOT(digits()));
    connect(ui->button_5,SIGNAL(clicked()),this,SLOT(digits()));
    connect(ui->button_6,SIGNAL(clicked()),this,SLOT(digits()));
    connect(ui->button_7,SIGNAL(clicked()),this,SLOT(digits()));
    connect(ui->button_8,SIGNAL(clicked()),this,SLOT(digits()));
    connect(ui->button_9,SIGNAL(clicked()),this,SLOT(digits()));
    connect(ui->button_0,SIGNAL(clicked()),this,SLOT(digits()));

    connect(ui->undo_button,SIGNAL(clicked()),this,SLOT(undo()));
    connect(ui->plus_minus_button,SIGNAL(clicked()),this,SLOT(simple_ops()));
    connect(ui->precent_button,SIGNAL(clicked()),this,SLOT(simple_ops()));

    connect(ui->plus_button,SIGNAL(clicked()),this,SLOT(math_ops()));
    //connect(ui->plus_button,SIGNAL(clicked()),this,SLOT(clear_scr()));

    connect(ui->minus_button,SIGNAL(clicked()),this,SLOT(math_ops()));
    //connect(ui->minus_button,SIGNAL(clicked()),this,SLOT(clear_scr()));

    connect(ui->multi_button,SIGNAL(clicked()),this,SLOT(math_ops()));
    //connect(ui->multi_button,SIGNAL(clicked()),this,SLOT(clear_scr()));

    connect(ui->div_button,SIGNAL(clicked()),this,SLOT(math_ops()));
    //connect(ui->div_button,SIGNAL(clicked()),this,SLOT(clear_scr()));
    connect(ui->mod_button,SIGNAL(clicked()),this,SLOT(math_ops()));

    connect(ui->sqr_root_button,SIGNAL(clicked()),this,SLOT(simple_ops()));
    connect(ui->cube_root_button,SIGNAL(clicked()),this,SLOT(simple_ops()));
    connect(ui->pow_2_button,SIGNAL(clicked()),this,SLOT(simple_ops()));
    connect(ui->pow_3_button,SIGNAL(clicked()),this,SLOT(simple_ops()));

    ui->plus_button->setCheckable(true);
    ui->minus_button->setCheckable(true);
    ui->multi_button->setCheckable(true);
    ui->div_button->setCheckable(true);
    ui->mod_button->setCheckable(true);
}

CalcWindow::~CalcWindow()
{
    delete ui;
}

void CalcWindow::reset_scr(QString const & str)
{
ui->result->setText(str);
}



void CalcWindow::digits()
{
    QPushButton *button = static_cast<QPushButton*>(sender());

    if((ui->plus_button->isChecked()||ui->minus_button->isChecked()
            ||ui->multi_button->isChecked()||ui->div_button->isChecked()
            ||ui->mod_button->isChecked())
                &&clear_checked())

               {

                reset_scr();

                clear_checked()=false;

                }

    //double multi_numbers;
    //QString new_label;

    //multi_numbers = (ui->result->text() + button->text()).toDouble();
    //new_label = QString::number(multi_numbers, 'g', 15);
    //ui->result->setText(new_label);
    if(!(ui->result->text()=="0"))

        ui->result->setText(ui->result->text() + button->text());

    else

        ui->result->setText(button->text());

}

void CalcWindow::undo()
{
    QString text(ui->result->text());

    if (text.length()<=1)

        ui->result->setText("0");

    else{

        text.chop(1);

        ui->statusbar->showMessage(text);

        ui->result->setText(text);

    }
}

void CalcWindow::simple_ops()
{
    QPushButton *button = static_cast<QPushButton*>(sender());
    double all_numbers;
    QString new_label;

    if(button->text() == "+/-"){

        all_numbers = (ui->result->text()).toDouble();
        all_numbers=-(all_numbers);
        new_label = QString::number(all_numbers, 'g', 15);

        ui->result->setText(new_label);
    }

        else if(button->text() == "%")
    {
        all_numbers = (ui->result->text()).toDouble();
        all_numbers*=0.01;
        new_label = QString::number(all_numbers, 'g', 15);

        ui->result->setText(new_label);
    }
        else if(button==ui->sqr_root_button)
    {
        ui->result->setText(QString::number(qSqrt(ui->result->text().toDouble())));
    }
        else if(button==ui->pow_2_button)
    {
        ui->result->setText(QString::number(qPow(ui->result->text().toDouble(),2)));
    }
        else if(button==ui->pow_3_button)
    {
        ui->result->setText(QString::number(qPow(ui->result->text().toDouble(),3)));
    }
        else if(button==ui->cube_root_button)
    {
        ui->result->setText(QString::number(qPow(ui->result->text().toDouble(),(1/3.))));
    }
}

void CalcWindow::math_ops()
{
    QPushButton *button = static_cast<QPushButton*>(sender());

    this->buffer_ = ui->result->text().toDouble();

    //ui->statusbar->showMessage(QString::number(this->buffer_, 'g', 15)+"ABCD");

    //ui->result->setText("");
    //ui->result->setText(button->text());

    button->setChecked(true);

    //ui->statusbar->showMessage(QString::number(button->isChecked()));

    clear_checked()=true;

    reset_scr(button->text());

}



void CalcWindow::on_default_color_clicked()
{
    this->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0,0,0);");
    ui->result->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0,0,0);border: 1px solid black; border-width: 1px;");
}

void CalcWindow::on_color_1_clicked()
{
    //this->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,    stop: 0 #77b9ea, stop: 1 #1593ee);");
    this->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,    stop: 0 #77b9ea, stop: 1 #73bff5);");
    ui->result->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,    stop: 0 #2d7ab2, stop: 1 #77c3fa);"
                                "color: white; border: 1px solid black; border-width: 1px;");
}

void CalcWindow::on_color_2_clicked()
{
    this->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,    stop: 0 #2d7ab2, stop: 1 #ae32a0);"
                        "color: white;");
    ui->result->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0,0,0); border: 1px solid black; border-width: 1px;");
//"qlineargradient( x1:0 y1:0, x2:1 y2:0, stop:0 cyan, stop:1 blue);");
}

void CalcWindow::on_dot_button_clicked()
{
    if(!(ui->result->text().contains('.')))
        ui->result->setText(ui->result->text() + ".");
}

void CalcWindow::on_equal_button_clicked()
{
    double tempNumber, num_second; //temp input value,
                                    //and second user
                                    //input value
    QString new_label;

    num_second = ui->result->text().toDouble();

    if(ui->plus_button->isChecked()){

        tempNumber = this->buffer_ + num_second;

        new_label = QString::number(tempNumber, 'g', 15);

        ui->result->setText(new_label);
        ui->plus_button->setChecked(false);

    } else if(ui->minus_button->isChecked()){

        tempNumber = this->buffer_ - num_second;

        new_label = QString::number(tempNumber, 'g', 15);

        ui->result->setText(new_label);
        ui->minus_button->setChecked(false);

    } else if(ui->multi_button->isChecked()){

        tempNumber = this->buffer_ * num_second;

        new_label = QString::number(tempNumber, 'g', 15);

        ui->result->setText(new_label);
        ui->multi_button->setChecked(false);

    } else if(ui->div_button->isChecked()){

            if(num_second==0){

            ui->result->setText("0");

                } else {

            tempNumber = this->buffer_ / num_second;

            new_label = QString::number(tempNumber, 'g', 15);

             ui->result->setText(new_label);

                }

            ui->div_button->setChecked(false);

    }   else if(ui->mod_button->isChecked()){

        if(num_second==0){

        ui->result->setText("0");

            } else {
        int temp1=static_cast<int>(this->buffer_);
        int temp2=static_cast<int>(num_second);

        int tempres = temp1 % temp2;

        new_label = QString::number(tempres, 'g', 15);
        ui->statusbar->showMessage(new_label);
        ui->result->setText(new_label);
                }
        ui->mod_button->setChecked(false);

    }
}



void CalcWindow::on_clear_button_clicked()
{
    ui->plus_button->setChecked(false);
    ui->minus_button->setChecked(false);
    ui->multi_button->setChecked(false);
    ui->div_button->setChecked(false);
    ui->mod_button->setChecked(false);

    ui->result->setText("0");
}
