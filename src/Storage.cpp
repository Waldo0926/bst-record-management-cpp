#include "Storage.hpp"

#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

namespace {
constexpr const char* kHeader = "BST_TEACHERS_V1";

void setError(std::string* error, const std::string& message) {
    if (error) {
        *error = message;
    }
}
} // namespace

namespace Storage {

bool save(const BinarySearchTree& tree, const std::string& path, std::string* error) {
    std::ofstream out(path, std::ios::trunc);
    if (!out) {
        setError(error, "Could not open file for writing: " + path);
        return false;
    }

    out << kHeader << '\n';
    for (const Teacher& teacher : tree.preorder()) {
        out << teacher.id << '\t'
            << std::quoted(teacher.name) << '\t'
            << std::quoted(teacher.gender) << '\t'
            << teacher.age << '\t'
            << std::quoted(teacher.title) << '\n';
    }

    if (!out) {
        setError(error, "Failed while writing file: " + path);
        return false;
    }
    return true;
}

bool load(BinarySearchTree& tree, const std::string& path, std::string* error) {
    std::ifstream in(path);
    if (!in) {
        setError(error, "Could not open file for reading: " + path);
        return false;
    }

    std::string header;
    if (!std::getline(in, header) || header != kHeader) {
        setError(error, "Unsupported or invalid file format: " + path);
        return false;
    }

    BinarySearchTree loaded;
    std::string line;
    std::size_t lineNumber = 1;

    while (std::getline(in, line)) {
        ++lineNumber;
        if (line.empty()) {
            continue;
        }

        std::istringstream row(line);
        Teacher teacher;
        if (!(row >> teacher.id >> std::quoted(teacher.name)
                  >> std::quoted(teacher.gender) >> teacher.age
                  >> std::quoted(teacher.title))) {
            setError(error, "Malformed record at line " + std::to_string(lineNumber));
            return false;
        }

        std::string trailing;
        if (row >> trailing) {
            setError(error, "Unexpected trailing data at line " + std::to_string(lineNumber));
            return false;
        }

        if (!loaded.insert(teacher)) {
            setError(error, "Duplicate teacher ID at line " + std::to_string(lineNumber));
            return false;
        }
    }

    tree = std::move(loaded);
    return true;
}

} // namespace Storage
