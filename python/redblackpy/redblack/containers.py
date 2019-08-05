from redblack import algorithms as rb

class Set:
    def __init__(self):
        self.root = None
        self._size = 0

    def __str__(self):
        return "[{}]".format(", ".join(str(a) for a in self))

    def __repr__(self):
        return "<Set {} object at {}>".format(str(self), id(self))

    def __iter__(self):
        return rb.iterator(self.root)

    def __reversed__(self):
        return rb.reversed_iterator(self.root)

    def __len__(self):
        return self._size

    def add(self, key):
        node = rb.Node(key)
        self.root, grew = rb.insert(self.root, node)
        if grew:
            self._size += 1

    def __contains__(self, key):
        return bool(rb.search(self.root, key))

    def clear(self):
        self.root = None
        self._size = 0

class Map:
    def __init__(self):
        self.root = None
        self._size = 0

    def __str__(self):
        return "[{}]".format(", ".join(
            "({}, {})".format(k, v) for k, v in self))

    def __repr__(self):
        return "<Map {} object at {}>".format(str(self), id(self))

    def __iter__(self):
        return rb.iterator(self.root)

    def __reversed__(self):
        return rb.reversed_iterator(self.root)

    def __len__(self):
        return self._size

    def __setitem__(self, key, value):
        node = rb.Node(key)
        node.value = value
        self.root, grew = rb.insert(self.root, node)
        if grew:
            self._size += 1

    def __getitem__(self, key):
        node = rb.search(self.root, key)
        if node:
            return node.value
        raise KeyError(key)

    def __contains__(self, key):
        return bool(rb.search(self.root, key))

    def clear(self):
        self.root = None
        self._size = 0

    def get(self, key, val):
        try:
            return self[key]
        except KeyError:
            return val
