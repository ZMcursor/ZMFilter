#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ZMT_tree.h"

//用来区分左右子树
#define RED ((int8)1)
#define BALCK ((int8)-1)

#define TREE_OFFSET (2 * sizeof(struct Node *) + 8)
#define NODE_OFFSET (5 * sizeof(struct Node *) + 4)

#define LOW_LOAD_FACTER ((float)(0.6))
#define LOAD_FACTER ((float)(0.75))
#define OVER_LOAD_FACTER ((float)(0.9))

int zmDumpTree(ZMT_tree *tree, FILE *fp) {
  uint8 *data = ((uint8 *)tree) + TREE_OFFSET;
  if (!fwrite(data, sizeof(ZMT_tree) - TREE_OFFSET, 1, fp)) {
    return False;
  }
  if (tree->head != NULL) {
    Node *node = tree->head;
    do {
      data = ((uint8 *)node) + NODE_OFFSET;
      if (!fwrite(data, tree->idLen, node->len, fp)) {
        return False;
      }
      node = node->next;
    } while (node != tree->head);
  }
  return True;
}

static Node *buildTree(Node *head, uint32 nodeCount) {
  Node *node;
  switch (nodeCount) {
    case 1:
      node = head;
      node->left = NULL;
      node->right = NULL;
      node->high = 0;
      break;
    case 2:
      node = head->next;
      head->left = NULL;
      head->right = NULL;
      head->high = 0;
      head->parent = node;
      head->color = RED;
      node->left = head;

      node->right = NULL;

      node->high = 1;
      break;
    case 3:
      node = head->next;
      head->left = NULL;
      head->right = NULL;
      head->high = 0;
      head->parent = node;
      head->color = RED;
      node->left = head;

      head = node->next;
      head->left = NULL;
      head->right = NULL;
      head->high = 0;
      head->parent = node;
      head->color = BALCK;
      node->right = head;

      node->high = 1;
      break;
    case 4:
      node = head->next->next;
      node->left = buildTree(head, 2);
      node->left->parent = node;
      node->left->color = RED;

      head = node->next;
      head->left = NULL;
      head->right = NULL;
      head->high = 0;
      head->parent = node;
      head->color = BALCK;
      node->right = head;

      node->high = 2;
      break;

    default:
      node = head;
      uint32 mid = nodeCount >> 1;
      uint32 temp = mid;
      while (temp--) node = node->next;
      node->left = buildTree(head, mid);
      node->left->parent = node;
      node->left->color = RED;
      node->right = buildTree(node->next, nodeCount - mid - 1);
      node->right->parent = node;
      node->right->color = BALCK;
      node->high = node->left->high > node->right->high ? node->left->high + 1
                                                        : node->right->high + 1;
      break;
  }
  return node;
}

ZMT_tree *zmLoadTree(FILE *fp) {
  ZMT_tree *tree = malloc(sizeof(ZMT_tree));

  uint8 *data = ((uint8 *)tree) + TREE_OFFSET;
  if (!fread(data, sizeof(ZMT_tree) - TREE_OFFSET, 1, fp)) {
    free(tree);
    return NULL;
  }
  if (tree->version != VERSION) {
    free(tree);
    return NULL;
  }
  tree->maxLen = LIST_SIZE / tree->idLen;
  tree->minLen = (tree->maxLen + 1) >> 1;
  if (!tree->size) {
    tree->root = NULL;
    tree->head = NULL;
    tree->nodeCount = 0;
    return tree;
  }

  uint32 nodeCount = tree->size / ((uint32)(tree->maxLen * LOAD_FACTER));
  uint16 len;
  if (nodeCount) {
    if (tree->size >= nodeCount * ((uint32)(tree->maxLen * OVER_LOAD_FACTER))) {
      nodeCount++;
      if (tree->size <=
          nodeCount * ((uint32)(tree->maxLen * LOW_LOAD_FACTER + 0.5))) {
        nodeCount--;
        if (tree->size > nodeCount * tree->maxLen) {
          nodeCount++;
        }
      }
    }
    tree->nodeCount = nodeCount;
    len = tree->size / tree->nodeCount;
    nodeCount -= tree->size % tree->nodeCount;
  } else {
    nodeCount = 1;
    tree->nodeCount = nodeCount;
    len = tree->size;
  }

  tree->head = malloc(NODE_SIZE);
  tree->head->prev = tree->head;
  tree->head->next = tree->head;
  Node *node = tree->head;
  node->len = len;
  data = ((uint8 *)node) + NODE_OFFSET;
  if (!fread(data, tree->idLen, node->len, fp)) {
    free(node);
    free(tree);
    return NULL;
  }
  Node *newNode;
  uint32 i;
  for (i = 1; i < nodeCount; i++) {
    newNode = malloc(NODE_SIZE);
    newNode->next = node->next;
    newNode->next->prev = newNode;
    newNode->prev = node;
    node->next = newNode;
    newNode->len = len;
    data = ((uint8 *)newNode) + NODE_OFFSET;
    if (!fread(data, tree->idLen, newNode->len, fp)) {
      zmDeleteTree(tree);
      return NULL;
    }
    node = newNode;
  }
  len++;
  for (; i < tree->nodeCount; i++) {
    newNode = malloc(NODE_SIZE);
    newNode->next = node->next;
    newNode->next->prev = newNode;
    newNode->prev = node;
    node->next = newNode;
    newNode->len = len;
    data = ((uint8 *)newNode) + NODE_OFFSET;
    if (!fread(data, tree->idLen, newNode->len, fp)) {
      zmDeleteTree(tree);
      return NULL;
    }
    node = newNode;
  }
  if (fread(&i, 1, 1, fp) || !feof(fp)) {
    zmDeleteTree(tree);
    return NULL;
  }
  tree->root = buildTree(tree->head, tree->nodeCount);
  tree->root->parent = NULL;
  tree->root->color = 0;
  if (!zmCheck(tree)) {
    zmDeleteTree(tree);
    return NULL;
  }
  return tree;
}

