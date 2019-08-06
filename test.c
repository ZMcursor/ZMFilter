#include <time.h>
#include "ZMT_tree.c"

// 反转数组
static void memrev(uint8 *key, uint8 len) {
  int8 i = -1;
  while (++i < --len) key[i] ^= key[len] ^= key[i] ^= key[len];
}

uint32 number = RAND_MAX >> 1;
// 生成整型随机数
static uint32 nextInt() {
  uint32 n;
#ifdef __linux__
  n = rand();
  number--;
#elif defined _WIN32
  n = (rand() << 16) + rand();
  number--;
  number--;
#endif
  if (!number) {
    number = RAND_MAX >> 1;
    srand(n);
  }
  return n;
}
// 生成长整型随机数
static uint64 nextLong() { return (((uint64)nextInt()) << 32) + nextInt(); }

static void add(ZMT_tree *tree, uint32 num) {
  number = RAND_MAX >> 1;
  srand((uint32)time(0));
  uint64 id;
  while (num--) {
    id = nextLong();
    zmAdd(tree, (uint8 *)&id);
  }
}

static void search(ZMT_tree *tree, uint32 num) {
  number = RAND_MAX >> 1;
  srand((uint32)time(0));
  uint64 id;
  while (num--) {
    id = nextLong();
    zmSearch(tree, (uint8 *)&id);
  }
}

// 测试写入读取速度
static void test(uint32 num) {
  ZMT_tree *tree = zmNewTree(8, 4);
  printf("testing add\n");
  clock_t st = clock();
  add(tree, num);
  st = clock() - st;
  printf("size:%lld,node count:%lld\ntime consuming:%lldms\n", tree->size,
         tree->nodeCount, st * 1000 / CLOCKS_PER_SEC);
  printf("count per millisecond:%lld\n",
         tree->size / (st * 1000 / CLOCKS_PER_SEC));

  printf("searching\n");
  st = clock();
  search(tree, num);
  st = clock() - st;
  printf("time consuming:%lldms\n", st * 1000 / CLOCKS_PER_SEC);
  printf("count per millisecond:%lld\n", num / (st * 1000 / CLOCKS_PER_SEC));
  printf("finish testing\n");
  zmDeleteTree(tree);
}

// 测试序列化
static void testDump() {
  printf("testing dump\n");
  ZMT_tree *tree = zmNewTree(4, 2);
  uint32 i, j;
  for (i = 0; i < 10000000; i++) {
    j = i;
    memrev((uint8 *)&j, tree->idLen);
    zmAdd(tree, (uint8 *)&j);
  }
  printf("size:%d,node:%d\n", tree->size, tree->nodeCount);
  FILE *fp;
#ifdef __linux__
  fp = fopen("赵名.dat", "wb");
#elif defined _WIN32
  fp = _wfopen(L"赵名.dat", L"wb");
#endif
  if (fp) {
    clock_t st = clock();
    if (!zmDumpTree(tree, fp)) printf("dump error\n");
    st = clock() - st;
    printf("time consuming:%lldms\n", st * 1000 / CLOCKS_PER_SEC);
    fclose(fp);
  }
  zmDeleteTree(tree);
}

// 测试反序列化
static void testLoad() {
  printf("testing load\n");
  FILE *fp;
#ifdef __linux__
  fp = fopen("赵名.dat", "rb");
#elif defined _WIN32
  fp = _wfopen(L"赵名.dat", L"rb");
#endif
  if (fp) {
    clock_t st = clock();
    ZMT_tree *tree = zmLoadTree(fp);
    st = clock() - st;
    printf("time consuming:%lldms\n", st * 1000 / CLOCKS_PER_SEC);
    fclose(fp);
    if (tree) {
      printf("size:%d,node:%d\n", tree->size, tree->nodeCount);
      zmDeleteTree(tree);
    } else
      printf("load error\n");
  }
}

int main() {
  // test(10000000);
  testDump();
  testLoad();
  return 0;
}