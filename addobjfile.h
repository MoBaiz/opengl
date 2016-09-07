#ifndef ADDOBJFILE_H
#define ADDOBJFILE_H

#include <QDialog>

namespace Ui {
class addobjfile;
}

class addobjfile : public QDialog
{
    Q_OBJECT

public:
    explicit addobjfile(QWidget *parent = 0);
    ~addobjfile();

private slots:
    void on_pushButton_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_buttonBox_accepted();

    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::addobjfile *ui;
    bool distype=true;   //false 点  true点+面
    bool isdisplay=false; //false 不显示
    QString path;
    int tovox();
    int read();
    double  vsize=0;
    int voxnum=10;
    double length=0;
};

#endif // ADDOBJFILE_H
