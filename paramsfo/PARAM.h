#pragma once
#include <QtCore/qvector.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qfile.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>


class PARAM
{
public:
	PARAM(QString path);
	~PARAM();
	bool isValidParam(QDataStream & in);
	struct SFO {
		friend QDataStream &operator>>(QDataStream &in, SFO &s);
		friend QDataStream &operator<<(QDataStream &out, SFO &s);
		struct header {
			quint32 magic = 0x00505346;
			quint32 version = 0x01010000;
			quint32 key_table_start;
			quint32 data_table_start;
			quint32 tables_entries;
		} header;
		struct index_table {
			quint16 key_offset;
			quint16 data_fmt;
			quint32 data_len;
			quint32 data_max_len;
			quint32 data_offset;
		};
		QVector <index_table> index_table;
		QVector <QByteArray> key_table;
		QVector <QByteArray> data_table;
	} sfo;
private:
	QFile f;
};

