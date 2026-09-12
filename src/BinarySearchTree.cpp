#include "BinarySearchTree.hpp"

#include <utility>

bool BinarySearchTree::insert(const Teacher& teacher) {
    if (!insert(root_, teacher)) {
        return false;
    }
    ++size_;
    return true;
}

bool BinarySearchTree::insert(std::unique_ptr<Node>& node, const Teacher& teacher) {
    if (!node) {
        node = std::make_unique<Node>(teacher);
        return true;
    }

    if (teacher.id < node->data.id) {
        return insert(node->left, teacher);
    }
    if (teacher.id > node->data.id) {
        return insert(node->right, teacher);
    }

    return false; // Duplicate key.
}

Teacher* BinarySearchTree::search(int id) {
    Node* node = search(root_.get(), id);
    return node ? &node->data : nullptr;
}

const Teacher* BinarySearchTree::search(int id) const {
    const Node* node = search(root_.get(), id);
    return node ? &node->data : nullptr;
}

BinarySearchTree::Node* BinarySearchTree::search(Node* node, int id) {
    while (node) {
        if (id == node->data.id) {
            return node;
        }
        node = id < node->data.id ? node->left.get() : node->right.get();
    }
    return nullptr;
}

const BinarySearchTree::Node* BinarySearchTree::search(const Node* node, int id) {
    while (node) {
        if (id == node->data.id) {
            return node;
        }
        node = id < node->data.id ? node->left.get() : node->right.get();
    }
    return nullptr;
}

bool BinarySearchTree::update(int id, const Teacher& replacement) {
    Teacher* current = search(id);
    if (!current) {
        return false;
    }

    // The BST key is intentionally immutable during an update. Changing it in
    // place would break the BST ordering invariant.
    current->name = replacement.name;
    current->gender = replacement.gender;
    current->age = replacement.age;
    current->title = replacement.title;
    return true;
}

bool BinarySearchTree::remove(int id) {
    if (!remove(root_, id)) {
        return false;
    }
    --size_;
    return true;
}

bool BinarySearchTree::remove(std::unique_ptr<Node>& node, int id) {
    if (!node) {
        return false;
    }

    if (id < node->data.id) {
        return remove(node->left, id);
    }
    if (id > node->data.id) {
        return remove(node->right, id);
    }

    // Case 1: leaf node.
    if (!node->left && !node->right) {
        node.reset();
        return true;
    }

    // Case 2: exactly one child.
    if (!node->left) {
        node = std::move(node->right);
        return true;
    }
    if (!node->right) {
        node = std::move(node->left);
        return true;
    }

    // Case 3: two children. Replace the current value with its in-order
    // successor (smallest key in the right subtree), then remove the successor.
    Node* successor = node->right.get();
    while (successor->left) {
        successor = successor->left.get();
    }

    node->data = successor->data;
    return remove(node->right, successor->data.id);
}

bool BinarySearchTree::empty() const noexcept {
    return size_ == 0;
}

std::size_t BinarySearchTree::size() const noexcept {
    return size_;
}

void BinarySearchTree::clear() noexcept {
    root_.reset();
    size_ = 0;
}

std::vector<Teacher> BinarySearchTree::inorder() const {
    std::vector<Teacher> result;
    result.reserve(size_);
    inorder(root_.get(), result);
    return result;
}

void BinarySearchTree::inorder(const Node* node, std::vector<Teacher>& result) {
    if (!node) {
        return;
    }
    inorder(node->left.get(), result);
    result.push_back(node->data);
    inorder(node->right.get(), result);
}

std::vector<Teacher> BinarySearchTree::preorder() const {
    std::vector<Teacher> result;
    result.reserve(size_);
    preorder(root_.get(), result);
    return result;
}

void BinarySearchTree::preorder(const Node* node, std::vector<Teacher>& result) {
    if (!node) {
        return;
    }
    result.push_back(node->data);
    preorder(node->left.get(), result);
    preorder(node->right.get(), result);
}
