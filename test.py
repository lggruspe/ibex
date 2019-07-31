from redblack.algorithms import Color, minimum, successor, Node
from redblack.collections import Set, Map
import math
import random
import unittest

def height(node: Node) -> int:
    if not node:
        return 0
    return 1 + max(height(node.left), height(node.right))

def weight(node: Node) -> int:
    if not node:
        return 0
    return 1 + weight(node.left) + weight(node.right)

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

class RedBlackTestCase(unittest.TestCase):
    def setUp(self):
        self.set = Set()
        for i in range(100):
            self.set.insert(i)

    def tearDown(self):
        self.set.clear()

    def test_is_red_black(self):
        self.assertEqual(self.set.root.color, Color.BLACK)
        self.assertNotEqual(black_height(self.set.root), -1)

        node = minimum(self.set.root)
        while node:
            if node.color == Color.RED:
                self.assertTrue(node.parent and node.parent.color ==\
                        Color.BLACK)
            node = successor(node)

    def test_is_balanced(self):
        n = weight(self.set.root)
        h = height(self.set.root)
        self.assertEqual(n, 100)
        self.assertLessEqual(h, 2*math.log2(1 + n))

    def test_is_inserted(self):
        self.assertEqual(list(range(100)), list(self.set))

if __name__ == "__main__":
    unittest.main()
