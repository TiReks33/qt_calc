#include "calcwindow.h"
#include "ui_calcwindow.h"
#include <QtMath>
#include <cmath> // for fmod()

bool & CalcWindow::buffer_checked(){static bool buf=true;return buf;}

CalcWindow::CalcWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CalcWindow)
    , buffer_(Buffer::instance().data()) //singleton instance initialization
{
    buffer_=0.0;
    buffer_checked();
    buffer_checked()=true;
    math_switch("");
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

//    ui->plus_button->setCheckable(true);
//    ui->minus_button->setCheckable(true);
//    ui->multi_button->setCheckable(true);
//    ui->div_button->setCheckable(true);
//    ui->mod_button->setCheckable(true);
}

CalcWindow::~CalcWindow()
{
    delete ui;
}

void CalcWindow::reset_scr(QString const & str)
{
ui->result->setText(str);
}

void CalcWindow::bool_check()
{
    ui->statusbar->showMessage(QString::number(ui->plus_button->isChecked())+" "
   +QString::number(ui->minus_button->isChecked())+" "+QString::number(ui->multi_button->isChecked())+" "
   +QString::number(ui->div_button->isChecked())+" "+QString::number(ui->mod_button->isChecked()));
}

bool & CalcWindow::math_switch(QString str)
{
    static bool plus,minus,multi,div,mod,default_=false;
         if(str=="plus" ||str=="+") {return plus ;}
    else if(str=="minus"||str=="-") {return minus;}
    else if(str=="multi"||str=="*") {return multi;}
    else if(str=="div"  ||str=="/") {return div;  }
    else if(str=="mod"  ||str=="%") {return mod;  }
    return default_;
}

void CalcWindow::digits()
{
    QPushButton *button = static_cast<QPushButton*>(sender());

    if((math_switch("+")||math_switch("-")
            ||math_switch("*")||math_switch("/")
            ||math_switch("%"))
                &&clear_checked())

               {

                reset_scr();

                clear_checked()=false;

                }



    if(!(ui->result->text()=="0"))

        ui->result->setText(ui->result->text() + button->text());

    else

        ui->result->setText(button->text());
//math_switch("-")=true;
//ui->statusbar->showMessage(QString::number(math_switch("-")));
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
        buffer_=all_numbers;
    }

        else if(button->text() == "%")
    {
        all_numbers = (ui->result->text()).toDouble();
        all_numbers*=0.01;
        new_label = QString::number(all_numbers, 'g', 15);

        ui->result->setText(new_label);
        buffer_=all_numbers;
    }
        else if(button==ui->sqr_root_button)
    {
        all_numbers = qSqrt(ui->result->text().toDouble());
        ui->result->setText(QString::number(all_numbers));
        buffer_=all_numbers;
    }
        else if(button==ui->pow_2_button)
    {
        all_numbers = qPow(ui->result->text().toDouble(),2);
        ui->result->setText(QString::number(all_numbers));
        buffer_ = all_numbers;
    }
        else if(button==ui->pow_3_button)
    {
        all_numbers = qPow(ui->result->text().toDouble(),3);
        ui->result->setText(QString::number(all_numbers));
        buffer_ = all_numbers;
    }
        else if(button==ui->cube_root_button)
    {
        all_numbers = qPow(ui->result->text().toDouble(),(1/3.));
        ui->result->setText(QString::number(all_numbers));
        buffer_ = all_numbers;
    }
}

