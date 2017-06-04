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
#include "media_listwidget.h"
#include <media_widget.h>

//****************************************************************************/
MediaListWidget::MediaListWidget (QWidget* parent) :
    QListWidget (parent)
{
    // Define display mode
    setMouseTracking(true);
    //setAcceptDrops(true);
    //setDropIndicatorShown(true);
    setViewMode(QListView::ListMode);
    setWrapping(true);
    setWordWrap(true);
    setSpacing(0);
    setUniformItemSizes(true);
    setMovement(QListView::Snap);
    setFlow(QListView::LeftToRight);
    //setStyleSheet("background: yellow");
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

    QListWidgetItem* item = new QListWidgetItem(this);
    QSize size = mediaWidget->size();
    item->setSizeHint(size); // Must be present, otherwise the mediaWidget is not shown
    setItemWidget(item, mediaWidget);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    addItem(item);
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
