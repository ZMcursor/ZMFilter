#ifndef _ZM_T_TREE_H_
#define _ZM_T_TREE_H_

#include <stdio.h>

//-128<-->127
#define int8 signed char
// 0<-->255
#define uint8 unsigned char
// 0<-->65,535
#define uint16 unsigned short
// 0<-->4,294,967,295
#define uint32 unsigned int
// ?
#define uint64 unsigned long long

// error
#define ERROR -1
// bool
#define True 1
#define False 0

// 控制序列化版本
#define VERSION 0

// 控制节点大小
#define NODE_SIZE (tree->nodeSize * 1024)
// 节点中列表大小
#define LIST_SIZE (NODE_SIZE - 5 * sizeof(struct Node *) - 4)

// T-tree节点，所有节点根据key值(也就是T-tree中序遍历顺序)连成双向循环链表
typedef struct Node {
  struct Node *parent, *left, *right, *prev, *next;
  // 节点高度
  uint8 high;
  // 用来区分当前节点为父节点的左右节点
  int8 color;
  // 列表保存的key数
  uint16 len;
  // key列表
  uint8 list[4];
} Node;

// T-tree结构
typedef struct {
  // 根节点
  Node *root;
  // 串成的链表头节点
  Node *head;
  // 节点数
  uint32 nodeCount;
  // 一个节点最多保存的key数
  uint16 maxLen;
  // 一个节点最少保存的key数
  uint16 minLen;
  // T-tree保存的总key数
  uint32 size;
  // T-tree版本
  uint16 version;
  // 每个节点占内存大小，单位为KB
  uint8 nodeSize;
  // key长度，单位为Byte
  uint8 idLen;
} ZMT_tree;

ZMT_tree *zmNewTree(uint8 id_len, uint8 node_size);

void zmDeleteTree(ZMT_tree *tree);

int zmSearch(ZMT_tree *tree, uint8 *key);

int zmAdd(ZMT_tree *tree, uint8 *key);

// 检查T-tree结构是否正确
int zmCheck(ZMT_tree *tree);

int zmDumpTree(ZMT_tree *tree, FILE *fp);

ZMT_tree *zmLoadTree(FILE *fp);

#endif