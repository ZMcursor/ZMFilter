#include "ZMT_tree.h"
#include <stdlib.h>
#include <string.h>

// bool
#define True 1
#define False 0

//左右子树
#define RED ((int8)1)
#define BALCK ((int8)-1)

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
    if (node->high == 2) {
      Node *tNode = node->left;
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
        pNode->right = node;
        node->color = BALCK;

        tNode->right = NULL;
        tNode->high = 0;
        tNode->parent = pNode;
        pNode->left = tNode;

        return pNode;
      }
    }
    Node *tNode = node->left;
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
    if (node->high == 2) {
      Node *tNode = node->right;
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

        return pNode;
      }
    }
    Node *tNode = node->right;
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
          if (result == 0)
            return True;
          else if (result < 0)
            right = mid - 1;
          else
            left = mid + 1;
        }
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
          if (result == 0) {
            // find key
            return False;
          } else if (result < 0)
            right = mid - 1;
          else
            left = mid + 1;
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

ZMT_tree *zmNew() {
  ZMT_tree *tree = malloc(sizeof(ZMT_tree));
  tree->root = NULL;
  tree->head = NULL;
  tree->size = 0;
  tree->nodeCount = 0;
  tree->idLen = 0;
  tree->nodeSize = 4;
  return tree;
}

void zmInit(ZMT_tree *tree, uint8 id_len, uint8 node_size) {
  tree->idLen = id_len;
  if (node_size) tree->nodeSize = node_size;
  tree->maxLen = LIST_SIZE / id_len;
  tree->minLen = (tree->maxLen + 1) >> 1;
}

void zmDeleteTree(ZMT_tree *tree) {
  if (tree->head != NULL) {
    Node *node = tree->head->prev;
    while (node != tree->head) {
      node = node->prev;
      free(node->next);
    }
    free(tree->head);
  }
  free(tree);
}

static uint8 checkBalance(Node *node, int *notBalanceNum) {
  uint8 l = 0, r = 0;
  if (node->left != NULL) l = checkBalance(node->left, notBalanceNum) + 1;
  if (node->right != NULL) r = checkBalance(node->right, notBalanceNum) + 1;
  int8 f = l - r;
  if (f > 1 || f < -1) (*notBalanceNum)++;
  return l > r ? l : r;
}

int zmCheckBalance(ZMT_tree *tree) {
  if (tree->root == NULL) return True;
  int notBalanceNum = 0;
  uint8 high = checkBalance(tree->root, &notBalanceNum);
  if (notBalanceNum > 0) return False;
  return True;
}

int zmCheck(ZMT_tree *tree) {
  if (tree->head == NULL) return True;
  Node *node = tree->head;
  uint8 k[tree->idLen];
  memmove(k, node->list, tree->idLen);
  uint16 i = 1;
  uint32 size = 1;
  do {
    if (!node->high && (node->left != NULL || node->right != NULL))
      return False;
    while (i < node->len) {
      if (memcmp(k, node->list + tree->idLen * i, tree->idLen) >= 0)
        return False;
      memmove(k, node->list + tree->idLen * i, tree->idLen);
      i++;
      size++;
    }
    node = node->next;
    i = 0;
  } while (node != tree->head);
  if (size != tree->size) return False;
  return True;
}
