import ZMFilter
import random
import time

flt = ZMFilter.Filter(4, 2)

t = time.time()
for i in range(1000000):
    flt.add(random.randint(0, 1000000000))
    if (i + 1) % 10000 == 0:
        print(flt.size(), flt.nodeCount(), flt.check())
print(time.time() - t)
