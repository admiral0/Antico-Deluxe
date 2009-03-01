#include <QtCore>
#include <QString>
#include <Sound>

int main(int argc, char **argv)
{
	QCoreApplication app(argc, argv);
	qDebug() << "begin";
	AmeSystemSound *snd = new AmeSystemSound();
	snd->setSound(AmeSystemSound::Popup);
	snd->play();
	return app.exec();
}

