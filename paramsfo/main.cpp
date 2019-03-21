#include <QtCore/QCoreApplication>
#include <QtCore/qdebug.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qfile.h>
#include <QtCore/qvector.h>

typedef struct {
	quint32 magic;
	quint32 version;
	quint32 key_table_start;
	quint32 data_table_start;
	quint32 tables_entries;
} header;

typedef struct {
	quint16 key_offset;
	quint16 data_fmt;
	quint32 data_len;
	quint32 data_max_len;
	quint32 data_offset;
} index_table;

typedef struct {
	header header;
	QVector <index_table> index_table;
	QVector <QByteArray> key_table;
	QVector <QByteArray> data_table;
} sfo;

QDataStream &operator<<(QDataStream &out, sfo &s) {
	out.device()->reset();
	out.setByteOrder(QDataStream::BigEndian);
	out << s.header.magic << s.header.version;
	out.setByteOrder(QDataStream::LittleEndian);
	s.header.key_table_start = 0x14 + s.index_table.count() * 0x10;
	s.header.data_table_start = s.header.key_table_start;
	s.header.tables_entries = s.index_table.count();
	for (auto key : s.key_table)
		s.header.data_table_start += key.length();
	if (s.header.data_table_start % 4 != 0)
		s.header.data_table_start = (s.header.data_table_start / 4 + 1) * 4;
	out << s.header.key_table_start << s.header.data_table_start << s.header.tables_entries;
	for (int i = 0; i < s.header.tables_entries; ++i)
		out << s.index_table[i].key_offset << s.index_table[i].data_fmt << s.index_table[i].data_len
		<< s.index_table[i].data_max_len << s.index_table[i].data_offset;
	for (int i = 0; i < s.header.tables_entries; ++i)
		out.writeRawData(s.key_table[i].data(), s.key_table[i].length());
	out.device()->seek(s.header.data_table_start);
	for (int i = 0; i < s.header.tables_entries; ++i)
		out.writeRawData(s.data_table[i].data(), s.data_table[i].length());
	return out;
}

QDataStream &operator>>(QDataStream &in, sfo &s) {
	in.device()->reset();
	in.setByteOrder(QDataStream::BigEndian);
	in >> s.header.magic >> s.header.version;
	in.setByteOrder(QDataStream::LittleEndian);
	in >> s.header.key_table_start >> s.header.data_table_start >> s.header.tables_entries;
	s.index_table.resize(s.header.tables_entries);
	for (int i = 0; i < s.header.tables_entries; ++i)
		in >> s.index_table[i].key_offset >> s.index_table[i].data_fmt >> s.index_table[i].data_len
		>> s.index_table[i].data_max_len >> s.index_table[i].data_offset;
	for (int i = 0; i < s.header.tables_entries; ++i) {
		quint8 byte;
		QByteArray key;
		do {
			in >> byte;
			key.append(byte);
		} while (byte);
		s.key_table << key;
	}
	in.device()->seek(s.header.data_table_start);
	for (int i = 0; i < s.header.tables_entries; ++i) {
		QByteArray data(s.index_table[i].data_max_len, '\0');
		in.readRawData(data.data(), s.index_table[i].data_max_len);
		s.data_table << data;
	}
	return in;
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	QFile f("c:\\x.sfo");
	f.open(QIODevice::ReadWrite);
	sfo sfo;
	QDataStream ds(&f);
	ds >> sfo;
	for(auto s: sfo.key_table)
		qDebug() << s;
	for (auto s : sfo.data_table)
		qDebug() << s;
	f.resize(0);
	ds << sfo;
	f.close();
	getchar();
	//return a.exec();
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
