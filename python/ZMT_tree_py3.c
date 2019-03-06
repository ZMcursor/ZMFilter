#include <Python.h>
#include "ZMT_treeobject.c"

static PyMethodDef ZMT_TreeModuleMethods[] = {
    // {"search", _search, METH_O, "search one item in tree"},
    {NULL, NULL, 0, NULL}};

static PyModuleDef ZMT_TreeModule = {PyModuleDef_HEAD_INIT,
                                     .m_name = "ZMT_Tree", .m_size = -1};

PyMODINIT_FUNC PyInit_ZMT_tree(void) {
  if (PyType_Ready(&ZMT_tree_Type) < 0) return NULL;

  PyObject *module = PyModule_Create(&ZMT_TreeModule);
  if (module == NULL) return NULL;

  Py_INCREF(&ZMT_tree_Type);
  PyModule_AddObject(module, "ZMT_tree", (PyObject *)&ZMT_tree_Type);
  return module;
}
