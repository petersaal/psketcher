#include <QApplication>
#include "Ark3DWindow.h"

int main(int argc, char *argv[])
{
	QApplication app( argc, argv );
	Ark3DWindow mainWin;
    mainWin.show();
	return app.exec();
}
