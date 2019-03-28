import java.nio.ByteBuffer;

public class ZMFilter {

    /**
     * 加载动态链接库，暂时使用绝对路径，修改为自己的路径
     */
    static {
        System.load("E:\\work\\MyCode\\ZMFilter\\java\\ZMT_tree.dll");
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
     * id占用的长度，以byte为单位
     */
    private final short idLen;
    /**
     * T-tree中一个Node的大小，以KB为单位。
     */
    private final short nodeSize;

    /**
     * 新建一个滤重实例
     * 
     * @param idLen id占用的长度，以byte为单位 注意，idLen必须处于(0,256)区间内
     * 
     * @throws IllegalArgumentException 如果参数不在规定区间内
     */
    public ZMFilter(int idLen) {
        this(idLen, 0);
    }

    /**
     * 新建一个滤重实例
     * 
     * @param idLen    id占用的长度，以byte为单位 注意，idLen必须处于(0,256)区间内
     * 
     * @param nodeSize T-tree中一个Node的大小，默认值为4，以KB为单位。
     *                 注意，nodeSize必须处于(0,256)区间内，0代表采用默认值即4。
     *                 它是影响性能的重要参数，过长或过短都会降低性能。
     * 
     * @throws IllegalArgumentException 如果参数不在规定区间内
     */
    public ZMFilter(int idLen, int nodeSize) {
        if (idLen > 0 && idLen < 256 && nodeSize >= 0 && nodeSize < 256) {
            this.idLen = (short) idLen;
            this.nodeSize = nodeSize == 0 ? 4 : (short) nodeSize;
            tree = mCreate(this.idLen, this.nodeSize);
        } else
            throw new IllegalArgumentException("the value of idlen most in (0,256) and nodeSize most in [0,256)");
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
     * 获取保存的id的数量
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
     * 获取T-tree的id大小
     * 
     * @throws NullPointerException 滤重已经被释放
     */
    public int idLen() {
        if (tree == 0)
            throw new NullPointerException("Filter have being released");
        return idLen;
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
     * 添加一个id
     * 
     * @param key 添加的id
     * 
     * @return true 添加成功，false key已存在
     * 
     * @throws NullPointerException     滤重已经被释放
     * 
     * @throws IllegalArgumentException key的有效长度大于idLen
     */
    public boolean add(long key) {
        if (tree == 0)
            throw new NullPointerException("Filter have being released");
        byte[] bs = ByteBuffer.allocate(8).putLong(key).array();
        int result;
        synchronized (this) {
            result = mAdd(tree, bs);
            if (result > 0)
                size++;
        }
        if (result < 0)
            throw new IllegalArgumentException("key is longer than idlen");
        else if (result > 0)
            return true;
        else
            return false;
    }

    /**
     * 搜索一个id
     * 
     * @param key 搜索的id
     * 
     * @return true key存在，false key不存在
     * 
     * @throws NullPointerException     滤重已经被释放
     * 
     * @throws IllegalArgumentException key的有效长度大于idLen
     */
    public boolean search(long key) {
        if (tree == 0)
            throw new NullPointerException("Filter have being released");
        byte[] bs = ByteBuffer.allocate(8).putLong(key).array();
        int result;
        synchronized (this) {
            result = mSearch(tree, bs);
        }
        if (result < 0)
            throw new IllegalArgumentException("key is longer than idlen");
        else if (result > 0)
            return true;
        else
            return false;
    }

    private static native long mCreate(short idLen, short nodeSize);

    private static native void mFree(long tree);

    private static native long mNodeCount(long tree);

    private static native boolean mCheck(long tree);

    private static native int mAdd(long tree, byte[] key);

    private static native int mSearch(long tree, byte[] key);
}