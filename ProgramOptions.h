#pragma once

#include <iostream>
#include <vector>
#include <boost/program_options.hpp>
#include "Archiver.h"

namespace po = boost::program_options;

void addFileInArchive(Archiver& archive, const std::string& pathForFile)
{
    try
    {
        cout << "Trying to write a file - " << pathForFile << endl;
        archive.addFile(pathForFile);
        cout << "The file has been successfully archived!" << endl << endl;
    }
    catch (exception& ex)
    {
        cout << "Error! " << ex.what() << endl << endl;
    }
}

void zip(const po::variables_map& vm)
{
    std::vector<std::string> files;
    std::string outputFileDirectory = "C:\\Users\\2020k\\source\\repos\\Archiver\\",
        outputFileName = "Archiv";

    if (vm.count("file"))
        files = vm["file"].as<std::vector <std::string> >();
    else
        throw std::exception("You must specify at least one file to be archived!");

    if (vm.count("output"))
        outputFileName = vm["output"].as<std::string>();

    if (vm.count("dir"))
        outputFileDirectory = vm["dir"].as<std::string>();

    Archiver archive(outputFileDirectory + outputFileName);
    for (const auto& file : files) {
        addFileInArchive(archive, file);
    }
}

void unzip(const po::variables_map& vm)
{
    std::vector<std::string> archives;
    std::string outputFileDirectory = ".\\";

    if (vm.count("file"))
        archives = vm["file"].as<std::vector <std::string> >();
    else
        throw std::exception("You must specify at least one file to be archived!");

    if (vm.count("dir"))
        outputFileDirectory = vm["dir"].as<std::string>();

    std::cout << outputFileDirectory;
}

void showHelpOptions(const po::options_description& desc)
{
    std::cout << desc;
    system("pause>null");
}