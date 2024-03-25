#include "calcwindow.h"
#include "ui_calcwindow.h"
#include <QtMath>
#include <cmath> // for fmod() (float version of modulo division C-function)
#include <QDebug>



//bool & CalcWindow::buffer_checked(){static bool buf=true;return buf;}

CalcWindow::CalcWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CalcWindow)
    , buffer_(Buffer::instance().data()) // singleton instance initialization
    , buffer_2(Buffer::instance().brackets())
{
    buffer_=0.0; // buffer value initialize

    ui->setupUi(this);

    // move widget to the @centre@ of the screen
    this->move(this->geometry().center());

    // starting "theme"
    this->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0,0,0);");
    ui->result->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0,0,0);border: 1px solid black; border-width: 1px;");

    // numbers input connect to digits() func
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

    // "undo" button signal
    connect(ui->undo_button,SIGNAL(clicked()),this,SLOT(undo()));

    // unarian ops signal to simple_ops() slot
    connect(ui->plus_minus_button,SIGNAL(clicked()),this,SLOT(simple_ops()));
    connect(ui->precent_button,SIGNAL(clicked()),this,SLOT(simple_ops()));
    connect(ui->sqr_root_button,SIGNAL(clicked()),this,SLOT(simple_ops()));
    connect(ui->cube_root_button,SIGNAL(clicked()),this,SLOT(simple_ops()));
    connect(ui->pow_2_button,SIGNAL(clicked()),this,SLOT(simple_ops()));
    connect(ui->pow_3_button,SIGNAL(clicked()),this,SLOT(simple_ops()));

    // binary ops
    connect(ui->plus_button,SIGNAL(clicked()),this,SLOT(math_ops()));

    connect(ui->minus_button,SIGNAL(clicked()),this,SLOT(math_ops()));

    connect(ui->multi_button,SIGNAL(clicked()),this,SLOT(math_ops()));

    connect(ui->div_button,SIGNAL(clicked()),this,SLOT(math_ops()));

    connect(ui->mod_button,SIGNAL(clicked()),this,SLOT(math_ops()));


    // resizing(font scalling) for buttons

    ui->button_1->installEventFilter(this);
    ui->button_2->installEventFilter(this);
    ui->button_3->installEventFilter(this);
    ui->button_4->installEventFilter(this);
    ui->button_5->installEventFilter(this);
    ui->button_6->installEventFilter(this);
    ui->button_7->installEventFilter(this);
    ui->button_8->installEventFilter(this);
    ui->button_9->installEventFilter(this);
    ui->button_0->installEventFilter(this);

    ui->undo_button->installEventFilter(this);
    ui->dot_button->installEventFilter(this);

    ui->plus_minus_button->installEventFilter(this);
    ui->precent_button->installEventFilter(this);
    ui->sqr_root_button->installEventFilter(this);
    ui->cube_root_button->installEventFilter(this);
    ui->pow_2_button->installEventFilter(this);
    ui->pow_3_button->installEventFilter(this);

    ui->plus_button->installEventFilter(this);
    ui->minus_button->installEventFilter(this);
    ui->multi_button->installEventFilter(this);
    ui->div_button->installEventFilter(this);
    ui->mod_button->installEventFilter(this);

    ui->equal_button->installEventFilter(this);

    ui->clear_button->installEventFilter(this);

      ui->result->installEventFilter(this);

      //BUFFER 2 (BRACKETS)

      connect(ui->left_br_button,SIGNAL(clicked()),this,SLOT(brackets()));

      connect(ui->right_br_button,SIGNAL(clicked()),this,SLOT(brackets()));

      ui->right_br_button->setEnabled(false);
      ui->right_br_button->setStyleSheet("color: gray");

      ui->right_br_button->installEventFilter(this);
      ui->left_br_button->installEventFilter(this);
}



CalcWindow::~CalcWindow()
{
    delete ui;
}


bool CalcWindow::eventFilter(QObject *object, QEvent *event)
{
    QPushButton *button = static_cast<QPushButton*>(object);

    if (button && event->type() == QEvent::Resize) {

        QFont font = button->font();

static double const scale_koef = this->minimumSize().height()/font.pointSize();

        if(strcmp(button->metaObject()->className(),"QPushButton")==0)
        font.setPointSize(this->size().height()/scale_koef*0.75);

        else if(strcmp(button->metaObject()->className(),"QLabel")==0)
        font.setPointSize(this->size().height()/scale_koef*2.5);


        button->setFont(font);

    }



        return QMainWindow::eventFilter(object, event);
}


