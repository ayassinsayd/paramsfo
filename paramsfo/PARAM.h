#pragma once
#include <QtCore/qfile.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qdebug.h>

class PARAM
{
public:
	PARAM(QString path);
	~PARAM();
private:
	QString path;
	bool isValidParam(QDataStream & in);
	QByteArray readData(QDataStream & in, quint16 offest, int len);
	bool writeData(QDataStream & out, quint16 Offest, QByteArray data, int len);
	bool sfoEditor(QByteArray key, QByteArray data, quint16 fmt, quint32 max_len);
};

