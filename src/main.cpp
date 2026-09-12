#include "BinarySearchTree.hpp"
#include "Storage.hpp"
#include "Teacher.hpp"

#include <iomanip>
#include <iostream>
#include <limits>
#include <optional>
#include <string>
#include <vector>

namespace {

void clearBadInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::optional<int> readInt(const std::string& prompt) {
    std::cout << prompt;
    int value{};
    if (!(std::cin >> value)) {
        clearBadInput();
        return std::nullopt;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string value;
    std::getline(std::cin, value);
    return value;
}

Teacher readTeacher(int fixedId = -1) {
    Teacher teacher;

    if (fixedId >= 0) {
        teacher.id = fixedId;
    } else {
        while (true) {
            auto id = readInt("Teacher ID: ");
            if (id && *id > 0) {
                teacher.id = *id;
                break;
            }
            std::cout << "Please enter a positive integer ID.\n";
        }
    }

    teacher.name = readLine("Name: ");
    teacher.gender = readLine("Gender: ");

    while (true) {
        auto age = readInt("Age: ");
        if (age && *age > 0 && *age < 130) {
            teacher.age = *age;
            break;
        }
        std::cout << "Please enter a valid age (1-129).\n";
    }

    teacher.title = readLine("Academic title: ");
    return teacher;
}

void printHeader() {
    std::cout << std::left
              << std::setw(10) << "ID"
              << std::setw(24) << "Name"
              << std::setw(12) << "Gender"
              << std::setw(8) << "Age"
              << "Title\n"
              << std::string(72, '-') << '\n';
}

void printTeacher(const Teacher& teacher) {
    std::cout << std::left
              << std::setw(10) << teacher.id
              << std::setw(24) << teacher.name
              << std::setw(12) << teacher.gender
              << std::setw(8) << teacher.age
              << teacher.title << '\n';
}

void listTeachers(const BinarySearchTree& tree) {
    const std::vector<Teacher> teachers = tree.inorder();
    if (teachers.empty()) {
        std::cout << "No teacher records are loaded.\n";
        return;
    }

    printHeader();
    for (const Teacher& teacher : teachers) {
        printTeacher(teacher);
    }
    std::cout << "\nTotal records: " << tree.size() << "\n";
}

void findTeacher(const BinarySearchTree& tree) {
    auto id = readInt("Teacher ID to find: ");
    if (!id) {
        std::cout << "Invalid ID.\n";
        return;
    }

    const Teacher* teacher = tree.search(*id);
    if (!teacher) {
        std::cout << "No teacher found with ID " << *id << ".\n";
        return;
    }

    printHeader();
    printTeacher(*teacher);
}

void addTeacher(BinarySearchTree& tree) {
    Teacher teacher = readTeacher();
    if (!tree.insert(teacher)) {
        std::cout << "A teacher with ID " << teacher.id << " already exists.\n";
        return;
    }
    std::cout << "Teacher added.\n";
}

void updateTeacher(BinarySearchTree& tree) {
    auto id = readInt("Teacher ID to update: ");
    if (!id) {
        std::cout << "Invalid ID.\n";
        return;
    }
    if (!tree.search(*id)) {
        std::cout << "No teacher found with ID " << *id << ".\n";
        return;
    }

    std::cout << "Enter replacement details (the teacher ID remains unchanged).\n";
    Teacher replacement = readTeacher(*id);
    tree.update(*id, replacement);
    std::cout << "Teacher updated.\n";
}

void deleteTeacher(BinarySearchTree& tree) {
    auto id = readInt("Teacher ID to delete: ");
    if (!id) {
        std::cout << "Invalid ID.\n";
        return;
    }

    if (!tree.remove(*id)) {
        std::cout << "No teacher found with ID " << *id << ".\n";
        return;
    }
    std::cout << "Teacher deleted.\n";
}

void saveTree(const BinarySearchTree& tree, std::string& activePath) {
    std::string path = readLine("Save path [" + activePath + "]: ");
    if (path.empty()) {
        path = activePath;
    }

    std::string error;
    if (!Storage::save(tree, path, &error)) {
        std::cout << "Save failed: " << error << '\n';
        return;
    }
    activePath = path;
    std::cout << "Saved " << tree.size() << " record(s) to " << activePath << ".\n";
}

void loadTree(BinarySearchTree& tree, std::string& activePath) {
    std::string path = readLine("Load path [" + activePath + "]: ");
    if (path.empty()) {
        path = activePath;
    }

    std::string error;
    if (!Storage::load(tree, path, &error)) {
        std::cout << "Load failed: " << error << '\n';
        return;
    }
    activePath = path;
    std::cout << "Loaded " << tree.size() << " record(s) from " << activePath << ".\n";
}

void printMenu(const std::string& activePath) {
    std::cout << "\n=== BST Teacher Record Manager ===\n"
              << "Active file: " << activePath << "\n"
              << "1. List teachers (sorted by ID)\n"
              << "2. Find teacher\n"
              << "3. Add teacher\n"
              << "4. Update teacher\n"
              << "5. Delete teacher\n"
              << "6. Save\n"
              << "7. Load\n"
              << "0. Exit\n";
}

} // namespace

int main(int argc, char* argv[]) {
    BinarySearchTree tree;
    std::string activePath = argc > 1 ? argv[1] : "teachers.db";

    if (argc > 1) {
        std::string error;
        if (Storage::load(tree, activePath, &error)) {
            std::cout << "Loaded " << tree.size() << " record(s) from " << activePath << ".\n";
        } else {
            std::cout << "Could not load initial file: " << error << '\n';
        }
    }

    while (true) {
        printMenu(activePath);
        auto choice = readInt("Select an option: ");
        if (!choice) {
            std::cout << "Invalid menu choice.\n";
            continue;
        }

        switch (*choice) {
            case 1:
                listTeachers(tree);
                break;
            case 2:
                findTeacher(tree);
                break;
            case 3:
                addTeacher(tree);
                break;
            case 4:
                updateTeacher(tree);
                break;
            case 5:
                deleteTeacher(tree);
                break;
            case 6:
                saveTree(tree, activePath);
                break;
            case 7:
                loadTree(tree, activePath);
                break;
            case 0:
                std::cout << "Goodbye.\n";
                return 0;
            default:
                std::cout << "Choose a number from 0 to 7.\n";
                break;
        }
    }
}
