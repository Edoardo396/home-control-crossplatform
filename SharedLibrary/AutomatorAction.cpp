#include "../SharedLibrary/stdafx.h"
#include "AutomatorAction.h"
#include <algorithm>
#include <string>
#include <boost/format.hpp>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeParser.h>
#include <Poco/DateTimeFormat.h>


AutomatorAction::AutomatorAction(const std::string & day_of_weeks, const std::string & t, const std::string & command) {

    this->weekDays = ParseWeekDays(day_of_weeks);

    auto parser = Poco::DateTimeParser();
    int roba;
    this->t = Poco::DateTimeParser::parse(Poco::DateTimeFormat::ISO8601_FORMAT, t, roba);
    this->command = command;
}

std::vector<AutomatorAction::DayOfWeek> AutomatorAction::ParseWeekDays(std::string s) {

    using std::string;

    auto vector = std::vector<DayOfWeek>();

    if (s.find("sun") != string::npos)
        vector.push_back(DayOfWeek::Sunday);
    if (s.find("mon") != string::npos)
        vector.push_back(DayOfWeek::Monday);
    if (s.find("tue") != string::npos)
        vector.push_back(DayOfWeek::Tuesday);
    if (s.find("wed") != string::npos)
        vector.push_back(DayOfWeek::Wednesday);
    if (s.find("thu") != string::npos)
        vector.push_back(DayOfWeek::Thursday);
    if (s.find("fri") != string::npos)
        vector.push_back(DayOfWeek::Friday);
    if (s.find("sat") != string::npos)
        vector.push_back(DayOfWeek::Saturday);

    return vector;
}

bool AutomatorAction::isActive() {

    using namespace Poco;
    
    auto now = LocalDateTime();

    if((now-*lastStopTime).days() >= 1) {
        lastStopTime = nullptr;
        enabled = true;
    }

    if (!enabled) return false;

    auto nowt = Timespan(0, now.hour(), now.minute(), now.second(), now.microsecond());
    auto tt = Timespan(0, t.hour(), t.minute(), t.second(), t.microsecond());

 //   std::cout << tt.hours() << " " << tt.minutes() << " " << tt.seconds();

    Timespan time = nowt - tt;

    if (t.year() != 1970 && time.days() != 0) return false;
    if (time.hours() != 0) return false;
    if (time.minutes() < 0 || time.minutes() > 15) return false;

    if (t.year() == 1970)
        return std::find(weekDays.begin(), weekDays.end(), static_cast<DayOfWeek>(now.dayOfWeek())) != weekDays.end();
    else
        return t.day() == now.day() && t.month() == now.month() && t.year() == now.year();
}

std::string AutomatorAction::DayString() {
    using std::string;
    using std::find;

    string s = "";

    if (find(weekDays.begin(), weekDays.end(), DayOfWeek::Sunday) != weekDays.end())
        s += "sun,";
    if (find(weekDays.begin(), weekDays.end(), DayOfWeek::Monday) != weekDays.end())
        s += "mon,";
    if (find(weekDays.begin(), weekDays.end(), DayOfWeek::Tuesday) != weekDays.end())
        s += "tue,";
    if (find(weekDays.begin(), weekDays.end(), DayOfWeek::Wednesday) != weekDays.end())
        s += "wed,";
    if (find(weekDays.begin(), weekDays.end(), DayOfWeek::Thursday) != weekDays.end())
        s += "thu,";
    if (find(weekDays.begin(), weekDays.end(), DayOfWeek::Friday) != weekDays.end())
        s += "fri,";
    if (find(weekDays.begin(), weekDays.end(), DayOfWeek::Saturday) != weekDays.end())
        s += "sat,";

    remove(s.end() - 2, s.end(), ',');

    return s;

}

std::string AutomatorAction::ToString() {  
    auto dt = Poco::DateTimeFormatter::format(t, "%e/%n/%Y %H:%M:%S");

    return (boost::format("%1%;%2%;%3%;%4%;%5%") % std::to_string(id) % dt % DayString() % command % (enabled ? "true" : "false")).str();
}
