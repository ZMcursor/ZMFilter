#include <Python.h>
#include "../ZMT_tree.h"

typedef struct {
  PyObject_HEAD ZMT_tree *tree;
} ZMT_treeObject;

static PyObject *mNew(PyTypeObject *type, PyObject *args, PyObject *kwds) {
  ZMT_treeObject *self;
  self = (ZMT_treeObject *)type->tp_alloc(type, 0);
  self->tree = zmNew();
  return (PyObject *)self;
}

static int mInit(ZMT_treeObject *self, PyObject *args, PyObject *kwds) {
  int id_Len, nodeSize;
  if (PyArg_ParseTuple(args, "II", &id_Len, &nodeSize)) {
    if (id_Len > 0 && id_Len < 256 && nodeSize >= 0 && nodeSize < 256) {
      zmInit(self->tree, id_Len, nodeSize);
      // printf("idlen:%d\n", id_Len);
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
  // printf("mDealloc\n");
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
        if (zmAdd(self->tree, keyCache) > 0)
          Py_RETURN_TRUE;
        else
          Py_RETURN_FALSE;
        // for (int i = 0; i < self->tree->idLen; i++) {
        //   printf("%X,", keyCache[i]);
        // }
        // printf("\n");
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
        if (zmSearch(self->tree, keyCache) > 0)
          Py_RETURN_TRUE;
        else
          Py_RETURN_FALSE;
      }
    }
  }
  Py_RETURN_NONE;
}

static PyObject *mCheck(ZMT_treeObject *self) {
  if (zmCheck(self->tree) > 0)
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
        NULL,
        // &PyType_Type,
        0) "ZMT-tree", /* For printing, in format "<module>.<name>" */
    /* For allocation */
    sizeof(ZMT_treeObject), /*tp_basicsize*/
    0,                      /*tp_itemsize*/
    /* Methods to implement standard operations */
    (destructor)mDealloc, /*tp_dealloc*/
    0,                    /*tp_print*/
    0,                    /*tp_getattr*/
    0,                    /*tp_setattr*/
    0,                    /*tp_compare*/
    0,                    /*tp_repr*/
    /* Method suites for standard classes */
    0, /*tp_as_number*/
    0, /*tp_as_sequence*/
    0, /*tp_as_mapping*/
    /* More standard operations (here for binary compatibility) */
    0, /*tp_hash */
    0, /*tp_call */
    0, /*tp_str */
    0, /*tp_getattro */
    0, /*tp_setattro */
    /* Functions to access object as input/output buffer */
    0, /*tp_as_buffer */
    /* Flags to define presence of optional/expanded features */
    Py_TPFLAGS_DEFAULT, /*tp_flags */
    0,                  /*tp_doc */
    /* Assigned meaning in release 2.0 */
    /* call function for all accessible objects */
    0, /*tp_traverse */
    /* delete references to contained objects */
    0, /*tp_clear */
    /* Assigned meaning in release 2.1 */
    /* rich comparisons */
    0, /*tp_richcompare */
    /* weak reference enabler */
    0, /*tp_weaklistoffset */
    /* Added in release 2.2 */
    /* Iterators */
    0, /*tp_iter */
    0, /*tp_iternext */
    /* Attribute descriptor and subclassing stuff */
    ZMT_tree_methods, /*tp_methods */
    0,                /*tp_members */
    0,                /*tp_getset */
    0,                /*tp_base */
    0,                /*tp_dict */
    0,                /*tp_descr_get */
    0,                /*tp_descr_set */
    0,                /*tp_dictoffset */
    (initproc)mInit,  /*tp_init */
    0,                /*tp_alloc */
    mNew,             /*tp_new */
    0,                /*tp_free */
    0,                /*tp_is_gc */
    0,                /*tp_bases */
    0,                /*tp_mro */
    0,                /*tp_cache */
    0,                /*tp_subclasses */
    0,                /*tp_weaklist */
    0,                /*tp_del */
    /* Type attribute cache version tag. Added in version 2.6 */
    0, /*tp_version_tag */
};