static Node *LLBalance(ZMT_tree *tree, Node *node) {
  Node *tNode = node->left, *pNode = tNode->right;

  if (pNode != NULL) {
    pNode->parent = node;
    pNode->color = RED;
  }
  node->left = pNode;

  tNode->parent = node->parent;
  tNode->color = node->color;
  if (tNode->color > 0)
    tNode->parent->left = tNode;
  else if (tNode->color < 0)
    tNode->parent->right = tNode;
  else
    tree->root = tNode;

  node->parent = tNode;
  tNode->right = node;
  node->color = BALCK;
  node->high = tNode->high - 1;

  return tNode;
}

static Node *RRBalance(ZMT_tree *tree, Node *node) {
  Node *tNode = node->right, *pNode = tNode->left;

  if (pNode != NULL) {
    pNode->parent = node;
    pNode->color = BALCK;
  }
  node->right = pNode;

  tNode->parent = node->parent;
  tNode->color = node->color;
  if (tNode->color > 0)
    tNode->parent->left = tNode;
  else if (tNode->color < 0)
    tNode->parent->right = tNode;
  else
    tree->root = tNode;

  node->parent = tNode;
  tNode->left = node;
  node->color = RED;
  node->high = tNode->high - 1;

  return tNode;
}

static Node *balanceNode(ZMT_tree *tree, Node *node, int8 fact) {
  if (fact > 1) {
    Node *tNode = node->left;
    if (node->high == 2) {
      if (tNode->left != NULL) {
        tNode->parent = node->parent;
        tNode->color = node->color;
        if (tNode->color > 0)
          tNode->parent->left = tNode;
        else if (tNode->color < 0)
          tNode->parent->right = tNode;
        else
          tree->root = tNode;

        node->parent = tNode;
        tNode->right = node;
        node->color = BALCK;
        node->left = NULL;

        tNode->high = 1;
        node->high = 0;

        return tNode;
      } else {
        // printf("lr,");
        Node *pNode = tNode->right;

        pNode->parent = node->parent;
        pNode->color = node->color;
        if (pNode->color > 0)
          pNode->parent->left = pNode;
        else if (pNode->color < 0)
          pNode->parent->right = pNode;
        else
          tree->root = pNode;

        node->left = NULL;
        node->high = 0;
        node->parent = pNode;
        node->color = BALCK;
        pNode->right = node;

        tNode->right = NULL;
        tNode->high = 0;
        tNode->parent = pNode;
        pNode->left = tNode;

        pNode->high = 1;

        return pNode;
      }
    }
    fact = tNode->left->high - tNode->right->high;
    if (fact < 0) {
      Node *pNode = tNode->right, *qNode = pNode->left;

      pNode->parent = node;
      node->left = pNode;
      pNode->color = RED;

      if (qNode != NULL) {
        qNode->parent = tNode;
        qNode->color = BALCK;
      }
      tNode->right = qNode;
      tNode->high = tNode->left->high + 1;

      tNode->parent = pNode;
      pNode->left = tNode;
      pNode->high = tNode->high + 1;
    }
    return LLBalance(tree, node);
  } else if (fact < -1) {
    Node *tNode = node->right;
    if (node->high == 2) {
      if (tNode->right != NULL) {
        tNode->parent = node->parent;
        tNode->color = node->color;
        if (tNode->color > 0)
          tNode->parent->left = tNode;
        else if (tNode->color < 0)
          tNode->parent->right = tNode;
        else
          tree->root = tNode;

        node->parent = tNode;
        tNode->left = node;
        node->color = RED;
        node->right = NULL;

        tNode->high = 1;
        node->high = 0;

        return tNode;
      } else {
        // printf("rl,");
        Node *pNode = tNode->left;

        pNode->parent = node->parent;
        pNode->color = node->color;
        if (pNode->color > 0)
          pNode->parent->left = pNode;
        else if (pNode->color < 0)
          pNode->parent->right = pNode;
        else
          tree->root = pNode;

        node->right = NULL;
        node->high = 0;
        node->parent = pNode;
        pNode->left = node;
        node->color = RED;

        tNode->left = NULL;
        tNode->high = 0;
        tNode->parent = pNode;
        pNode->right = tNode;

        pNode->high = 1;

        return pNode;
      }
    }
    fact = tNode->left->high - tNode->right->high;
    if (fact > 0) {
      Node *pNode = tNode->left, *qNode = pNode->right;

      pNode->parent = node;
      node->right = pNode;
      pNode->color = BALCK;

      if (qNode != NULL) {
        qNode->parent = tNode;
        qNode->color = RED;
      }
      tNode->left = qNode;
      tNode->high = tNode->right->high + 1;

      tNode->parent = pNode;
      pNode->right = tNode;
      pNode->high = tNode->high + 1;
    }
    return RRBalance(tree, node);
  }
  return node;
}

