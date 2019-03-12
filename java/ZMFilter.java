import java.nio.ByteBuffer;

class NullFilterError extends RuntimeException {

    public NullFilterError(String msg) {
        super(msg);
    }

    public NullFilterError() {
        super("Filter have be free");
    }
}

class IllegalValueError extends RuntimeException {

    public IllegalValueError(String msg) {
        super(msg);
    }
}

public class ZMFilter {

    static {
        System.load("E:\\work\\MyCode\\ZMFilter\\java\\ZMT_tree.dll");
    }

    private volatile long tree = 0;

    public ZMFilter(int idLen) {
        this(idLen, 0);
    }

    public ZMFilter(int idLen, int nodeSize) {
        if (idLen > 0 && idLen < 256 && nodeSize >= 0 && nodeSize < 256) {
            tree = mCreate((short) idLen, (short) nodeSize);
        } else
            throw new IllegalValueError("the value of idlen mast in (0,256) and nodeSize mast in [0,256)");
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
        super.finalize();
    }

    public long size() {
        if (tree == 0)
            throw new NullFilterError();
        return mSize(tree);
    }

    public int nodeSize() {
        if (tree == 0)
            throw new NullFilterError();
        return mNodeSize(tree);
    }

    public long nodeCount() {
        if (tree == 0)
            throw new NullFilterError();
        return mNodeCount(tree);
    }

    public int idLen() {
        if (tree == 0)
            throw new NullFilterError();
        return mIdLen(tree);
    }

    public synchronized boolean checkBalance() {
        if (tree == 0)
            throw new NullFilterError();
        return mCheckBalance(tree);
    }

    public synchronized boolean check() {
        if (tree == 0)
            throw new NullFilterError();
        return mCheck(tree);
    }

    public synchronized boolean add(long key) {
        if (tree == 0)
            throw new NullFilterError();
        byte[] bs = ByteBuffer.allocate(8).putLong(key).array();
        int result = mAdd(tree, bs);
        if (result < 0)
            throw new IllegalValueError("key is long then idlen");
        else if (result > 0)
            return true;
        else
            return false;
    }

    public synchronized boolean search(long key) {
        if (tree == 0)
            throw new NullFilterError();
        byte[] bs = ByteBuffer.allocate(8).putLong(key).array();
        int result = mSearch(tree, bs);
        if (result < 0)
            throw new IllegalValueError("key is long then idlen");
        else if (result > 0)
            return true;
        else
            return false;
    }

    private static native long mCreate(short idLen, short nodeSize);

    private static native void mFree(long tree);

    private static native long mSize(long tree);

    private static native int mNodeSize(long tree);

    private static native long mNodeCount(long tree);

    private static native int mIdLen(long tree);

    private static native boolean mCheckBalance(long tree);

    private static native boolean mCheck(long tree);

    private static native int mAdd(long tree, byte[] key);

    private static native int mSearch(long tree, byte[] key);
}