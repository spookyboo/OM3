/****************************************************************************
**
** Copyright (C) 2017, Henry van Merode
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

#ifndef __OM3_RESOURCE_PLUGIN_H__
#define __OM3_RESOURCE_PLUGIN_H__

#include "plugin_interface.h"

/****************************************************************************
 Defines the interface of a resource plugin. Resources are file system, subversion
 git, database, etc.
 ***************************************************************************/
class QWidget;
class MediaWidget;
class _OM3Export PluginResourceProviderInterface : public PluginInterface
{
    public:
        PluginResourceProviderInterface (void) {}
        virtual ~PluginResourceProviderInterface (void) {}

        /** @see PluginInterface
        */
        virtual const std::string& getType (void) const {return PLUGIN_TYPE_RESOURCE;}

        /** Pull is a generic way to retrieve a resource from the resource provider.
         * The resource provider 'knows'  its resource and the way resources are extracted.
         * The result of a 'pull' is a copy of the resource on the filesystem. The
         * location of the file is updated in the assetMetaData (fullQualifiedFileNamePulled).
         * @param assetMetaData
         * @return bool, indicating wether the pull was succesful.
        */
        virtual bool pull (AssetMetaData* assetMetaData) = 0;

        /** Push is a generic way to store a resource to a location determined by the resource
         * provider on the location defined by fullQualifiedFileNameOrReference. The resource
         * provider also checks whether the originals resource was already updated.
         * The result of a succesful 'push' is that the original resource is replaced by the new
         * version.
         * @param assetMetaData
         * @return bool, indicating wether the push was succesful.
        */
        virtual bool push (AssetMetaData* assetMetaData) = 0;
};

#endif
