#pragma once
#include <map>

static class Support {
public:
    template <class Key, class Value>
    static Key GetKeyByValueInMap(std::map<Key, Value> map, Value val) {
        for (auto it = map.begin(); it != map.end(); ++it) {
            if (it->second == val)
                return it->first;
        }

        throw std::runtime_error(std::string("Item not found"));
    }


};

