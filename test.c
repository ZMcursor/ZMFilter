#include <time.h>
#include "ZMT_tree.c"

//反转数组
static void memrev(uint8 *key, uint8 len) {
  int8 i = -1;
  while (++i < --len) key[i] ^= key[len] ^= key[i] ^= key[len];
}

static void test() {
  ZMT_tree *tree = zmNew();
  zmInit(tree, 4, 2);
  // srand((uint32)time(0));
  // clock_t st = clock();
  uint32 i, j;
  for (i = 0; i < 500; i++) {
    // j = rand();
    j = i;
    memrev((uint8 *)&j, tree->idLen);
    zmAdd(tree, (uint8 *)&j);
  }
  printf("size:%d,node:%d\n", tree->size, tree->nodeCount);
  FILE *fp = fopen("tree.dat", "w");
  if (fp) {
    dumpTree(tree, fp);
    fclose(fp);
  }
  zmDeleteTree(tree);
}

int main() {
  // printf("tree size:%d\n", sizeof(ZMT_tree));
  // printf("node size:%d\n", sizeof(Node));
  test();
}