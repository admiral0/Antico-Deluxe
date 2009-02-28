#ifndef __AMEICONTHEME_H
#define __AMEICONTHEME_H

#include <QtCore/QString>
#include <QtCore/QPair>
#include <QtGui/QIcon>
#include <QtGui/QPixmap>

#include <Global>
#include <AmeDirs>
#include <Settings>

class AmeIconThemeDir
{
public:
	explicit AmeIconThemeDir(const QString &dir, const AmeSettings &config);
	~AmeIconThemeDir();

	bool isValid() const;
	QString dir() const;
	QStringList icons() const;
	QString iconPath(const QString &name) const;
private:
	bool mValid;
	QString mDir;
};


class AME_EXPORT AmeIconTheme : public QObject
{

public:

	enum States {
		DefaultState,
		ActiveState,
		DisabledState,
	};

	enum Context {
		Any,
		Action,
		Application,
		Device,
		FileSystem,
		MimeType,
		Animation,
		Category,
		Emblem,
		Emote,
		International,
		Place,
		StatusIcon,
	};

	explicit AmeIconTheme(const QString &dir, QObject *parent=0);
	virtual ~AmeIconTheme();


	static AmeIconTheme* global();
	
	QString name() const;
	QString description() const;

	QString dir() const;
	QString inherits() const;

	bool setTheme(const QString &theme);

	QPixmap *find(const QString &name, int ctx, int size) const;
	QPixmap *loadIcon(const QString &name, int ctx, int size, int state) const;

private:

protected:
	QString themeName;
	QString themeDir;
	QString themeParent;
	QString themeDescription;
	
	QMap <QPair<int, int>, AmeIconThemeDir *> mDirs;
	
	void paint(QPainter *paint, const QRect &rect, QIcon::Mode mode, QIcon::State state); 
	
};

#endif /* __AMEICONTHEME_H */

