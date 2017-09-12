#pragma once
#include <boost/thread.hpp>

class AutomatorSyncronizer {

    boost::thread* thread = nullptr;
    std::list<class Device*>* devices;

    void automator_func();

public:
    AutomatorSyncronizer(std::list<Device*>* devices) { this->devices = devices; }

    void start();

    ~AutomatorSyncronizer();
};
