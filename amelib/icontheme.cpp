#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtGui/QIcon>
#include <QtGui/QPixmap>

#include <IconTheme>
#include <AmeDirs>


AmeIconThemeDir::AmeIconThemeDir(const QString &dir, const AmeSettings &config)
{
}

AmeIconThemeDir::~AmeIconThemeDir()
{
}

bool AmeIconThemeDir::isValid() const
{
	return mValid;
}

QString AmeIconThemeDir::dir() const
{
	return mDir;
}

QStringList AmeIconThemeDir::icons() const
{
	return QStringList();
}

QString AmeIconThemeDir::iconPath(const QString &name) const 
{
	// FINISH THIS
	return QString();
}


AmeIconTheme::AmeIconTheme(const QString &dir, QObject *parent)
  : QObject(parent),
	themeDir(dir)
{
	setTheme(themeDir + "/default/");
}

AmeIconTheme::~AmeIconTheme()
{
}

QString AmeIconTheme::name() const
{
	return themeName;
}

QString AmeIconTheme::description() const
{
	return themeDescription;
}

QString AmeIconTheme::dir() const
{
	return themeDir;
}

QString AmeIconTheme::inherits() const
{
	return themeParent;
}


bool AmeIconTheme::setTheme(const QString &theme)
{
	// 1. check if file index.theme or index.desktop exists
	QString full_path = themeDir + theme + "/";
	QString fileName, themeFile(full_path + "index.theme"), desktopFile(full_path + "index.desktop");
	if (QFile::exists(themeFile)) 
		fileName = themeFile;
	else if (QFile::exists(desktopFile))
		fileName = desktopFile;
	else
		return false;
	
	themeName = theme;
	// 2. parse theme config file
	AmeSettings config(fileName);

	QStringList::ConstIterator it;
	const QStringList dirs = config.pathListValue("Directories");
	for (it = dirs.begin(); it != dirs.end(); ++it) {
		config.beginGroup(*it);
		// check subdir existence
		if (QDir(full_path + *it + "/").exists()) {
		}
		config.endGroup();
	}

	// FINISH THIS
	return false;
}

QPixmap *AmeIconTheme::find(const QString &name, int ctx, int size) const
{
	//if (!QDir::isRelativePath(name))
	// FINISH THIS
	return NULL;
}

QPixmap *AmeIconTheme::loadIcon(const QString &name, int ctx, int size, int state) const
{
	// FINISH THIS
	return NULL;
}


AME_GLOBAL_STATIC_WITH_ARGS(AmeIconTheme, globalIconTheme, 
	(AmeDirs::global()->stdDir(AmeDirs::IconTheme)))




