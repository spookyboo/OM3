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

#ifndef MEDIA_LISTWIDGET_H
#define MEDIA_LISTWIDGET_H

#include <QListWidget>
#include <QMap>
#include <QMenu>
#include "plugin_interface.h"

/****************************************************************************
Main class that displays a list of media files, represented by means of
MediaWidgets. It is used in OM3's workspace and in the
OM3ResourceProviderFilePlugin.

Note, that the MediaListWidget is used as a convenient widget to display
MediaWidgets. It is not mandatory to use the MediaListWidget class in a
resource provider plugin that wants to display MediaWidgets. MediaWidgets
can also be displayed in a custom widget. It is up to the resource provider
plugin developer how to implement this.
***************************************************************************/
class QListWidgetItem;
class MediaWidget;
class MediaListWidget : public QListWidget
{
    public:
        // Enums and typedefs
        enum ContextMenuContext
        {
            CONTEXT_MEDIA_ITEMS_SELECTED,
            CONTEXT_OVERALL
        };
        typedef QMap<QString, ContextMenuContext> ContextMap;
        typedef QMap<QListWidgetItem*, AssetMetaData> AssetMap;

        // Constructor/destructor
        MediaListWidget (QWidget* parent = 0);
        virtual ~MediaListWidget (void);

        /** Add a Media Widget to this list.
         */
        virtual void addMediaWidget (MediaWidget* mediaWidget);

        /** Remove a specific MediaWidget, identified by assetId
         */
        virtual void removeMediaWidgetByAssetId (double assetId);

        /** Remove one or more MediaWidgets, identified by topLevelPath
         */
        virtual void removeMediaWidgetsByTopLevelPath (const QString& topLevelPath);

        /** Remove one or more MediaWidgets, identified by path
         */
        virtual void removeMediaWidgetsByPath (const QString& path);

        /** Remove one or more MediaWidget, identified by origin and the toplevel path
        @remarks The difference between the previous function is that multiple plugins
        may have the same path, so more MediaWidget are unintendedly removed
        */
        virtual void removeMediaWidgetsByOriginAndTopLevelPath (const QString& origin, const QString& topLevelPath);

        /** Delete one item, including the associated widget from the MediaListWidget
        */
        virtual void deleteItem(QListWidgetItem* item);

        /** Add an item to the contextmenu. The menu item is only visible if it meets the criteria of the context.
         * E.g. if an actionText is passed with the CONTEXT_MEDIA_ITEMS_SELECTED, then this menu item is only
         * visible when MediaWidgets are selected.
         * If the item is selected, the contextMenuItemSelected event is emitted.
        */
        virtual void addContextMenuItem (const QString& actionText, ContextMenuContext context);

        /** Filter the mediawidgets on a list of 1 or more tags
        */
        virtual void filterOnTags (const QVector<QString>& tags);

    signals:
        // Emitted when an asset is added
        void resourceAdded (AssetMetaData* assetMetaData);

        // Emitted when an asset is deleted
        void resourceDeleted (AssetMetaData* assetMetaData);

    protected:
        AssetMap mAssetMap;
        QMenu* mContextMenu;
        ContextMap mContextMap;

        void buildContextmenu (void);
};

#endif
