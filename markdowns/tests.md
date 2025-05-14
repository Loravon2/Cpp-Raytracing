# Tests
Cpp-Raytracing supports automated code tests. There are a variety of different unit, integration and end-to-end tests implemented in the `tests` directory.

To run the automated testing procedure, first compile the project with `make` and then run:
```
ctest
```
This will automatically run all tests. If you want additional output regarding failed tests, you can run
```
ctest --output-on-failure
```