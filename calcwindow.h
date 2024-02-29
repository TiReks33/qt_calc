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

    void digits();

    void undo();

    void simple_ops();

    void math_ops();

    void reset_scr(QString const & str="");

    bool & clear_checked(){static bool clr=false;return clr;}

    bool & buffer_checked();

    bool & math_switch(QString);

    void on_default_color_clicked();

    void on_color_1_clicked();

    void on_color_2_clicked();

    void on_dot_button_clicked();

    void on_equal_button_clicked();


    void on_clear_button_clicked();

private:
    Ui::CalcWindow *ui;
    double& buffer_;   //singleton object reference
};

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
