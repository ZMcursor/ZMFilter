# -*- coding: utf-8 -*-
import ZMFilter
import random
import time


def test(num):
    flt = ZMFilter.Filter(8)
    print("testing add")
    t = time.time()
    for i in range(num):
        flt.add(random.randint(0, 2**32))
    t = time.time() - t
    print("size:%d,node count:%d\ntime consuming:%dms" % (flt.size(),
                                                          flt.nodeCount(),
                                                          int(1000 * t)))
    print("count per millisecond:%d" % (flt.size() / int(1000 * t)))

    print("searching")
    t = time.time()
    for i in range(num):
        flt.search(random.randint(0, 2**32))
    t = time.time() - t
    print("time consuming:%dms" % int(1000 * t))
    print("count per millisecond:%d" % (num / int(1000 * t)))
    print("finish testing")
    flt.free()


def testDump():
    flt = ZMFilter.Filter(8)
    print("testing dump")
    for i in range(10000000):
        flt.add(i)
    print("size:%d,node count:%d" % (flt.size(), flt.nodeCount()))
    f = "赵名.dat"
    if flt.dump(f):
        flt.free()
        flt = ZMFilter.load(f)
        print("size:%d,node count:%d" % (flt.size(), flt.nodeCount()))
    flt.free()


if __name__ == "__main__":
    test(1000000)
    # testDump()
