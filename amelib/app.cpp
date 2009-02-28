#include "app.h"

AmeApplication::AmeApplication(int& argc, char** argv)
  : QApplication(argc, argv)
{
	theme = new AmeIconTheme(QString("default"), this);
}

AmeApplication::~AmeApplication()
{
}
  
