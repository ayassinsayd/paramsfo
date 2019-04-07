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
	PARAM(const QString &path);
	~PARAM();
	bool isparam();
	bool insert(const QByteArray &key, const QByteArray &data, quint32 data_max_len);
	bool remove(const QByteArray &key);
	QByteArray at(const QByteArray &key);
	int length();
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
	QString keymap[41] = {
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
};