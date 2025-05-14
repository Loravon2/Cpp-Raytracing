# Debug
If you are experimenting with the code or are confronted with some issue, you might want to build the project in *debug* mode.
For this, run
```
cmake -DCMAKE_BUILD_TYPE=Debug ..
```
in the build directory. 

The debug mode activates additional output messages, for example on every dynamic memory deallocation. 

It also disables all optimization flags. This will lead to severly decreased performance, so you might want to stick to simpler scenes when debugging.