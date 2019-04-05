#include <QtCore/QCoreApplication>
#include <QtCore/qdebug.h>
#include "PARAM.h"


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	PARAM("c:\\x.sfo").remove("app_ver");
	getchar();
	//return a.exec();
}