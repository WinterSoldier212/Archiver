#pragma once

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
            cout << outputFileName << endl << endl;
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

    Archiver archive(pathForNewArchive);
    for (const auto& file : files) 
    {
        addFileInArchive(archive, file);
    }
}

void unzip(const po::variables_map& vm)
{
    string archive;
    string outputFileDirectory;

    {
        if (vm.count("archive"))
        {
            archive = vm["archive"].as<string>();
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

    bool archiveHasFile = false;
    Unarchiver unarchiver(archive);
    do {
        archiveHasFile = extractFileFromArchive(unarchiver, outputFileDirectory);
    } while (archiveHasFile);
}

void showHelpOptions(const po::options_description& desc)
{
    cout << desc;
    system("pause>null");
}