import java.nio.ByteBuffer;
import java.util.Random;
import java.util.concurrent.CountDownLatch;

class test {

    public static void main(String[] args) throws Throwable {
        test(10000000);
        // testMultiThread(100000002, 3);
        // testBalance();
    }

    public static void testMultiThread(int num, int threadCount) throws Throwable {
        ZMFilter tree = new ZMFilter(8);
        System.out.println("adding");
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
        tree.free();
    }

    public static void test(int num) {
        ZMFilter tree = new ZMFilter(8);
        System.out.println("adding");
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