

/****************************************************************************
**
** This is a simple QGLWidget displaying an openGL wireframe box
**
** The OpenGL code is mostly borrowed from Brian Pauls "spin" example
** in the Mesa distribution
**
****************************************************************************/

#include "glbox.h"
#include "glbox2.h"
#include <QAxAggregated>
#include <QUuid>
//! [0]
#include <objsafe.h>
//! [0]
#include <QOpenGLFunctions>
#include<glut>
#if defined(Q_CC_MSVC)
#pragma warning(disable:4305) // init: truncation from const double to float
#endif


/*!
  Create a GLBox widget
*/

GLBox::GLBox( QWidget* parent, const char* name )
    : QGLWidget( parent )
{
    xRot = yRot = zRot = 0.0;           // default object rotation
    scale = 1.0;                       // default object scale
    object = 0;
}


/*!
  Release allocated resources
*/

GLBox::~GLBox()
{
    makeCurrent();
    glDeleteLists( object, 1 );
}


/*!
  Paint the box. The actual openGL commands for drawing the box are
  performed here.
*/

void GLBox::paintGL()
{
 //   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    GLfloat light0_direction[] = {0.0f, 0.0f, 10.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light0_direction);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef( 0.0, 0.0, -5.0 ); //景深
    glScalef( scale, scale, scale );

    glRotatef( xRot, 1.0, 0.0, 0.0 );
    glRotatef( yRot, 0.0, 1.0, 0.0 );
    glRotatef( zRot, 0.0, 0.0, 1.0 );
    //cube();
    glCallList( object );
   // glutSolidTeapot(0.5);
    glPopMatrix();

}


/*!
  Set up the OpenGL rendering state, and define display list
*/

void GLBox::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, (float)600/(float)500, 0.01f, 200.0f);

        glClearColor(0, 0, 0, 1);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);



        glEnable(GL_CULL_FACE);

        // Setup other misc features.
        glEnable( GL_LIGHTING );
        glEnable( GL_NORMALIZE );
        glShadeModel(GL_SMOOTH);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        // Setup lighting model.
        GLfloat light_model_ambient[] = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat light0_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};

        GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

        glLightfv(GL_LIGHT0, GL_AMBIENT, light_model_ambient); //环境
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);//漫反射
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);//镜面

       // GLfloat ambient[]={0.8,0.8,0.8,1.0};
     //   GLfloat diffuse[]={0.8,0.0,0.8,1.0};
      //  GLfloat specular[]={1.0,0.0,1.0,1.0};
      //  GLint shine = 100;
       // glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,ambient);
     //   glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diffuse);
     //   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular);
      //  glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,shine);
      //  glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambient);
        glEnable( GL_LIGHT0 );
        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);



}



/*!
  Set up the OpenGL view port, matrix mode, etc.
*/

void GLBox::resizeGL( int w, int h )
{/*
    glViewport( 0, 0, (GLint)w, (GLint)h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glFrustum( -1.0, 1.0, -1.0, 1.0, 5.0, 15.0 );
    glMatrixMode( GL_MODELVIEW );
   */


        // Reset the viewport
        glViewport(0, 0, w, h);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, (float)w/(float)h, 0.01f, 200.0f);
        glMatrixMode( GL_MODELVIEW );
}
void GLBox::loadmoudle(QString s){
  //  initializeGL();
    glClearColor(0, 0, 0, 1);
    scale=1.0;
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    Points.clear();
    Faces.clear();
    object=makeObject(s,distype);
   // paintGL();
    updateGL();
}

/*!
  Generate an OpenGL display list for the object to be shown, i.e. the box
*/
/*

/*!
  Set the rotation angle of the object to \e degrees around the X axis.
*/

void GLBox::setXRotation( int degrees )
{
    xRot = (GLfloat)(degrees % 360);
    updateGL();
}


/*!
  Set the rotation angle of the object to \e degrees around the Y axis.
*/

void GLBox::setYRotation( int degrees )
{
    yRot = (GLfloat)(degrees % 360);
    updateGL();
}


/*!
  Set the rotation angle of the object to \e degrees around the Z axis.
*/

void GLBox::setZRotation( int degrees )
{
    zRot = (GLfloat)(degrees % 360);
    updateGL();
}

