### Witch Hut Finder (C++)

A port & partial (WIP) reimplementation of [hube12/WitchHutFinder](https://github.com/hube12/WitchHutFinder/) in C++ for Minecraft version 1.18.

To build, you will want CMake and a working C++ compiler.

```sh
# Clone the repository, and Cubiomes (required dependency)
git clone --recursive https://github.com/DesktopFolder/WitchHuts && cd WitchHuts
# Create the build directory (for cleanliness)
mkdir build && cd build
# Generate the build files & build the project
# -DOPTIMIZE_SEED=1234 (for example seed 1234) if you know the seed prior to running the tool
# Otherwise, see below for using the CLI to specify a seed
cmake .. -DOPTIMIZE_SEED=YOURSEED && make
# Run the finder
./WitchHutFinder
```

#### Quick Documentation

**Providing an output file:**
```sh
# --output/-o
./WitchHutFinder -o output_file
# Will print progress indicators every 10 huts found.
```

**Providing a seed:**
```sh
# --seed/-s
./WitchHutFinder -s some_seed
```
