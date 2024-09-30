# Cpp-Raytracing
## Navigation
- [UML-Class diagram](resources/uml_class_diagram.pdf)
## Prerequisites
For installing, compiling and running Cpp-Raytracing, there are some prerequisites.
Required are:
- `cmake` with at least version 3.14
- `OpenCV4`

Optional dependencies are:
- `Eigen3` - if not found, it will be automatically installed and built in the project directory
- `nlohmann_json`- if not found, it will be automatically installed and built in the project directory
- `Doxygen` is necessary to generate the html Doc-String with code documentation
- Some standard web browser (firefox, chrome, chromium) is required to open the generated Doc-String via cmake
## Compilation
After cloning Cpp-Raytracing, you should create a build directory inside the projects directory.
```
cd Cpp-Raytracing
mkdir build 
```
Then navigate into the build directory and call cmake on the parent directory like this:
```
cd build
cmake ..
```
This will check for dependencies, fetch possible missing libraries from GitHub (**not** opencv) and create a comprehensive makefile. 
Alternatively you can build the program in [debug mode](markdowns/debug.md).

Now you can compile the program simply by typing make inside the build directory:
```
make
```
## Running the program
Once successfully compiled, you can find `Cpp-Raytracing.exe` inside the build directory. Run it with
```
./Cpp-Raytracing.exe
```
The program needs a [scene description](markdowns/file_input.md) to function. 
You can either create your own, or use one of our example scenes. 
For the first, please put it into a file and, when prompted, give the program its path relative to the projects top directory. That means, if you put `input.json` into the Cpp-Raytracing directory, you can just type
```
input.json
```
into the console, once the program asks you for the file.

If you want to use one of the examples, just type the examples number, when the program asks you. For some details regarding the examples, see [examples.md](markdowns/examples.md).

Once the image generation is finished, you can find the `output.png` in the build directory.