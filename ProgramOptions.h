#pragma once

#include <iostream>
#include <vector>
#include <boost/program_options.hpp>
#include "Archiver.h"
#include "Unarchiver.h"

namespace po = boost::program_options;

void zip(const po::variables_map& vm)
{
    vector<string> files;
    string outputFileDirectory;
    string outputFileName;
   
    {
        if (vm.count("file"))
        {
            files = vm["file"].as<vector <string> >();
        }
        else
        {
            throw exception("You must specify at least one file to be archived!");
        }
    }
    {
        if (vm.count("output"))
        {
            outputFileName = vm["output"].as<string>();
        }
        else
        {
            outputFileName = getFileNameFromPath(files.at(0));
        }
    }
    {
        if (vm.count("dir"))
        {
            outputFileDirectory = vm["dir"].as<string>();
        }
        else
        {
            outputFileDirectory = getCurrentDirectory();
        }
    }

    string pathForNewArchive = outputFileDirectory + "\\" + outputFileName;

    Archiver archive;
    archive.Create(pathForNewArchive);
    for (const auto& file : files) 
    {
        AddFileInArchive(archive, file);
    }
}

void unzip(const po::variables_map& vm)
{
    vector<string> archives;
    string outputFileDirectory;

    {
        if (vm.count("archive"))
        {
            archives = vm["archive"].as<std::vector<string> >();
        }
        else
        {
            throw exception("You must specify the archive that needs to be unpacked!");
        }
    }
    {
        if (vm.count("dir"))
        {
            outputFileDirectory = vm["dir"].as<string>();
        }
        else
        {
            outputFileDirectory = getCurrentDirectory();
        }
    }

    for (auto archive : archives)
    {
        Unarchiver unarchiver;
        unarchiver.Open(archive);

        ExtractAllFilesFromArchive(unarchiver, outputFileDirectory);
    }
}

void showHelpOptions(const po::options_description& desc)
{
    cout << desc;
}