import java.io.File;
import java.nio.ByteBuffer;

public class ZMFilter {

    /**
     * 加载动态链接库，暂时使用绝对路径，修改为自己的路径
     */
    static {
        // System.load(System.getProperty("user.dir") + File.separator + "java" +
        // File.separator + "ZMT_tree.dll");
        System.loadLibrary("ZMT_tree");
    }

    /**
     * 保存Native代码中T-tree实例的指针
     */
    private volatile long tree = 0;
    /**
     * 已经保存的数据数量
     */
    private volatile long size = 0;
    /**
     * key占用的长度，以byte为单位
     */
    private final short keyLen;
    /**
     * T-tree中一个Node的大小，以KB为单位。
     */
    private final short nodeSize;

    /**
     * 新建一个滤重实例
     * 
     * @param keyLen key占用的长度，以byte为单位 注意，keyLen必须处于(0,256)区间内
     * 
     * @throws IllegalArgumentException 如果参数不在规定区间内
     */
    public ZMFilter(int keyLen) {
        this(keyLen, 0);
    }

    /**
     * 新建一个滤重实例
     * 
     * @param keyLen   key占用的长度，以byte为单位 注意，keyLen必须处于(0,256)区间内
     * 
     * @param nodeSize T-tree中一个Node的大小，默认值为4，以KB为单位。
     *                 注意，nodeSize必须处于(0,256)区间内，0代表采用默认值即4。
     *                 它是影响性能的重要参数，过长或过短都会降低性能。
     * 
     * @throws IllegalArgumentException 如果参数不在规定区间内
     */
    public ZMFilter(int keyLen, int nodeSize) {
        if (keyLen > 0 && keyLen < 256 && nodeSize >= 0 && nodeSize < 256) {
            this.keyLen = (short) keyLen;
            this.nodeSize = nodeSize == 0 ? 4 : (short) nodeSize;
            tree = mCreate(this.keyLen, this.nodeSize);
        } else
            throw new IllegalArgumentException("the value of keyLen most in (0,256) and nodeSize most in [0,256)");
    }

    /**
     * 新建一个反序列化来的滤重实例
     * 
     * @see #load(String)
     * 
     */
    private ZMFilter(long tree, long size, short keyLen, short nodeSize) {
        this.tree = tree;
        this.size = size;
        this.keyLen = keyLen;
        this.nodeSize = nodeSize;
    }

    /**
     * 释放滤重占用的内存
     */
    public void free() {
        if (tree != 0) {
            long temp = tree;
            tree = 0;
            size = 0;
            mFree(temp);
        }
    }

    /**
     * 防止用户没有主动释放滤重占用的内存
     */
    @Override
    protected void finalize() throws Throwable {
        free();
        super.finalize();
    }

    /**
     * 获取保存的key的数量
     * 
     * @throws NullPointerException 滤重已经被释放
     */
    public long size() {
        if (tree == 0)
            throw new NullPointerException("Filter have being released");
        return size;
    }

    /**
     * 获取T-tree的节点大小，以KB为单位。
     * 
     * @throws NullPointerException 滤重已经被释放
     */
    public int nodeSize() {
        if (tree == 0)
            throw new NullPointerException("Filter have being released");
        return nodeSize;
    }

    /**
     * 获取T-tree的节点数量，可以用来计算滤重占用的内存大小
     * 
     * @throws NullPointerException 滤重已经被释放
     */
    public long nodeCount() {
        if (tree == 0)
            throw new NullPointerException("Filter have being released");
        return mNodeCount(tree);
    }

    /**
     * 获取T-tree的版本
     * 
     * @throws NullPointerException 滤重已经被释放
     */
    public long version() {
        if (tree == 0)
            throw new NullPointerException("Filter have being released");
        return mVersion(tree);
    }

    /**
     * 获取T-tree的key长度
     * 
     * @throws NullPointerException 滤重已经被释放
     */
    public int keyLen() {
        if (tree == 0)
            throw new NullPointerException("Filter have being released");
        return keyLen;
    }

    /**
     * 检查T-tree结构是否正确
     * 
     * @throws NullPointerException 滤重已经被释放
     */
    public synchronized boolean check() {
        if (tree == 0)
            throw new NullPointerException("Filter have being released");
        return mCheck(tree);
    }

    /**
     * 添加一个key
     * 
     * @param key 添加的key
     * 
     * @see #add(byte[])
     */
    public boolean add(long key) {
        byte[] bs = ByteBuffer.allocate(8).putLong(key).array();
        return add(bs);
    }

    /**
     * 添加一个key
     * 
     * @param key 添加的key
     * 
     * @return true 添加成功，false key已存在
     * 
     * @throws NullPointerException     滤重已经被释放
     * 
     * @throws IllegalArgumentException key的有效长度大于keyLen
     */
    public boolean add(byte[] key) {
        if (tree == 0)
            throw new NullPointerException("Filter have being released");
        int result;
        // synchronized (this) {
        result = mAdd(tree, key);
        if (result > 0)
            size++;
        // }
        if (result < 0)
            throw new IllegalArgumentException("key is longer than keyLen");
        else
            return result > 0;
    }

    /**
     * 搜索一个key
     * 
     * @param key 搜索的key
     * 
     * @see #search(byte[])
     */
    public boolean search(long key) {
        byte[] bs = ByteBuffer.allocate(8).putLong(key).array();
        return search(bs);
    }

    /**
     * 搜索一个key
     * 
     * @param key 搜索的key
     * 
     * @return true key存在，false key不存在
     * 
     * @throws NullPointerException     滤重已经被释放
     * 
     * @throws IllegalArgumentException key的有效长度大于keyLen
     */
    public boolean search(byte[] key) {
        if (tree == 0)
            throw new NullPointerException("Filter have being released");
        int result;
        synchronized (this) {
            result = mSearch(tree, key);
        }
        if (result < 0)
            throw new IllegalArgumentException("key is longer than keyLen");
        else
            return result > 0;
    }

    public boolean dump(String filePath) {
        if (tree == 0)
            throw new NullPointerException("Filter have being released");
        return mDump(tree, filePath);
    }

    public static ZMFilter load(String filePath) {
        long tree = mLoad(filePath);
        if (tree == 0)
            throw new RuntimeException();
        return new ZMFilter(tree, mSize(tree), mKeyLen(tree), mNodeSize(tree));
    }

    private static native long mCreate(short keyLen, short nodeSize);

    private static native void mFree(long tree);

    private static native long mNodeCount(long tree);

    private static native boolean mCheck(long tree);

    private static native int mAdd(long tree, byte[] key);

    private static native int mSearch(long tree, byte[] key);

    private static native boolean mDump(long tree, String filePath);

    private static native long mLoad(String filePath);

    private static native long mSize(long tree);

    private static native short mKeyLen(long tree);

    private static native short mNodeSize(long tree);

    private static native short mVersion(long tree);
}