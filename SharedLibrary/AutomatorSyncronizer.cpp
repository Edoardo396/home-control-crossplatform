#include "../SharedLibrary/stdafx.h"
#include "AutomatorSyncronizer.h"
#include <thread>
#include "Device.h"
#include "DeviceAutomator.h"

void AutomatorSyncronizer::automator_func() {
    while (true) {
        for (auto it = devices->begin(); it != devices->end(); ++it) {
            if ((*it)->automator == nullptr) continue;
            (*it)->automator->Check();
        }
        

        for (int i = 0; i < 1; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(10));
            boost::this_thread::interruption_point();
        }
    }
}

void AutomatorSyncronizer::start() { thread = new boost::thread(&AutomatorSyncronizer::automator_func, this); }

AutomatorSyncronizer::~AutomatorSyncronizer() {
}
