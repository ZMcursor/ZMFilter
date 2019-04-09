# -*- coding: utf-8 -*-
import ZMFilter
import random
import time

flt = ZMFilter.Filter(4, 2)
t = time.time()
for i in range(100000):
    flt.add(random.randint(0, 1000000000))
print(time.time() - t)
s = "赵名.dat"
print(flt.size(), flt.nodeCount())
flt.dump(s)
flt = None
flt = ZMFilter.load(s)
# print(flt.size(), flt.nodeCount())
flt = None