#include <iostream>
#include <vector>
#include <boost/program_options.hpp>
#include "ProgramOptions.h"

namespace po = boost::program_options;



int main()
{
    int argc = 4;
    const char* argv[] = { "Archiver.exe", "--zip", "-f fasdf\\asdfa.txt", "-f asdfasdf\\dfasdfasd.txt"};

    po::options_description desc("Required options");
    desc.add_options()
        ("zip,z", "Main flag-option for zipping file(s).")
        ("unzip,u", "Main flag-option for unzipping the file(s).")
        ;

    po::options_description zip_desc("Zip options");
    zip_desc.add_options()
        ("file,f", po::value<std::vector<std::string> >(), "File that will be zipping. Must be indicated!")
        ("output,o", po::value<std::string>(), "Output file-archive name.")
        ("dir,d", po::value<std::string>(), "Directory output file-archive.")
        ;

    po::options_description unzip_desc("Unzip options");
    unzip_desc.add_options()
        ("file,f", po::value<std::vector<std::string> >(), "Archive that will be unzipping. Must be indicated!")
        ("dir,d", po::value<std::string>(), "Directory output files.")
        ;

    po::variables_map vm;

    try 
    {    
        po::store(po::command_line_parser(argc, argv).
                  options(desc).allow_unregistered().run(), vm);
        po::notify(vm);

        if (vm.count("zip")) {
            desc.add(zip_desc);
            po::store(po::parse_command_line(argc, argv, desc), vm);
            zip(vm);
        } 
        else if (vm.count("unzip")) {
            desc.add(unzip_desc);
            po::store(po::parse_command_line(argc, argv, desc), vm);
            unzip(vm);
        }
        else {
            desc.add(zip_desc).add(unzip_desc);
            showHelpOptions(desc);
        }
    }
    catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }

    return 0;
}