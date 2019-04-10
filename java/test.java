import java.util.Random;
import java.util.concurrent.CountDownLatch;
import java.io.File;

class test {

    public static void main(String[] args) throws Throwable {
        // testAdd(10000000);
        testMultiThread(10000002, 3);
        // testBalance();
        // testDump();
    }

    public static void testMultiThread(int num, int threadCount) throws Throwable {
        ZMFilter tree = new ZMFilter(8);
        System.out.println("testing add thread:" + threadCount);
        final CountDownLatch countAdd = new CountDownLatch(threadCount);
        Runnable addRunnable = new Runnable() {
            @Override
            public void run() {
                add(tree, num / threadCount);
                countAdd.countDown();
            }
        };
        long t = System.currentTimeMillis();
        for (int i = 0; i < threadCount; i++) {
            new Thread(addRunnable).start();
        }
        countAdd.await();
        t = System.currentTimeMillis() - t;
        // System.out.println("threadCount:" + Thread.activeCount());
        System.out.println("size:" + tree.size() + ";time consuming:" + t + "ms");
        System.out.println("count per millisecond:" + tree.size() / t + "/ms");
        System.out.println("check:" + tree.check());
        System.out.println("searching");
        final CountDownLatch countSearch = new CountDownLatch(threadCount);
        addRunnable = new Runnable() {
            @Override
            public void run() {
                search(tree, num / threadCount);
                countSearch.countDown();
            }
        };
        t = System.currentTimeMillis();
        for (int i = 0; i < threadCount; i++) {
            new Thread(addRunnable).start();
        }
        countSearch.await();
        t = System.currentTimeMillis() - t;
        // System.out.println("threadCount:" + Thread.activeCount());
        System.out.println("time consuming:" + t + "ms");
        System.out.println("count per millisecond:" + num / t + "/ms");
        System.out.println("finish testing");
        tree.free();
    }

    public static void testAdd(int num) {
        ZMFilter tree = new ZMFilter(8);
        System.out.println("testing add");
        long t = System.currentTimeMillis();
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
            tree = ZMFilter.load("赵名.dat");
            System.out.println("load success");
            System.out.println("size:" + tree.size() + ";nodeCount:" + tree.nodeCount());
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