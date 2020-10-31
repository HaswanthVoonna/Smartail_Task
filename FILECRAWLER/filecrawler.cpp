#include <iostream>
#include <filesystem>

// for readability
namespace fs = std::experimental::filesystem;

int main(int argc, char* argv[])
{
    if(!argv[1])
    {
        std::cerr << "require 2 parameters, search directory and filename\n";
        return EXIT_FAILURE;
    }

    fs::path search_dir = argv[1];

    if(!fs::is_directory(search_dir))
    {
        std::cerr << "First parameter must be a directory: " << search_dir << '\n';
        return EXIT_FAILURE;
    }

    if(!argv[2])
    {
        std::cerr << "Expected filename to search for\n";
        return EXIT_FAILURE;
    }

    // file to search for
    fs::path file_name = argv[2];

    const fs::directory_iterator dir_end; // directory end sentinel

    // used to iterate through each subdirectory of search_dir
    fs::directory_iterator dir_iter(search_dir);

    for(; dir_iter != dir_end; ++dir_iter)
    {
        // skip non directories
        if(!fs::is_directory(dir_iter->path()))
            continue;

        // check directory for file

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
    }

    // Not found
    std::cout << file_name << " was not found in " << search_dir.string() << '\n';

    return EXIT_FAILURE;
}