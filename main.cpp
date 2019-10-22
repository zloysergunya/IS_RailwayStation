#include "mainwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MainWidget w;
	w.resize(400, 300);
	w.show();

	return a.exec();
}
