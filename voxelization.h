#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<vector>

using namespace std;

#define GAP 0.05
#define XPLUS 0
#define XMINUS 1
#define YPLUS 2
#define YMINUS 3
#define ZPLUS 4
#define ZMINUS 5

typedef int DIR;

double xmax=0, xmin=0, ymax=0, ymin=0, zmax=0, zmin=0;



struct Point
{
    double x;
    double y;
    double z;
    double point_array[3];
    Point(double x, double y, double z) :x(x), y(y), z(z) { }
    Point() {}
    double* getpoint() {
        point_array[0] = x;
        point_array[1] = y;
        point_array[2] = z;
        return point_array;
    }
};

typedef Point Vector3;  //表示三维向量

//向量的运算
Vector3 operator + (const Vector3& A, const Vector3& B)
{
    return Vector3(A.x + B.x, A.y + B.y, A.z + B.z);
}
Vector3 operator - (const Point& A, const Point& B)
{
    return Vector3(A.x - B.x, A.y - B.y, A.z - B.z);
}
Vector3 operator * (const Vector3& A, double p)
{
    return Vector3(A.x*p, A.y*p, A.z*p);
}
Vector3 operator / (const Vector3& A, double p)
{
    return Vector3(A.x / p, A.y / p, A.z / p);
}

//点乘
double Dot(const Vector3& A, const Vector3& B)
{
    return A.x*B.x + A.y*B.y + A.z*B.z;
}
//求向量长度
double Length(const Vector3& A)
{
    return sqrt(Dot(A, A));
}
//求夹角
double Angle(const Vector3& A, const Vector3& B)
{
    return acos(Dot(A, B) / Length(A) / Length(B));
}
//叉乘
Vector3 Cross(const Vector3& A, const Vector3& B)
{
    return Vector3(A.y*B.z - A.z*B.y, A.z*B.x - A.x*B.z, A.x*B.y - A.y*B.x);
}
//三角形面积
double Area2(const Point& A, const Point& B, const Point& C)
{
    return Length(Cross(B - A, C - A));
}

const double eps = 1e-8;

int dcmp(double x) {
    if (fabs(x) < eps) return 0;
    else return x < 0 ? -1 : 1;
}

// p1和p2是否在线段a-b的同侧
bool SameSide(const Point& p1, const Point& p2, const Point& a, const Point& b) {
    return dcmp(Dot(Cross(b - a, p1 - a), Cross(b - a, p2 - a))) >= 0;
}

// 点P在三角形P0, P1, P2中
bool PointInTri(const Point& P, const Point& P0, const Point& P1, const Point& P2) {
    return SameSide(P, P0, P1, P2) && SameSide(P, P1, P0, P2) && SameSide(P, P2, P0, P1);
}

// 三角形P0P1P2是否和线段AB相交
bool TriSegIntersection(const Point& P0, const Point& P1, const Point& P2, const Point& A, const Point& B, Point& P) {
    Vector3 n = Cross(P1 - P0, P2 - P0);
    if (dcmp(Dot(n, B - A)) == 0)
        return false; // 线段A-B和平面P0P1P2平行或共面
    else { // 平面A和直线P1-P2有惟一交点
        double t = Dot(n, P0 - A) / Dot(n, B - A);
        if (dcmp(t) < 0 || dcmp(t - 1) > 0)
            return false; // 不在线段AB上
        P = A + (B - A)*t; // 交点
        return PointInTri(P, P0, P1, P2);
    }
}




