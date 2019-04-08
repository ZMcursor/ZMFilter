#include <Python.h>
#include "../ZMT_tree.h"

typedef struct {
  PyObject_HEAD ZMT_tree *tree;
} ZMT_treeObject;

static PyObject *mNew(PyTypeObject *type, PyObject *args, PyObject *kwds) {
  ZMT_treeObject *self;
  self = (ZMT_treeObject *)type->tp_alloc(type, 0);
  self->tree = NULL;
  // printf("new tree\n");
  return (PyObject *)self;
}

static int mInit(ZMT_treeObject *self, PyObject *args, PyObject *kwds) {
  int idLen, nodeSize;
  if (PyArg_ParseTuple(args, "II", &idLen, &nodeSize)) {
    if (idLen > 0 && idLen < 256 && nodeSize >= 0 && nodeSize < 256) {
      self->tree = zmNewTree(idLen, nodeSize);
      // printf("idLen:%d,nodeSize:%d\n", idLen, nodeSize);
      return 0;
    }
  }
  return -1;
}

static void mFree(ZMT_treeObject *self) {
  if (self->tree != NULL) {
    zmDeleteTree(self->tree);
    self->tree = NULL;
  }
  // printf("mFree\n");
}

static void mDealloc(ZMT_treeObject *self) {
  if (self->tree != NULL) {
    zmDeleteTree(self->tree);
    self->tree = NULL;
  }
  Py_TYPE(self)->tp_free(self);
  printf("mDealloc\n");
}

static PyObject *mSize(ZMT_treeObject *self) {
  return PyLong_FromLong(self->tree->size);
}

static PyObject *mNodeSize(ZMT_treeObject *self) {
  return PyLong_FromLong(self->tree->nodeSize);
}

static PyObject *mNodeCount(ZMT_treeObject *self) {
  return PyLong_FromLong(self->tree->nodeCount);
}

static PyObject *mIdLen(ZMT_treeObject *self) {
  return PyLong_FromLong(self->tree->idLen);
}

static PyObject *mAdd(ZMT_treeObject *self, PyObject *key) {
  if (PyLong_CheckExact(key)) {
    uint8 byteLen = (_PyLong_NumBits(key) + 7) >> 3;
    if (byteLen <= self->tree->idLen) {
      uint8 keyCache[self->tree->idLen];
      if (_PyLong_AsByteArray((PyLongObject *)key, keyCache, self->tree->idLen,
                              0, 0) == 0) {
        // for (int i = 0; i < self->tree->idLen; i++) {
        //   printf("%X,", keyCache[i]);
        // }
        // printf("\n");
        if (zmAdd(self->tree, keyCache))
          Py_RETURN_TRUE;
        else
          Py_RETURN_FALSE;
      }
    }
  }
  Py_RETURN_NONE;
}

static PyObject *mSearch(ZMT_treeObject *self, PyObject *key) {
  if (PyLong_CheckExact(key)) {
    uint8 byteLen = (_PyLong_NumBits(key) + 7) >> 3;
    if (byteLen <= self->tree->idLen) {
      uint8 keyCache[self->tree->idLen];
      if (_PyLong_AsByteArray((PyLongObject *)key, keyCache, self->tree->idLen,
                              0, 0) == 0) {
        if (zmSearch(self->tree, keyCache))
          Py_RETURN_TRUE;
        else
          Py_RETURN_FALSE;
      }
    }
  }
  Py_RETURN_NONE;
}

static PyObject *mCheck(ZMT_treeObject *self) {
  if (zmCheck(self->tree))
    Py_RETURN_TRUE;
  else
    Py_RETURN_FALSE;
}

static PyMethodDef ZMT_tree_methods[] = {
    {"size", (PyCFunction)mSize, METH_NOARGS, "Return item size"},
    {"nodeSize", (PyCFunction)mNodeSize, METH_NOARGS, "Return size of node"},
    {"nodeCount", (PyCFunction)mNodeCount, METH_NOARGS, "Return count of node"},
    {"idLen", (PyCFunction)mIdLen, METH_NOARGS, "Return lenght of item"},
    {"check", (PyCFunction)mCheck, METH_NOARGS, "Return lenght of item"},
    {"free", (PyCFunction)mFree, METH_NOARGS, "Release memory of tree"},
    {"add", (PyCFunction)mAdd, METH_O, "Return lenght of item"},
    {"search", (PyCFunction)mSearch, METH_O, "Return lenght of item"},
    {NULL, NULL, 0, NULL} /* Sentinel */
};

static PyTypeObject ZMT_tree_Type = {
    PyVarObject_HEAD_INIT(
        // &PyType_Type,
        NULL, 0)
        .tp_name = "ZMT-tree",
    .tp_basicsize = sizeof(ZMT_treeObject),
    .tp_dealloc = (destructor)mDealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_methods = ZMT_tree_methods,
    .tp_init = (initproc)mInit,
    .tp_new = mNew,
};