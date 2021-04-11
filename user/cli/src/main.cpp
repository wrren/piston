#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <filesystem>
#include <piston/process/injector.h>
#include <piston/process/memory/scanner.h>
#include <piston/ipc/router.h>
#include <piston/ipc/messages/log.h>
#include <piston/ipc/messages/connect.h>
#include <piston/ipc/messages/scan_request.h>
#include <piston/ipc/messages/scan_response.h>
#include <piston/ipc/messages/write_request.h>
#include <piston/ipc/messages/write_response.h>
#include <operations.h>
#include <thread>
#include <regex>

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
		return Piston::Process::FindByName(Piston::Strings::StringToWideString(IDOrName));	
	}
}

int main(int argc, char** argv)
{
	Piston::Log::Instance()->AddBackend(
		Piston::Log::Backend::PointerType(new Piston::ConsoleBackend())
	);

	PISTON_LOG_DEBUG("My process ID: ", Piston::Process::CurrentProcess()->GetID());

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
		std::regex SetHealthRegex("set health ([0-9]{1,3})");
		std::regex ScanForRegex("scan for ([0-9]{1,3})");
		std::regex ReScanForRegex("rescan for ([0-9]{1,3})");
		std::regex WriteRegex("write ([0-9]{1,3}) to ([0-9]+)");
		std::smatch Matches;
		Piston::MemoryRegion::ListType ScannedRegions;

		uint32_t Health = 100;
		Piston::String Input;

		PISTON_LOG_DEBUG("Target Address: ", Piston::MemoryRegion::AddressType(&Health));

		while(true)
		{
			std::cout << "Health: " << Health << std::endl;
			std::getline(std::cin, Input);

			if(Input == "q")
			{
				break;
			}
			else if(Input == "get health")
			{
				continue;
			}
			else if(std::regex_match(Input, Matches, SetHealthRegex) && Matches.size() == 2)
			{
				Health = std::stoi(Matches[1]);
			}
			else if(std::regex_match(Input, Matches, ScanForRegex) && Matches.size() == 2)
			{
				uint32_t Target = std::stoi(Matches[1]);
				auto ThisProcess = Piston::Process::CurrentProcess();
				ScannedRegions = ThisProcess->ListMemoryRegions();
				Piston::MemoryValueScanner Scanner(Target);
				Piston::MemoryRegion::ListType Hits;

				for(auto Region : ScannedRegions)
				{
					std::unique_ptr<Piston::byte> Buffer(new Piston::byte[Region.GetSize()]);
					size_t BytesRead;
					if(ThisProcess->ReadMemory(Region, Buffer.get(), BytesRead) && BytesRead > 0)
					{
						for(auto Hit : Scanner.Scan(Buffer.get(), BytesRead))
						{
							Hits.push_back(Piston::MemoryRegion(Region.GetStartAddress() + Hit, sizeof(Target)));
							PISTON_LOG_DEBUG("Hit! ", Region.GetStartAddress() + Hit);
						}
					}
				}

				ScannedRegions = Hits;
			}
			else if(std::regex_match(Input, Matches, ReScanForRegex) && Matches.size() == 2)
			{
				uint32_t Target = std::stoi(Matches[1]);
				auto ThisProcess = Piston::Process::CurrentProcess();
				Piston::MemoryValueScanner Scanner(Target);
				Piston::MemoryRegion::ListType Hits;

				for(auto Region : ScannedRegions)
				{
					std::unique_ptr<Piston::byte> Buffer(new Piston::byte[Region.GetSize()]);
					size_t BytesRead;
					if(ThisProcess->ReadMemory(Region, Buffer.get(), BytesRead) && BytesRead > 0)
					{
						for(auto Hit : Scanner.Scan(Buffer.get(), BytesRead))
						{
							Hits.push_back(Piston::MemoryRegion(Region.GetStartAddress() + Hit, sizeof(Target)));
							PISTON_LOG_DEBUG("Hit! ", Region.GetStartAddress() + Hit);
						}
					}
					else
					{
						PISTON_LOG_ERROR("Failed to read region ", Region.GetStartAddress(), " (", Region.GetSize(), " Bytes)");
					}

					ScannedRegions = Hits;
				}
			}
			else if(std::regex_match(Input, Matches, WriteRegex) && Matches.size() == 3)
			{
				uint32_t Value = std::stoi(Matches[1]);
				auto TargetAddress = std::stoull(Matches[2]);

				Piston::MemoryRegion TargetRegion(TargetAddress, sizeof(Value));
				auto ThisProcess = Piston::Process::CurrentProcess();
				size_t BytesWritten;
				if(ThisProcess->WriteMemory(TargetAddress, &Value, sizeof(Value), BytesWritten))
				{
					PISTON_LOG_DEBUG("Write successful.");
				}
				else
				{
					PISTON_LOG_ERROR("Write failed!");
				}
			}
			else
			{
				std::cout << "Huh?" << std::endl;
			}
		}	
	}
	else if(argc >= 4 && !strcmp(argv[1], "command"))
	{
		Piston::IPC::Router::IDType RouterID = argv[2];
		auto ThisProcess = Piston::Process::CurrentProcess();
		Piston::IPC::Router Router(ThisProcess->GetID(), RouterID);
		Piston::IPC::LogMessage::RegisterWith(Router.GetMessageFactory());
		Piston::IPC::ConnectMessage::RegisterWith(Router.GetMessageFactory());
		Piston::IPC::ScanRequest::RegisterWith(Router.GetMessageFactory());
		Piston::IPC::ScanResponse::RegisterWith(Router.GetMessageFactory());
		Piston::IPC::WriteRequest::RegisterWith(Router.GetMessageFactory());
		Piston::IPC::WriteResponse::RegisterWith(Router.GetMessageFactory());

		auto TargetProcess = GetProcessByIDOrName(argv[3]);

		if(!TargetProcess)
		{
			usage();
		}

		auto Channel = Router.OpenChannel(TargetProcess->GetID());
		Channel->Send(Piston::IPC::Message::PointerType(new Piston::IPC::ConnectMessage()));

		std::regex ScanForRegex("scan for ([0-9]{1,3})");
		std::regex ReScanForRegex("rescan for ([0-9]{1,3})");
		std::regex WriteRegex("write ([0-9]{1,3}) to ([0-9]+)");
		std::smatch Matches;
		Piston::MemoryRegion::ListType ScannedRegions;

		Piston::String Input;

		while(true)
		{
			std::getline(std::cin, Input);

			if(Input == "q")
			{
				break;
			}
			else if(std::regex_match(Input, Matches, ScanForRegex) && Matches.size() == 2)
			{
				uint32_t Target = std::stoi(Matches[1]);
				
				Channel->Send(Piston::IPC::Message::PointerType(
					new Piston::IPC::ScanRequest(Piston::IPC::ScanRequest::ScanType::FullScan, reinterpret_cast<byte*>(&Target), sizeof(Target))
				));

				Piston::IPC::Message::PointerType Message;

				while(true)
				{
					Router.Pump();
					Message = Channel->Receive();

					if(!Message)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					}
					else
					{
						break;
					}
				}

				PISTON_LOG_DEBUG("Received scan response!");

				if(Message->GetCommand() != Piston::IPC::Commands::SCAN_RESPONSE)
				{
					PISTON_LOG_ERROR("Unexpected message type: ", Message->GetCommand());
					break;
				}

				auto Response = std::reinterpret_pointer_cast<Piston::IPC::ScanResponse>(Message);
				ScannedRegions = Response->GetResults();

				for(auto& Hit : ScannedRegions)
				{
					PISTON_LOG_DEBUG("Hit: ", Hit.GetStartAddress());
				}
			}
			else if(std::regex_match(Input, Matches, ReScanForRegex) && Matches.size() == 2)
			{
				uint32_t Target = std::stoi(Matches[1]);
				
				Channel->Send(Piston::IPC::Message::PointerType(
					new Piston::IPC::ScanRequest(Piston::IPC::ScanRequest::ScanType::ReScan, reinterpret_cast<byte*>(&Target), sizeof(Target), ScannedRegions)
				));

				Piston::IPC::Message::PointerType Message;

				while(true)
				{
					Router.Pump();
					Message = Channel->Receive();

					if(!Message)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					}
					else
					{
						break;
					}
				}

				if(Message->GetCommand() != Piston::IPC::Commands::SCAN_RESPONSE)
				{
					PISTON_LOG_ERROR("Unexpected message type: ", Message->GetCommand());
					break;
				}

				auto Response = std::reinterpret_pointer_cast<Piston::IPC::ScanResponse>(Message);
				ScannedRegions = Response->GetResults();

				for(auto& Hit : ScannedRegions)
				{
					PISTON_LOG_DEBUG("Hit: ", Hit.GetStartAddress());
				}
			}
			else if(std::regex_match(Input, Matches, WriteRegex) && Matches.size() == 3)
			{
				uint32_t Value = std::stoi(Matches[1]);
				auto TargetAddress = std::stoull(Matches[2]);

				Channel->Send(Piston::IPC::Message::PointerType(new Piston::IPC::WriteRequest(
					TargetAddress, reinterpret_cast<byte*>(&Value), sizeof(Value)
				)));

				Piston::IPC::Message::PointerType Message;

				while(true)
				{
					Router.Pump();
					Message = Channel->Receive();

					if(!Message)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					}
				}

				if(Message->GetCommand() != Piston::IPC::Commands::WRITE_RESPONSE)
				{
					PISTON_LOG_ERROR("Unexpected message type: ", Message->GetCommand());
					break;
				}

				auto Response = std::reinterpret_pointer_cast<Piston::IPC::WriteResponse>(Message);

				if(Response->Succeeded())
				{
					PISTON_LOG_DEBUG("Write Succeeded");
				}
				else
				{
					PISTON_LOG_ERROR("Write Failed");
				}
				
			}
			else
			{
				std::cout << "Huh?" << std::endl;
			}
		}			
	}
	else if(argc >= 4 && !strcmp(argv[1], "listen"))
	{
		Piston::IPC::Router::IDType RouterID = argv[2];
		auto ThisProcess = Piston::Process::CurrentProcess();
		Piston::IPC::Router Router(ThisProcess->GetID(), RouterID);
		Piston::IPC::LogMessage::RegisterWith(Router.GetMessageFactory());
		Piston::IPC::ConnectMessage::RegisterWith(Router.GetMessageFactory());

		if(argc >= 4)
		{
			auto TargetProcess = GetProcessByIDOrName(argv[3]);

			if(!TargetProcess)
			{
				usage();
			}

			auto Channel = Router.OpenChannel(TargetProcess->GetID());
			Channel->Send(Piston::IPC::Message::PointerType(new Piston::IPC::ConnectMessage()));
		}
		
		size_t ChannelCount = Router.ListOpenChannels().size();

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