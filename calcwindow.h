#ifndef CALCWINDOW_H
#define CALCWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class CalcWindow; }
QT_END_NAMESPACE


class CalcWindow : public QMainWindow
{
    Q_OBJECT

public:
    CalcWindow(QWidget *parent = nullptr);
    ~CalcWindow();



private slots:

 // input digits to screen to operate with
    void digits();

 // delete digits from screen 1-by-1 to zero
    void undo();

 // unarian operations such as sqrt and ^2
    void simple_ops();

 // binary operations such as + and /
    void math_ops();

 // clear screen or insert text on screen
    void reset_scr(QString const & str="");

 // check that this is a new input to screen
    bool & clear_checked(){static bool clr=false;return clr;}

 // logical func used to save info from screen to buffer_
 // (for next number buffering after reset)
    bool & buffer_checked(){static bool buf=true;return buf;}

 // check what binary operation needed to execute
    bool & math_switch(QString);

 // customize style of calc widget
    void on_default_color_clicked();

    void on_color_1_clicked();

    void on_color_2_clicked();

 // separate unary func for float number
    void on_dot_button_clicked();

 // finalize the ariphmetic equation
 //   and show result on main screen
    void on_equal_button_clicked();

 // "C" button on calculator keyboard
    void on_clear_button_clicked();

private:
    Ui::CalcWindow *ui;

     // singleton instance reference
    // (for buffering temporary
   // result of equations)
    double& buffer_;
};

// class of buffer singleton
struct Buffer
{
public:
    static Buffer & instance() {
        static Buffer buf;
        return buf;
    }

    double & data()  {return data_;}
private:
    Buffer(){}

    Buffer(Buffer const&);
    Buffer& operator=(Buffer const&);

    double data_;
};

#endif // CALCWINDOW_H
