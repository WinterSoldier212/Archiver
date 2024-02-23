#pragma once
#include <iostream>
#include <vector>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

void zip(const po::variables_map& vm)
{
    std::vector<std::string> files;
    std::string outputFileDirectory = ".\\",
        outputFileName = "Archiv.alzip";

    if (vm.count("file"))
        files = vm["file"].as<std::vector <std::string> >();
    else
        throw std::exception("You must specify at least one file to be archived!");

    if (vm.count("output"))
        outputFileName = vm["output"].as<std::string>();

    if (vm.count("dir"))
        outputFileDirectory = vm["dir"].as<std::string>();

    for (const auto& file : files)
        std::cout << file << std::endl;

    std::cout << outputFileDirectory << std::endl;
    std::cout << outputFileName << std::endl;
}

void unzip(const po::variables_map& vm)
{
    std::string outputFileDirectory;

    if (vm.count("dir"))
        outputFileDirectory = vm["dir"].as<std::string>();

    std::cout << outputFileDirectory;
}

void showHelpOptions(const po::options_description& desc)
{
    std::cout << desc;
    system("pause>null");
}