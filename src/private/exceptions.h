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

#ifndef _RaspiCam__Exceptions_h
#define _RaspiCam__Exceptions_h
#include <string>
#include <fstream> 
#include <cstdarg>
namespace raspicam {

/**
 * Types of valid exceptions
 */
class   Exceptions {
public:
    static const int Generic=81799; 
};




/*!
 The standard   exception class.
 */
class   Exception : public std::exception
{
public:
    /*!
     Default constructor
     */
    Exception() {
        code = 0;
        line = 0;
    }
    /*!
     Full constructor. Normally the constuctor is not called explicitly.
     Instead, the macros CV_Error(), CV_Error_() and CV_Assert() are used.
    */
    Exception(int _code, const std::string& _err, const std::string& _func, const std::string& _file, int _line)
            : code(_code), err(_err), func(_func), file(_file), line(_line)
    {
        formatMessage();
    }
    virtual ~Exception() throw() {}

    /*!
     \return the error description and the context as a text string.
    */
    virtual const char *what() const throw() {
        return msg.c_str();
    }
    void formatMessage()
    {
        if ( func.size() > 0 )
            msg = format("%s:%d: error: (%d) %s in function %s\n", file.c_str(), line, code, err.c_str(), func.c_str());
        else
            msg = format("%s:%d: error: (%d) %s\n", file.c_str(), line, code, err.c_str());
    }

    std::string msg; ///< the formatted error message

    int code; ///< error code @see CVStatus
    std::string err; ///< error description
    std::string func; ///< function name. Available only when the compiler supports __func__ macro
    std::string file; ///< source file name where the error has occured
    int line; ///< line number in the source file where the error has occured

private:
    std::string format( const char* fmt, ... )
    {
        char buf[1 << 16];
        va_list args;
        va_start( args, fmt );
        vsprintf( buf, fmt, args );
        return std::string(buf);
    }
};

}
#endif
