#pragma once
#include <QtCore/qvector.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qfile.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qdebug.h>


class PARAM
{
public:
	enum key {
		ACCOUNT_ID,
		ACCOUNTID,
		ANALOG_MODE,
		APP_VER,
		ATTRIBUTE,
		BOOTABLE,
		CATEGORY,
		CONTENT_ID,
		DETAIL,
		GAMEDATA_ID,
		ITEM_PRIORITY,
		LANG,
		LICENSE,
		NP_COMMUNICATION_ID,
		NPCOMMID,
		PADDING,
		PARAMS,
		PARAMS2,
		PARENTAL_LEVEL_x,
		PARENTAL_LEVEL,
		PARENTALLEVEL,
		PATCH_FILE,
		PS3_SYSTEM_VER,
		REGION_DENY,
		RESOLUTION,
		SAVEDATA_DETAIL,
		SAVEDATA_DIRECTORY,
		SAVEDATA_FILE_LIST,
		SAVEDATA_LIST_PARAM,
		SAVEDATA_PARAMS,
		SAVEDATA_TITLE,
		SOUND_FORMAT,
		SOURCE,
		SUB_TITLE,
		TARGET_APP_VER,
		TITLE,
		TITLE_ID,
		TITLE_xx,
		TITLEID0xx,
		VERSION,
		XMB_APPS
	};
	PARAM(const QString &path);
	~PARAM();
	bool isparam();
	bool insert(key key, const QByteArray & data);
	bool remove(key key);
	QByteArray at(key key);
	int length();
	struct SFO {
		friend QDataStream &operator>>(QDataStream &in, SFO &s);
		friend QDataStream &operator<<(QDataStream &out, SFO &s);
		struct header {
			quint32 magic;
			quint32 version;
			quint32 key_table_start;
			quint32 data_table_start;
			quint32 tables_entries;
		} header;
		struct index {
			quint16 key_offset;
			quint16 data_fmt;
			quint32 data_len;
			quint32 data_max_len;
			quint32 data_offset;
		};
		QVector <index> index_table;
		QVector <QByteArray> key_table;
		QVector <QByteArray> data_table;
	};
private:
	SFO s;
	QFile f;
	bool commit;
	QByteArray key_name[41] = {
		"ACCOUNT_ID",
		"ACCOUNTID",
		"ANALOG_MODE",
		"APP_VER",
		"ATTRIBUTE",
		"BOOTABLE",
		"CATEGORY",
		"CONTENT_ID",
		"DETAIL",
		"GAMEDATA_ID",
		"ITEM_PRIORITY",
		"LANG",
		"LICENSE",
		"NP_COMMUNICATION_ID",
		"NPCOMMID",
		"PADDING",
		"PARAMS",
		"PARAMS2",
		"PARENTAL_LEVEL_x",
		"PARENTAL_LEVEL",
		"PARENTALLEVEL",
		"PATCH_FILE",
		"PS3_SYSTEM_VER",
		"REGION_DENY",
		"RESOLUTION",
		"SAVEDATA_DETAIL",
		"SAVEDATA_DIRECTORY",
		"SAVEDATA_FILE_LIST",
		"SAVEDATA_LIST_PARAM",
		"SAVEDATA_PARAMS",
		"SAVEDATA_TITLE",
		"SOUND_FORMAT",
		"SOURCE",
		"SUB_TITLE",
		"TARGET_APP_VER",
		"TITLE",
		"TITLE_ID",
		"TITLE_xx",
		"TITLEID0xx",
		"VERSION",
		"XMB_APPS"
	};
	int data_max_len[41] = {
		0x010,
		0x010,
		0x004,
		0x008,
		0x004,
		0x004,
		0x004,
		0x030,
		0x400,
		0x020,
		0x004,
		0x004,
		0x200,
		0x010,
		0x010,
		0x008,
		0x400,
		0x00C,
		0x004,
		0x004,
		0x004,
		0x020,
		0x008,
		0x004,
		0x004,
		0x400,
		0x040,
		0xC60,
		0x008,
		0x080,
		0x080,
		0x004,
		0x004,
		0x080,
		0x008,
		0x080,
		0x010,
		0x080,
		0x010,
		0x008,
		0x004
	};
};

//ACCOUNT_ID					utf8 - S 					16 		(0x010)
//ACCOUNTID 					utf8 						16 		(0x010)
//ANALOG_MODE 					int32	 					4 		(0x004)
//APP_VER 						utf8 						8		(0x008)
//ATTRIBUTE 					int32 						4 		(0x004)
//BOOTABLE 						int32 						4 		(0x004)
//CATEGORY 						utf8 						4 		(0x004)
//CONTENT_ID 					utf8 						48		(0x030)
//DETAIL 						utf8 		 				1024 	(0x400)
//GAMEDATA_ID 					utf8 		 				32 		(0x020)
//ITEM_PRIORITY 				int32	 					4 		(0x004)
//LANG 							int32 						4 		(0x004)
//LICENSE 						utf8 					 	512 	(0x200)
//NP_COMMUNICATION_ID 			utf8 						16 		(0x010)
//NPCOMMID 						utf8 						16 		(0x010)
//PADDING 						utf8 - S 					8 		(0x008)
//PARAMS 						utf8 - S 					1024 	(0x400)
//PARAMS2 						utf8 - S 					12 		(0x00C)
//PARENTAL_LEVEL_x 				int32 						4 		(0x004)
//PARENTAL_LEVEL 				int32 						4		(0x004)
//PARENTALLEVEL 				int32 						4 		(0x004)
//PATCH_FILE 					utf8 	 					32 		(0x020)
//PS3_SYSTEM_VER 				utf8 						8 		(0x008)
//REGION_DENY 					int32 						4 		(0x004)
//RESOLUTION 					int32 						4 		(0x004)
//SAVEDATA_DETAIL 				utf8 					 	1024 	(0x400)
//SAVEDATA_DIRECTORY 			utf8 					 	64 		(0x040)
//SAVEDATA_FILE_LIST			utf8 - S 					3168 	(0xC60)
//SAVEDATA_LIST_PARAM 			utf8 						8 		(0x008)
//SAVEDATA_PARAMS 				utf8 - S					128 	(0x080)
//SAVEDATA_TITLE 				utf8 				 		128 	(0x080)
//SOUND_FORMAT 					int32 						4 		(0x004)
//SOURCE						int32 						4 		(0x004)
//SUB_TITLE 					utf8 			 			128 	(0x080)
//TARGET_APP_VER 				utf8					 	8		(0x008)
//TITLE 						utf8 		 				128 	(0x080)
//TITLE_ID 						utf8 						16		(0x010)
//TITLE_xx 						utf8 			 			128 	(0x080)
//TITLEID0xx 					utf8 						16		(0x010)
//VERSION 						utf8 						8 		(0x008)
//XMB_APPS 						int32 						4 		(0x004)