#pragma once

#include <vector>
#include <boost/program_options.hpp>
#include "Archiver.h"
#include "Unarchiver.h"

namespace po = boost::program_options;

vector<string> getFilesFromVM(const po::variables_map& vm)
{
    if (vm.count("file"))
        return vm["file"].as<vector <string> >();
    else
        throw exception("You must specify at least one file to be archived!");
}

string getArchiveFromVM(const po::variables_map& vm)
{
    if (vm.count("archive"))
        return vm["archive"].as<string>();
    else
        throw exception("You must specify the archive that needs to be unpacked!");
}

string getOutputFileDirectoryFromVM(const po::variables_map& vm)
{
    if (vm.count("output"))
        return vm["output"].as<string>();
    else
        return "Archive";
}

string getOutputFileNameFromVM(const po::variables_map& vm)
{
    if (vm.count("dir"))
        return vm["dir"].as<string>();
    else
        return getCurrentDirectory();
}

void zip(const po::variables_map& vm)
{
    vector<string> files = getFilesFromVM(vm);
    string outputFileDirectory = getOutputFileDirectoryFromVM(vm);
    string outputFileName = getOutputFileNameFromVM(vm);
   
    string pathForNewArchive = outputFileDirectory + outputFileName;
    
    Archiver archive(pathForNewArchive);
    for (const auto& file : files) {
        addFileInArchive(archive, file);
    }
}

void unzip(const po::variables_map& vm)
{
    string archive = getArchiveFromVM(vm);
    string outputFileDirectory = getOutputFileDirectoryFromVM(vm);

    Unarchiver unarchiver(archive);
    while (true) {
        getFileFromArchive(unarchiver, outputFileDirectory);
    }
}

void showHelpOptions(const po::options_description& desc)
{
    cout << desc;
    system("pause>null");
}