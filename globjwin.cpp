/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "globjwin.h"
#include "glbox.h"
#include <QPushButton>
#include <QSlider>
#include <QLayout>
#include <QFrame>
#include <QMenuBar>
#include <QMenu>
#include <QApplication>
#include "addobjfile.h"
   GLObjectWindow::GLObjectWindow(QWidget* parent)
    : QWidget(parent)
{
    //create action
       createAction();
    // Create a menu
    QMenu *file = new QMenu( this );
    file->addAction(fileOpenAction);
    file->addAction("体素化",this,SLOT(addobj()));
  //  file->addAction("快速体素化",this,SLOT(addfast()));
    file->addAction( "退出",qApp, SLOT(quit())/*, CTRL+Key_Q*/);
    // Create a menu bar
    QMenuBar *m = new QMenuBar( this );
    m->addMenu(file)->setText("&文件");

    // Create a nice frame to put around the OpenGL widget
    QFrame* f = new QFrame(this);
    f->setFrameStyle( QFrame::Sunken | QFrame::Panel );
    f->setLineWidth( 2 );

    // Create our OpenGL widget
    c = new GLBox( f, "glbox");

    // Create the three sliders; one for each rotation axis
    QSlider* x = new QSlider(Qt::Vertical, this);
    x->setMaximum(360);
    x->setPageStep(60);
    x->setTickPosition( QSlider::TicksLeft );
    QObject::connect( x, SIGNAL(valueChanged(int)),c,SLOT(setXRotation(int)) );

    QSlider* y = new QSlider(Qt::Vertical, this);
    y->setMaximum(360);
    y->setPageStep(60);
    y->setTickPosition( QSlider::TicksLeft );
    QObject::connect( y, SIGNAL(valueChanged(int)),c,SLOT(setYRotation(int)) );

    QSlider* z = new QSlider(Qt::Vertical, this);
    z->setMaximum(360);
    z->setPageStep(60);
    z->setTickPosition( QSlider::TicksLeft );
    QObject::connect( z, SIGNAL(valueChanged(int)),c,SLOT(setZRotation(int)) );


    //Creat two buttom to zoom moudle
    QPushButton *zoom_in=new QPushButton("放大");
    QPushButton *zoom_out=new QPushButton("缩小");
    QPushButton *back=new QPushButton("还原");
    QObject::connect( zoom_in, SIGNAL(released()),c,SLOT(zoomin()) );
    QObject::connect( zoom_out, SIGNAL(released()),c,SLOT(zoomout()) );
    QObject::connect( back, SIGNAL(released()),c,SLOT(back()) );
    //创建三个类型选择按钮
    QHBoxLayout* typeout = new QHBoxLayout();
    QRadioButton *disply_type0=new QRadioButton("点",this);
    QRadioButton *disply_type1=new QRadioButton("线",this);
    QRadioButton *disply_type2=new QRadioButton("面",this);
    disply_type2->setChecked(true);
    QObject::connect(disply_type0, SIGNAL(clicked()),c,SLOT(setdisplaymode0()));
    QObject::connect(disply_type1, SIGNAL(clicked()),c,SLOT(setdisplaymode1()));
    QObject::connect(disply_type2, SIGNAL(clicked()),c,SLOT(setdisplaymode2()));
    typeout->addWidget(disply_type0);
    typeout->addWidget(disply_type1);
    typeout->addWidget(disply_type2);
    QVBoxLayout* setout = new QVBoxLayout();
    setout->addWidget(zoom_in);
    setout->addWidget(zoom_out);
    setout->addWidget(back);
    setout->addLayout(typeout);
    // Now that we have all the widgets, put them into a nice layout

    // Top level layout, puts the sliders to the left of the frame/GL widget
    QHBoxLayout* hlayout = new QHBoxLayout(this);

    // Put the sliders on top of each other
    QVBoxLayout* vlayout = new QVBoxLayout();
    vlayout->addWidget( x );
    vlayout->addWidget( y );
    vlayout->addWidget( z );
    // Put the GL widget inside the frame
    QHBoxLayout* flayout = new QHBoxLayout(f);
    flayout->setMargin(0);
    flayout->addWidget( c, 1 );
    hlayout->setMenuBar( m );
    hlayout->addLayout( vlayout );
    hlayout->addWidget( f, 1 );
    hlayout->addLayout( setout);
}

   void GLObjectWindow::createAction()
   {
      //创建打开文件动作
      fileOpenAction = new QAction("打开文件", this);
      //设置打开文件动作的快捷键
      fileOpenAction->setShortcut(tr("Ctrl + O"));
      //设置打开文件动作的提示信息
      fileOpenAction->setStatusTip(tr("打开一个文件"));
      //关联打开文件动作的信号和槽
      connect(fileOpenAction, SIGNAL(triggered()), this, SLOT(openFile()));
   /* fileCloseAction = new QAction(QIcon(tr("images/close.ico")), tr("关闭文件"), this);
      fileCloseAction->setShortcut(tr("Ctrl + Q"));
      fileCloseAction->setStatusTip(tr("关闭一个文件"));
      connect(fileCloseAction, SIGNAL(triggered()), this, SLOT(fileCloseActionSlot()));  */
   }
   void GLObjectWindow::openFile()
   {
       QString filename = QFileDialog::getOpenFileName(
          this,
          "Open Document",
          QDir::currentPath(),
          "Document files (*.obj);;All files(*.*)");
       if (!filename.isNull()) { //用户选择了文件
          qDebug("%s",filename.toStdString().c_str());
          c->loadmoudle(filename);
       //   QMessageBox::information(this, "Document", "Has document", QMessageBox::Ok | QMessageBox::Cancel);
       }
      // else // 用户取消选择
       //   QMessageBox::information(this, "Document", "No document", QMessageBox::Ok | QMessageBox::Cancel);

   }
   void GLObjectWindow::addobj(){
       addobjfile add;
     //  add.show();
       add.exec();
   }

   void  GLObjectWindow::addmoudle(QString path,bool type){
     if(!type)
       c->setdisplaymode(0);
     else
       c->setdisplaymode(2);

       c->loadmoudle(path);
   }
