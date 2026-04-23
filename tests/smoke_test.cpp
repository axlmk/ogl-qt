#include <ft2build.h>
#include <stb_image_write.h>

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QOffscreenSurface>
#include <QOpenGLContext>

#include "assimp/Importer.hpp"
#include "glm/glm.hpp"

int smoke_test(int argc, char* argv[])
{
	qInstallMessageHandler([](QtMsgType, const QMessageLogContext&, const QString& msg) { fprintf(stderr, "%s\n", msg.toLocal8Bit().constData()); });
	QApplication app(argc, argv);

	qInfo() << "Working dir:" << QDir::currentPath();
	qInfo() << "App dir:" << QCoreApplication::applicationDirPath();

	QString appDir = QCoreApplication::applicationDirPath();
	QString path = QDir(appDir).filePath("resources/models/sphere/sphere.obj");

	if (!QFile::exists(path))
	{
		qCritical() << "Missing resource file!";
		return 1;
	}

	QOpenGLContext ctx;
	if (!ctx.create())
	{
		qCritical() << "Failed to create OpenGL context!";
		return 2;
	}

	QOffscreenSurface surface;
	surface.create();

	if (!ctx.makeCurrent(&surface))
	{
		qCritical() << "Failed to make OpenGL context current!";
		return 3;
	}

	qInfo() << "Qt platform:" << QGuiApplication::platformName();
	qInfo() << "OpenGL version: " << (const char*)glGetString(GL_VERSION);
	qInfo() << "= = = Everything's OK! = = =";

	return 0;
}