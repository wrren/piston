#ifndef PISTON_IPC_ROUTER_H
#define PISTON_IPC_ROUTER_H

#include <piston/process/process.h>
#include <piston/ipc/channel.h>
#include <piston/ipc/factory.h>

namespace Piston::IPC
{
    class Router
    {
    public:

        typedef String IDType;
        struct PlatformHandle;

        /**
         * @brief Router event listener.
         * 
         */
        class Listener
        {
        public:

            /**
             * @brief Called when a new channel is opened.
             * 
             * @param channel New channel.
             */
            virtual void OnChannelOpened(const Channel& channel);
        };

        /**
         * @brief Construct a new Router object
         * 
         * @param CurrentProcessID ID of the current process.
         * @param RouterID ID of the Router, will restrict IPC to only other routers with the same ID
         */
        Router(Process::IDType CurrentProcessID, IDType RouterID);

        /**
         * @brief Opens a new channel to the target process or returns the existing channel if it's already been opened.
         * 
         * @param TargetProcessID Target process ID
         * @return channel::ptr_type Channel Pointer
         */
        Channel::PointerType OpenChannel(Process::IDType TargetProcessID);

        /**
         * @brief Pumps the router, which sends outgoing messages and places incoming messages into their
         * corresponding channels' inboxes.
         */
        void Pump();

        /**
         * @brief Add a new router event listener
         * 
         * @param NewListener Listener to be added
         */
        void AddListener(Listener* NewListener);

        /**
         * @brief Remove a router event listener
         * 
         * @param OldListener Listener to be removed
         */
        void RemoveListener(Listener* OldListener);

        /**
         * @brief Destroy the router object
         * 
         */
        ~Router();

    private:

        // Router ID
        IDType mID;
        // ID of this process
        Process::IDType mCurrentProcessID;
        // Router event listeners
        std::vector<Listener*> mListeners;
        // Open channels
        std::vector<Channel::PointerType> mChannels;
        // Platform router handle
        PlatformHandle* mHandle;
    };
}

#endif // PISTON_IPC_ROUTER_H