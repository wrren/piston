#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <filesystem>
#include <piston/process/injector.h>
#include <operations.h>

void usage()
{
	std::cout << "Piston CLI - Usage" << std::endl;
	std::cout << "\tcli list" << std::endl;
	std::cout << "\tcli open file" << std::endl;
	std::cout << "\tcli load library" << std::endl;
	std::cout << "\tcli inject <library path> --into <process id>" << std::endl;

	exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{
	if(argc >= 2 && !strcmp(argv[1], "list"))
	{
		Piston::cli::list(std::cout, std::cerr);
	}
	else if(argc >= 3 && !strcmp(argv[1], "open"))
	{
		Piston::cli::read_file(std::cout, std::cerr, argv[2]);
	}
	else if(argc >= 3 && !strcmp(argv[1], "load"))
	{
		Piston::cli::load(std::cout, std::cerr, argv[2]);
	}
	else if(argc >= 5 && !strcmp(argv[1], "inject") && !strcmp(argv[3], "--into"))
	{
		Piston::Injector::LibraryPath library_path = argv[2];
		Piston::Injector::ExecutablePath executable_path = argv[4];
		
		if(!std::filesystem::exists(library_path))
		{
			std::cerr << "The specified library could not be found." << std::endl;
			return EXIT_FAILURE;
		}

		if(!std::filesystem::exists(executable_path))
		{
			try
			{
				Piston::Process::IDType process_id;

				auto process = Piston::Process::FindByName(argv[4]);
				if(process)
				{
					process_id = process->GetID();
				}
				else
				{
					size_t index;
					process_id = std::stoi(argv[4], &index);
					std::cout << index << ", " << argv[4] << std::endl;
				}
				
				Piston::cli::inject_into_process(std::cout, std::cerr, library_path, process_id);
			}
			catch(const std::invalid_argument&)
			{
				usage();
			}
			catch(const std::exception& e)
			{
				usage();
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			Piston::Injector::ArgumentList arguments;

			for(int i = 5; i < argc; i++)
			{
				arguments.push_back(argv[i]);
			}

			try
			{
				Piston::cli::inject_into_executable(std::cout, std::cerr, library_path, executable_path, arguments);
			}
			catch(const std::invalid_argument&)
			{
				
			}
			catch(const std::exception& e)
			{
				usage();
				std::cerr << e.what() << std::endl;
			}
		}
	}
	else
	{
		usage();
	}

    return EXIT_SUCCESS;
}