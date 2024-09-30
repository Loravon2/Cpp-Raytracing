# Documentation
A html Doc-String with the code documentation for Cpp-Raytracing can easily be generated. All you need is Doxygen installed on your system. 

Before building the documentation, you should have went once through the steps in the [README](../README.md) to initialize the cmake build.

Then you can easily build the html Doc-String
by calling
```
make documentation
```
in the build directory. All html pages will be created in `build/doc_doxygen/html`. To open the start page, either open `index.html` in a web browser of your choice or run
```
make open_doc
```
to open it automatically.