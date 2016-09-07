
#include "addobjfile.h"
#include "ui_addobjfile.h"
#include <QFileDialog>
#include "globjwin.h""
#include "voxelization.h"
#include <QByteArray>
#include <QMessageBox>
int test(char *linbuff){
    char ch;
    int i=0;
    while(ch!='\0'){
     if((ch=linbuff[i++])=='/')
         return 0;
    }
    return 1;
}
addobjfile::addobjfile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addobjfile)
{
    ui->setupUi(this);
}

addobjfile::~addobjfile()
{
    delete ui;
}

void addobjfile::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                   this,
                   "Open Document",
                   QDir::currentPath(),
                   "Document files (*.obj);;All files(*.*)");
                if (!filename.isNull()) { //用户选择了文件
                   qDebug("%s",filename.toStdString().c_str());
                }
   path=filename;
   ui->textEdit->setText(filename);
   ui->textBrowser->append("添加路径成功，请点击读取文件\n");
}

void addobjfile::on_radioButton_clicked()//点
{
    distype=false;
}

void addobjfile::on_radioButton_2_clicked()//点和面
{
    distype=true;
}

void addobjfile::on_buttonBox_accepted()
{
    if(isdisplay){
            GLObjectWindow *w=new GLObjectWindow;
            w->resize( 600, 500 );
            w->show();
            if(distype)
            w->addmoudle(path,true);
            else
            w->addmoudle(path,false);
        }
}

void addobjfile::on_checkBox_stateChanged(int arg1)
{
    if(arg1)
        isdisplay=true;
    else isdisplay=false;
}

int addobjfile::tovox()
{   voxels.clear();
    //设置进度条
    ui->progressBar->setMaximum(voxnum);
    qDebug("voxbegin\n");
    ui->textBrowser->append("体素化开始\n");
    int count=0;
    //对bounding box进行分割
    for (double i_x = (xmin + 0.5*vsize); i_x <= xmax; i_x += vsize)
    {
         ui->progressBar->setValue(count++);
         qDebug("count%d\n",count);
        for (double i_y = (ymin + 0.5*vsize); i_y <= ymax; i_y += vsize)
        {
            for (double i_z = (zmin + 0.5*vsize); i_z <= zmax; i_z += vsize)
            {
                //printf("doing(%lf,%lf,%lf):", i_x, i_y, i_z);
                //判断点(i_x,i_y,i_z)是否在模型内;

                Point center = Point(i_x, i_y, i_z);
                double *temp=center.getpoint();
                //先判断选取的射线方向:
                DIR dirs = direction(center);
                //再遍历这个方向的所有面,求出交点数量
                int number = 0;

                for (int i = 0; i < faces.size(); i++)
                {
                     double *temp=center.getpoint();

                    if (faces[i].judge(center, dirs))
                        number++;
                }
                //printf(" %d,", number);
                if (number % 2 == 0)
                    //printf("no\n");
                    continue;
                else
                    voxels.push_back(center);
                    //printf("yes\n");

            }
        }
    }
    //ui->textBrowser->append("TOTAL VOXELS: %d\n",voxels.size());
  //  ui->textBrowser->append("TOTAL VOXELS: %d\n");

    printf("GENERRATING VOXELIZED MESH\n");
    //生成obj模型
    FILE *fw;
    path+="voxnum_";
    path.append(QString("%1").arg(voxnum));
    path+=".obj";
    char * Filename;
    QByteArray ba = path.toLatin1();
    Filename=ba.data();
    //ui->textBrowser->append(path);
    if ((fw = fopen(Filename, "w")) == NULL)
    {
        qDebug("CANNOT CREATE OBJ FILE\n");
        ui->textBrowser->append("创建文件失败\n");
        return -1;
    }
    //打印点的信息
    for (int i = 0; i < voxels.size(); i++)
    {
        //1
        fprintf(fw, "v %lf %lf %lf\n", (voxels[i].x - 0.5*vsize), (voxels[i].y - 0.5*vsize), (voxels[i].z - 0.5*vsize));
        //2
        fprintf(fw, "v %lf %lf %lf\n", (voxels[i].x + 0.5*vsize), (voxels[i].y - 0.5*vsize), (voxels[i].z - 0.5*vsize));
        //3
        fprintf(fw, "v %lf %lf %lf\n", (voxels[i].x + 0.5*vsize), (voxels[i].y + 0.5*vsize), (voxels[i].z - 0.5*vsize));
        //4
        fprintf(fw, "v %lf %lf %lf\n", (voxels[i].x - 0.5*vsize), (voxels[i].y + 0.5*vsize), (voxels[i].z - 0.5*vsize));
        //5
        fprintf(fw, "v %lf %lf %lf\n", (voxels[i].x - 0.5*vsize), (voxels[i].y - 0.5*vsize), (voxels[i].z + 0.5*vsize));
        //6
        fprintf(fw, "v %lf %lf %lf\n", (voxels[i].x + 0.5*vsize), (voxels[i].y - 0.5*vsize), (voxels[i].z + 0.5*vsize));
        //7
        fprintf(fw, "v %lf %lf %lf\n", (voxels[i].x + 0.5*vsize), (voxels[i].y + 0.5*vsize), (voxels[i].z + 0.5*vsize));
        //8
        fprintf(fw, "v %lf %lf %lf\n", (voxels[i].x - 0.5*vsize), (voxels[i].y + 0.5*vsize), (voxels[i].z + 0.5*vsize));
    }
    //打印面的信息
    if(distype)
    for (int k = 0; k < voxels.size(); k++)
    {
        int j = k * 8;
        fprintf(fw, "f %d %d %d\n", 3 + j, 2 + j, 1 + j);
        fprintf(fw, "f %d %d %d\n", 1 + j, 4 + j, 3 + j);

        fprintf(fw, "f %d %d %d\n", 1 + j, 2 + j, 6 + j);
        fprintf(fw, "f %d %d %d\n", 6 + j, 5 + j, 1 + j);

        fprintf(fw, "f %d %d %d\n", 2 + j, 3 + j, 6 + j);
        fprintf(fw, "f %d %d %d\n", 3 + j, 7 + j, 6 + j);

        fprintf(fw, "f %d %d %d\n", 8 + j, 4 + j, 1 + j);
        fprintf(fw, "f %d %d %d\n", 1 + j, 5 + j, 8 + j);

        fprintf(fw, "f %d %d %d\n", 3 + j, 4 + j, 8 + j);
        fprintf(fw, "f %d %d %d\n", 8 + j, 7 + j, 3 + j);

        fprintf(fw, "f %d %d %d\n", 5 + j, 6 + j, 7 + j);
        fprintf(fw, "f %d %d %d\n", 7 + j, 8 + j, 5 + j);
    }
    fclose(fw);
    ui->progressBar->setValue(voxnum);

    //printf("%d\n",points.size());
    //for (int i = 0; i < points.size(); i++)
    //{
    //	printf("%lf,%lf,%lf\n", points[i].x, points[i].y, points[i].z);
    //}
    qDebug("over\n");
    ui->textBrowser->append(QString("文件已存至%1").arg(path));

    return 0;
}

