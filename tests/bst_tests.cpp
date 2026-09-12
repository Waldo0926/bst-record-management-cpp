#include "BinarySearchTree.hpp"
#include "Storage.hpp"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

void require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

Teacher makeTeacher(int id, const std::string& name = "Teacher") {
    return Teacher{id, name, "N/A", 40, "Lecturer"};
}

void requireIds(const std::vector<Teacher>& values,
                const std::vector<int>& expected,
                const std::string& context) {
    require(values.size() == expected.size(), context + ": size mismatch");
    for (std::size_t i = 0; i < values.size(); ++i) {
        require(values[i].id == expected[i], context + ": unexpected ID at index " + std::to_string(i));
    }
}

BinarySearchTree standardTree() {
    BinarySearchTree tree;
    require(tree.insert(makeTeacher(50, "A")), "insert 50");
    require(tree.insert(makeTeacher(30, "B")), "insert 30");
    require(tree.insert(makeTeacher(70, "C")), "insert 70");
    require(tree.insert(makeTeacher(20, "D")), "insert 20");
    require(tree.insert(makeTeacher(40, "E")), "insert 40");
    require(tree.insert(makeTeacher(60, "F")), "insert 60");
    require(tree.insert(makeTeacher(80, "G")), "insert 80");
    return tree;
}

void testInsertSearchAndTraversal() {
    BinarySearchTree tree = standardTree();
    require(tree.size() == 7, "tree size after insertion");
    require(tree.search(40) != nullptr, "search existing key");
    require(tree.search(999) == nullptr, "search missing key");
    require(!tree.insert(makeTeacher(40, "Duplicate")), "duplicate key must be rejected");
    require(tree.size() == 7, "duplicate must not change size");
    requireIds(tree.inorder(), {20, 30, 40, 50, 60, 70, 80}, "in-order traversal");
    requireIds(tree.preorder(), {50, 30, 20, 40, 70, 60, 80}, "pre-order traversal");
}

void testUpdateKeepsKeyStable() {
    BinarySearchTree tree = standardTree();
    Teacher replacement{999, "Updated Name", "X", 55, "Professor"};
    require(tree.update(30, replacement), "update existing record");

    const Teacher* updated = tree.search(30);
    require(updated != nullptr, "updated record remains at original key");
    require(updated->id == 30, "update must keep key stable");
    require(updated->name == "Updated Name", "update name");
    require(updated->age == 55, "update age");
    require(tree.search(999) == nullptr, "replacement ID must not become a key");
}

void testDeleteLeaf() {
    BinarySearchTree tree = standardTree();
    require(tree.remove(20), "delete leaf");
    require(tree.search(20) == nullptr, "leaf removed");
    require(tree.size() == 6, "size after leaf deletion");
    requireIds(tree.inorder(), {30, 40, 50, 60, 70, 80}, "leaf deletion order");
}

void testDeleteOneChild() {
    BinarySearchTree tree;
    require(tree.insert(makeTeacher(50)), "insert 50");
    require(tree.insert(makeTeacher(30)), "insert 30");
    require(tree.insert(makeTeacher(20)), "insert 20");
    require(tree.remove(30), "delete one-child node");
    requireIds(tree.inorder(), {20, 50}, "one-child deletion order");
    require(tree.size() == 2, "size after one-child deletion");
}

void testDeleteTwoChildren() {
    BinarySearchTree tree = standardTree();
    require(tree.remove(70), "delete two-child node");
    require(tree.search(70) == nullptr, "two-child node removed");
    require(tree.size() == 6, "size after two-child deletion");
    requireIds(tree.inorder(), {20, 30, 40, 50, 60, 80}, "two-child deletion order");
}

void testDeleteRootWithTwoChildren() {
    BinarySearchTree tree = standardTree();
    require(tree.remove(50), "delete root with two children");
    require(tree.search(50) == nullptr, "root key removed");
    require(tree.size() == 6, "size after root deletion");
    requireIds(tree.inorder(), {20, 30, 40, 60, 70, 80}, "root deletion order");
}

void testRemoveMissingKey() {
    BinarySearchTree tree = standardTree();
    require(!tree.remove(999), "missing key removal must return false");
    require(tree.size() == 7, "missing key removal must not change size");
}

void testPersistencePreservesPreorder() {
    const std::string path = "bst_test_data.tmp";
    BinarySearchTree original = standardTree();
    std::string error;

    require(Storage::save(original, path, &error), "save failed: " + error);

    BinarySearchTree loaded;
    require(Storage::load(loaded, path, &error), "load failed: " + error);
    require(original.preorder() == loaded.preorder(), "pre-order must survive round-trip");
    require(original.inorder() == loaded.inorder(), "in-order must survive round-trip");

    std::remove(path.c_str());
}

void testMalformedLoadDoesNotDestroyExistingTree() {
    const std::string path = "bst_bad_data.tmp";
    {
        std::ofstream out(path);
        require(static_cast<bool>(out), "create malformed test file");
        out << "BST_TEACHERS_V1\n"
            << "10 \"Alice\" \"F\" not-an-age \"Professor\"\n";
    }

    BinarySearchTree tree;
    require(tree.insert(makeTeacher(42, "Existing")), "insert existing record");
    std::string error;
    require(!Storage::load(tree, path, &error), "malformed file must be rejected");
    require(!error.empty(), "malformed load should report an error");
    require(tree.size() == 1, "failed load must preserve destination tree");
    require(tree.search(42) != nullptr, "existing record must remain after failed load");

    std::remove(path.c_str());
}

void testDuplicateIdsInFileAreRejected() {
    const std::string path = "bst_duplicate_data.tmp";
    {
        std::ofstream out(path);
        require(static_cast<bool>(out), "create duplicate test file");
        out << "BST_TEACHERS_V1\n"
            << "10 \"Alice\" \"F\" 40 \"Professor\"\n"
            << "10 \"Bob\" \"M\" 35 \"Lecturer\"\n";
    }

    BinarySearchTree tree;
    std::string error;
    require(!Storage::load(tree, path, &error), "duplicate IDs in file must be rejected");
    require(tree.empty(), "failed duplicate load must not partially populate destination");

    std::remove(path.c_str());
}

} // namespace

int main() {
    try {
        testInsertSearchAndTraversal();
        testUpdateKeepsKeyStable();
        testDeleteLeaf();
        testDeleteOneChild();
        testDeleteTwoChildren();
        testDeleteRootWithTwoChildren();
        testRemoveMissingKey();
        testPersistencePreservesPreorder();
        testMalformedLoadDoesNotDestroyExistingTree();
        testDuplicateIdsInFileAreRejected();
    } catch (const std::exception& ex) {
        std::cerr << "TEST FAILURE: " << ex.what() << '\n';
        return 1;
    }

    std::cout << "All BST tests passed.\n";
    return 0;
}
