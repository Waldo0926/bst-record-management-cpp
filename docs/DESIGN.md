# Design

## BST invariant

For every node with key `k`:

- every key in the left subtree is `< k`;
- every key in the right subtree is `> k`;
- duplicate teacher IDs are rejected.

The teacher ID is therefore not modified in place. Changing a key without repositioning the
node would violate the BST invariant.

## Deletion

Deletion has three structural cases:

1. **Leaf** — remove the node directly.
2. **One child** — replace the owning pointer with the node's only child.
3. **Two children** — copy the in-order successor (minimum node in the right subtree) into
   the target node and recursively remove the successor.

## Persistence

The tree is serialized using **pre-order traversal**:

```text
root -> left subtree -> right subtree
```

When records with unique keys are inserted again in the same pre-order sequence, the same
BST shape is reconstructed. This preserves the core persistence idea used in the original
coursework while replacing raw binary struct dumps with a portable text format.

File format:

```text
BST_TEACHERS_V1
<id> "<name>" "<gender>" <age> "<title>"
...
```

Quoted strings allow spaces in names and titles.

## Complexity

Let `h` be the height of the tree and `n` the number of records.

| Operation | Time | Extra space |
| --- | --- | --- |
| Search | `O(h)` | `O(1)` iterative search |
| Insert | `O(h)` | `O(h)` recursion stack |
| Delete | `O(h)` | `O(h)` recursion stack |
| In-order traversal | `O(n)` | `O(h)` recursion stack |
| Save / load | `O(n)` average traversal + insertion cost | `O(n)` output/input records |

A plain BST can degrade to `h = n` for already sorted insertion sequences. A balanced tree
(AVL / Red-Black Tree) would provide stronger worst-case guarantees, but this project keeps
the plain BST because the purpose is to demonstrate the original data-structures exercise.
