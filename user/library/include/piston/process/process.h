#ifndef PISTON_PROCESS_H
#define PISTON_PROCESS_H

#include <piston/core/core.h>
#include <piston/process/memory/region.h>
#include <memory>
#include <vector>
#include <optional>

namespace Piston
{
    class Process
    {
    public:

        typedef uintptr_t AddressType;
        typedef Piston::Platform::ProcessID IDType;
        typedef Piston::Platform::ProcessName NameType;
        typedef std::shared_ptr<Process> PointerType;
        typedef std::vector<PointerType> ListType;

        /**
         * @brief Exception type thrown when a failure occurs during process listing.
         * 
         */
        class ListException : public std::exception
        {
        public:

            virtual const char* what() const noexcept override;
        };

        /**
         * @brief Construct a new process object
         * 
         */
        Process();

		/**
		 * @brief Construct a new process object
		 * 
		 * @param id process ID
		 * @param name process name
		 */
		Process(IDType id, const NameType& name);

		/**
		 * @brief Get the ID of this process.
		 * 
		 * @return Process ID
		 */
		IDType GetID() const;

        /**
         * @brief Get the base address of this process
         * 
         * @return base_address Process base address
         */
        std::optional<AddressType> GetBaseAddress() const;

        /**
         * @brief List regions of memory currently in use by this process.
         * 
         * @return Process memory range
         */
        std::vector<MemoryRegion> ListMemoryRegions() const;

        /**
         * @brief Read data from this process' memory
         * 
         * @param address Address from which the data should be read
         * @param buffer Buffer into which the data should be written
         * @param bytes Number of bytes to read
         * @param bytes_read Number of bytes that were read
         * @return true If the process memory was read successfully
         * @return false Otherwise
         */
        bool ReadMemory(AddressType address, void* buffer, size_t bytes, size_t& bytes_read);

        /**
         * @brief Read data from this process' memory
         * 
         * @param region Region to be read
         * @param buffer Buffer into which the data should be written
         * @param bytes_read Number of bytes that were read
         * @return true If the process memory was read successfully
         * @return false Otherwise
         */
        bool ReadMemory(const MemoryRegion& region, void* buffer, size_t& bytes_read);

        /**
         * @brief Write data to this process' memory
         * 
         * @param address Address to which the data should be written
         * @param buffer Buffer containing the data to be written
         * @param bytes Number of bytes to write
         * @param bytes_written Number if bytes that were written
         * @return true If the process memory was written successfully
         * @return false Otherwise
         */
        bool WriteMemory(AddressType address, void* buffer, size_t bytes, size_t& bytes_written);

        /**
         * @brief Get the current process.
         * 
         * @return ptr_type Pointer to the current process.
         */
        static PointerType CurrentProcess();

        /**
         * @brief Find the process with the specified name.
         * 
         * @param name Name to search for
         * @return ptr_type Pointer to the located process
         */
        static PointerType FindByName(const NameType& name);

		/**
		 * @brief Get the name of this process.
		 * 
		 * @return Process Name
		 */
		const NameType& GetName() const;

        /**
         * @brief List running processes
         * 
         * @return list_type List of running processes.
         */
        static ListType List();

        /**
         * @brief Comparison operator. Compares with a process ID.
         * 
         * @param process_id Process ID
         * @return true If this process object's ID matches the one provided
         * @return false Otherwise
         */
        bool operator==(IDType process_id) const;

    private:

        // Process ID
        IDType mID;
        // Process Name
        NameType mName;
    };
}

#endif // PISTON_PROCESS_H