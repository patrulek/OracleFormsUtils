#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

#define WIN32_LEAN_AND_MEAN

#include "FAPIWrapper.h"
#include "FAPIContext.h"
#include "FAPIModule.h"
#include "FAPILogger.h"

#include "hookhandler.h"
#include "modulemanager.h"
#include "autocompleter.h"

using namespace CPPFAPIWrapper;

int main(int argc, char *argv[])
{
    FAPILogger::changePath("builderutil.log");
    FAPILogger::setLevel(easylogger::LogLevel::LEVEL_DEBUG);

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    FAPILogger::debug("engine load");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    QQuickWindow * wnd = reinterpret_cast<QQuickWindow *>(engine.rootObjects()[0]);

    FAPILogger::debug("setting hooks");

    HookHandler::instance()->setHooks();

    FAPILogger::debug("creating module manager");

    ModuleManager::instance();

    FAPILogger::debug("creating autocompleter");

    Autocompleter::instance()->setWindow(wnd);

    FAPILogger::debug("running app");

    return app.exec();
}