static void rebalance(ZMT_tree *tree, Node *node) {
  uint8 maxHigh, leftHigh, rightHigh;
  while (node != NULL) {
    leftHigh = 0;
    rightHigh = 0;
    if (node->left != NULL) leftHigh = node->left->high + 1;
    if (node->right != NULL) rightHigh = node->right->high + 1;
    maxHigh = leftHigh > rightHigh ? leftHigh : rightHigh;
    if (node->high == maxHigh) return;
    node->high = maxHigh;
    node = balanceNode(tree, node, (int8)(leftHigh - rightHigh));
    node = node->parent;
  }
}

int zmSearch(ZMT_tree *tree, uint8 *key) {
  Node *node = tree->root;
  int result;
  while (node != NULL) {
    result = memcmp(key, node->list, tree->idLen);
    if (result < 0)
      node = node->left;
    else {
      result =
          memcmp(key, node->list + tree->idLen * (node->len - 1), tree->idLen);
      if (result > 0)
        node = node->right;
      else {
        uint16 left = 0, mid, right = node->len - 1;
        while (left <= right) {
          mid = (left + right) >> 1;
          result = memcmp(key, node->list + mid * tree->idLen, tree->idLen);
          if (result > 0)
            left = mid + 1;
          else if (result < 0)
            right = mid - 1;
          else
            return True;
        }
        return False;
      }
    }
  }
  return False;
}

