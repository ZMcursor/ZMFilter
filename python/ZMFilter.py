import ZMT_tree
import threading

try:
    # Python2
    _num = long
except Exception as e:
    _num = int


class Filter(object):
    def __init__(self, idLen, nodeSize=0, filename=None):
        assert isinstance(idLen, int) and isinstance(
            nodeSize, int) and (filename is None or isinstance(filename, str))
        self.__tree = ZMT_tree.ZMT_tree(idLen, nodeSize, filename)
        self.__mutex = threading.Lock()

    def add(self, key):
        with self.__mutex:
            result = self.__tree.add(_num(key))
        if result is None:
            raise ValueError
        return result

    def dump(self, filename):
        assert isinstance(filename, str)
        with self.__mutex:
            return self.__tree.dump(filename)

    def search(self, key):
        with self.__mutex:
            result = self.__tree.search(_num(key))
        if result is None:
            raise ValueError
        return result

    def free(self):
        with self.__mutex:
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
