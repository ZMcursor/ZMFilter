gcc -std=c99 -Ofast -fPIC -shared -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" ..\ZMT_tree.c ZMFilter.c -o ZMT_tree.dll
pause