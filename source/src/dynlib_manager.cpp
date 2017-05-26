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

#include "dynlib_manager.h"

//-----------------------------------------------------------------------
template<> DynLibManager* Singleton<DynLibManager>::msSingleton = 0;
DynLibManager* DynLibManager::getSingletonPtr(void)
{
    return msSingleton;
}

//-----------------------------------------------------------------------
DynLibManager& DynLibManager::getSingleton(void)
{
    return ( *msSingleton );
}

//-----------------------------------------------------------------------
DynLibManager::DynLibManager()
{
}

//-----------------------------------------------------------------------
DynLib* DynLibManager::load (const std::string& filename)
{
    DynLibList::iterator i = mLibList.find(filename);
    if (i != mLibList.end())
    {
        return i->second;
    }
    else
    {
        DynLib* pLib = new DynLib(filename);
        pLib->load();
        mLibList[filename] = pLib;
        return pLib;
    }
}

//-----------------------------------------------------------------------
void DynLibManager::unload (DynLib* lib)
{
    DynLibList::iterator i = mLibList.find(lib->getName());
    if (i != mLibList.end())
    {
        mLibList.erase(i);
    }
    lib->unload();
    delete lib;
}

//-----------------------------------------------------------------------
DynLibManager::~DynLibManager()
{
    // Unload & delete resources in turn
    for( DynLibList::iterator it = mLibList.begin(); it != mLibList.end(); ++it )
    {
        it->second->unload();
        delete it->second;
    }

    // Empty the list
    mLibList.clear();
}
