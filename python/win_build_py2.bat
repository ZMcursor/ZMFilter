gcc -std=c99 -DMS_WIN64 -Ofast -fPIC -shared -I"C:\Program Files\Python27\include" -L"C:\Program Files\Python27\libs" ..\ZMT_tree.c ZMT_treeObject.c -o ZMT_tree.pyd -lpython27
pause