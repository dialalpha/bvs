#include "BVSMaster.h"



BVSMaster::BVSMaster(std::map<std::string, BVSModule*, std::less<std::string>>& bvsModuleMap, BVSConfig& config)
    : bvsModuleMap(bvsModuleMap)
    , handleMap()
    , masterModules()
    , threadedModules()
    , controller()
    , threadMutex()
    , logger("BVSMaster")
    , config(config)
{

}



BVSMaster& BVSMaster::load(const std::string& moduleName, bool asThread)
{
    // prepare path and load the lib
    std::string modulePath = "./lib" + moduleName + ".so";
    LOG(3, moduleName << " will be loaded from " << modulePath);
    void* dlib = dlopen(modulePath.c_str(), RTLD_NOW);

    // check for errors
    if (dlib == NULL)
    {
        LOG(0, "While loading " << modulePath << ", following error occured: " << dlerror());
        exit(-1);
    }

    // look for bvsRegisterModule in loaded lib, check for errors and execute register function
    typedef void (*bvsRegisterModule_t)(BVSConfig& config);
    bvsRegisterModule_t bvsRegisterModule;
    *reinterpret_cast<void**>(&bvsRegisterModule)=dlsym(dlib, "bvsRegisterModule");

    // check for errors
    char* dlerr = dlerror();
    if (dlerr)
    {
        LOG(0, "Loading function bvsRegisterModule() in " << modulePath << " resulted in: " << dlerr);
        exit(-1);
    }
    bvsRegisterModule(config);
    LOG(2, moduleName << " loaded and registered!");

    // save handle for later use
    handleMap[moduleName] = dlib;

    //std::thread t1(&BVSMaster::call_from_thread, this, bvsModuleMap[moduleName]);
    //std::cout << "me main" << std::endl;
    //t1.join();
    //std::cout << "me joined" << std::endl;


    // check thread status
    if (asThread==true)
    {
        LOG(2, moduleName << " now running in own thread!");
        threadedModules.push_back(std::thread(&BVSMaster::threadController, this, bvsModuleMap[moduleName]));
    }
    else
    {
        LOG(2, moduleName << " controlled by BVSMaster!");
        masterModules.push_back(bvsModuleMap[moduleName]);
    }



    /*
    LOG(0, "me main");

    for (auto &it : foo)
    {
        it.join();
        LOG(0, "me joined");
    }
    LOG(0, "all joined");
    */

    return *this;
}



BVSMaster& BVSMaster::unload(const std::string& moduleName)
{
    // close lib and check for errors
    std::string modulePath = "./lib" + moduleName + ".so";
    LOG(3, moduleName << " will be closed using " << modulePath);

    // get handle from internals
    void* dlib = handleMap[moduleName];
    if (dlib==nullptr)
    {
        LOG(0, "Requested module " << moduleName << " not found!");
        exit(-1);
    }

    // close the module
    dlclose(dlib);

    // check for errors
    char* dlerr = dlerror();
    if (dlerr)
    {
        LOG(0, "While closing " << modulePath << " following error occured: " << dlerror());
        exit(-1);
    }

    // deregister module from system
    bvsModuleMap.erase(moduleName);
    LOG(2, moduleName << " unloaded and deregistered!");

    // remove handle
    handleMap.erase(moduleName);

    // TODO remove module from (master|threaded)Modules

    return *this;
}

void BVSMaster::call_from_thread(BVSModule* module)
{
    LOG(0, "me thread");
    module->execute();
    for (int i=0; i<1000; i++)
    {
        config.loadConfigFile("BVSConfig.txt");
        //LOG(0, config.getValue<std::string>("BVS.module"));
        //LOG(0, config.getValue<std::string>("BVS.module").size());
        LOG(0, (config.getValue<std::string>("BVS.module").size())/2);
        config.loadConfigFile("BVSConfig.txt");
        LOG(0, (config.getValue<std::string>("BVS.module").size())/2);
        LOG(0, "abcdefghiklmnopqrstuvwxyz");
    }
    //LOG(0, "abcdefghiklmnopqrstuvwxyz");
}


void BVSMaster::masterController()
{
    for (int i=0; i<10; i++)
    {
        controller.notify_all();
        for (auto it: masterModules)
        {
            it->execute();
        }
    }

}

void BVSMaster::threadController(BVSModule* module)
{
    (void) module;
    for (int i=0; i<10; i++)
    {
        for (auto &it: threadedModules)
        {
            (void) it;
            std::unique_lock<std::mutex> lock(threadMutex);
            std::thread::id id = it.get_id();
            LOG(0, "ROUND: " << i << " from: " << id);
            controller.wait(lock);
            //it.execute();
        }
    }

}



void BVSMaster::threadJoinAll()
{
    LOG(0, "Main: waiting for threads to join!");

    for (auto &it : threadedModules)
    {
        it.join();
        LOG(0, "Thread joined!");
    }
    LOG(0, "Main: all joined");
}
// TODO master/thread control
//
// condition variable
// BVSModuleStatus array
// number of threads waiting
//
// master: run through all modules not in own thread in specified order
//
// threads: execute, wait on condition variable
