#include <time.h>
#include "ZMT_tree.c"

//反转数组
static void memrev(uint8 *key, uint8 len) {
  int8 i = -1;
  while (++i < --len) key[i] ^= key[len] ^= key[i] ^= key[len];
}

static void testSave() {
  ZMT_tree *tree = zmNew();
  srand((uint32)time(0));
  // clock_t st = clock();
  zmInit(tree, 4, 2);
  uint32 i, j;
  for (i = 0; i < 8888888; i++) {
    j = rand() * rand();
    // j = i;
    // memrev((uint8 *)&j, tree->idLen);
    zmAdd(tree, (uint8 *)&j);
  }
  printf("maxLen:%d,size:%d,node:%d\n", tree->maxLen, tree->size,
         tree->nodeCount);
  FILE *fp;
  fp = fopen("tree.dat", "wb");
  if (fp) {
    if (zmDumpTree(tree, fp) != 1) printf("save error\n");
    fclose(fp);
  }
  zmDeleteTree(tree);
}

static void testLoad() {
  testSave();
  ZMT_tree *tree = zmNew();
  FILE *fp;
  fp = fopen("tree.dat", "rb");
  if (fp) {
    if (zmLoadTree(tree, fp) == 1) {
      printf("maxLen:%d,size:%d,node:%d\n", tree->maxLen, tree->size,
             tree->nodeCount);
      printf("check:%d", zmCheck(tree));
    } else
      printf("load error\n");
    fclose(fp);
  }
  zmDeleteTree(tree);
}

int main() {
  testLoad();
  // testSave();
}