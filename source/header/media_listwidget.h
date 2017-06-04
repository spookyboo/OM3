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
#include "plugin_interface.h"

/****************************************************************************
Main class that contains Media widgets. It is typically used to show a list
of media files. It is used to show which media files are part of the workbench
***************************************************************************/
class QListWidgetItem;
class MediaListWidget : public QListWidget
{
    public:
        typedef QMap<QListWidgetItem*, AssetMetaData> AssetMap;

        MediaListWidget (QWidget* parent = 0);
        virtual ~MediaListWidget (void);

        /** Add a resource to this list. Based on its characteristics, the media type is determined
         * and the correct widget is created, which is added to the list
         */
        void addResource (const AssetMetaData& assetMetaData);

        /** Remove a specific resource, identified by assetId
         */
        void removeResourceByAssetId (double assetId);

        /** Remove one or more resource, identified by topLevelPath
         */
        void removeResourcesByTopLevelPath (const QString& topLevelPath);

        /** Remove one or more resource, identified by path
         */
        void removeResourcesByPath (const QString& path);

        /** Remove one or more resource, identified by origin and the toplevel path
        @remarks The difference between the previous function is that multiple plugins
        may have the same path, so more resources are unintendedly removed
        */
        void removeResourcesByOriginAndTopLevelPath (const QString& origin, const QString& topLevelPath);

    signals:
        // Emitted when an asset is added
        void resourceAdded (const AssetMetaData& assetMetaData);

        // Emitted when an asset is deleted
        void resourceDeleted (const AssetMetaData& assetMetaData);

    protected:
        void deleteItem(QListWidgetItem* item);
        //virtual void keyPressEvent(QKeyEvent* event);
        //virtual void dropEvent(QDropEvent* event);
        //virtual void dragEnterEvent(QDragEnterEvent *event);
        //virtual void dragMoveEvent(QDragMoveEvent *event);

    private:
        AssetMap mAssetMap;
};

#endif
