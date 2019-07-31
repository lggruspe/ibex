class Color:
    BLACK = "BLACK"
    RED = "RED"

class Node:
    def __init__(self, key, color=Color.RED):
        self.key = key
        self.color = color
        self.parent = None
        self.left = None
        self.right = None
        self.value = None

    def __repr__(self):
        return "<Node {} {} {} {} {} object at {}>".format(
                    self.key,
                    self.color,
                    self.parent,
                    self.left,
                    self.right,
                    id(self)
                )

def rotate_left(root, x):
    assert root and x and x.right
    y = x.right

    x.right = y.left
    if x.right:
        x.right.parent = x

    y.left = x
    y.parent = x.parent
    x.parent = y
    if y.parent:
        if y.parent.left == x:
            y.parent.left = y
        elif y.parent.right == x:
            y.parent.right = y
    else:
        root = y
    return root

def rotate_right(root, y):
    assert root and y and y.left
    x = y.left

    y.left = x.right
    if y.left:
        y.left.parent = y

    x.right = y
    x.parent = y.parent
    y.parent = x
    if x.parent:
        if x.parent.right == y:
            x.parent.right = x
        elif x.parent.left == y:
            x.parent.left = x
    else:
        root = x
    return root

def repair(root, node):
    if not node:
        return root
    if not root:
        node.color = Color.BLACK
        return node
    if not node.parent or node.parent.color == Color.BLACK:
        return root

    parent = node.parent
    grandparent = parent.parent if parent else None
    uncle = None
    if grandparent:
        if grandparent.left == parent:
            uncle = grandparent.right
        else:
            uncle = grandparent.left

        if not uncle:
            if grandparent.left == parent and parent.right == node:
                root = rotate_left(root, parent)
            elif grandparent.right == parent and parent.left == node:
                root = rotate_right(root, parent)

            if grandparent.left == parent and parent.left == node:
                parent.color = Color.BLACK
                grandparent.color = Color.RED
                root = rotate_right(root, grandparent)
            elif grandparent.right == parent and parent.right == node:
                parent.color = Color.BLACK
                grandparent.color = Color.RED
                root = rotate_left(root, grandparent)
            root.color = Color.BLACK
            return root
        elif uncle.color == Color.RED:
            grandparent.color = Color.RED
            parent.color = Color.BLACK
            uncle.color = Color.BLACK
            return repair(root, grandparent)

    root.color = Color.BLACK
    return root

# assume tree rooted at tree is well-formed
def insert(root, node):
    if not node:
        return root

    node.left = None
    node.right = None
    node.parent = None
    if not root:
        node.color = Color.BLACK
        return node

    node.color = Color.RED
    closest = closest_match(root, node.key)
    if closest.key == node.key:
        # replace closest with node
        node.color = closest.color
        node.left = closest.left
        node.right = closest.right
        node.parent = closest.parent
        if node.left:
            node.left.parent = node
        if node.right:
            node.right.parent = node
        if not node.parent:
            root = node
        elif node.parent.left == closest:
            node.parent.left = node
        else:
            node.parent.right = node
    else:
        node.parent = closest
        if node.key < closest.key:
            closest.left = node
        else:
            closest.right = node
    return repair(root, node)

def closest_match(root, key):
    parent = None
    child = root
    while child and child.key != key:
        parent = child
        child = child.left if key < child.key else child.right
    return child if child else parent


def search(root, key):
    node = root
    while node and node.key != key:
        node = node.left if key < node.key else node.right
    return node

def minimum(node):
    if not node:
        return None
    while node.left:
        node = node.left
    return node

def maximum(node):
    if not node:
        return None
    while node.right:
        node = node.right
    return node

def predecessor(node):
    if not node:
        return None
    if node.left:
        return maximum(node.left)
    child = node
    parent = child.parent
    while parent and parent.right != node:
        child = parent
        parent = parent.parent
    return parent

def successor(node):
    if not node:
        return None
    if node.right:
        return minimum(node.right)
    child = node
    parent = child.parent
    while parent and parent.left != child:
        child = parent
        parent = parent.parent
    return parent

def predecessor(node):
    if not node:
        return None
    if node.left:
        return maximum(node.left)
    child = node
    parent = child.parent
    while parent and parent.right != child:
        child = parent
        parent = parent.parent
    return parent

def iterator(root):
    node = minimum(root)
    while node:
        if node.value is not None:
            yield (node.key, node.value)
        else:
            yield node.key
        node = successor(node)
