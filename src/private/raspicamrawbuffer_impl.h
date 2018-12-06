//
// Created by Stepan Dyatkovskiy on 12/6/18.
//


#ifndef RaspiCamRawBuffer_Impl_H
#define RaspiCamRawBuffer_Impl_H

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
             * Constructs raw buffer reference wrapper object.
             * Note here we don't count
             * @param buffer
             */
            explicit RaspiCamRawBufferImpl();

            /**
             * Disposes reference wrapper.
             * If buffer has been acquired (reference been coutned for this wrapper),
             * then buffer is released.
             */
            ~RaspiCamRawBufferImpl();

            /**
             * Copy constructor.
             * Copies reference to internal buffer,
             * increases references counter of buffer to be copied.
             * @param src buffer reference to be copied
             */
            RaspiCamRawBufferImpl(const RaspiCamRawBufferImpl& src);

            /**
             * Assignment operator.
             * Copies references to internal buffer,
             * increases references counter of buffer to be copied,
             * decreases reference counter of buffer to be overwritten.
             * @param src buffer reference to be copied
             * @return reference to this object.
             */
            RaspiCamRawBufferImpl& operator=(const RaspiCamRawBufferImpl& src);

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
            void setMmalBufferHeader(MMAL_BUFFER_HEADER_T *buffer, bool acquire = true);

            /**
             * Get buffer contents
             * @return buffer contents.
             */
            void* getBuffer();

            /**
             * Lock buffer's memory.
             */
            void lock();

            /**
             * Unlocks buffer's memory.
             */
            void unlock();

        private:

            void dispose();
            void construct(const RaspiCamRawBufferImpl& src);

            /**
             * Buffer we're going to wrap by this impl object.
             */
            MMAL_BUFFER_HEADER_T* _buffer;

            /**
             * True if 'acquire' operation has been performed, and
             * 'release' operation is required on wrapper dispose stage.
             */
            bool _acquired;
        };
    }
}

#endif



