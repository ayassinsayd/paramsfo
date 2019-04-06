#include <QtCore/QCoreApplication>
#include <QtCore/qdebug.h>
#include "PARAM.h"


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	PARAM("c:\\x.sfo").remove("target_app_ver");
	PARAM("c:\\x.sfo").insert("target_app_ver", "00.01", 8);
	qDebug() << "finish";
	getchar();
	//return a.exec();
}