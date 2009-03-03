//////////////////////////////////////////
//  File      : settings.cpp			//
//  Written by: ludmiloff@gmail.com		//
//  Copyright : GPL2					//
//////////////////////////////////////////

#include <Settings>

AmeSettings::AmeSettings(const QString &fileName, Format format, QObject *parent)
  : QSettings(fileName, format, parent)
{
}

QStringList AmeSettings::pathListValue(const QString &key, char sep) const
{
	const QString data = value(key, QVariant("")).toString();
	return data.split(sep, QString::SkipEmptyParts);
}

QStringList AmeSettings::pathListValue(const QByteArray &key, char sep) const 
{
	return pathListValue(QString(key), sep);
}

QStringList AmeSettings::pathListValue(const char *key, char sep) const
{
	return pathListValue(QString(key), sep);
}
