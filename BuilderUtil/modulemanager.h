#ifndef MODULEMANAGER_H
#define MODULEMANAGER_H

#include "FAPIContext.h"

#include <unordered_set>
#include <QObject>
#include <QTimer>

class ModuleManager : public QObject
{
    Q_OBJECT

public:
    static ModuleManager * instance();
    void connectToDB(std::string _userid);

    ModuleManager(const ModuleManager & _mgr) = delete;
    ModuleManager(ModuleManager && _mgr) = delete;
    ModuleManager & operator=(const ModuleManager & _mgr) = delete;

    void reloadModule();
    bool isReloading();
private:
    Q_SLOT void timerProcess();

    ModuleManager();
    static ModuleManager * s_instance;

    std::unique_ptr<CPPFAPIWrapper::FAPIContext> ctx;

    std::string active_module;
    std::unordered_set<std::string> loading_modules;

    QTimer timer;
    boolean is_reloading;
};

#endif // MODULEMANAGER_H
