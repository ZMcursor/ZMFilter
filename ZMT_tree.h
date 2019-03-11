#ifndef _ZM_T_TREE_H_
#define _ZM_T_TREE_H_

//-128<-->127
#define int8 signed char
// 0<-->255
#define uint8 unsigned char
// 0<-->65,535
#define uint16 unsigned short
// 0<-->4,294,967,295
#define uint32 unsigned int
//?
#define uint64 unsigned long long

//控制版本
#define VERSION 0

//控制大小长度
#define NODE_SIZE (tree->nodeSize * 1024)
#define LIST_SIZE (NODE_SIZE - 5 * sizeof(struct Node *) - 4)

// bool
#define True 1
#define False 0

typedef struct Node {
  struct Node *parent, *left, *right, *prev, *next;
  uint8 high;
  int8 color;
  uint16 len;
  uint8 list[1];
} Node;

typedef struct {
  Node *root;
  Node *head;
  uint32 size;
  uint32 nodeCount;
  uint8 idLen;
  uint8 nodeSize;
  uint16 maxLen;
  uint16 minLen;
} ZMT_tree;

ZMT_tree *zmNew();

void zmInit(ZMT_tree *tree, uint8 id_len, uint8 node_size);

void zmDeleteTree(ZMT_tree *tree);

int zmSearch(ZMT_tree *tree, uint8 *key);

int zmAdd(ZMT_tree *tree, uint8 *key);

int zmCheckBalance(ZMT_tree *tree);

int zmCheck(ZMT_tree *tree);

#endif