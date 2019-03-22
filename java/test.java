import java.nio.ByteBuffer;
import java.util.Random;

class test {

    public static void main(String[] args) throws Throwable {
        ZMFilter tree = new ZMFilter(8);
        testBalance(tree);
        // testAdd(tree);
        // testSearch(tree, 100000000);
        tree.free();
    }

    private static void testAdd(ZMFilter tree) {
        System.out.println("adding");
        Random random = new Random(System.currentTimeMillis());
        long t = System.currentTimeMillis();
        for (int i = 0; i < 100000000; i++) {
            tree.add(random.nextLong());
        }
        t = System.currentTimeMillis() - t;
        System.out.println("size:" + tree.size());
        System.out.println("time consuming:" + t + "ms");
        System.out.println("count per millisecond:" + tree.size() / t + "/ms");
        System.out.println("check:" + tree.check());
    }

    private static void testBalance(ZMFilter tree) {
        System.out.println("testing balance");
        Random random = new Random(System.currentTimeMillis());
        for (int i = 0; i < 10000; i++) {
            for (int j = 0; j < 500; j++) {
                tree.add(random.nextLong());
            }
            if (!tree.check())
                System.out.println("not order");
            // System.out.println("balance:" + tree.checkBalance() + ";order:" +
            // tree.check());
        }
    }

    private static void testSearch(ZMFilter tree, int num) {
        System.out.println("searching");
        Random random = new Random(System.currentTimeMillis());
        long t = System.currentTimeMillis();
        for (int i = 0; i < num; i++) {
            tree.search(random.nextLong());
        }
        t = System.currentTimeMillis() - t;
        System.out.println("time consuming:" + t + "ms");
        System.out.println("count per millisecond:" + num / t + "/ms");
    }

}