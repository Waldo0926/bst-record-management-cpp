#pragma once

#include "BinarySearchTree.hpp"

#include <string>

namespace Storage {

// Saves records in BST pre-order. Loading and inserting the same sequence
// reconstructs the same BST shape (assuming unique keys).
bool save(const BinarySearchTree& tree, const std::string& path, std::string* error = nullptr);

// Loads a complete file into a temporary tree first; the destination tree is
// replaced only if the entire file is valid.
bool load(BinarySearchTree& tree, const std::string& path, std::string* error = nullptr);

} // namespace Storage
