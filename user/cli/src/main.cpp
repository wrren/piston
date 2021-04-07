#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <filesystem>
#include <piston/process/injector.h>
#include <piston/ipc/router.h>
#include <piston/ipc/messages/log_message.h>
#include <piston/ipc/messages/connect.h>
#include <operations.h>
#include <thread>

void usage()
{
	std::cout << "Piston CLI - Usage" << std::endl;
	std::cout << "\tcli list" << std::endl;
	std::cout << "\tcli open file" << std::endl;
	std::cout << "\tcli load library" << std::endl;
	std::cout << "\tcli listen router-id" << std::endl;
	std::cout << "\tcli inject <library path> --into <process id>" << std::endl;

	exit(EXIT_FAILURE);
}

Piston::Process::PointerType GetProcessByIDOrName(const Piston::String& IDOrName)
{
	try
	{
		Piston::Process::IDType ProcessID = std::stoi(IDOrName);
		return Piston::Process::FindByID(ProcessID);
	}
	catch(const std::invalid_argument&)
	{
		return Piston::Process::FindByName(Piston::Strings::StringToWideString(IDOrName));	}
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
	else if(argc >= 2 && !strcmp(argv[1], "target"))
	{
		uint32_t Health = 100;
		while(true)
		{
			std::cout << "Update Health Value (Currently " << Health << ")" << std::endl;
			std::cin >> Health;
		}	
	}
	else if(argc >= 4 && !strcmp(argv[1], "listen"))
	{
		Piston::IPC::Router::IDType RouterID = argv[2];
		auto ThisProcess = Piston::Process::CurrentProcess();
		Piston::IPC::Router Router(ThisProcess->GetID(), RouterID);
		Piston::IPC::LogMessage::RegisterWith(Router.GetMessageFactory());

		auto TargetProcess = GetProcessByIDOrName(argv[3]);

		if(!TargetProcess)
		{
			usage();
		}

		auto Channel = Router.OpenChannel(TargetProcess->GetID());
		Channel->Send(Piston::IPC::Message::PointerType(new Piston::IPC::ConnectMessage()));

		size_t ChannelCount = 1;

		while(true)
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			Router.Pump();

			auto OpenChannels = Router.ListOpenChannels();

			if(OpenChannels.size() != ChannelCount)
			{
				ChannelCount = OpenChannels.size();
				std::cout << "Channel Count: " << ChannelCount << std::endl;
			}

			for(auto Channel : OpenChannels)
			{
				while(auto Message = Channel->Receive())
				{
					if(Message->GetCommand() == Piston::IPC::LogMessage::Command)
					{
						auto LogMessage = std::reinterpret_pointer_cast<Piston::IPC::LogMessage>(Message);

						if(LogMessage)
						{
							std::cout << "Received log message from process " << LogMessage->GetSourceProcessID() << ": " << LogMessage->GetMessage() << std::endl;
						}
					}
				}
			}
		}
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
			auto TargetProcess = GetProcessByIDOrName(argv[4]);

			if(TargetProcess)
			{
				Piston::cli::inject_into_process(std::cout, std::cerr, library_path, TargetProcess->GetID());
			}
			else
			{
				usage();
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