#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <filesystem>
#include <piston/process/injector.h>
#include <operations.h>

void usage()
{
	std::cout << "Piston CLI - Usage" << std::endl;
	std::cout << "\tcli --list" << std::endl;
	std::cout << "\tcli --open file" << std::endl;
	std::cout << "\tcli --inject <library path> --into <process id>" << std::endl;

	exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{
	if(argc == 2 && !strcmp(argv[1], "--list"))
	{
		piston::cli::list(std::cout, std::cerr);
	}
	else if(argc == 3 && !strcmp(argv[1], "--open"))
	{
		piston::cli::read_file(std::cout, std::cerr, argv[2]);
	}
	else if(argc == 5 && !strcmp(argv[1], "--inject") && !strcmp(argv[3], "--info") && std::filesystem::exists(argv[2]))
	{
		piston::injector::library_path library_path = argv[2];
		piston::injector::executable_path executable_path = argv[4];

		if(!std::filesystem::exists(executable_path))
		{
			try
			{
				piston::cli::inject_into_process(std::cout, std::cerr, library_path, std::stoi(argv[4]));
			}
			catch(const std::invalid_argument&)
			{
				usage();
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			// TODO: Implement executable injection
		}
	}
	else
	{
		usage();
	}

    return EXIT_SUCCESS;
}