void CalcWindow::reset_scr(QString const & str)
{
ui->result->setText(str);
}



bool & CalcWindow::math_switch(QString str)
{
    static bool plus     = false,
                minus    = false,
                multi    = false,
                div      = false,
                mod      = false,
                default_ = false; // yeah, i know, this is "meeh".

         if(str=="plus" ||str=="+") {return plus ;}
    else if(str=="minus"||str=="-") {return minus;}
    else if(str=="multi"||str=="*") {return multi;}
    else if(str=="div"  ||str=="/") {return div;  }
    else if(str=="mod"  ||str=="%") {return mod;  }

    return default_; // the weather is cold tomorrow i guess. %-)
}

void CalcWindow::digits()
{
    // casting input signal to button class object
    // (for using methods of button for this signal)
    QPushButton *button = static_cast<QPushButton*>(sender());

    if(
            (

            (
            math_switch("+")||math_switch("-")
          ||math_switch("*")||math_switch("/")
          ||math_switch("%")
            )

          ||brackets_check()

            )
          &&
            clear_checked()
            )
    // cleaning the screen if this is new string of number
               {

                reset_scr();

                clear_checked()=false;

                }

    if(!(ui->result->text()=="0"))

        ui->result->setText(ui->result->text() + button->text());

    else
    //delete first "zero" from number input
        ui->result->setText(button->text());
}


// "undo" button
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


// unarian ops
void CalcWindow::simple_ops()
{
    QPushButton *button = static_cast<QPushButton*>(sender());
    double all_numbers;
    QString new_label;

    // positive to negative number
    if(button->text() == "+/-"){

        all_numbers = (ui->result->text()).toDouble();
        all_numbers=-(all_numbers);
        new_label = QString::number(all_numbers, 'g', 15);

        ui->result->setText(new_label);
        buffer_=all_numbers;
    }
    // precent from number/operation
        else if(button->text() == "%")
    {

        all_numbers = (ui->result->text()).toDouble();

        if(math_switch("plus")||math_switch("minus")||math_switch("mod"))
            all_numbers*=0.01*buffer_;

//        else if(math_switch("multi")||math_switch("div"))
//            all_numbers*=0.01;

        else //if(!math_switch())
            all_numbers*=0.01;

        new_label = QString::number(all_numbers, 'g', 15);

        ui->result->setText(new_label);


        on_equal_button_clicked();


    }
    // square root
        else if(button==ui->sqr_root_button)
    {
        all_numbers = qSqrt(ui->result->text().toDouble());
        ui->result->setText(QString::number(all_numbers));
        buffer_=all_numbers;
    }
    // power^2
        else if(button==ui->pow_2_button)
    {
        all_numbers = qPow(ui->result->text().toDouble(),2);
        ui->result->setText(QString::number(all_numbers));
        buffer_ = all_numbers;
    }
    // power^3
        else if(button==ui->pow_3_button)
    {
        all_numbers = qPow(ui->result->text().toDouble(),3);
        ui->result->setText(QString::number(all_numbers));
        buffer_ = all_numbers;
    }
    // cube root
        else if(button==ui->cube_root_button)
    {
        all_numbers = qPow(ui->result->text().toDouble(),(1/3.));
        ui->result->setText(QString::number(all_numbers));
        buffer_ = all_numbers;
    }
}


