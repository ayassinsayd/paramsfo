#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qbytearray.h>




bool isValidSignature(quint32 signature)
{
	return signature == 0x00505346;
}


bool isValidVersion(quint32 version)
{
	return version == 0x01010000;
}


QByteArray readValue(QDataStream &input, quint16 offest, int len)
{
	qint64 pos = input.device()->pos();
	input.device()->seek(offest);
	QByteArray value(len, '\0');
	input.readRawData(value.data(), len);
	input.device()->seek(pos);
	return value;
}


bool writeValue(QDataStream &output, quint16 Offest, QByteArray value, int len)
{
	qint64 pos = output.device()->pos();
	output.device()->seek(Offest);
	output.writeRawData(value.leftJustified(len, '\0').data(), len);
	output.device()->seek(pos);
	return true;
}


bool param(QString path, QByteArray keyname, QByteArray value)
{
	quint32 magic, version, key_table_start, new_key_table_start, data_table_start,
		new_data_table_start, tables_entries, new_tables_entries, data_len, data_max_len, data_offset, new_data_offset;
	quint16 key_offset, new_key_offset, data_fmt;
	QFile paramFile(path);
	paramFile.open(QFile::ReadWrite);
	QDataStream in(&paramFile);
	QByteArray buffer, key, data;
	QDataStream out(&buffer, QIODevice::WriteOnly);
	in >> magic >> version;
	if (!isValidSignature(magic) && !isValidVersion(version))
		return false;
	out << magic << version;
	in.setByteOrder(QDataStream::LittleEndian);
	out.setByteOrder(QDataStream::LittleEndian);
	in >> key_table_start >> data_table_start >> tables_entries;
	new_key_table_start = key_table_start + 0x10;
	new_data_table_start = new_key_table_start + readValue(in, key_table_start,
		data_table_start - key_table_start).replace('\0', ' ').trimmed().length() + 1 + keyname.length() + 1;
	if (new_data_table_start % 4 != 0)
		new_data_table_start = (new_data_table_start / 4 + 1) * 4;
	new_tables_entries = tables_entries + 1;
	out << new_key_table_start << new_data_table_start << new_tables_entries;
	for (int i = 0; i < tables_entries; i++) {
		in >> key_offset >> data_fmt >> data_len >> data_max_len >> data_offset;
		out << key_offset << data_fmt << data_len << data_max_len << data_offset;
		key = readValue(in, key_table_start + key_offset, 20).data();
		writeValue(out, new_key_table_start + key_offset, key, key.length());
		data = readValue(in, data_table_start + data_offset, data_max_len);
		writeValue(out, new_data_table_start + data_offset, data, data.length());
	}
	new_key_offset = key_offset + key.length() + 1;
	new_data_offset = data_offset + data_max_len;
	out << new_key_offset << data_fmt << value.length() << 8 << new_data_offset;
	writeValue(out, new_key_table_start + new_key_offset, keyname.toUpper().data(), keyname.length() + 1);
	writeValue(out, new_data_table_start + new_data_offset, value, 8);
	paramFile.resize(0);
	in.writeRawData(buffer.data(), buffer.length());
	return true;
}


//bool isValidParam()
//{
//	return (isValidSignature() && isValidVersion());
//}


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	param("c:\\a.sfo", "target_app_ver", "00.99");
	param("c:\\a.sfo", "app_ver", "00.99");
	param("c:\\a.sfo", "version", "00.99");
	//getchar();
	//return a.exec();
}
