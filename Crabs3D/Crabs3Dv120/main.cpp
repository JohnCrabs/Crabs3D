#include "main_window.h"
#include <QApplication>
#include <GL/freeglut.h>

int main(int argc, char *argv[])
{
    glutInit(&argc, argv); //Initialize glut library
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
