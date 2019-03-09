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
	quint32 magic, version, sfo_key_table_start, buf_key_table_start, sfo_data_table_start, buf_data_table_start, sfo_tables_entries
		, buf_tables_entries, buf_data_len, buf_data_max_len, sfo_data_offset, new_data_offset;
	quint16 sfo_key_offset, new_key_offset, buf_data_fmt;
	bool isValidParam(QDataStream & in);
	QByteArray readData(QDataStream & in, quint16 offest, int len);
	bool writeData(QDataStream & out, quint16 Offest, QByteArray data, int len);
	bool sfoEditor(QByteArray key, QByteArray data, quint16 fmt, quint32 max_len);
};

