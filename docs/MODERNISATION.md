# Modernisation Notes

This repository is a modernised version of an earlier Data Structures coursework project.
The original program implemented a teacher-information management system backed by a
binary search tree (BST).

## What was retained

The modern version intentionally keeps the main ideas of the coursework:

- teacher ID as the BST key;
- recursive BST insertion;
- lookup by teacher ID;
- in-order traversal for sorted output;
- insertion, update, and deletion operations;
- pre-order persistence so the tree can be reconstructed with the same shape.

## What was changed

### 1. Ownership and memory safety

The original implementation allocated tree nodes with `malloc()` and released them with
`free()`. The modern implementation uses `std::unique_ptr`, so ownership is explicit and
nodes are released automatically through RAII.

### 2. Safe input handling

The legacy implementation used `gets()`, which was removed from modern C/C++ because it
cannot prevent buffer overflow. The modern CLI uses `std::string`, `std::getline()`, and
validated integer input.

### 3. Correct BST deletion

The original dispatch logic checked the one-child case using:

```cpp
q->lchild != NULL || q->rchild != NULL
```

A node with two children also satisfies that condition, so the two-child branch could not
be reached. The modern implementation treats leaf, one-child, and two-child cases as
mutually exclusive. For two children, it replaces the node with its in-order successor and
then deletes that successor.

### 4. Separation of concerns

The original coursework placed the data model, BST algorithms, persistence, and CLI in one
source file. The modern version separates them into:

- `Teacher.hpp` — record model;
- `BinarySearchTree.*` — tree algorithms;
- `Storage.*` — persistence;
- `main.cpp` — command-line interface.

### 5. Portable persistence

The coursework wrote raw structs directly to a binary file. That representation can depend
on compiler-specific padding and fixed-size character arrays. The modern implementation
uses a versioned, human-readable text format with quoted strings.

### 6. Automated tests

The project now includes tests for:

- insertion and duplicate rejection;
- search;
- sorted in-order traversal;
- updates;
- leaf deletion;
- one-child deletion;
- two-child deletion;
- root deletion;
- save/load round-trips;
- malformed-file handling.
