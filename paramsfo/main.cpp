#include <QtCore/QCoreApplication>
#include <QtCore/qdebug.h>
#include "PARAM.h"


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	PARAM("c:\\x.sfo").remove(PARAM::TARGET_APP_VER);
	PARAM("c:\\x.sfo").insert(PARAM::TARGET_APP_VER, "00.01");
	qDebug() << PARAM("c:\\x.sfo").at(PARAM::TITLE_ID);
	getchar();
	//return a.exec();
}