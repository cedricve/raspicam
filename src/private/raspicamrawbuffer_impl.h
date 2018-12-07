//
// Created by Stepan Dyatkovskiy on 12/6/18.
//


#ifndef RaspiCamRawBuffer_Impl_H
#define RaspiCamRawBuffer_Impl_H

#include <memory>

#if 0
    // Enables detailed trace about buffer lifetime.
    #define RASPICAM_RAW_BUFFER_ENABLE_TRACE
#endif

struct MMAL_BUFFER_HEADER_T;

namespace raspicam {
    namespace _private {

        /**
         * Raspberry Camera RAW buffer implementation.
         * In fact this is a wrapper for MMAL buffer.
         */
        class RaspiCamRawBufferImpl {
        public:

            /**
             * Move contents
             * Moves reference from source to current reference holder.
             * After move source refers to nullptr.
             * @param src buffer reference to be moved.
             */
            void moveFrom(RaspiCamRawBufferImpl& src);

            /**
             * Setup MMAL buffer header.
             * @param buffer MMAL buffer to be wrapped.
             * @param acquire true if you want acquire/release to be called for this buffer.
             */
            void setMmalBufferHeader(MMAL_BUFFER_HEADER_T *buffer);

            /**
             * Get buffer contents
             * @return buffer contents.
             */
            void* getBuffer();

            /**
             * Get buffer contents
             * @return buffer contents.
             */
            const void* getBuffer() const;

            /**
             * Lock buffer's memory.
             */
            void lock();

            /**
             * Unlocks buffer's memory.
             */
            void unlock();

            long getUseCount() const;

        private:

            struct Deleter {
                void operator()(MMAL_BUFFER_HEADER_T* _buffer) const;
            };

            std::shared_ptr<MMAL_BUFFER_HEADER_T> _buffer;
        };
    }
}

#endif



