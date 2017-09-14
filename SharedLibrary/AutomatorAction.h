#pragma once
#include <vector>
#include <Poco/DateTime.h>
#include <memory>

class AutomatorAction {
    friend class DeviceAutomator;
    enum class DayOfWeek {Sunday = 0, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday};

    int id;
    std::vector<DayOfWeek> weekDays;
    Poco::DateTime t;
    std::unique_ptr<Poco::DateTime> lastStopTime = nullptr;
    bool enabled = true;


public:
    std::string command;
    AutomatorAction(const std::vector<DayOfWeek>& day_of_weeks, const Poco::DateTime& t, const std::string& command,
        int id)
        : id(id),
          weekDays(day_of_weeks),
          t(t),
          command(command) {
    }

    AutomatorAction(const std::string& day_of_weeks, const std::string& t, const std::string& command);

private:



public:
    static std::vector<DayOfWeek> ParseWeekDays(std::string);
    bool isActive();
    std::string DayString();
    std::string ToString();
};


