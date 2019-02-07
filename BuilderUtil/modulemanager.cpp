#include "modulemanager.h"

#define _mm_prefetch _mm_prefetch_name_changed_to_avoid_linkage_error

#include <thread>
#include <chrono>
#include "FAPIWrapper.h"
#include "WinUtils.h"
#include "Utils.h"
#include "FAPILogger.h"
#include "FAPIModule.h"

using namespace CPPFAPIWrapper;
using namespace OracleFormsUtils;

ModuleManager * ModuleManager::s_instance = nullptr;

ModuleManager::ModuleManager()
    : is_reloading(false)
{
    ctx = createContext();
    timer.setParent(this);
    timer.start(1000);
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerProcess()));
}

ModuleManager * ModuleManager::instance() {
    if( !s_instance )
        s_instance = new ModuleManager();

    return s_instance;
}

void ModuleManager::connectToDB(std::string _userid) {
    ctx->connectContextToDB(_userid);
}

bool ModuleManager::isReloading() {
    return is_reloading;
}

void ModuleManager::reloadModule() {
    is_reloading = true;
    FAPILogger::debug("Reloading " + active_module);


    ctx->removeModule(active_module);
    ctx->loadModule(active_module);

    FAPILogger::debug("Realoding of " + active_module + " done");
    is_reloading = false;
}

void ModuleManager::timerProcess() {
    std::string title = getWindowTitle(NULL);

    if( !isOracleFormsWindow(title) )
        return;

    active_module = getPathFromBuilderWindowTitle(title);

    if( loading_modules.find(active_module) != loading_modules.end() || ctx->hasModule(active_module) )
        return;

    FAPILogger::info("Load " + active_module + " module");
    loading_modules.insert(active_module);
    ctx->loadModuleWithSources(active_module);
    loading_modules.erase(active_module);
    FAPILogger::info("Loading of " + active_module + " done");
}
