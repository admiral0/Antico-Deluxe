//////////////////////////////////////////
//  File      : amedirs.cpp				//
//  Written by: ludmiloff@gmail.com		//
//  Copyright : GPL						//
//////////////////////////////////////////

#include <stdlib.h>
#include <QtCore/QByteArray>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QCoreApplication>
//#include <QtCore/QIODevice>

#include <AmeDirs>
#include <Utils>
#include <Global>

static QString *s_localResources;
static QString *s_systemResources;
static QString *s_configsPath;
static QString *s_desktopPath;
static QString *s_autostartPath;
static QString *s_documentsPath; 
static QString *s_iconThemePath;

static QString readEnvPath(const char *env)
{
	QByteArray c_path = getenv(env);
	if (c_path.isEmpty())
		return QString();
	return QFile::decodeName(c_path);
}

AmeDirs::AmeDirs()
{
	addStdDirs();
}

AmeDirs::~AmeDirs()
{
	delete s_localResources;
	//delete s_systemResources;
	delete s_configsPath;
	delete s_desktopPath;
	delete s_autostartPath;
	delete s_documentsPath;
	delete s_iconThemePath;
}

QString AmeDirs::stdDir(int type) 
{
	switch (type) {
		case AmeDirs::Local : 
			return *s_localResources;
			break;

		case AmeDirs::System :
			return *s_systemResources;
			break;
		
		case AmeDirs::Configs :
			return *s_configsPath;
			break;

		case AmeDirs::Desktop :
			return *s_desktopPath;
			break;
	
		case AmeDirs::Autostart:
			return *s_autostartPath;
			break;

		case AmeDirs::Documents:
			return *s_documentsPath;

		case AmeDirs::IconTheme:
			return *s_iconThemePath;
			break;

		case AmeDirs::ThisApplication:
			return QCoreApplication::applicationDirPath();
		
		default:
			return QString();
	}
}

void AmeDirs::addStdDirs()
{
	QString home = AmeUtils::tildeExpand(QDir::homePath());
	QString localAmeDir = readEnvPath(getuid() ? "AMEHOME" : "AMEROOTHOME");
	if (!localAmeDir.isEmpty())
	{
		if (!localAmeDir.endsWith('/'))
			localAmeDir.append('/');
	} else {
#ifdef Q_WS_MACX
		localAmeDir = home + "/Library/Prefrerences/AME/");
#else
		localAmeDir = home + "/.ame/";
#endif
	}
	
	s_localResources = new QString(localAmeDir);
	
	s_configsPath = new QString(localAmeDir + "config/");

	s_desktopPath = new QString(home + "/Desktop/");

	s_autostartPath = new QString(home + "/Autostart/");

	s_documentsPath = new QString(home + "/Documents/");

	s_iconThemePath = new QString(localAmeDir + "icons/");
}

bool AmeDirs::createStdDirs()
{
	// TODO:
	return false;
}

AME_GLOBAL_STATIC(AmeDirs, gAmeDirs)

AmeDirs *AmeDirs::global()
{
	return gAmeDirs;
}