int zmAdd(ZMT_tree *tree, uint8 *key) {
  if (tree->root == NULL) {
    // init root node
    tree->root = malloc(NODE_SIZE);
    tree->root->parent = NULL;
    tree->root->left = NULL;
    tree->root->right = NULL;
    tree->root->prev = tree->root;
    tree->root->next = tree->root;
    tree->root->len = 1;
    tree->root->high = 0;
    tree->root->color = 0;
    tree->head = tree->root;
    memmove(tree->root->list, key, tree->idLen);
    tree->nodeCount++;
    tree->size++;
    return True;
  }
  Node *node = tree->root;
  int result;
  for (;;) {
    // compare with the left edge
    result = memcmp(key, node->list, tree->idLen);
    if (result < 0) {
      // go left
      if (node->left != NULL)
        node = node->left;
      else {
        // try add to the prev node
        if (node != tree->head && node->prev->len < tree->maxLen) {
          node = node->prev;
          memmove(node->list + tree->idLen * node->len, key, tree->idLen);
          node->len++;
        } else if (node->len < tree->maxLen) {
          memmove(node->list + tree->idLen, node->list,
                  tree->idLen * node->len);
          memmove(node->list, key, tree->idLen);
          node->len++;
        } else {
          // split node
          Node *newNode = malloc(NODE_SIZE);
          newNode->len = tree->minLen;
          node->len -= newNode->len;
          memmove(newNode->list, node->list + tree->idLen * node->len,
                  tree->idLen * newNode->len);
          memmove(node->list + tree->idLen, node->list,
                  tree->idLen * node->len);
          memmove(node->list, key, tree->idLen);
          node->len++;
          newNode->next = node->next;
          newNode->next->prev = newNode;
          newNode->prev = node;
          node->next = newNode;
          tree->nodeCount++;
          if (node->right == NULL) {
            newNode->parent = node;
            newNode->left = NULL;
            newNode->right = NULL;
            newNode->high = 0;
            newNode->color = BALCK;
            node->right = newNode;
            rebalance(tree, node);
          } else {
            newNode->parent = node->parent;
            newNode->left = node;
            newNode->right = node->right;
            newNode->high = node->high;
            newNode->color = node->color;
            if (node->color > 0)
              newNode->parent->left = newNode;
            else if (node->color < 0)
              newNode->parent->right = newNode;
            else
              tree->root = newNode;
            newNode->right->parent = newNode;
            node->parent = newNode;
            node->right = NULL;
            node->high = 0;
            node->color = RED;
          }
        }
        tree->size++;
        return True;
      }
    } else {
      // compare with the right edge
      result =
          memcmp(key, node->list + tree->idLen * (node->len - 1), tree->idLen);
      if (result > 0) {
        // go right
        if (node->right != NULL)
          node = node->right;
        else {
          if (node->len < tree->maxLen) {
            memmove(node->list + tree->idLen * node->len, key, tree->idLen);
            node->len++;
          } else if (node->next != tree->head &&
                     node->next->len < tree->maxLen) {
            node = node->next;
            memmove(node->list + tree->idLen, node->list,
                    tree->idLen * node->len);
            memmove(node->list, key, tree->idLen);
            node->len++;
          } else {
            // split node
            Node *newNode = malloc(NODE_SIZE);
            newNode->len = tree->minLen;
            node->len -= newNode->len;
            memmove(newNode->list, node->list + tree->idLen * node->len,
                    tree->idLen * newNode->len);
            memmove(newNode->list + tree->idLen * newNode->len, key,
                    tree->idLen);
            newNode->len++;
            newNode->next = node->next;
            newNode->next->prev = newNode;
            newNode->prev = node;
            node->next = newNode;
            tree->nodeCount++;

            // add to the right
            newNode->parent = node;
            newNode->left = NULL;
            newNode->right = NULL;
            newNode->high = 0;
            newNode->color = BALCK;
            node->right = newNode;
            if (node->left == NULL) rebalance(tree, node);
          }
          tree->size++;
          return True;
        }
      } else {
        // key must in this node or should insert into
        uint16 left = 0, mid, right = node->len - 1;
        while (left <= right) {
          mid = (left + right) >> 1;
          result = memcmp(key, node->list + mid * tree->idLen, tree->idLen);
          if (result > 0) {
            // find key
            left = mid + 1;
          } else if (result < 0)
            right = mid - 1;
          else
            return False;
        }
        // key will insert into node at postion left
        if (node->len < tree->maxLen) {
          memmove(node->list + tree->idLen * (left + 1),
                  node->list + tree->idLen * left,
                  tree->idLen * (node->len - left));
          memmove(node->list + tree->idLen * left, key, tree->idLen);
          node->len++;
        } else {
          // split node
          Node *newNode = malloc(NODE_SIZE);
          newNode->len = tree->minLen;
          node->len -= newNode->len;
          memmove(newNode->list, node->list + tree->idLen * node->len,
                  tree->idLen * newNode->len);
          if (left < node->len) {
            memmove(node->list + tree->idLen * (left + 1),
                    node->list + tree->idLen * left,
                    tree->idLen * (node->len - left));
            memmove(node->list + tree->idLen * left, key, tree->idLen);
            node->len++;
          } else if (left > node->len) {
            left -= node->len;
            memmove(newNode->list + tree->idLen * (left + 1),
                    newNode->list + tree->idLen * left,
                    tree->idLen * (newNode->len - left));
            memmove(newNode->list + tree->idLen * left, key, tree->idLen);
            newNode->len++;
          } else {
            memmove(node->list + tree->idLen * left, key, tree->idLen);
            node->len++;
          }
          newNode->next = node->next;
          newNode->next->prev = newNode;
          newNode->prev = node;
          node->next = newNode;
          tree->nodeCount++;
          // leaf node
          if (node->high == 0) {
            newNode->parent = node;
            newNode->left = NULL;
            newNode->right = NULL;
            newNode->high = 0;
            newNode->color = BALCK;
            node->right = newNode;
            rebalance(tree, node);
          } else if (node->left == NULL) {
            newNode->parent = node->parent;
            newNode->left = node;
            newNode->right = node->right;
            newNode->high = node->high;
            newNode->color = node->color;
            if (node->color > 0)
              newNode->parent->left = newNode;
            else if (node->color < 0)
              newNode->parent->right = newNode;
            else
              tree->root = newNode;
            newNode->right->parent = newNode;
            node->parent = newNode;
            node->right = NULL;
            node->high = 0;
            node->color = RED;
          } else if (node->right == NULL) {
            newNode->parent = node;
            newNode->left = NULL;
            newNode->right = NULL;
            newNode->high = 0;
            newNode->color = BALCK;
            node->right = newNode;
          } else {
            // node both have left and right child
            if (newNode->next->right != NULL) {
              // add to next node's left
              node = newNode->next;
              newNode->parent = node;
              newNode->left = NULL;
              newNode->right = NULL;
              newNode->high = 0;
              newNode->color = RED;
              node->left = newNode;
            } else if (node->prev->left != NULL) {
              // add to prev node's right
              newNode->parent = node->parent;
              newNode->left = node->left;
              newNode->right = node->right;
              newNode->high = node->high;
              newNode->color = node->color;
              if (newNode->color > 0)
                newNode->parent->left = newNode;
              else if (newNode->color < 0)
                newNode->parent->right = newNode;
              else
                tree->root = newNode;
              newNode->left->parent = newNode;
              newNode->right->parent = newNode;
              newNode = node->prev;
              node->parent = newNode;
              node->left = NULL;
              node->right = NULL;
              node->high = 0;
              node->color = BALCK;
              newNode->right = node;
            } else {
              node = newNode->next;
              newNode->parent = node;
              newNode->left = NULL;
              newNode->right = NULL;
              newNode->high = 0;
              newNode->color = RED;
              node->left = newNode;
              rebalance(tree, node);
            }
          }
        }
        tree->size++;
        return True;
      }
    }
  }
}

