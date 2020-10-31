# FILE CRAWLER

## A File crawler is implemented from scratch using C++. It finds the document type and path of the file. Inputs are the search directory and the file name.

## Code Explanation

### Used FileSystems library from C++. The Filesystem library provides facilities for performing operations on file systems and their components, such as paths, regular files, and directories.

```c++
#include <filesystem>
```

### Inputs are taken as arguments to main function.

### File to search for is stored in file_name

```c++
fs::path file_name = argv[2];
```

### directory iterator in file system is used to iterate through each subdirectory of search_dir

```c++
const fs::directory_iterator dir_end; // directory end sentinel
// used to iterate through each subdirectory of search_dir
fs::directory_iterator dir_iter(search_dir);
```

### Iterate through every directory till directory end sentinal is reached

```c++
for(; dir_iter != dir_end; ++dir_iter)
{
```

### Skip the non directories

```c++
// skip non directories
    if(!fs::is_directory(dir_iter->path()))
        continue;
```
### Iterate through every file in the sub directory using dir_iter->path() to find the file and print the path to the file and file type to the console

```c++
// iterate through files in this subdirectory dir_iter->path()
        auto file_iter = fs::directory_iterator(dir_iter->path());

        for(; file_iter != dir_end; ++file_iter)
        {
            // ignore directories and wrong filenames
            if(fs::is_directory(file_iter->path())
            || file_iter->path().filename() != file_name)
                continue;

            // Ok we found it (the first one)
            // printing file path and file type 
            std::cout << "path: " << file_iter->path().string() << '\n';
            std::cout << "Filetype: " << file_iter->file_type() << '\n';

            return EXIT_SUCCESS;
        }
```




