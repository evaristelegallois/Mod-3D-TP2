/*THIEL Samantha, PAREL Gabriel, M1 Informatique - TP1 Modélisation de surfaces 3D (05/03/2021)*/

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}
