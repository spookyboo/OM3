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

#ifndef __OM3_MEDIA_WIDGET_PLUGIN_H__
#define __OM3_MEDIA_WIDGET_PLUGIN_H__

#include "plugin_interface.h"

/****************************************************************************
 Defines the interface of a media widget plugin. A media widget plugin is used
 to show a widget on a MediaListWidget. E.g. a Qt widget that displays an
 audio file - including support to playback the audio file - is wrapped in a
 specific plugin. This makes it possible to create a widget for very custom
 media files.
 ***************************************************************************/
class MediaWidget;
class _OM3Export PluginMediaWidgetInterface : public PluginInterface
{
    public:
        typedef std::vector<std::string> SupportedExtensions;

        PluginMediaWidgetInterface (void) {}
        virtual ~PluginMediaWidgetInterface (void) {}

        /** @see PluginInterface
        */
        virtual const std::string& getType (void) const {return PLUGIN_TYPE_MEDIA_WIDGET;}

        /** Creates a MediaWidget, based on the info of AssetMetaData.
         * @remarks Note, that only a MediaWidget is created when the
         * extension of the asset is supported.
        */
        virtual MediaWidget* createMediaWidget (const AssetMetaData& assetMetaData, QWidget* parent = 0) = 0;

        /** Returns the supported extensions. This information is used by OM3 and
         * OM3 plugins to determine which plugin is needed to display a certain asset.
        */
        virtual const SupportedExtensions& getSupportedExtensions (void) const = 0;
};

#endif
