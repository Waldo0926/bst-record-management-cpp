#pragma once

#include "Teacher.hpp"

#include <cstddef>
#include <memory>
#include <vector>

class BinarySearchTree {
public:
    BinarySearchTree() = default;
    BinarySearchTree(const BinarySearchTree&) = delete;
    BinarySearchTree& operator=(const BinarySearchTree&) = delete;
    BinarySearchTree(BinarySearchTree&&) noexcept = default;
    BinarySearchTree& operator=(BinarySearchTree&&) noexcept = default;
    ~BinarySearchTree() = default;

    bool insert(const Teacher& teacher);
    Teacher* search(int id);
    const Teacher* search(int id) const;
    bool update(int id, const Teacher& replacement);
    bool remove(int id);

    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] std::size_t size() const noexcept;
    void clear() noexcept;

    [[nodiscard]] std::vector<Teacher> inorder() const;
    [[nodiscard]] std::vector<Teacher> preorder() const;

private:
    struct Node {
        explicit Node(const Teacher& value) : data(value) {}

        Teacher data;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
    };

    std::unique_ptr<Node> root_;
    std::size_t size_{0};

    static bool insert(std::unique_ptr<Node>& node, const Teacher& teacher);
    static Node* search(Node* node, int id);
    static const Node* search(const Node* node, int id);
    static bool remove(std::unique_ptr<Node>& node, int id);
    static void inorder(const Node* node, std::vector<Teacher>& result);
    static void preorder(const Node* node, std::vector<Teacher>& result);
};
