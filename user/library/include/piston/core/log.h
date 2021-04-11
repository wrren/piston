#ifndef PISTON_CORE_LOG_H
#define PISTON_CORE_LOG_H

#include <piston/core/types.h>
#include <piston/core/time.h>
#include <piston/core/format.h>

namespace Piston
{
    class Log
    {
    public:

        enum class LevelType
        {
            Debug,
            Info,
            Warn,
            Error,
            Fatal
        };

        struct EntryType
        {
            LevelType           Level;
            String              Message;
            Time::Timestamp     Timestamp;
        };

        /**
         * @brief Describes the interface for log back-ends that receive
         * log messages and store/output them.
         */
        class Backend
        {
        public:

            typedef std::shared_ptr<Backend> PointerType;

            /**
             * @brief Write a log entry to the back-end.
             * 
             * @param Entry Log entry
             */
            virtual void Write(const EntryType& Entry) = 0;
        };

        /**
         * @brief Get the singleton log instance.
         * 
         * @return Log* Pointer to the singleton instance
         */
        static Log* Instance();

        /**
         * @brief Write a message to the log.
         * 
         * @param Level Message level
         * @param Message Message
         */
        void Write(LevelType Level, const String& Message);

        /**
         * @brief Add a new back-end. This back-end will begin receiving
         * new log messages as soon as they're written to the log.
         * 
         * @param NewBackend Back-end to be added
         */
        void AddBackend(Backend::PointerType NewBackend);

        /**
         * @brief Remove a registered back-end
         * 
         * @param OldBackend Back-end to be removed
         */
        void RemoveBackend(Backend::PointerType OldBackend);

    private:

        // Log Singleton
        static Log* gLogInstance;
        // Log back-ends
        std::vector<Backend::PointerType> mBackends;
    };
}

#define PISTON_LOG(Level, Message) { Piston::Log::Instance()->Write(Level, Message); }
#define PISTON_LOG_DEBUG(...)   PISTON_LOG(Piston::Log::LevelType::Debug,   Piston::Format(__VA_ARGS__))
#define PISTON_LOG_INFO(...)    PISTON_LOG(Piston::Log::LevelType::Info,    Piston::Format(__VA_ARGS__))
#define PISTON_LOG_WARN(...)    PISTON_LOG(Piston::Log::LevelType::Warn,    Piston::Format(__VA_ARGS__))
#define PISTON_LOG_ERROR(...)   PISTON_LOG(Piston::Log::LevelType::Error,   Piston::Format(__VA_ARGS__))
#define PISTON_LOG_FATAL(...)   PISTON_LOG(Piston::Log::LevelType::Fatal,   Piston::Format(__VA_ARGS__))

#endif // PISTON_CORE_LOG_H