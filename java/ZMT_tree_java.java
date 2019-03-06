import java.nio.ByteBuffer;

public class ZMT_tree_java {

    static {
        System.load("E:\\work\\multitoutiao\\MyCode\\T_tree\\java\\ZMT_tree.dll");
        // System.loadLibrary("ZMT_tree");
    }

    private long tree = 0;

    public ZMT_tree_java(int idLen) {
        tree = mCreate(idLen);
        // System.out.println(tree);
    }

    public void free() {
        if (tree != 0) {
            long temp = tree;
            tree = 0;
            mFree(temp);
        }
    }

    @Override
    protected void finalize() throws Throwable {
        free();
        System.out.println("free");
        super.finalize();
    }

    public int size() {
        if (tree == 0)
            return 0;
        return mSize(tree);
    }

    public int nodeSize() {
        if (tree == 0)
            return 0;
        return mNodeSize(tree);
    }

    public int idLen() {
        if (tree == 0)
            return 0;
        return mIdLen(tree);
    }

    public boolean checkBalance() {
        if (tree == 0)
            return false;
        return mCheckBalance(tree);
    }

    public boolean check() {
        if (tree == 0)
            return false;
        return mCheck(tree);
    }

    public boolean add(long key) {
        if (tree == 0)
            return false;
        byte[] bs = ByteBuffer.allocate(8).putLong(key).array();
        return mAdd(tree, bs);
    }

    public boolean search(long key) {
        if (tree == 0)
            return false;
        byte[] bs = ByteBuffer.allocate(8).putLong(key).array();
        return mSearch(tree, bs);
    }

    private static native long mCreate(int idLen);

    private static native void mFree(long tree);

    private static native int mSize(long tree);

    private static native int mNodeSize(long tree);

    private static native int mIdLen(long tree);

    private static native boolean mCheckBalance(long tree);

    private static native boolean mCheck(long tree);

    private static native boolean mAdd(long tree, byte[] key);

    private static native boolean mSearch(long tree, byte[] key);
}