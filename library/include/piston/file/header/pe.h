#ifndef PISTON_FILE_HEADER_PE_H
#define PISTON_FILE_HEADER_PE_H

#include <piston/file/header/header.h>

namespace piston
{
    class pe_header : public header
    {
    public:

        class image_file_header : public header
        {
        public:

            /**
             * @brief Deserialize an image file header from the given stream
             * 
             * @param stream Stream from which to deserialize the header data
             * @return true If the data deserialized correctly
             * @return false Otherwise
             */
            virtual bool deserialize(std::istream& stream) override;
        };

        class image_optional_header : public header
        {
        public:

            /**
             * @brief Deserialize an image optional header from the given stream
             * 
             * @param stream Stream from which to deserialize the header data
             * @return true If the data deserialized correctly
             * @return false Otherwise
             */
            virtual bool deserialize(std::istream& stream) override;
        };

        /**
         * @brief Deserialize a PE header from the given stream
         * 
         * @param stream Stream from which to deserialize the header data
         * @return true If the data deserialized correctly
         * @return false Otherwise
         */
        virtual bool deserialize(std::istream& stream) override;
    };
}

#endif // PISTON_FILE_HEADER_PE_H