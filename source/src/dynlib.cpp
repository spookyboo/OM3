/****************************************************************************
**
** Copyright (C) 2016 - 2017
**
** This file is part of the Magus toolkit
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
****************************************************************************/

#include "dynlib.h"

#if WIN32 || WIN64
#  include <windows.h>
#endif

//-----------------------------------------------------------------------
DynLib::DynLib( const std::string& name )
{
    mName = name;
    mInst = NULL;
}

//-----------------------------------------------------------------------
DynLib::~DynLib()
{
}

//-----------------------------------------------------------------------
void DynLib::load()
{
    // Log library load
    std::string name = mName;
    std::wstring stemp;
#if WIN32 || WIN64
    // Although LoadLibraryEx will add .dll itself when you only specify the library name,
    // if you include a relative path then it does not. So, add it to be sure.
    if(name.substr(name.find_last_of(".") + 1) != "dll")
        name += ".dll";

    stemp = std::wstring(name.begin(), name.end());
#endif
    mInst = (DYNLIB_HANDLE)DYNLIB_LOAD( stemp.c_str() );
    LoadLibraryEx( stemp.c_str(), NULL, 0 ); // TEST
}

//-----------------------------------------------------------------------
void DynLib::unload()
{
	if( DYNLIB_UNLOAD( mInst ) )
    {
		// Exception
    }
}

//-----------------------------------------------------------------------
void* DynLib::getSymbol( const std::string& strName ) const throw()
{
    return (void*)DYNLIB_GETSYM( mInst, strName.c_str() );
}

//-----------------------------------------------------------------------
std::wstring DynLib::dynlibError( void )
{
    std::wstring ret;
#if WIN32 || WIN64
    LPTSTR lpMsgBuf; 
    FormatMessage( 
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM | 
        FORMAT_MESSAGE_IGNORE_INSERTS, 
        NULL, 
        GetLastError(), 
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
        (LPTSTR)&lpMsgBuf, 
        0, 
        NULL 
        ); 
    ret = lpMsgBuf;
    // Free the buffer.
    LocalFree( lpMsgBuf );
#endif
	return ret;
}
