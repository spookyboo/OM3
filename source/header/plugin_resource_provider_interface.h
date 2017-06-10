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

        /** Pass the information of an asset from the resource provider plugin to the main
         * OM3 application. As a result, a pointer to a created MediaWidget is returned,
         * which should be used by the plugin to display the asset.
        */
        virtual MediaWidget* createMediaWidget (const AssetMetaData& assetMetaData, QWidget* parent = 0) = 0;
};

#endif
