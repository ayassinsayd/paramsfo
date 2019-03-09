#include "PARAM.h"


PARAM::PARAM(QString path):path(path)
{
}


PARAM::~PARAM()
{
}


bool PARAM::isValidParam(QDataStream &in)
{
	in >> magic >> version;
	return ((magic == 0x00505346) && (version == 0x01010000));
}


QByteArray PARAM::readData(QDataStream &in, quint16 offest, int len)
{
	qint64 pos = in.device()->pos(); in.device()->seek(offest);
	QByteArray data(len, '\0');
	in.readRawData(data.data(), len);
	in.device()->seek(pos);
	return data;
}


bool PARAM::writeData(QDataStream &out, quint16 Offest, QByteArray data, int len)
{
	qint64 pos = out.device()->pos(); out.device()->seek(Offest);
	out.writeRawData(data.leftJustified(len, '\0').data(), len);
	out.device()->seek(pos);
	return true;
}


bool PARAM::sfoEditor(QByteArray key, QByteArray data, quint16 fmt, quint32 max_len)
{
	QFile sfo(path);
	sfo.open(QFile::ReadWrite);
	QDataStream in(&sfo);
	QByteArray buf, buf_key, buf_data;
	QDataStream out(&buf, QIODevice::WriteOnly);
	if (!isValidParam(in))
		return false;
	out << magic << version;
	in.setByteOrder(QDataStream::LittleEndian);
	out.setByteOrder(QDataStream::LittleEndian);
	in >> sfo_key_table_start >> sfo_data_table_start >> sfo_tables_entries;
	QByteArray keys = readData(in, sfo_key_table_start, sfo_data_table_start - sfo_key_table_start).replace('\0', ' ').trimmed();
	buf_key_table_start = sfo_key_table_start;
	buf_data_table_start = sfo_data_table_start;
	buf_tables_entries = sfo_tables_entries;
	if (!keys.contains(key) && !data.isEmpty()) {
		buf_key_table_start += 0x10;
		buf_data_table_start += keys.length() + 1 + key.length() + 1;
		buf_tables_entries += 1;
	}
	else if (keys.contains(key) && data.isEmpty()) {
		buf_key_table_start -= 0x10;
		buf_data_table_start -= keys.length() + 1 + key.length() + 1;
		buf_tables_entries -= 1;
	}
	if (buf_data_table_start % 4 != 0)
		buf_data_table_start = (buf_data_table_start / 4 + 1) * 4;
	out << buf_key_table_start << buf_data_table_start << buf_tables_entries;
	quint16 buf_key_offset = 0;
	quint32 buf_data_offset = 0;
	for (int i = 0; i < buf_tables_entries; i++) {
		if (i <= sfo_tables_entries) {
			in >> sfo_key_offset >> buf_data_fmt >> buf_data_len >> buf_data_max_len >> sfo_data_offset;
			buf_key = readData(in, sfo_key_table_start + sfo_key_offset, 20).data();
			if (buf_key.contains(key) && data.isEmpty())
				continue;
			buf_data = readData(in, sfo_data_table_start + sfo_data_offset, buf_data_max_len);
		}
		if ((i > sfo_tables_entries) || (buf_key.contains(key) && !data.isEmpty())) {
			buf_key = key;
			buf_data = data;
			buf_data_fmt = fmt;
			buf_data_len = data.length();
			buf_data_max_len = max_len;
		}
		out << buf_key_offset << buf_data_fmt << buf_data_len << buf_data_max_len << buf_data_offset;
		writeData(out, buf_key_table_start + buf_key_offset, buf_key, buf_key.length());
		writeData(out, buf_data_table_start + buf_data_offset, buf_data, buf_data.length());
		buf_key_offset += key.length() + 1;
		buf_data_offset += buf_data_max_len;
	}
	sfo.resize(0);
	in.writeRawData(buf.data(), buf.length());
	return true;
}


//ACCOUNT_ID 			utf8 - S
//ACCOUNTID 			utf8
//ANALOG_MODE			int32
//APP_VER 				utf8
//ATTRIBUTE 			int32
//BOOTABLE 				int32
//CATEGORY 				utf8
//CONTENT_ID			utf8
//DETAIL 				utf8
//GAMEDATA_ID			utf8
//ITEM_PRIORITY			int32
//LANG 					int32
//LICENSE 				utf8
//NP_COMMUNICATION_ID 	utf8
//NPCOMMID 				utf8
//PADDING 				utf8 - S
//PARAMS 				utf8 - S
//PARAMS2 				utf8 - S
//PATCH_FILE 			utf8
//PS3_SYSTEM_VER 		utf8
//REGION_DENY 			int32
//RESOLUTION 			int32
//SAVEDATA_DETAIL 		utf8
//SAVEDATA_DIRECTORY 	utf8
//SAVEDATA_FILE_LIST 	utf8 - S
//SAVEDATA_LIST_PARAM 	utf8
//SAVEDATA_PARAMS 		utf8 - S
//SAVEDATA_TITLE 		utf8
//SOUND_FORMAT		 	int32
//SOURCE				int32
//SUB_TITLE				utf8
//TARGET_APP_VER		utf8
//TITLE 				utf8
//TITLE_ID				utf8
//PARENTAL_LEVEL 		int32
//PARENTALLEVEL 		int32
//VERSION 				utf8
//XMB_APPS 				int32
//PARENTAL_LEVEL_x 		int32
//TITLE_xx 				utf8
//TITLEID0xx			utf8
