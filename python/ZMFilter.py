import ZMT_tree


class Filter(object):
    def __init__(self, idLen, nodeSize=0, filename=None):
        assert isinstance(idLen, int) and isinstance(
            nodeSize, int) and (filename is None or isinstance(filename, str))
        self.__tree = ZMT_tree.ZMT_tree(idLen, nodeSize, filename)

    def add(self, key):
        result = self.__tree.add(key)
        # result = self.__tree.add(long(key))
        if result is None:
            raise ValueError
        return result

    def dump(self, filename):
        assert isinstance(filename, str)
        return self.__tree.dump(filename)

    def search(self, key):
        result = self.__tree.search(key)
        # result = self.__tree.search(long(key))
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

    def version(self):
        return self.__tree.version()

    def keyLen(self):
        return self.__tree.idLen()

    def check(self):
        return self.__tree.check()


def load(filename):
    return Filter(0, 0, filename)
