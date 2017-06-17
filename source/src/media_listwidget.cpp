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
#include <QHBoxLayout>
#include <QPixmap>
#include <QLabel>
#include "media_listwidget.h"
#include "media_widget.h"

//****************************************************************************/
MediaListWidget::MediaListWidget (QWidget* parent) :
    QListWidget (parent)
{
    // Define display mode
    setMouseTracking(true);
    setViewMode(QListView::ListMode);
    setWrapping(true);
    setWordWrap(true);
    setSpacing(0);
    setUniformItemSizes(true);
    setMovement(QListView::Snap);
    setFlow(QListView::LeftToRight);
    setResizeMode(QListView::Adjust);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setDragEnabled(false);
    setDragDropMode(QAbstractItemView::DropOnly); // test
    setDropIndicatorShown(true); // TEST
    //setDragDropOverwriteMode(true); // test

    // Create context menu
    mContextMenu = new QMenu(this);
    setContextMenuPolicy(Qt::CustomContextMenu);
}

//****************************************************************************/
MediaListWidget::~MediaListWidget(void)
{
}

//****************************************************************************/
void MediaListWidget::addMediaWidget (MediaWidget* mediaWidget)
{
    if (!mediaWidget)
        return;

    QListWidgetItem* item = new QListWidgetItem();
    QSize size = mediaWidget->size();
    item->setSizeHint(size); // Must be present, otherwise the mediaWidget is not shown
    addItem(item);
    setItemWidget(item, mediaWidget);
    mAssetMap[item] = mediaWidget->getAssetMetaData();
}

//****************************************************************************/
void MediaListWidget::removeMediaWidgetByAssetId (double assetId)
{
    // TODO
}

//****************************************************************************/
void MediaListWidget::removeMediaWidgetsByPath (const QString& path)
{
    // TODO
}

//****************************************************************************/
void MediaListWidget::removeMediaWidgetsByTopLevelPath (const QString& path)
{
    // TODO
}

//****************************************************************************/
void MediaListWidget::removeMediaWidgetsByOriginAndTopLevelPath (const QString& origin, const QString& topLevelPath)
{
    QListWidgetItem* item;
    AssetMetaData assetMetaData;
    AssetMap::iterator it = mAssetMap.begin();
    AssetMap::iterator itEnd = mAssetMap.end();
    std::string stdOrigin = origin.toStdString();
    std::string stdTopLevelPath = topLevelPath.toStdString();
    while (it != itEnd)
    {
        item = it.key();
        assetMetaData = it.value();
        if (assetMetaData.origin == stdOrigin && assetMetaData.topLevelPath == stdTopLevelPath)
        {
            it = mAssetMap.erase(it);
            deleteItem(item);
        }
        else
        {
            ++it;
        }
    }
}

//****************************************************************************/
void MediaListWidget::deleteItem(QListWidgetItem* item)
{
    QWidget* widget = itemWidget(item);
    int r = row(item);
    removeItemWidget(item);
    takeItem(r);
    delete widget;
    delete item;
}

//****************************************************************************/
void MediaListWidget::addContextMenuItem (const QString& actionText, MediaListWidget::ContextMenuContext context)
{
    //mContextMenu->addAction(new QAction(actionText));
    mContextMap[actionText] = context;
}

//****************************************************************************/
void MediaListWidget::filterOnTags (const QVector<QString>& tags)
{
    // TODO
    // - Make the MediaWidgets that comply to the tag vector visible. If they not comply, make them invisible
}

//****************************************************************************/
void MediaListWidget::buildContextmenu (void)
{
    // First, remove all actions from the contextmenu
    mContextMenu->clear();

    // Show all items in the context menu that are in the context of selected items
    bool hasSelectedItems = !selectedItems().empty();
    if (hasSelectedItems)
    {
        ContextMap::const_iterator it = mContextMap.begin();
        ContextMap::const_iterator itEnd = mContextMap.end();
        while (it != itEnd)
        {
            if (it.value() == ContextMenuContext::CONTEXT_MEDIA_ITEMS_SELECTED)
            {
                mContextMenu->addAction(new QAction(it.key()));
            }
            ++it;
        }
    }

    if (selectedItems().size() == 1)
    {
        /* There is only 1 item selected. Add the specific actions of that MediaWidget to the
         * contextmenu. This is done by going through its action vector.
         */
        QListWidgetItem* item = selectedItems().at(0);
        MediaWidget* mediaWidget = static_cast<MediaWidget*>(itemWidget(item));
        std::vector <AssetMetaData::MediaWidgetAction> actionVec = mediaWidget->getAssetMetaData().mediaWidgetActionVec;
        std::vector <AssetMetaData::MediaWidgetAction>::const_iterator it = actionVec.begin();
        std::vector <AssetMetaData::MediaWidgetAction>::const_iterator itEnd = actionVec.end();
        AssetMetaData::MediaWidgetAction action;
        while (it != itEnd)
        {
            action = *it;
            mContextMenu->addAction(new QAction(action.actionText.c_str()));
            ++it;
        }
    }
}