/*
class Face
{
private:
    Point& a;
    Point& b;
    Point& c;
public:
    Face(Point& _a, Point& _b, Point& _c) :a(_a), b(_b), c(_c) {}
    ~Face() {}
    Point& geta() {
        return a;
    }
    Point& getb() {
        return b;
    }
    Point& getc() {
        return c;
    }

    //判断一个点延某一方向的射线是否与这个三角面相交
    bool judge(const Point& p, DIR dir)
    {
        //确定要计算的线段是什么
        //然后带入计算
        Point px;
        qDebug("1inter judge dir：%d\n",dir);
        qDebug("a.x %d\n",a.x);
        qDebug("p.x %d\n",p.x);
        qDebug("a.x%d p.x%d b.x %d p.x %d c.x %dp.x %d\n",a.x,p.x,b.x,p.x,c.x,p.x);
        qDebug("2 inter judge dir：%d\n",dir);

        switch (dir)
        {
        case XPLUS:
            if (a.x < p.x && b.x < p.x && c.x < p.x)	//这个面所有的点都在中心点的x轴负方向
                return false;
            else
                return TriSegIntersection(a, b, c, p, Point(xmax, p.y, p.z), px);
            break;
        case XMINUS:
            if (a.x > p.x && b.x > p.x && c.x > p.x)
                return false;
            else
                return TriSegIntersection(a, b, c, p, Point(xmin, p.y, p.z), px);
            break;
        case YPLUS:
            if (a.y < p.y && b.y < p.y && c.y < p.y)
                return false;
            else
                return TriSegIntersection(a, b, c, p, Point(p.x, ymax, p.z), px);
            break;
        case YMINUS:
            if (a.y > p.y && b.y > p.y && c.y > p.y)
                return false;
            else
                return TriSegIntersection(a, b, c, p, Point(p.x, ymin, p.z), px);
            break;
        case ZPLUS:
            if (a.z < p.z && b.z < p.z && c.z < p.z)
                return false;
            else
                return TriSegIntersection(a, b, c, p, Point(p.x, p.y, zmax), px);
            break;
        case ZMINUS:
            if (a.z > p.z && b.z > p.z && c.z > p.z)
                return false;
            else
                return TriSegIntersection(a, b, c, p, Point(p.x, p.y, zmin), px);
            break;
        default:
            break;
        }
        qDebug("out judge\n");
    }
};
*/
class Face
{
private:
    Point& a;
    Point& b;
    Point& c;
public:
    Face(Point& _a, Point& _b, Point& _c) :a(_a), b(_b), c(_c) {}
    ~Face() {}
    Point& geta() {
        return a;
    }
    Point& getb() {
        return b;
    }
    Point& getc() {
        return c;
    }

    //判断一个点延某一方向的射线是否与这个三角面相交
    bool judge(const Point& p, DIR dir)
    {
        //确定要计算的线段是什么
        //然后带入计算
        Point px;


        switch (dir)
        {
        case XPLUS:
            if (a.x < p.x && b.x < p.x && c.x < p.x)	//这个面所有的点都在中心点的x轴负方向
                return false;
            else
                return TriSegIntersection(a, b, c, p, Point(xmax, p.y, p.z), px);
            break;
        case XMINUS:
            if (a.x > p.x && b.x > p.x && c.x > p.x)
                return false;
            else
                return TriSegIntersection(a, b, c, p, Point(xmin, p.y, p.z), px);
            break;
        case YPLUS:
            if (a.y < p.y && b.y < p.y && c.y < p.y)
                return false;
            else
                return TriSegIntersection(a, b, c, p, Point(p.x, ymax, p.z), px);
            break;
        case YMINUS:
            if (a.y > p.y && b.y > p.y && c.y > p.y)
                return false;
            else
                return TriSegIntersection(a, b, c, p, Point(p.x, ymin, p.z), px);
            break;
        case ZPLUS:
            if (a.z < p.z && b.z < p.z && c.z < p.z)
                return false;
            else
                return TriSegIntersection(a, b, c, p, Point(p.x, p.y, zmax), px);
            break;
        case ZMINUS:

            if (a.z > p.z && b.z > p.z && c.z > p.z)
                return false;
            else
                return TriSegIntersection(a, b, c, p, Point(p.x, p.y, zmin), px);
            break;
        default:
            break;
        }
        qDebug("out judge\n");
    }
};


//点和面的集合
vector<Point> points;
vector<Face> faces;
vector<Point> voxels;

//给定一个点,判断选取射线的方向:
DIR direction(const Point& p)
{
    int xn, yn, zn, m;
    xn = 0;
    yn = 0;
    zn = 0;
    m = points.size();

    int min=0;
    DIR mindir;

    for (int i = 0; i < m; i++)
    {
        if (points[i].x > p.x)
            xn++;
        if (points[i].y > p.y)
            yn++;
        if (points[i].z > p.z)
            zn++;
    }
    /*if (xn <= yn && xn <= zn && xn <= (m - xn) && xn <= (m - yn) && xn <= (m - zn))
    {
        return XPLUS;
    }
    else if ((m - xn) <= xn && (m - xn) <= yn && (m - yn) <= zn && (m - yn) <= (m - yn) && (m - xn) <= (m - zn))
    {
        return XMINUS;
    }*/

    //判断六个值中最小的那个是谁
    min = xn;
    mindir = XPLUS;
    if (min > (m - xn))
    {
        min = (m - xn);
        mindir = XMINUS;
    }
    if (min > (m - yn))
    {
        min = (m - yn);
        mindir = YMINUS;
    }
    if (min > (m - zn))
    {
        min = (m - zn);
        mindir = ZMINUS;
    }
    if (min > yn)
    {
        min = yn;
        mindir = YPLUS;
    }
    if (min > zn)
    {
        min = zn;
        mindir = ZPLUS;
    }
    return mindir;
}
