#ifndef __AMEAPPLICATION_H
#define __AMEAPPLICATION_H

#include <Global>
#include <IconTheme>
#include <QtGui/QApplication>

class AME_EXPORT AmeApplication : public QApplication
{
public:
	explicit AmeApplication(int& argc, char** argv);
	virtual ~AmeApplication();

private:

protected:
	AmeIconTheme* theme;

};

#endif /* __AMEAPPLICATION_H */
