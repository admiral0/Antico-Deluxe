#include <QtCore>
#include <QString>
#include <Sound>

int main(int argc, char **argv)
{
	QCoreApplication app(argc, argv);
	qDebug() << "begin";
	AmeSystemSound *snd = new AmeSystemSound();
	snd->setEmbedSound(AmeSystemSound::Popup);
	snd->play();
	while (snd->isRunning()) {
	    qDebug() << "waiting";
	}
	return app.exec();
}

