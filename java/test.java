import java.nio.ByteBuffer;
import java.util.Random;

class test {
    public static void main(String[] args) {
        ZMT_tree_java tree = new ZMT_tree_java(4);
        Random random = new Random(3545);
        long t = System.currentTimeMillis();
        for (int i = 0; i < 10000000; i++) {
            tree.add(random.nextInt(50000000));
        }
        System.out.println(System.currentTimeMillis() - t);
        System.out.println(tree.size());
        System.out.println(tree.nodeSize());
        System.out.println((tree.size() * 4) / (tree.nodeSize() * 4096.0));
        System.out.println(tree.check());
        System.out.println(tree.checkBalance());
        // fun1();
    }

    static void fun1() {
        long l = 3545;
        for (byte b : ByteBuffer.allocate(8).putLong(l).array()) {
            System.out.format("%x ", b);
        }
    }
}