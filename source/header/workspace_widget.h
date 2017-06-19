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

#ifndef WORKSPACE_WIDGET_H
#define WORKSPACE_WIDGET_H

#include "media_listwidget.h"

/****************************************************************************
Sub class of MediaListWidget. Added to prevent nasty linking errors with
signal/slots if they were implemented in MediaListWidget itself.
***************************************************************************/
class AssetsDockWidget;
class WorkspaceWidget : public MediaListWidget
{
    Q_OBJECT

    public:
        WorkspaceWidget (AssetsDockWidget* assetsDockWidget, QWidget* parent = 0);
        virtual ~WorkspaceWidget (void);

        /** Determine the (custom) mimetype, based on QMimeData
        */
        const QString& determineMimeType (const QMimeData* mimeData);

    protected:
        virtual void mousePressEvent (QMouseEvent* e );
        virtual void dragEnterEvent (QDragEnterEvent * event); // MUST be implemented to make drop event working
        virtual void dragMoveEvent (QDragMoveEvent* event); // MUST be implemented to make drop event working
        virtual void dropEvent (QDropEvent *event); // MUST be implemented to make drop event working

    protected slots:
        void handleContextMenuItemSelected(QAction* action);

    signals:
        /** Emitted when an menu item from the contextmenu is selected
        */
        void contextMenuItemSelected (QAction* action);

    private:
        AssetsDockWidget* mAssetsDockWidget;
        QString mMimeType;
        QMap<QString, QString> mMimeTypeMap;
};

#endif
