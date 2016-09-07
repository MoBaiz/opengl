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

/****************************************************************************
**
** This is a simple QGLWidget displaying an openGL wireframe box
**
****************************************************************************/

#ifndef GLBOX_H
#define GLBOX_H

#include <QtOpenGL>
//! [0]
#include <QAxBindable>
#include<QtOpenGL/glut>
#include<vector>
#include<stdio.h>
#include<string.h>
#include<math.h>
class GLBox : public QGLWidget,
              public QAxBindable
{
    Q_OBJECT
 public:

    GLBox( QWidget* parent, const char* name = 0 );
    ~GLBox();

    QAxAggregated *createAggregate();
    void                setdisplaymode(int type){
        distype=type;
    }

public slots:

    void                setXRotation( int degrees );
    void                setYRotation( int degrees );
    void                setZRotation( int degrees );
    void                zoomin();
    void                zoomout();
    void                back();
    void                loadmoudle(QString s);
    void                setdisplaymode0();
    void                setdisplaymode1();
    void                setdisplaymode2();
protected:
    void                initializeGL();
    void                paintGL();
    void                resizeGL( int w, int h );
    void                load(QString s);
    virtual GLuint      makeObject(QString s,int type);
    virtual GLuint      makeObject(int type);

private:
    int                distype=2;
    void               polygon(int a, int b, int c, int d,int color);
    GLuint             cube();
    void               Unitizemodule();
    int                test(char *linbuff);
    int max(int a, int b) {
        if (a > b)
            return a;
        else return b;
    }
    GLuint object;
    GLfloat xRot, yRot, zRot, scale;

};

#endif // GLBOX_H