int addobjfile::read(){
    char *Filename;
    char linebuf[2048];

    double x = 0, y = 0, z = 0;
    int a = 0, b = 0, c = 0, d = 0;
    //初始化
   QByteArray ba = path.toLatin1();
   Filename=ba.data();
    //读入数据
    FILE *fp;
    if ((fp = fopen(Filename, "r")) == NULL)
    {
        qDebug("CANNOT OPEN OBJ FILE\n");
        ui->textBrowser->append("FILE OPENED\n");
        return -1;
    }
    else
    {
       // qDebug("FILE OPENED\n");
        ui->textBrowser->append("文件打开成功\n");
        qDebug("文件打开成功\n");
    }
    points.clear();
    faces.clear();
    //set max and min
    while (!feof(fp))
    {
        fgets(linebuf, 2048, fp);
        if (linebuf[0] == 'v' && linebuf[1] != 't' && linebuf[1] != 'n' && linebuf[1] != 'p')
        {
            sscanf(linebuf, "%*c %lf %lf %lf", &x, &y, &z);
            xmax = x;
            xmin = x;
            ymax = y;
            ymin = y;
            zmax = z;
            zmin = z;
            points.push_back(Point(x,y,z));
            break;
        }
        else
            continue;
    }
    while (!feof(fp))
    {
        fgets(linebuf,2048,fp);
        //printf("%s", linebuf);
        if (linebuf[0] == '#')
            continue;
        //记录点
        if (linebuf[0] == 'v' && linebuf[1]!='t' && linebuf[1]!='n' && linebuf[1] != 'p')
        {
            //printf("%s", linebuf);
            sscanf(linebuf,"%*c %lf %lf %lf",&x,&y,&z);
           //  qDebug("%lf,%lf,%lf\n",x,y,z);
            if (x > xmax)
                xmax = x;
            else if (x < xmin)
                xmin = x;
            if (y > ymax)
                ymax = y;
            else if (y < ymin)
                ymin = y;
            if (z > zmax)
                zmax = z;
            else if (z < zmin)
                zmin = z;
            Point(x,y,z);
            points.push_back(Point(x,y,z));
        }
        //记录面
        if (linebuf[0] == 'f' && linebuf[1] == ' ' )
        {       if(!test(linebuf))
                sscanf(linebuf, "f %d//%*d %d//%*d %d//%*d", &a, &b, &c);
                else
                     sscanf(linebuf,"f %d %d %d", &a,&b,&c);
                 faces.push_back(Face(points[a - 1], points[b - 1], points[c - 1]));
        }
    }
    fclose(fp);
    qDebug("读取成功\n");
    //printf("facs number: %d\n", faces.size());
    ui->textBrowser->append(QString("面数: %1\n").arg(faces.size()));
    qDebug("points number: %d\n",points.size());
     ui->textBrowser->append(QString("点数: %1\n").arg(points.size()));
    qDebug("Data read complete!\n\n");
     ui->textBrowser->append("读取成功\n");
    qDebug("xmax:%lf,\nxmin:%lf,\nymax:%lf,\nymin:%lf,\nzmax:%lf,\nzmin:%lf\n", xmax, xmin, ymax, ymin, zmax, zmin);
    qDebug("\nx-scale: %lf\ny-scale: %lf\nz-scale: %lf\n", xmax - xmin, ymax - ymin, zmax - zmin);
    ui->textBrowser->append("包围盒大小：\n");
    ui->textBrowser->append(QString("长%1宽%2高%3").arg(xmax - xmin).arg(ymax - ymin).arg(zmax - zmin));
    length=xmax-xmin;
    //获取体素的大小
    qDebug("\nPlease input voxel size:");
//    scanf("%lf", &vsize);
     ui->textBrowser->append("请设置切分大小(正整数，默认值是10)\n");
}

void addobjfile::on_pushButton_2_clicked()  //添加文件
{
    read();
}


void addobjfile::on_pushButton_3_clicked() //设置voxnum
{
    voxnum=ui->textEdit_2->toPlainText().toInt();
    vsize=length/voxnum;
}

void addobjfile::on_pushButton_4_clicked() //开始体素化
{
    voxnum=ui->textEdit_2->toPlainText().toInt();
    vsize=length/voxnum;
    tovox();
}

