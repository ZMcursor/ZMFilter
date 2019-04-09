#include <time.h>
#include "ZMT_tree.c"

//反转数组
static void memrev(uint8 *key, uint8 len) {
  int8 i = -1;
  while (++i < --len) key[i] ^= key[len] ^= key[i] ^= key[len];
}

static void testSave() {
  ZMT_tree *tree = zmNewTree(4, 2);
  srand((uint32)time(0));
  // clock_t st = clock();
  uint32 i, j;
  for (i = 0; i < 8888; i++) {
    j = rand() * rand();
    // j = i;
    // memrev((uint8 *)&j, tree->idLen);
    zmAdd(tree, (uint8 *)&j);
  }
  printf("maxLen:%d,size:%d,node:%d\n", tree->maxLen, tree->size,
         tree->nodeCount);
  FILE *fp;
#ifdef linux
  fp = fopen("赵名.dat", "wb");
#endif
#ifdef _WIN32
  fp = _wfopen(L"赵名.dat", L"wb");
#endif
  if (fp) {
    if (zmDumpTree(tree, fp) != 1) printf("save error\n");
    fclose(fp);
  }
  zmDeleteTree(tree);
}

static void testLoad() {
  testSave();
  FILE *fp = fopen("赵名.dat", "rb");
  if (fp) {
    ZMT_tree *tree = zmLoadTree(fp);
    fclose(fp);
    if (tree) {
      printf("maxLen:%d,size:%d,node:%d\n", tree->maxLen, tree->size,
             tree->nodeCount);
      printf("check:%d", zmCheck(tree));
      zmDeleteTree(tree);
    } else
      printf("load error\n");
  }
}

int main() {
  // testLoad();
  testSave();
  return 0;
}