//////////////////////////////////////////
//  File      : utils.cpp				//
//  Written by: ludmiloff@gmail.com		//
//  Copyright : GPL						//
//////////////////////////////////////////

#include <Utils>
#include <pwd.h>

namespace AmeUtils
{
}

QString AmeUtils::homeDir(const QString &user)
{
	if (user.isEmpty())
		return QDir::homePath();
	const passwd *p = getpwnam(user.toAscii());
	if (p) {
		return QString::fromLocal8Bit(p->pw_dir);
	}
	return QString("");
}


#ifdef Q_WS_WIN
#define ESCAPE '^'
#else
#define ESCAPE '\\'
#endif

QString AmeUtils::tildeExpand(const QString &path)
{
	if (path.length() && path[0] == QLatin1Char('~')) {
		int pos = path.indexOf(QLatin1Char('/'));
		if (pos < 0)
			return homeDir(path.mid(1));
		QString ret = homeDir(path.mid(1, pos-1));
		if (!ret.isNull())
			ret += path.mid(pos);
		return ret;
	} else if (path.length() > 1 && path[0] == QLatin1Char(ESCAPE) && path[1] == QLatin1Char('~')) {
		return path.mid(1);
	}
	return path;
}

bool AmeUtils::makeDir(const QString &dir, int mode)
{
	if (QDir::isRelativePath(dir))
		return false;
	QString target = dir;
	uint len = target.length();
	
	if (!dir.endsWith('/'))
		target.append('/');
	
	QString base("");
	uint i = 1;
	
	while (i < len) {
		int pos = target.indexOf('/', i);
		base += target.mid(i-1, pos - i + 1);
		QByteArray baseEncoded = QFile::encodeName(base);

	}
	return false;
}

