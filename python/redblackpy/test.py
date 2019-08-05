from redblack.algorithms import Color, minimum, successor, Node
from redblack.containers import Set, Map
import math
import random
import unittest

def height(node: Node) -> int:
    if not node:
        return 0
    return 1 + max(height(node.left), height(node.right))

def black_height(node: Node) -> int:
    """Black-height of node.

    Returns -1 if node is not a red-black tree.
    """
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

    height = left
    if node.color == Color.BLACK:
        height += 1
    return height

def red_child_black_parent(root):
    node = minimum(root)
    while node:
        if node.color == Color.RED:
            if not node.parent or node.parent.color == Color.RED:
                return False
        node = successor(node)
    return True

def is_balanced(container):
    n = len(container)
    h = height(container.root)
    return h <= 2*math.log2(1 + n)

class RedBlackSetTestCase(unittest.TestCase):
    def setUp(self):
        self.set = Set()
        for i in range(100):
            self.set.add(i)

    def tearDown(self):
        self.set.clear()

    def test_is_red_black(self):
        self.assertEqual(self.set.root.color, Color.BLACK)
        self.assertNotEqual(black_height(self.set.root), -1)
        self.assertTrue(red_child_black_parent(self.set.root))

    def test_is_balanced(self):
        n = len(self.set)
        self.assertEqual(n, 100)
        self.assertTrue(is_balanced(self.set))

    def test_is_inserted(self):
        self.assertEqual(list(range(100)), list(self.set))

    def test_has_no_duplicates(self):
        self.set.clear()
        for i in range(10):
            self.set.add(0)
        self.assertEqual(len(self.set), 1)
        self.assertTrue(0 in self.set)

class RedBlackMapTestCase(unittest.TestCase):
    def setUp(self):
        self.map = Map()

    def tearDown(self):
        self.map.clear()

    def populate_0_to_100(self):
        for i in range(100):
            self.map[i] = i**2

    def test_is_red_black(self):
        self.populate_0_to_100()
        self.assertEqual(self.map.root.color, Color.BLACK)
        self.assertNotEqual(black_height(self.map.root), -1)
        self.assertTrue(red_child_black_parent(self.map.root))

    def test_is_balanced(self):
        self.populate_0_to_100()
        n = len(self.map)
        self.assertEqual(n, 100)
        self.assertTrue(is_balanced(self.map))

    def test_is_inserted(self):
        self.populate_0_to_100()
        self.assertEqual(dict(self.map), { i:i**2 for i in range(100) })

    def test_has_no_duplicates(self):
        for i in range(10):
            self.map[0] = i
        self.assertEqual(len(self.map), 1)
        self.assertTrue(0 in self.map and self.map[0] == 9)

if __name__ == "__main__":
    unittest.main()
