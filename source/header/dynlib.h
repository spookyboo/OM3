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

#ifndef _DYNLIB_H__
#define _DYNLIB_H__

#include "prerequisites.h"
#include "constants.h"
#include "plugin_interface.h"
#include <string>

#if WIN32 || WIN64
#    define DYNLIB_HANDLE hInstance
#    define DYNLIB_LOAD( a ) LoadLibraryEx( a, NULL, 0 ) // we can not use LOAD_WITH_ALTERED_SEARCH_PATH with relative paths
#    define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#    define DYNLIB_UNLOAD( a ) !FreeLibrary( a )
#endif

struct HINSTANCE__;
typedef struct HINSTANCE__* hInstance;

/****************************************************************************
* Wrapper of a platform-specific library. This is a .dll on Windows and a .so
* on Linux. Note, that only the Windows part implementation is present.
*
* Each library must implement the external functions 'createPlugin' and
* 'deletePlugin', getting a reference to the Ogam application by means of
* passing a pointer to the 'AssetsDockWidget' instance
***************************************************************************/
class _OgamExport DynLib
{
    protected:
		std::string mName;

        /// Gets the last loading error
        std::wstring dynlibError(void);
    
	public:
        /** Default constructor - used by DynLibManager.
            @warning
                Do not call directly
        */
        DynLib( const std::string& name );

        /** Default destructor.
        */
        ~DynLib();

        /** Load the library
        */
        void load();

        /** Unload the library
        */
        void unload();

        /// Get the name of the library
        const std::string& getName(void) const { return mName; }

        /**
            Returns the address of the given symbol from the loaded library.
            @param
                strName The name of the symbol to search for
            @return
                If the function succeeds, the returned value is a handle to
                the symbol.
            @par
                If the function fails, the returned value is <b>NULL</b>.

        */
        void* getSymbol( const std::string& strName ) const throw();

    protected:
        /// Handle to the loaded library.
        DYNLIB_HANDLE mInst;
};

#endif
