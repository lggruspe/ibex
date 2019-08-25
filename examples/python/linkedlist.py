class ListNode:
    def __init__(self, data=None, left=None, right=None):
        self.data = data
        self.left = left
        self.right = right

    def insert_before(self, node):
        self.left.right = node
        node.left = self.left
        node.right = self
        self.left = node

    def insert_after(self, node):
        self.right.left = node
        node.right = self.right
        node.left = self
        self.right = node

    def delete(self):
        self.left.right = self.right
        self.right.left = self.left

    def __repr__(self):
        return f"<ListNode {self.data}>"

class LinkedList:
    def __init__(self, seq=None):
        self.null = ListNode()
        self.null.left = self.null
        self.null.right = self.null
        self.size = 0

        if seq:
            for item in seq:
                self.append(item)

    def __len__(self):
        return self.size

    @property
    def head(self):
        return self.null.right

    @property
    def tail(self):
        return self.null.left

    def appendleft(self, data):
        self.size += 1
        self.head.insert_before(ListNode(data))

    def append(self, data):
        self.size += 1
        self.tail.insert_after(ListNode(data))

    def delete_node(self, node):
        """Deletes node without checking if it belongs in the list."""
        node.delete()
        self.size -= 1

    def search_node(self, data):
        self.null.data = data
        node = self.head
        while node.data != data:
            node = node.right
        return node if node != self.null else None

    def __contains__(self, data):
        node = self.search_node(data)
        return node is not None

    def __iter__(self):
        def iterator():
            node = self.head
            while node != self.null:
                yield node.data
                node = node.right
        return iterator()

    def __reversed__(self):
        def iterator():
            node = self.tail
            while node != self.null:
                yield node.data
                node = node.left
        return iterator()

    def __repr__(self):
        return "[" + ", ".join([str(item) for item in self]) + "]"
