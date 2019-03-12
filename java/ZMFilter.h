/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class ZMFilter */

#ifndef _Included_ZMFilter
#define _Included_ZMFilter
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     ZMFilter
 * Method:    mCreate
 * Signature: (SS)J
 */
JNIEXPORT jlong JNICALL Java_ZMFilter_mCreate
  (JNIEnv *, jclass, jshort, jshort);

/*
 * Class:     ZMFilter
 * Method:    mFree
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_ZMFilter_mFree
  (JNIEnv *, jclass, ZMT_tree *);

/*
 * Class:     ZMFilter
 * Method:    mSize
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_ZMFilter_mSize
  (JNIEnv *, jclass, ZMT_tree *);

/*
 * Class:     ZMFilter
 * Method:    mNodeSize
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_ZMFilter_mNodeSize
  (JNIEnv *, jclass, ZMT_tree *);

/*
 * Class:     ZMFilter
 * Method:    mNodeCount
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_ZMFilter_mNodeCount
  (JNIEnv *, jclass, ZMT_tree *);

/*
 * Class:     ZMFilter
 * Method:    mIdLen
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_ZMFilter_mIdLen
  (JNIEnv *, jclass, ZMT_tree *);

/*
 * Class:     ZMFilter
 * Method:    mCheckBalance
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_ZMFilter_mCheckBalance
  (JNIEnv *, jclass, ZMT_tree *);

/*
 * Class:     ZMFilter
 * Method:    mCheck
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_ZMFilter_mCheck
  (JNIEnv *, jclass, ZMT_tree *);

/*
 * Class:     ZMFilter
 * Method:    mAdd
 * Signature: (J[B)I
 */
JNIEXPORT jint JNICALL Java_ZMFilter_mAdd
  (JNIEnv *, jclass, ZMT_tree *, jbyteArray);

/*
 * Class:     ZMFilter
 * Method:    mSearch
 * Signature: (J[B)I
 */
JNIEXPORT jint JNICALL Java_ZMFilter_mSearch
  (JNIEnv *, jclass, ZMT_tree *, jbyteArray);

#ifdef __cplusplus
}
#endif
#endif