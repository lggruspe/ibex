from redblack import algorithms as rb

class Set:
    def __init__(self):
        self.root = None

    def __str__(self):
        return "[{}]".format(", ".join(str(a) for a in self))

    def __repr__(self):
        return "<Set {} object at {}>".format(str(self), id(self))

    def __iter__(self):
        return rb.iterator(self.root)

    def __reversed__(self):
        return rb.reversed_iterator(self.root)

    def add(self, key):
        node = rb.Node(key)
        self.root = rb.insert(self.root, node)

    def __contains__(self, key):
        return bool(rb.search(self.root, key))

    def clear(self):
        self.root = None

class Map:
    def __init__(self):
        self.root = None

    def __str__(self):
        return "[{}]".format(", ".join(
            "({}, {})".format(k, v) for k, v in self))

    def __repr__(self):
        return "<Map {} object at {}>".format(str(self), id(self))

    def __iter__(self):
        return rb.iterator(self.root)

    def __reversed__(self):
        return rb.reversed_iterator(self.root)

    def __setitem__(self, key, value):
        node = rb.Node(key)
        node.value = value
        self.root = rb.insert(self.root, node)

    def __getitem__(self, key):
        node = rb.search(self.root, key)
        return node.value if node else None

    def __contains__(self, key):
        return bool(rb.search(self.root, key))

    def clear(self):
        self.root = None
