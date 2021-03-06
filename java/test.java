import java.util.Random;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.CyclicBarrier;

class test {

    private static long t;

    public static void main(String[] args) throws Throwable {
        // 测试key写入读取速度
        testAdd(10000000);
        // 多线程测试
        // testMultiThread(10000002, 3);
        // 测试 Native代码 平衡逻辑正确性
        // testBalance();
        // 测试序列化和反序列化功能
        // testDump();
    }

    // 让多线程同时开始任务
    private static CyclicBarrier cyclicBarrier;

    public static void testMultiThread(int num, int threadCount) throws Throwable {
        ZMFilter tree = new ZMFilter(8);
        System.out.println("testing add thread:" + threadCount);

        cyclicBarrier = new CyclicBarrier(threadCount, () -> t = System.currentTimeMillis());

        final CountDownLatch cdlAdd = new CountDownLatch(threadCount);
        final CountDownLatch cdlSearch = new CountDownLatch(threadCount);

        for (int i = 0; i < threadCount; i++) {
            new Thread(() -> {
                try {
                    cyclicBarrier.await();
                    cyclicBarrier.reset();
                    add(tree, num / threadCount);
                    cdlAdd.countDown();

                    Thread.sleep(1000);

                    cyclicBarrier.await();
                    search(tree, num / threadCount);
                    cdlSearch.countDown();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }).start();
        }

        // 等待所有写入线程完成
        cdlAdd.await();
        t = System.currentTimeMillis() - t;

        System.out.println("size:" + tree.size() + ";time consuming:" + t + "ms");
        System.out.println("count per millisecond:" + tree.size() / t + "/ms");
        System.out.println("check:" + tree.check());

        System.out.println("searching");

        // 等待所有读取线程完成
        cdlSearch.await();
        t = System.currentTimeMillis() - t;

        System.out.println("time consuming:" + t + "ms");
        System.out.println("count per millisecond:" + num / t + "/ms");
        System.out.println("finish testing");

        tree.free();
        cyclicBarrier = null;
    }

    public static void testAdd(int num) {
        ZMFilter tree = new ZMFilter(8);
        System.out.println("testing add");

        t = System.currentTimeMillis();
        add(tree, num);
        t = System.currentTimeMillis() - t;

        System.out.println("size:" + tree.size() + ";time consuming:" + t + "ms");
        System.out.println("count per millisecond:" + tree.size() / t + "/ms");
        System.out.println("check:" + tree.check());
        System.out.println("searching");

        t = System.currentTimeMillis();
        search(tree, num);
        t = System.currentTimeMillis() - t;

        System.out.println("time consuming:" + t + "ms");
        System.out.println("count per millisecond:" + num / t + "/ms");
        System.out.println("finish testing");
        tree.free();
    }

    public static void testBalance() {
        ZMFilter tree = new ZMFilter(8);
        System.out.println("testing balance");
        Random random = new Random(System.currentTimeMillis());
        for (int i = 0; i < 10000; i++) {
            for (int j = 0; j < 500; j++) {
                tree.add(random.nextLong());
            }
            if (!tree.check())
                System.out.println("not order");
        }
        System.out.println("finish testing");
        tree.free();
    }

    public static void testDump() {
        ZMFilter tree = new ZMFilter(8);
        for (int i = 0; i < 100000; i++) {
            tree.add(i);
        }
        if (tree.dump("赵名.dat")) {
            System.out.println("save success");
            System.out.println("size:" + tree.size() + ";nodeCount:" + tree.nodeCount());
            tree.free();
            try {
                tree = ZMFilter.load("赵名.dat");
                System.out.println("load success");
                System.out.println("size:" + tree.size() + ";nodeCount:" + tree.nodeCount());
            } catch (Exception e) {
                System.out.println("load fail");
                return;
            }
        } else
            System.out.println("save fail");
        tree.free();
    }

    private static void add(ZMFilter tree, int num) {
        Random random = new Random(System.nanoTime());
        for (int i = 0; i < num; i++) {
            tree.add(random.nextLong());
        }
    }

    private static void search(ZMFilter tree, int num) {
        Random random = new Random(System.nanoTime());
        for (int i = 0; i < num; i++) {
            tree.search(random.nextLong());
        }
    }
}