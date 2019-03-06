#include <Python.h>
#include "ZMT_treeObject.c"

static PyMethodDef ZMT_TreeModuleMethods[] = {{NULL, NULL, 0, NULL}};

PyMODINIT_FUNC initZMT_tree(void) {
  if (PyType_Ready(&ZMT_tree_Type) < 0) return;

  PyObject *module = Py_InitModule("ZMT_tree", ZMT_TreeModuleMethods);
  if (module == NULL) return;

  Py_INCREF(&ZMT_tree_Type);
  PyModule_AddObject(module, "ZMT_tree", (PyObject *)&ZMT_tree_Type);
}
