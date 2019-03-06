import ZMT_tree
import random
import time

tree = ZMT_tree.ZMT_tree(4)

t = time.time()
for i in xrange(10000000):
    tree.add(long(random.randint(1, 100000000)))
    if i % 100000 == 0:
        print tree.size(), tree.nodeSize(), (tree.size()*4) / \
            (tree.nodeSize()*4096.0), tree.check(), tree.checkBalance()
print time.time()-t
