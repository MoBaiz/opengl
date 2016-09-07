#ifndef READOBJ_H
#define READOBJ_H

#endif // READOBJ_H
#include<vector>
#include<QtOpenGL/glut>
#include <QtOpenGL>
const int n = 5;
const GLfloat R = 0.5f;
const GLfloat Pi = 3.1415926536f;
typedef GLfloat point2[2];
class readobj{
    GLfloat dx = R*cos(18 * Pi / 180),
            dy = R*sin(18 * Pi / 180),
            bx = R*cos(54 * Pi / 180),
            by = -R*sin(54 * Pi / 180),
            Ax = R*sin(36 * Pi / 180)*cos(72 * Pi / 180) / sin(126 * Pi / 180),
            Ay = R*sin(18 * Pi / 180)*cos(36 * Pi / 180) / sin(126 * Pi / 180),
            Ex = 2 * Ax*sin(54 * Pi / 180),
            Ey = Ay - 2 * Ax*cos(18 * Pi / 180),
            Dy = Ay - (2 * Ax*cos(18 * Pi / 180) + 2 * Ax*cos(54 * Pi / 180));
    point2 vertices[10] = { { 0, R }, { bx, by }, { -dx, dy }, { dx, dy }, { -bx, by }, { Ax, Ay }, { -Ax, Ay }, { -Ex, Ey }, { 0, Dy }, { Ex, Ey } };
public:
   readobj();
   void load();
   virtual GLuint makeObject() ;
};