ZMT_tree *zmNewTree(uint8 id_len, uint8 node_size) {
  ZMT_tree *tree = malloc(sizeof(ZMT_tree));
  tree->root = NULL;
  tree->head = NULL;
  tree->size = 0;
  tree->nodeCount = 0;
  tree->version = VERSION;
  tree->idLen = id_len;
  tree->nodeSize = (node_size ? node_size : 4);
  tree->maxLen = LIST_SIZE / id_len;
  tree->minLen = (tree->maxLen + 1) >> 1;
  return tree;
}

void zmDeleteTree(ZMT_tree *tree) {
  if (tree->head != NULL) {
    Node *node = tree->head->prev;
    while (node != tree->head) {
      node = node->prev;
      free(node->next);
    }
    free(node);
  }
  free(tree);
}

int zmCheck(ZMT_tree *tree) {
  if (tree->head == NULL) return True;
  Node *node = tree->head;
  uint8 *key = node->list;
  uint16 pos = 1;
  uint32 size = 1, nodeCount = 0;
  uint8 leftHigh, rightHigh;
  int8 fact;
  do {
    leftHigh = 0;
    rightHigh = 0;
    if (node->left != NULL) leftHigh = node->left->high + 1;
    if (node->right != NULL) rightHigh = node->right->high + 1;
    if (node->high != (leftHigh > rightHigh ? leftHigh : rightHigh)) {
#ifdef zmDEBUG
      printf("ERROR high->node.high:%lld,leftHigh:%lld,rightHigh:%lld\n",
             node->high, leftHigh, rightHigh);
#endif
      return False;
    }
    fact = leftHigh - rightHigh;
    if (fact > 1 || fact < -1) {
#ifdef zmDEBUG
      printf("ERROR balance->node.high:%lld,leftHigh:%lld,rightHigh:%lld\n",
             node->high, leftHigh, rightHigh);
#endif
      return False;
    }
    while (pos < node->len) {
      if (memcmp(key, node->list + tree->idLen * pos, tree->idLen) >= 0) {
#ifdef zmDEBUG
        printf("ERROR sort->size:%lld,node:%lld,postion:%lld\n", size,
               nodeCount, pos);
#endif
        return False;
      }
      key = node->list + tree->idLen * pos;
      pos++;
      size++;
    }
    node = node->next;
    pos = 0;
    nodeCount++;
  } while (node != tree->head);
  if (size != tree->size || nodeCount != tree->nodeCount) {
#ifdef zmDEBUG
    printf(
        "ERROR "
        "size->size:%lld,tree.size:%lld;nodeCount:%lld,tree.nodeCount:%lld\n",
        size, tree->size, nodeCount, tree->nodeCount);
#endif
    return False;
  }
  return True;
}
