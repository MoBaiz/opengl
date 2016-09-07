#ifndef GLBOX2_H
#define GLBOX2_H

#endif // GLBOX2_H
using namespace std;
struct GPOINT {
    double x;
    double y;
    double z;
    GLfloat point_array[3];
    GPOINT(double x, double y, double z) :x(x), y(y), z(z) { }
    GPOINT() {}
    GLfloat* getpoint() {
        point_array[0] = (GLfloat)x;
        point_array[1] = (GLfloat)y;
        point_array[2] = (GLfloat)z;
        return point_array;
    }
};

class Face
{
private:
    GLfloat face_array[9];
    GLfloat* point_array;
    GLfloat normal[3];
    GLfloat x1[3];
    GLfloat x2[3];
    GLfloat x3[3];
    GPOINT& a;
    GPOINT& b;
    GPOINT& c;
    GPOINT  temp;
public:
    Face(GPOINT& _a, GPOINT& _b, GPOINT& _c) :a(_a), b(_b), c(_c) {}
    ~Face() {}
    GPOINT& geta() {
        return a;
    }
    GPOINT& getb() {
        return b;
    }
    GPOINT& getc() {
        return c;
    }
    GLfloat* getnormal(){
        temp = geta();
        point_array= temp.getpoint();
        x1[0] = point_array[0];
        x1[1] = point_array[1];
        x1[2] = point_array[2];
        temp = getb();
        point_array = temp.getpoint();
        x2[0] = point_array[0];
        x2[1] = point_array[1];
        x2[2] = point_array[2];
        temp = getc();
        point_array = temp.getpoint();
        x3[0] = point_array[0];
        x3[1] = point_array[1];
        x3[2] = point_array[2];
        x3[0]=x1[0]-x3[0];
        x3[1]=x1[1]-x3[1];
        x3[2]=x1[2]-x3[2];
        x2[0]=x2[0]-x1[0];
        x2[1]=x2[1]-x1[1];
        x2[2]=x2[2]-x1[2];
        x1[0]=x3[0];
        x1[1]=x3[1];
        x1[2]=x3[2];
        normal[0]=x1[1]*x2[2]-x1[2]*x2[1];
        normal[1]=x1[2]*x2[0]-x1[0]*x2[2];
        normal[2]=x1[0]*x2[1]-x1[1]*x2[0];
        return normal;
    }
};
vector<GPOINT> Points;
vector<Face> Faces;
