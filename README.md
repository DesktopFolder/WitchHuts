### Witch Hut Finder (C++)

A port & partial (WIP) reimplementation of [hube12/WitchHutFinder](https://github.com/hube12/WitchHutFinder/) in C++ for Minecraft version 1.18.

To build, you will want CMake and a working C++ compiler.

```
# Clone the repository, and Cubiomes (required dependency)
git clone --recursive https://github.com/DesktopFolder/WitchHuts && cd WitchHuts
# Create the build directory (for cleanliness)
mkdir build && cd build
# Generate the build files & build the project
cmake .. -DSEED_USED=YOURSEED && make
# Run the finder
./WitchHutFinder
```
