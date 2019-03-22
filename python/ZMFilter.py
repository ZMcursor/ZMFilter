import ZMT_tree


class Filter(object):
    def __init__(self, idLen, nodeSize=0):
        if isinstance(idLen, int) and isinstance(nodeSize, int):
            self.__tree = ZMT_tree.ZMT_tree(idLen, nodeSize)
        else:
            raise TypeError

    def add(self, key):
        result = self.__tree.add(long(key))
        if result is None:
            raise ValueError
        return result

    def search(self, key):
        result = self.__tree.search(long(key))
        if result is None:
            raise ValueError
        return result

    def free(self):
        self.__tree.free()
        self.__tree = None

    def size(self):
        return self.__tree.size()

    def nodeSize(self):
        return self.__tree.nodeSize()

    def nodeCount(self):
        return self.__tree.nodeCount()

    def idLen(self):
        return self.__tree.idLen()

    def check(self):
        return self.__tree.check()
