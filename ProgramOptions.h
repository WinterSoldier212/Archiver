#pragma once

#include <iostream>
#include <vector>
#include <boost/program_options.hpp>
#include "Archive.h"

namespace po = boost::program_options;

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

    Archive archive(outputFileDirectory + outputFileName);
    for (const auto& file : files) {
        cout << file << endl;
        archive.addFile(file);
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