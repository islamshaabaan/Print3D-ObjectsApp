#include <QtWidgets/QApplication>
#include <MainWindow.h>

/// <summary>
///		The Entry Point to the application.
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
int main(int argc, char* argv[]) {

	QApplication app(argc, argv);
	MainWindow mainWindow;
	//mainWindow.setWindowState(Qt::WindowMaximized);
	mainWindow.show();
	return app.exec();
}