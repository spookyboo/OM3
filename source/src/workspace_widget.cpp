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

// Include
#include <QMessageBox>
#include <QMouseEvent>
#include <QMimeData>
#include "workspace_widget.h"
#include "assets_dockwidget.h"

//****************************************************************************/
WorkspaceWidget::WorkspaceWidget (AssetsDockWidget* assetsDockWidget, QWidget* parent) :
    MediaListWidget (parent),
    mAssetsDockWidget (assetsDockWidget)
{
    // Define the (supported or unsupported) mimetypes of html pages
    mMimeType = "";
    mMimeTypeMap["youtube.com"] = "video/x-youtube";
    mMimeTypeMap["vimeo.com"] = "video/x-vimeo";

    setAcceptDrops(true);
    setDropIndicatorShown(true);
    connect(mContextMenu, SIGNAL(triggered(QAction*)), this, SLOT(handleContextMenuItemSelected(QAction*)));
}

//****************************************************************************/
WorkspaceWidget::~WorkspaceWidget (void)
{
}

//****************************************************************************/
void WorkspaceWidget::mousePressEvent (QMouseEvent* e)
{
    if (e->button() == Qt::RightButton)
    {
        QPoint pos;
        pos.setX(e->screenPos().x());
        pos.setY(e->screenPos().y());
        buildContextmenu (); // (Re)build the context menu, because it is very dynamic
        mContextMenu->popup(pos); // Open context menu
    }
    MediaListWidget::mousePressEvent(e); // Passthrough, so selecting an item still works
}

//****************************************************************************/
void WorkspaceWidget::handleContextMenuItemSelected (QAction* action)
{
    emit contextMenuItemSelected (action);
}

//****************************************************************************/
void WorkspaceWidget::dragEnterEvent (QDragEnterEvent * event)
{
    event->acceptProposedAction();
}

//****************************************************************************/
void WorkspaceWidget::dragMoveEvent (QDragMoveEvent* event)
{
    event->acceptProposedAction();
}

//****************************************************************************/
void WorkspaceWidget::dropEvent (QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasHtml())
    {
        // It contains HTML, so it must be a webpage
        if (mimeData->hasText())
        {
            // Add it to the workbench
            AssetMetaData assetMetaData;
            assetMetaData.origin = "OM3";
            assetMetaData.assetId = 0; // For now it has no meaning
            assetMetaData.topLevelPath = "";
            assetMetaData.path = "";
            assetMetaData.baseNameOrReference = mimeData->text().toStdString();
            assetMetaData.fullQualifiedFileNameOrReference = mimeData->text().toStdString();
            assetMetaData.fullQualifiedFileNameCopied = assetMetaData.fullQualifiedFileNameOrReference;
            assetMetaData.fullQualifiedFileNamePulled = "";
            assetMetaData.extensionOrMimeType = determineMimeType(mimeData).toStdString();
            //time_t rawTime = info.created().toTime_t();
            //assetMetaData.dateTimeAssetCreation = *gmtime(&rawTime);
            //rawTime = info.lastModified().toTime_t();
            //assetMetaData.dateTimeModified = *gmtime(&rawTime);
            //assetMetaData.byteSize = info.size();
            MediaWidget* mediaWidget = mAssetsDockWidget->createMediaWidget(&assetMetaData);
            addMediaWidget(mediaWidget);
        }
    }
    else if (mimeData->hasText())
    {
        // It must be something else
        // TODO: Implement file drop
    }

    event->acceptProposedAction();
}

//****************************************************************************/
const QString& WorkspaceWidget::determineMimeType (const QMimeData* mimeData)
{
    mMimeType = "text/html";
    QMap<QString, QString>::const_iterator it = mMimeTypeMap.begin();
    QMap<QString, QString>::const_iterator itEnd = mMimeTypeMap.end();
    while (it != itEnd)
    {
        if (mimeData->text().contains(it.key()))
            mMimeType = it.value();

        ++it;
    }
    return mMimeType;
}
