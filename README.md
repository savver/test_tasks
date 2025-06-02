# Test_tasks

## task_02
Files MD5 hash check

### Dependencies
The repository [MD5-hash-Calculator](https://github.com/vishal6969/MD5-hash-Calculator) is used.
To initially download this repository, use:
```
git clone --recurse-submodules https://github.com/savver/test_tasks.git
```
Or you can manually download the submodule later:
```
git submodule update --init --recursive
```

### Building with GCC + Make
```
cd ./task_02_files_checker
make all
```
To remove old compiled files:
```
make clean
```
### Building and debugging with Qt Creator (using GCC + CMake)
Qt Creator: File -> Open File or Project.. -> select file CMakeLists.txt