// binary ops
void CalcWindow::math_ops()
{
    QPushButton *button = static_cast<QPushButton*>(sender());

    double tempNumber, num_second; //temp input value,
                                    //and second user
                                    //input value
    QString new_label;

    num_second = ui->result->text().toDouble();



    if(buffer_checked()){ //<-{1} start -- if 1st math symbol detect


        buffer_checked()=false;

        buffer_=ui->result->text().toDouble();
        ui->statusbar->showMessage(QString::number(buffer_));

    //<-{1} end

    } else {   //<-{2} start -- if 2nd and so on..

            // plus
        if(math_switch("+")){

            tempNumber = this->buffer_ + num_second;

            // number to string
            new_label = QString::number(tempNumber, 'g', 15);

            buffer_=tempNumber;

            ui->statusbar->showMessage(new_label);

            // disable plus flag
            math_switch("+")=false;

            // minus
    } else if(math_switch("-")){

            tempNumber = this->buffer_ - num_second;

            new_label = QString::number(tempNumber, 'g', 15);

            buffer_=tempNumber;

            ui->statusbar->showMessage(new_label);

            math_switch("-")=false;

            // multiply
    } else if (math_switch("*")){

            tempNumber = this->buffer_ * num_second;

            new_label = QString::number(tempNumber, 'g', 15);

            buffer_ = tempNumber;

            ui->statusbar->showMessage(new_label);

            math_switch("*")=false;

            // division
    } else if (math_switch("/")){

            // div by zero case
            if(num_second==0){

                ui->statusbar->showMessage("Warning: division by zero!!");

                // for next number buffering after reset
                buffer_checked()=true;

                buffer_ = 0.0;

                } else {

            tempNumber = this->buffer_ / num_second;

            new_label = QString::number(tempNumber, 'g', 15);

            ui->statusbar->showMessage(new_label);

            buffer_ = tempNumber;

                }

            math_switch("/")=false;

            // modulo division
    } else if (math_switch("%")){

            // div by zero
            if(num_second==0){

                ui->statusbar->showMessage("Warning: division by zero!!");

                buffer_checked()=true;

                buffer_ = 0.0;

                } else {
//            int temp1=static_cast<int>(this->buffer_);
//            int temp2=static_cast<int>(num_second);

//            int tempres = temp1 % temp2;

//            new_label = QString::number(tempres, 'g', 15);

                double buftemp = buffer_;
                double modtemp = std::fmod(buftemp,num_second);
                new_label = QString::number(modtemp);

                ui->statusbar->showMessage(new_label);

//                buffer_ = static_cast<double>(tempres);

                buffer_ = modtemp;

                    }

            // disable modulo flag
            math_switch("%")=false;

    }



    } //<-{2} end

    // current binary ariphmetic operation
    // flag setup (enabled)
    math_switch(button->text())=true;

    // cleaning screen for next number
    clear_checked()=true;

    // current bin operation symbol to screen
    reset_scr(button->text());


}


// widget customize
void CalcWindow::on_default_color_clicked()
{
    this->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0,0,0);");
    ui->result->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0,0,0);border: 1px solid black; border-width: 1px;");
}

void CalcWindow::on_color_1_clicked()
{
    this->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,    stop: 0 #77b9ea, stop: 1 #73bff5);");
    ui->result->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,    stop: 0 #2d7ab2, stop: 1 #77c3fa);"
                                "color: white; border: 1px solid black; border-width: 1px;");
}

void CalcWindow::on_color_2_clicked()
{
    this->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,    stop: 0 #2d7ab2, stop: 1 #ae32a0);"
                        "color: white;");
    ui->result->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0,0,0); border: 1px solid black; border-width: 1px;");

}

// float number dot
void CalcWindow::on_dot_button_clicked()
{
    if(!(ui->result->text().contains('.')))
        ui->result->setText(ui->result->text() + ".");
}



