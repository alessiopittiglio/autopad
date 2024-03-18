#include <QApplication>
#include "MainWindow.h"
#include "Corrector.h"

int main(int argc, char *argv[])
{
	// instance main application
    QApplication a(argc, argv);

	// instance and show main window
	MainWindow *win = new MainWindow();
	win->show();

	// instance utilities
	Corrector::instance();

	// run main application
    a.exec();

	// destroy window
	delete win;

	return EXIT_SUCCESS;
}
