#include "../ZMT_tree.h"

#include <stdio.h>
#include <string.h>
#include "ZMT_tree_java.h"

#define ERROR -1

static void memrev(uint8 *key, uint8 len) {
  int8 i = -1;
  while (++i < --len) key[i] ^= key[len] ^= key[i] ^= key[len];
}

JNIEXPORT jlong JNICALL Java_ZMT_1tree_1java_mCreate(JNIEnv *env, jclass cls,
                                                     jint i) {
  if (i > 0 && i < 256) {
    ZMT_tree *tree = zmNew();
    zmInit(tree, i);
    // printf("%lld,%lld\n", i, tree);
    return (jlong)tree;
  }
  return 0;
}

JNIEXPORT void JNICALL Java_ZMT_1tree_1java_mFree(JNIEnv *env, jclass cls,
                                                  ZMT_tree *tree) {
  zmDeleteTree(tree);
}

JNIEXPORT jint JNICALL Java_ZMT_1tree_1java_mSize(JNIEnv *env, jclass cls,
                                                  ZMT_tree *tree) {
  return tree->size;
}

JNIEXPORT jint JNICALL Java_ZMT_1tree_1java_mNodeSize(JNIEnv *env, jclass cls,
                                                      ZMT_tree *tree) {
  return tree->nodeSize;
}

JNIEXPORT jint JNICALL Java_ZMT_1tree_1java_mIdLen(JNIEnv *env, jclass cls,
                                                   ZMT_tree *tree) {
  return tree->idLen;
}

JNIEXPORT jboolean JNICALL Java_ZMT_1tree_1java_mCheckBalance(JNIEnv *env,
                                                              jclass cls,
                                                              ZMT_tree *tree) {
  if (zmCheckBalance(tree) > 0) {
    return JNI_TRUE;
  }
  return JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_ZMT_1tree_1java_mCheck(JNIEnv *env, jclass cls,
                                                       ZMT_tree *tree) {
  if (zmCheckBalance(tree) > 0) {
    return JNI_TRUE;
  }
  return JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_ZMT_1tree_1java_mAdd(JNIEnv *env, jclass cls,
                                                     ZMT_tree *tree,
                                                     jbyteArray jba) {
  uint8 *key = (*env)->GetByteArrayElements(env, jba, NULL);
  uint8 len = (*env)->GetArrayLength(env, jba);
  short i;
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
        return JNI_FALSE;
        // return ERROR;
      }
    }
    i = zmAdd(tree, keyCache);
  }
  if (i > 0) {
    return JNI_TRUE;
  }
  return JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_ZMT_1tree_1java_mSearch(JNIEnv *env, jclass cls,
                                                        ZMT_tree *tree,
                                                        jbyteArray jba) {
  uint8 *key = (*env)->GetByteArrayElements(env, jba, NULL);
  uint8 len = (*env)->GetArrayLength(env, jba);
  short i;
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
        return JNI_FALSE;
        // return ERROR;
      }
    }
    i = zmSearch(tree, keyCache);
  }
  if (i > 0) {
    return JNI_TRUE;
  }
  return JNI_FALSE;
}