// finalize the last binary op,
// show result and clean buffer
void CalcWindow::on_equal_button_clicked()
{
    // temp input value,
    // and second(/last) user
    // input value
    double tempNumber, num_second;

    // to setup result on screen
    QString new_label;

    // last(current) number input
    num_second = ui->result->text().toDouble();

        // if '+' flag enabled
        // (if last operation before
        // equal is summarizing)
    if(math_switch("+")){

        tempNumber = this->buffer_ + num_second;

        new_label = QString::number(tempNumber, 'g', 15);

        ui->statusbar->showMessage(new_label);

        buffer_ = tempNumber;
        ui->result->setText(new_label);
        math_switch("+")=false;

        // if minus
    } else if(math_switch("-")){

        tempNumber = this->buffer_ - num_second;

        new_label = QString::number(tempNumber, 'g', 15);

        ui->statusbar->showMessage(new_label);

        buffer_ = tempNumber;
        ui->result->setText(new_label);
        math_switch("-")=false;

        // if multiply
    } else if(math_switch("*")){

        tempNumber = this->buffer_ * num_second;

        new_label = QString::number(tempNumber, 'g', 15);

        ui->statusbar->showMessage(new_label);

        buffer_ = tempNumber;
        ui->result->setText(new_label);
        math_switch("*")=false;

        // if div
    } else if(math_switch("/")){

        // div by zero case
            if(num_second==0){

                ui->statusbar->showMessage("Warning: division by zero!!");

                ui->result->setText("0");

                buffer_checked()=true;

                buffer_ = 0.0;

             } else {

                tempNumber = this->buffer_ / num_second;

                new_label = QString::number(tempNumber, 'g', 15);

                ui->statusbar->showMessage(new_label);

                buffer_ = tempNumber;

                ui->result->setText(new_label);

             }

            math_switch("/")=false;

            // if modulo div
    }   else if(math_switch("%")){

            // div by zero case
            if(num_second==0){

                ui->statusbar->showMessage("Warning: division by zero!!");

            ui->result->setText("0");

            buffer_checked()=true;

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


            buffer_checked()=true;

}


// "Clear" button
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

    //brackets clear-->>
    ui->equal_button->setEnabled(true);
    ui->equal_button->setStyleSheet("");
    ui->left_br_button->setEnabled(true);
    ui->left_br_button->setStyleSheet("");

    ui->right_br_button->setEnabled(false);
    ui->right_br_button->setStyleSheet("color: gray");

    brackets_check()=false;
    //<<--
}

void CalcWindow::brackets()
{
    static bool plus = false,
                minus = false,
                div = false,
                multi = false,
                mod = false;


    QPushButton *button = static_cast<QPushButton*>(sender());
    if(button==ui->left_br_button){

        //%%%%%%%%%%%%%
        // expr. backup
        //%%%%%%%%%%%%%
        if(math_switch("plus")){plus=true;math_switch("plus")=false;}
        else if(math_switch("minus")){minus=true;math_switch("minus")=false;}
        else if(math_switch("div")){div=true;math_switch("div")=false;}
        else if(math_switch("multi")){multi=true;math_switch("multi")=false;}
        else if(math_switch("mod")){mod=true;math_switch("mod")=false;}
        //%%%%%%%%%%%%%

        ui->equal_button->setEnabled(false);
        ui->equal_button->setStyleSheet("color: gray");
        ui->left_br_button->setEnabled(false);
        ui->left_br_button->setStyleSheet("color: gray");

        ui->right_br_button->setEnabled(true);
        ui->right_br_button->setStyleSheet("");

        buffer_2=buffer_;
        buffer_=0.0;
        buffer_checked()=true;
        clear_checked()=true;
        brackets_check()=true;

    } else
    if(button==ui->right_br_button){

        // checking if brackets include only digit (without expression symbol)
        if(!math_switch("plus")&&!math_switch("minus")&&!math_switch("div")
                               &&!math_switch("multi")&&!math_switch("mod"))
        {buffer_=ui->result->text().toDouble();}

        // calculate expression in brackets
        on_equal_button_clicked();

        //%%%%%%%%%%%%%
        // expr. restore
        //%%%%%%%%%%%%%
        if(plus){buffer_=buffer_2+buffer_;plus=false;}
        else if(minus){buffer_=buffer_2-buffer_;minus=false;}
        else if(div){if(!buffer_){buffer_=0;
                                  ui->statusbar->showMessage("Warning: division by zero!!");
                    }else buffer_=buffer_2/buffer_;div=false;}
        else if(multi){buffer_=buffer_2*buffer_;multi=false;}
        else if(mod){if(!buffer_){buffer_=0;
                                  ui->statusbar->showMessage("Warning: division by zero!!");
                    }else buffer_=std::fmod(buffer_2,buffer_);mod=false;}

            ui->statusbar->showMessage(QString::number(buffer_));
            ui->result->setText(QString::number(buffer_));
        //%%%%%%%%%%%%%

        ui->equal_button->setEnabled(true);
        ui->equal_button->setStyleSheet("");
        ui->left_br_button->setEnabled(true);
        ui->left_br_button->setStyleSheet("");

        ui->right_br_button->setEnabled(false);
        ui->right_br_button->setStyleSheet("color: gray");

        brackets_check()=false;
    }
}
