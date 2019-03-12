import java.nio.ByteBuffer;
import java.util.Random;

class test {

    public static void main(String[] args) throws Throwable {
        ZMFilter tree = new ZMFilter(8);
        Random random = new Random(System.currentTimeMillis());
        long t = System.currentTimeMillis();
        for (int i = 0; i < 1000000; i++) {
            tree.add(random.nextLong());
        }
        System.out.println("time:" + (System.currentTimeMillis() - t));
        // t = System.currentTimeMillis();
        // for (int i = 0; i < 10000; i++) {
        // tree.search(random.nextLong());
        // }
        // t = System.currentTimeMillis() - t;
        // System.out.println("time:" + t);
        // System.out.println("size/time:" + tree.size() / t);
        // Thread.sleep(5000);
        // t = System.currentTimeMillis();
        // for (int i = 0; i < 10000; i++) {
        // tree.search(random.nextLong());
        // }
        // t = System.currentTimeMillis() - t;
        // System.out.println("time:" + t);
        // System.out.println("size/time:" + tree.size() / t);
        // System.out.println("size:" + tree.size());
        // tree.free();
        // Thread.sleep(10000);
        // System.out.println("node count:" + tree.nodeCount());
        // System.out.println("mem:" + (tree.nodeSize() * tree.nodeCount()) / 1024.0);
        // System.out.println("ratio:" + (tree.size() * 8) / (tree.nodeSize() *
        // tree.nodeCount() * 1024.0));
        System.out.println(tree.check());
        System.out.println(tree.checkBalance());
    }

    // private static int BEGIN = 1;
    // private static int END = 1 << 30 - 1;
    // private static byte[] marks = new byte[(END - BEGIN + 7) / 8];
    // private static Random random = new Random(System.currentTimeMillis());

    // /* 不重复的随机数 */
    // private static int randomInt() {
    // int num, pos;
    // byte mark;
    // for (;;) {
    // num = random.nextInt(END - BEGIN);
    // pos = num / 8;
    // mark = ((byte) 1) << (num % 8);
    // if (mark & marks[pos])
    // continue;
    // marks[pos] += mark;
    // return num;
    // }
    // }
}