void GLBox::zoomin(){
    scale=scale*1.25;
    qDebug("%f",(double)scale);
    updateGL();
}
void GLBox::zoomout(){
    scale=scale*0.8;
    qDebug("%f",(double)scale);
    updateGL();
}
void GLBox::load(QString s) {
  //  char filename[] = "I:\\test3.obj";
    char linebuf[1024];
    double x, y, z;
    int a, b, c, d;
    FILE *fp;
    if ((fp = fopen(s.toStdString().c_str(), "r")) == NULL) {
       qDebug("file not exist!\n");
    }
    else
         qDebug("file opened\n" );
    while (!feof(fp))
    {
        fgets(linebuf, 2048, fp);
        //printf("%s", linebuf);
        if (linebuf[0] == '#')
            continue;
        //记录点
        if (linebuf[0] == 'v' && linebuf[1]!='t' && linebuf[1]!='n' && linebuf[1] != 'p')
        {
            //printf("%s", linebuf);
            sscanf(linebuf, "%*c %lf %lf %lf", &x, &y, &z);
            Points.push_back(GPOINT(x, y, z));

        }
        //记录面

        if (linebuf[0] == 'f' && linebuf[1] == ' ')
        {  if(!test(linebuf))
           sscanf(linebuf, "f %d//%*d %d//%*d %d//%*d", &a, &b, &c);
           else
                sscanf(linebuf,"f %d %d %d", &a,&b,&c);
            Faces.push_back(Face(Points[a - 1], Points[b - 1], Points[c - 1]));
        }

    }
    fclose(fp);
    qDebug("\nmoudle load finished");
}
int GLBox::test(char *linbuff){
    char ch;
    int i=0;
    while(ch!='\0'){
     if((ch=linbuff[i++])=='/')
         return 0;
    }
    return 1;
}
void GLBox::Unitizemodule()
{
    GLfloat maxx, minx, maxy, miny, maxz, minz, a, b, c;
    GLfloat cx, cy, cz, w, h, d;
    GLfloat scale;

    /* get the max/mins */
    maxx = minx = (GLfloat)Points[0].x;
    maxy = miny = (GLfloat)Points[0].y;
    maxz = minz = (GLfloat)Points[0].z;
    for (int i = 1; i<Points.size(); i++) {
        a = Points[i].x;
        b = Points[i].y;
        c = Points[i].z;
        if (maxx < a)
            maxx = a;
        if (minx > a)
            minx = a;

        if (maxy <b)
            maxy = b;
        if (miny > b)
            miny = b;

        if (maxz < c)
            maxz = c;
        if (minz > c)
            minz = c;
    }


    w = abs(maxx) + abs(minx);
    h = abs(maxy) + abs(miny);
    d = abs(maxz) + abs(minz);

    /* calculate center of the model */
    cx = (maxx + minx) / 2.0;
    cy = (maxy + miny) / 2.0;
    cz = (maxz + minz) / 2.0;

    /* calculate unitizing scale factor */
    scale = 2.0 / max(max(w, h), d);

    /* translate around center then scale */
    for (int i = 0; i <Points.size(); i++) {
        Points[i].x -= cx;
        Points[i].y -= cy;
        Points[i].z -= cz;
        Points[i].x *= scale;
        Points[i].y *= scale;
        Points[i].z *= scale;
    }
}
GLuint GLBox::makeObject(QString s, int type)
{
    GLuint list;

    list = glGenLists( 1 );

    glNewList( list, GL_COMPILE );
    load(s);
    Unitizemodule();
    switch(type){
    case 0:
        if(Points.size()==0)
             QMessageBox::information(this, "警告", "没有点的信息");
        glPointSize(1.0f);
        glBegin(GL_POINTS);
        for (int i = 0; i < Points.size(); i++){
                glVertex3fv(Points[i].getpoint());
            }
        glEnd();
        break;
    case 1:
        if(Faces.size()==0)
             QMessageBox::information(this, "警告", "没有面的信息");
        for (int i = 0; i < Faces.size(); i++) {
                glBegin(GL_LINE_STRIP);
                GLfloat *temp;
                temp=Faces[i].geta().getpoint();
                glVertex3fv(temp);
                temp = Faces[i].getb().getpoint();
                glVertex3fv(temp);
                temp = Faces[i].getc().getpoint();
                glVertex3fv(temp);
                glEnd();
            }
        break;
    case 2:
        if(Faces.size()==0)
             QMessageBox::information(this, "警告", "没有面的信息");
        for (int i = 0; i < Faces.size(); i++){
                glBegin(GL_POLYGON);
                GLfloat *temp,*NORMAL;
                NORMAL=Faces[i].getnormal();
                temp=Faces[i].geta().getpoint();
                glVertex3fv(temp);
                //glNormal3fv(NORMAL);
                temp = Faces[i].getb().getpoint();
                glVertex3fv(temp);
               // glNormal3fv(NORMAL);
                temp = Faces[i].getc().getpoint();
                glVertex3fv(temp);
               // glNormal3fv(NORMAL);
                glEnd();
            }
        break;
    }
    glEndList();

    return list;
}


