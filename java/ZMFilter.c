#include "../ZMT_tree.h"

#include <stdio.h>
#include <string.h>
#include "ZMFilter.h"

#define ERROR -1

static void memrev(uint8 *key, uint8 len) {
  int8 i = -1;
  while (++i < --len) key[i] ^= key[len] ^= key[i] ^= key[len];
}

JNIEXPORT jlong JNICALL Java_ZMFilter_mCreate(JNIEnv *env, jclass cls, jshort i,
                                              jshort j) {
  ZMT_tree *tree = zmNew();
  zmInit(tree, i, j);
  return (jlong)tree;
}

JNIEXPORT void JNICALL Java_ZMFilter_mFree(JNIEnv *env, jclass cls,
                                           ZMT_tree *tree) {
  zmDeleteTree(tree);
}

JNIEXPORT jlong JNICALL Java_ZMFilter_mSize(JNIEnv *env, jclass cls,
                                            ZMT_tree *tree) {
  return tree->size;
}

JNIEXPORT jint JNICALL Java_ZMFilter_mNodeSize(JNIEnv *env, jclass cls,
                                               ZMT_tree *tree) {
  return tree->nodeSize;
}

JNIEXPORT jlong JNICALL Java_ZMFilter_mNodeCount(JNIEnv *env, jclass cls,
                                                 ZMT_tree *tree) {
  return tree->nodeCount;
}

JNIEXPORT jint JNICALL Java_ZMFilter_mIdLen(JNIEnv *env, jclass cls,
                                            ZMT_tree *tree) {
  return tree->idLen;
}

JNIEXPORT jboolean JNICALL Java_ZMFilter_mCheckBalance(JNIEnv *env, jclass cls,
                                                       ZMT_tree *tree) {
  if (zmCheckBalance(tree)) return JNI_TRUE;
  return JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_ZMFilter_mCheck(JNIEnv *env, jclass cls,
                                                ZMT_tree *tree) {
  if (zmCheck(tree)) return JNI_TRUE;
  return JNI_FALSE;
}

JNIEXPORT jint JNICALL Java_ZMFilter_mAdd(JNIEnv *env, jclass cls,
                                          ZMT_tree *tree, jbyteArray jba) {
  uint8 *key = (*env)->GetByteArrayElements(env, jba, NULL);
  uint8 len = (*env)->GetArrayLength(env, jba);
  int i;
  if (len < tree->idLen) {
    uint8 temp[tree->idLen];
    i = tree->idLen - len;
    memcpy(temp + i, key, len);
    memset(temp, 0, i);
    i = zmAdd(tree, temp);
    (*env)->ReleaseByteArrayElements(env, jba, key, 0);
  } else {
    uint8 *keyCache = key;
    for (; len > tree->idLen; len--, keyCache++) {
      if ((*keyCache) != 0) {
        (*env)->ReleaseByteArrayElements(env, jba, key, 0);
        return ERROR;
      }
    }
    i = zmAdd(tree, keyCache);
    (*env)->ReleaseByteArrayElements(env, jba, key, 0);
  }
  return i;
}

JNIEXPORT jint JNICALL Java_ZMFilter_mSearch(JNIEnv *env, jclass cls,
                                             ZMT_tree *tree, jbyteArray jba) {
  uint8 *key = (*env)->GetByteArrayElements(env, jba, NULL);
  uint8 len = (*env)->GetArrayLength(env, jba);
  int i;
  if (len < tree->idLen) {
    uint8 temp[tree->idLen];
    i = tree->idLen - len;
    memcpy(temp + i, key, len);
    memset(temp, 0, i);
    i = zmSearch(tree, temp);
    (*env)->ReleaseByteArrayElements(env, jba, key, 0);
  } else {
    uint8 *keyCache = key;
    for (; len > tree->idLen; len--, keyCache++) {
      if ((*keyCache) != 0) {
        (*env)->ReleaseByteArrayElements(env, jba, key, 0);
        return ERROR;
      }
    }
    i = zmSearch(tree, keyCache);
    (*env)->ReleaseByteArrayElements(env, jba, key, 0);
  }
  return i;
}