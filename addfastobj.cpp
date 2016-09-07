/*
#include "addobjfile.h"
#include "addfastobj.h"
#include "ui_addfastobj.h"
#include "globjwin.h""
#include "voxelization.h"
#include <QByteArray>
addfastobj::addfastobj(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addfastobj)
{
    ui->setupUi(this);
}

addfastobj::~addfastobj()
{
    delete ui;
}

void addfastobj::on_horizontalSlider_valueChanged(int value)
{
    voxnum=value;
    QString s;
    s.append(voxnum);
    ui->textEdit->setText(s);
}

void addfastobj::on_buttonBox_accepted()
{
    GLObjectWindow *w=new GLObjectWindow;
    w->resize( 600, 500 );
    w->show();
   // w->addmoudle(path,true);
}
*/
