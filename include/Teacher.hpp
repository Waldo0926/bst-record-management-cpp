#pragma once

#include <string>

struct Teacher {
    int id{};
    std::string name;
    std::string gender;
    int age{};
    std::string title;

    bool operator==(const Teacher& other) const {
        return id == other.id &&
               name == other.name &&
               gender == other.gender &&
               age == other.age &&
               title == other.title;
    }
};