void CalcWindow::math_ops()
{
    QPushButton *button = static_cast<QPushButton*>(sender());
//bool_check();
    double tempNumber, num_second; //temp input value,
                                    //and second user
                                    //input value
    QString new_label;

    num_second = ui->result->text().toDouble();


    if(buffer_checked()){ //{1} if 1st math symbol

        buffer_checked()=false;
   //bool_check();
        //ui->statusbar->showMessage("buffer_check ==" + QString::number(buffer_checked()));

        buffer_=ui->result->text().toDouble();
  ui->statusbar->showMessage("miami first" + QString::number(buffer_));


    } else {

  //bool_check();
            if(math_switch("+")){ //{2} if 2nd and more..

        tempNumber = this->buffer_ + num_second;

        new_label = QString::number(tempNumber, 'g', 15);



        buffer_=tempNumber;

        ui->statusbar->showMessage("miami second plus" + new_label);

        math_switch("+")=false;
        //button->setChecked(true);
    } else if(math_switch("-")){

        tempNumber = this->buffer_ - num_second;

        new_label = QString::number(tempNumber, 'g', 15);

        //ui->result->setText(new_label);

        buffer_=tempNumber;

        ui->statusbar->showMessage(new_label);

        math_switch("-")=false;
        //button->setChecked(true);
    } else if (math_switch("*")){

        tempNumber = this->buffer_ * num_second;

        new_label = QString::number(tempNumber, 'g', 15);

        //ui->result->setText(new_label);

        buffer_ = tempNumber;

        ui->statusbar->showMessage(new_label);

        math_switch("*")=false;
        //button->setChecked(true);
    } else if (math_switch("/")){

        if(num_second==0){

        //ui->result->setText("0");

            ui->statusbar->showMessage("Warning: division by zero!!");

            } else {

        tempNumber = this->buffer_ / num_second;

        new_label = QString::number(tempNumber, 'g', 15);

        ui->statusbar->showMessage(new_label);

        buffer_ = tempNumber;

         //ui->result->setText(new_label);

            }

        math_switch("/")=false;
        //button->setChecked(true);
    } else if (math_switch("%")){

        if(num_second==0){

        //ui->result->setText("0");

            ui->statusbar->showMessage("Warning: division by zero!!");

            } else {
        int temp1=static_cast<int>(this->buffer_);
        int temp2=static_cast<int>(num_second);

        int tempres = temp1 % temp2;

        new_label = QString::number(tempres, 'g', 15);
        ui->statusbar->showMessage(new_label);
        //ui->result->setText(new_label);

        buffer_ = static_cast<double>(tempres);
                }

        math_switch("%")=false;
        //button->setChecked(true);

    }

//bool_check();

}

    math_switch(button->text())=true;

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

    if(math_switch("+")){

        tempNumber = this->buffer_ + num_second;

        new_label = QString::number(tempNumber, 'g', 15);

        ui->statusbar->showMessage(new_label);

        buffer_ = tempNumber;
        ui->result->setText(new_label);
        math_switch("+")=false;

    } else if(math_switch("-")){

        tempNumber = this->buffer_ - num_second;

        new_label = QString::number(tempNumber, 'g', 15);

        ui->statusbar->showMessage(new_label);

        buffer_ = tempNumber;
        ui->result->setText(new_label);
        math_switch("-")=false;

    } else if(math_switch("*")){

        tempNumber = this->buffer_ * num_second;

        new_label = QString::number(tempNumber, 'g', 15);

        ui->statusbar->showMessage(new_label);

        buffer_ = tempNumber;
        ui->result->setText(new_label);
        math_switch("*")=false;

    } else if(math_switch("/")){

            if(num_second==0){

                ui->statusbar->showMessage("Warning: division by zero!!");
            ui->result->setText("0");
            buffer_ = 0.0;

                } else {

            tempNumber = this->buffer_ / num_second;

            new_label = QString::number(tempNumber, 'g', 15);

            ui->statusbar->showMessage(new_label);

            buffer_ = tempNumber;

             ui->result->setText(new_label);

                }

            math_switch("/")=false;

    }   else if(math_switch("%")){

        if(num_second==0){

            ui->statusbar->showMessage("Warning: division by zero!!");

        ui->result->setText("0");

        buffer_ = 0.0;

            } else {
//        int temp1=static_cast<int>(this->buffer_);
//        int temp2=static_cast<int>(num_second);

//        int tempres = temp1 % temp2;

//        new_label = QString::number(tempres, 'g', 15);

            double buftemp = buffer_;
            double modtemp = std::fmod(buftemp,num_second);
            new_label = QString::number(modtemp);

        ui->statusbar->showMessage(new_label);

//        buffer_ = tempres;
        buffer_ = modtemp;

        ui->statusbar->showMessage(new_label);

        ui->result->setText(new_label);
                }
        math_switch("%")=false;

    }

    buffer_checked()=false;
}



void CalcWindow::on_clear_button_clicked()
{
    math_switch("+")=false;
    math_switch("-")=false;
    math_switch("*")=false;
    math_switch("/")=false;
    math_switch("%")=false;

    ui->result->setText("0");
    buffer_checked()=true;
    buffer_=0.0;
    //clear_checked()=false;
}