GLuint GLBox::makeObject(int type)
{
    GLuint list;

    list = glGenLists( 1 );

    glNewList( list, GL_COMPILE );

    Unitizemodule();
    switch(type){
    case 0:
        if(Points.size()==0)
             QMessageBox::information(this, "警告", "没有点的信息");
        glDisable( GL_LIGHTING );
        glPointSize(1.0f);
        glBegin(GL_POINTS);
        for (int i = 0; i < Points.size(); i++){
                glVertex3fv(Points[i].getpoint());
            }
        glEnd();
        break;
    case 1:
        if(Faces.size()==0)
             QMessageBox::information(this, "警告", "没有面的信息");
        glDisable( GL_LIGHTING );
        for (int i = 0; i < Faces.size(); i++) {
                glBegin(GL_LINE_STRIP);
                GLfloat *temp;
                temp=Faces[i].geta().getpoint();
                glVertex3fv(temp);
                temp = Faces[i].getb().getpoint();
                glVertex3fv(temp);
                temp = Faces[i].getc().getpoint();
                glVertex3fv(temp);
                glEnd();
            }
         break;
    case 2:
        if(Faces.size()==0)
             QMessageBox::information(this, "警告", "没有面的信息");
        glEnable( GL_LIGHTING );
        for (int i = 0; i < Faces.size(); i++){
                glBegin(GL_POLYGON);
                GLfloat *temp,*NORMAL;
                NORMAL=Faces[i].getnormal();
                temp=Faces[i].geta().getpoint();
                glVertex3fv(temp);
                glNormal3fv(NORMAL);
                temp = Faces[i].getb().getpoint();
                glVertex3fv(temp);
                glNormal3fv(NORMAL);
                temp = Faces[i].getc().getpoint();
                glVertex3fv(temp);
                glNormal3fv(NORMAL);
                glEnd();
            }
         break;
    }
    glEndList();

    return list;
}

//! [1]
class ObjectSafetyImpl : public QAxAggregated,
        public IObjectSafety
{
public:
    //! [1] //! [2]
    ObjectSafetyImpl() {}

    long queryInterface( const QUuid &iid, void **iface )
    {
        *iface = 0;
        if ( iid == IID_IObjectSafety )
            *iface = (IObjectSafety*)this;
        else
            return E_NOINTERFACE;

        AddRef();
        return S_OK;
    }

    //! [2] //! [3]
    QAXAGG_IUNKNOWN;

    //! [3] //! [4]
    HRESULT WINAPI GetInterfaceSafetyOptions( REFIID riid, DWORD *pdwSupportedOptions, DWORD *pdwEnabledOptions )
    {
        *pdwSupportedOptions = INTERFACESAFE_FOR_UNTRUSTED_DATA | INTERFACESAFE_FOR_UNTRUSTED_CALLER;
        *pdwEnabledOptions = INTERFACESAFE_FOR_UNTRUSTED_DATA | INTERFACESAFE_FOR_UNTRUSTED_CALLER;
        return S_OK;
    }
    HRESULT WINAPI SetInterfaceSafetyOptions( REFIID riid, DWORD pdwSupportedOptions, DWORD pdwEnabledOptions )
    {
        return S_OK;
    }
};
//! [4] //! [5]

QAxAggregated *GLBox::createAggregate()
{
    return new ObjectSafetyImpl();
}
//! [5]

GLfloat vertices[][3] = {
    {-1.0,-1.0,-1.0},{1.0,-1.0,-1.0},{1.0,1.0,-1.0},{-1.0,1.0,-1.0},{-1.0,-1.0,1.0},{1.0,-1.0,1.0},{1.0,1.0,1.0},{-1.0,1.0,1.0}
};
GLfloat COLORS[][3] = {
    {1.0,1.0,1.0},{1.0,0.0,0.0},{1.0,1.0,0.0},{0.0,1.0,0.0},{1.0,0.0,1.0},{0.0,1.0,1.0}
};
void GLBox::polygon(int a, int b, int c, int d,int color) {
    glBegin(GL_POLYGON);
    glColor3fv(COLORS[color]);
    glVertex3fv(vertices[a]);
    glColor3fv(COLORS[color]);
    glVertex3fv(vertices[b]);
    glColor3fv(COLORS[color]);
    glVertex3fv(vertices[c]);
    glColor3fv(COLORS[color]);
    glVertex3fv(vertices[d]);
    glEnd();
}
GLuint GLBox::cube() {


    polygon(0, 3, 2, 1,0);
    polygon(5,1,2,6,1);
    polygon(4,0,1,5,2);
    polygon(3,0,4,7,3);
    polygon(6,2,3,7,4);
    polygon(4,5,6,7,5);

}
void GLBox::setdisplaymode2(){
    qDebug("type---%d",2);
    distype=2;
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    object=makeObject(distype);
    updateGL();
}
void GLBox::setdisplaymode0(){
    distype=0;
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    object=makeObject(distype);
    updateGL();
}
void GLBox::setdisplaymode1(){
    qDebug("type---%d",1);
    distype=1;
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    object=makeObject(distype);
    updateGL();
}
void GLBox::back(){
    scale=1.0;
    updateGL();
}
