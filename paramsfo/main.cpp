#include <QtCore/QCoreApplication>
#include <QtCore/qdebug.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qfile.h>
//#include "param.h"
#include <QtCore/qvector.h>
#include <QtCore/qlist.h>

typedef struct {
	uint32_t magic;
	uint32_t version;
	uint32_t key_offset;
	uint32_t data_offset;
	uint32_t entries;
} header;

typedef struct {
	uint16_t key_offset;
	uint16_t data_fmt;
	uint32_t data_len;
	uint32_t data_max_len;
	uint32_t data_offset;
} index;

typedef struct {
	header header;
	QVector <index> index;
	QVector <QByteArray> key;
	QVector <QByteArray> data;
} sfo;

//QDataStream &operator<<(QDataStream &out, const sfo &s) {
//	out << s.header.magic;
//	out << s.header.version; 
//	out << s.header.key_start;
//	out << s.header.data_start; 
//	out << s.header.entries;
	//for (int i = 0; i < 2; ++i)
	//in >> hand_shake_pkt.Reserved1[i];
//	return out;
//}

QDataStream &operator>>(QDataStream &in, sfo &s) {
	in >> s.header.magic >> s.header.version;
	in.setByteOrder(QDataStream::LittleEndian);
	in >> s.header.key_offset >> s.header.data_offset >> s.header.entries;
	for (int i = 0; i < s.header.entries; ++i)
		in >> s.index[i].key_offset >> s.index[i].data_fmt >> s.index[i].data_len >> s.index[i].data_max_len >> s.index[i].data_offset;
	quint8 byte;
	QByteArray key;
	for (int i = 0; i < s.header.entries; ++i) {
		do {
			in >> byte;
			key.append(byte);
		} while (byte != 0);
		s.key << key;
	}
	for (int i = 0; i < s.header.entries; ++i) {
		QByteArray data(s.index[0].data_max_len, '\0');
		in.readRawData(data.data(), s.index[0].data_max_len);
		s.data << data;
	}
	return in;
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	QFile f("c:\\a.sfo");
	f.open(QIODevice::ReadWrite);
	sfo sfo;
	QDataStream in(&f);
	in >> sfo;
	qDebug() << sfo.data[0];
	getchar();
}




//return a.exec();
//PARAM("c:\\a.sfo").sfoEditor("TITLE_ID", "BLES23355", 0x402, 16);
//bool isValidSignature(quint32 signature)
//{
//	return signature == 0x00505346;
//}
//
//
//bool isValidVersion(quint32 version)
//{
//	return version == 0x01010000;
//}
//
//
//QByteArray readValue(QDataStream &input, quint16 offest, int len)
//{
//	qint64 pos = input.device()->pos();
//	input.device()->seek(offest);
//	QByteArray value(len, '\0');
//	input.readRawData(value.data(), len);
//	input.device()->seek(pos);
//	return value;
//}
//
//
//bool writeValue(QDataStream &output, quint16 Offest, QByteArray value, int len)
//{
//	qint64 pos = output.device()->pos();
//	output.device()->seek(Offest);
//	output.writeRawData(value.leftJustified(len, '\0').data(), len);
//	output.device()->seek(pos);
//	return true;
//}
//
//
//bool param(QString path, QByteArray keyname, QByteArray value)
//{
//	quint32 magic, version, key_table_start, new_key_table_start, data_table_start,
//		new_data_table_start, tables_entries, new_tables_entries, data_len, data_max_len, data_offset, new_data_offset;
//	quint16 key_offset, new_key_offset, data_fmt;
//	QFile paramFile(path);
//	paramFile.open(QFile::ReadWrite);
//	QDataStream in(&paramFile);
//	QByteArray buffer, key, data;
//	QDataStream out(&buffer, QIODevice::WriteOnly);
//	in >> magic >> version;
//	if (!isValidSignature(magic) && !isValidVersion(version))
//		return false;
//	out << magic << version;
//	in.setByteOrder(QDataStream::LittleEndian);
//	out.setByteOrder(QDataStream::LittleEndian);
//	in >> key_table_start >> data_table_start >> tables_entries;
//	new_key_table_start = key_table_start + 0x10;
//	new_data_table_start = new_key_table_start + readValue(in, key_table_start,
//		data_table_start - key_table_start).replace('\0', ' ').trimmed().length() + 1 + keyname.length() + 1;
//	if (new_data_table_start % 4 != 0)
//		new_data_table_start = (new_data_table_start / 4 + 1) * 4;
//	new_tables_entries = tables_entries + 1;
//	out << new_key_table_start << new_data_table_start << new_tables_entries;
//	for (int i = 0; i < tables_entries; i++) {
//		in >> key_offset >> data_fmt >> data_len >> data_max_len >> data_offset;
//		out << key_offset << data_fmt << data_len << data_max_len << data_offset;
//		key = readValue(in, key_table_start + key_offset, 20).data();
//		writeValue(out, new_key_table_start + key_offset, key, key.length());
//		data = readValue(in, data_table_start + data_offset, data_max_len);
//		writeValue(out, new_data_table_start + data_offset, data, data.length());
//	}
//	new_key_offset = key_offset + key.length() + 1;
//	new_data_offset = data_offset + data_max_len;
//	out << new_key_offset << data_fmt << value.length() << 8 << new_data_offset;
//	writeValue(out, new_key_table_start + new_key_offset, keyname.toUpper().data(), keyname.length() + 1);
//	writeValue(out, new_data_table_start + new_data_offset, value, 8);
//	paramFile.resize(0);
//	in.writeRawData(buffer.data(), buffer.length());
//	return true;
//}