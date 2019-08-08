"""Tests for redblack."""
import math
import unittest
from redblack.algorithms import Color, minimum, successor, Node
from redblack.containers import Set, Map

def height(node: Node) -> int:
    """Return max shortest distance between node and a leaf."""
    if not node:
        return 0
    return 1 + max(height(node.left), height(node.right))

def black_height(node: Node) -> int:
    """Return black-height of node if well-defined, 1 otherwise."""
    if not node:
        return 0
    left = 1 if not node.left else black_height(node.left)
    if left == -1:
        return -1
    right = 1 if not node.right else black_height(node.right)
    if right == -1:
        return -1
    if left != right:
        return -1
    return left if node.color == Color.RED else left + 1

def red_child_black_parent(root):
    """Check if the tree satisfies the red-child black-parent property."""
    node = minimum(root)
    while node:
        if node.color == Color.RED:
            if not node.parent or node.parent.color == Color.RED:
                return False
        node = successor(node)
    return True

def is_balanced(container):
    """Check if the height of container.root is O(lg(n))."""
    return height(container.root) <= 2*math.log2(1 + len(container))

class RedBlackSetTestCase(unittest.TestCase):
    """Test redblack.containers.Set."""

    def setUp(self):
        self.set = Set()
        for i in range(100):
            self.set.add(i)

    def tearDown(self):
        self.set.clear()

    def test_is_red_black(self):
        """Test if red-black properties are satisfied."""
        self.assertEqual(self.set.root.color, Color.BLACK)
        self.assertNotEqual(black_height(self.set.root), -1)
        self.assertTrue(red_child_black_parent(self.set.root))

    def test_is_balanced(self):
        """Test if tree is balanced in the worst case."""
        self.assertEqual(len(self.set), 100)
        self.assertTrue(is_balanced(self.set))

    def test_is_inserted(self):
        """Test if all items have been inserted."""
        self.assertEqual(list(range(100)), list(self.set))

    def test_has_no_duplicates(self):
        """Test if no duplicates are inserted into the set."""
        self.set.clear()
        for _ in range(10):
            self.set.add(0)
        self.assertEqual(len(self.set), 1)
        self.assertTrue(0 in self.set)

class RedBlackMapTestCase(unittest.TestCase):
    """Test redblack.containers.Map."""

    def setUp(self):
        self.map = Map()

    def tearDown(self):
        self.map.clear()

    def populate_0_to_100(self):
        """Populate map with worst-case inputs."""
        for i in range(100):
            self.map[i] = i**2

    def test_is_red_black(self):
        """Test if red-black properties are satisfied."""
        self.populate_0_to_100()
        self.assertEqual(self.map.root.color, Color.BLACK)
        self.assertNotEqual(black_height(self.map.root), -1)
        self.assertTrue(red_child_black_parent(self.map.root))

    def test_is_balanced(self):
        """Test if tree is balanced in the worst case."""
        self.populate_0_to_100()
        self.assertEqual(len(self.map), 100)
        self.assertTrue(is_balanced(self.map))

    def test_is_inserted(self):
        """Test if all items have been inserted."""
        self.populate_0_to_100()
        self.assertEqual(dict(self.map), {i:i**2 for i in range(100)})

    def test_has_no_duplicates(self):
        """Test if no duplicates are inserted into the map."""
        for i in range(10):
            self.map[0] = i
        self.assertEqual(len(self.map), 1)
        self.assertTrue(0 in self.map and self.map[0] == 9)

if __name__ == "__main__":
    unittest.main()
