#ifndef __AMESETTINGS_H
#define __AMESETTINGS_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QString>
#include <QtCore/QSettings>
#include <Global>

class AME_EXPORT AmeSettings : public QSettings
{
public:
	AmeSettings(const QString &fileName, Format format = QSettings::IniFormat, QObject *parent=0);

	QStringList pathListValue(const QString &key, char sep = ',') const;
	QStringList pathListValue(const QByteArray &key, char sep = ',') const;
	QStringList pathListValue(const char *key, char sep = ',') const;
};

#endif /* __AMESETTINGS_H */
