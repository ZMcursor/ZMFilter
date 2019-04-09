#include "../ZMT_tree.h"

#include <stdio.h>
#include <string.h>
#include "ZMFilter.h"

JNIEXPORT jlong JNICALL Java_ZMFilter_mCreate(JNIEnv *env, jclass cls, jshort i,
                                              jshort j) {
  return (jlong)zmNewTree(i, j);
}

JNIEXPORT void JNICALL Java_ZMFilter_mFree(JNIEnv *env, jclass cls,
                                           ZMT_tree *tree) {
  zmDeleteTree(tree);
}

JNIEXPORT jlong JNICALL Java_ZMFilter_mNodeCount(JNIEnv *env, jclass cls,
                                                 ZMT_tree *tree) {
  return tree->nodeCount;
}

JNIEXPORT jboolean JNICALL Java_ZMFilter_mCheck(JNIEnv *env, jclass cls,
                                                ZMT_tree *tree) {
  if (zmCheck(tree)) return JNI_TRUE;
  return JNI_FALSE;
}

JNIEXPORT jlong JNICALL Java_ZMFilter_mSize(JNIEnv *env, jclass cls,
                                            ZMT_tree *tree) {
  return tree->size;
}

JNIEXPORT jshort JNICALL Java_ZMFilter_mKeyLen(JNIEnv *env, jclass cls,
                                               ZMT_tree *tree) {
  return tree->idLen;
}

JNIEXPORT jshort JNICALL Java_ZMFilter_mNodeSize(JNIEnv *env, jclass cls,
                                                 ZMT_tree *tree) {
  return tree->nodeSize;
}

JNIEXPORT jshort JNICALL Java_ZMFilter_mVersion(JNIEnv *env, jclass cls,
                                                ZMT_tree *tree) {
  return tree->version;
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

JNIEXPORT jboolean JNICALL Java_ZMFilter_mDump(JNIEnv *env, jclass cls,
                                               ZMT_tree *tree,
                                               jstring filePath) {
  jboolean result = JNI_FALSE;
#ifdef __linux__
  const char *filename = (*env)->GetStringUTFChars(env, filePath, NULL);
#endif
#ifdef _WIN32
  const jchar *filename = (*env)->GetStringChars(env, filePath, NULL);
#endif
  if (filename) {
#ifdef __linux__
    FILE *fp = fopen(filename, "wb");
#endif
#ifdef _WIN32
    FILE *fp = _wfopen(filename, L"wb");
#endif
    if (fp) {
      if (zmDumpTree(tree, fp)) {
        result = JNI_TRUE;
      }
      fclose(fp);
    }
#ifdef __linux__
    (*env)->ReleaseStringUTFChars(env, filePath, filename);
#endif
#ifdef _WIN32
    (*env)->ReleaseStringChars(env, filePath, filename);
#endif
  }
  return result;
}

JNIEXPORT jlong JNICALL Java_ZMFilter_mLoad(JNIEnv *env, jclass cls,
                                            jstring filePath) {
  ZMT_tree *tree = 0;
#ifdef __linux__
  const char *filename = (*env)->GetStringUTFChars(env, filePath, NULL);
#endif
#ifdef _WIN32
  const jchar *filename = (*env)->GetStringChars(env, filePath, NULL);
#endif
  if (filename) {
#ifdef __linux__
    FILE *fp = fopen(filename, "rb");
#endif
#ifdef _WIN32
    FILE *fp = _wfopen(filename, L"rb");
#endif
    if (fp) {
      tree = zmLoadTree(fp);
      fclose(fp);
    }
#ifdef __linux__
    (*env)->ReleaseStringUTFChars(env, filePath, filename);
#endif
#ifdef _WIN32
    (*env)->ReleaseStringChars(env, filePath, filename);
#endif
  }
  return (jlong)tree;
}