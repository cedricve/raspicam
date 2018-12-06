/**********************************************************
 Software developed by AVA ( Ava Group of the University of Cordoba, ava  at uco dot es)
 Main author Rafael Munoz Salinas (rmsalinas at uco dot es)
 This software is released under BSD license as expressed below
-------------------------------------------------------------------
Copyright (c) 2013, AVA ( Ava Group University of Cordoba, ava  at uco dot es)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:

   This product includes software developed by the Ava group of the University of Cordoba.

4. Neither the name of the University nor the names of its contributors
   may be used to endorse or promote products derived from this software
   without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY AVA ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL AVA BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
****************************************************************/

#ifndef RaspiCamRawBuffer_H
#define RaspiCamRawBuffer_H

namespace raspicam {
    namespace _private {
        class RaspiCamRawBufferImpl;
    }

    /**
     * Raw internal buffer representation.
     * Provides user with contents of exactly the buffer,
     * camera sensor data has been saved into.
     */
    class RaspiCamRawBuffer {

    public:
        RaspiCamRawBuffer();
        ~RaspiCamRawBuffer();

        /**
         * Copy constructor.
         * Copies reference to internal buffer,
         * increases references counter of buffer to be copied.
         * @param src buffer reference to be copied
         */
        RaspiCamRawBuffer(const RaspiCamRawBuffer& src);

        /**
         * Assignment operator.
         * Copies references to internal buffer,
         * increases references counter of buffer to be copied,
         * decreases reference counter of buffer to be overwritten.
         * @param src buffer reference to be copied
         * @return
         */
        RaspiCamRawBuffer& operator=(const RaspiCamRawBuffer& src);

        /**
         * Move contents
         * Moves reference from source to current reference holder.
         * After move source refers to nullptr.
         * @param src buffer reference to be moved.
         */
        void moveFrom(RaspiCamRawBuffer& src);

        /**
         * Get buffer contents
         * @return buffer contents.
         */
        void* getBuffer();

        /**
         * Get buffer contents, constant implementation.
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

        /**
         * Access to implementation object.
         * @return non-const pointer to implementation object.
         */
        _private::RaspiCamRawBufferImpl* accessImpl() {
            return _impl;
        }

    private:
        _private::RaspiCamRawBufferImpl* _impl;
    };
}

#endif



