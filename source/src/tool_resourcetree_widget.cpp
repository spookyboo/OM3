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
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QHeaderView>
#include <QEvent>
#include <QImage>
#include <QPixmap>
#include <QTreeWidgetItem>
#include <QDrag>
#include <QMimeData>
#include "tool_resourcetree_widget.h"

//****************************************************************************/
QtResourceTreeWidget::QtResourceTreeWidget(const QString& iconDir, QWidget* parent) : QWidget(parent)
{
    mDraggedResourceName = "";
    assetIconWidth = TOOL_RESOURCETREE_DEFAULT_ICON_WIDTH;
    assetIconHeight = TOOL_RESOURCETREE_DEFAULT_ICON_HEIGHT;
    mActionCreateToplevelGroupText = TOOL_RESOURCETREE_ACTION_CREATE_TOPLEVEL_GROUP;
    mActionCreateSubGroupText = TOOL_RESOURCETREE_ACTION_CREATE_SUBGROUP;
    mActionCreateAssetText = TOOL_RESOURCETREE_ACTION_CREATE_ASSET;
    mActionImportAssetText = TOOL_RESOURCETREE_ACTION_IMPORT_ASSET;
    mActionDuplicateAssetText = TOOL_RESOURCETREE_ACTION_DUPLICATE_ASSET;
    mActionDeleteResourceText = TOOL_RESOURCETREE_ACTION_DELETE_RESOURCE;
    mActionCollapseExpandText = TOOL_RESOURCETREE_ACTION_COLLAPSE_EXPAND;
    mAddAssetAfterDuplicateAssetSelected = true;
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mIconDir = iconDir;
    mMaxDepth = 4;
    mCollapsed = true;
    mSubgroupIconName = QString("");
    mTopLevelGroupItemEditable = false;
    mSubLevelGroupItemEditable = false;
    mAssetItemEditable = false;
    mDeleteTopLevelGroupEnabled = true;
    mResourceTree = new QtTreeWidget(this);
    mResourceTree->setObjectName(TOOL_RESOURCETREE_OBJECT_NAME); // Only items dropped from widget with this objectname allowed
    mResourceTree->setAnimated(true);
    mResourceTree->setDragEnabled(true);
    mResourceTree->setAcceptDrops(true);
    mResourceTree->setDragDropOverwriteMode(true);
    mResourceTree->viewport()->installEventFilter(this);
    mResourceTree->header()->hide();

    // Set default values for contextmenu
    mToplevelGroupContextMenuItemEnabled [mActionCreateToplevelGroupText] = true;
    mToplevelGroupContextMenuItemEnabled [mActionCreateSubGroupText] = true;
    mToplevelGroupContextMenuItemEnabled [mActionCreateAssetText] = true;
    mToplevelGroupContextMenuItemEnabled [mActionImportAssetText] = true;
    mToplevelGroupContextMenuItemEnabled [mActionDuplicateAssetText] = true;
    mToplevelGroupContextMenuItemEnabled [mActionDeleteResourceText] = true;
    mSubGroupContextMenuItemEnabled [mActionCreateToplevelGroupText] = true;
    mSubGroupContextMenuItemEnabled [mActionCreateSubGroupText] = true;
    mSubGroupContextMenuItemEnabled [mActionCreateAssetText] = true;
    mSubGroupContextMenuItemEnabled [mActionImportAssetText] = true;
    mSubGroupContextMenuItemEnabled [mActionDuplicateAssetText] = true;
    mSubGroupContextMenuItemEnabled [mActionDeleteResourceText] = true;
    mAssetContextMenuItemEnabled [mActionCreateToplevelGroupText] = true;
    mAssetContextMenuItemEnabled [mActionCreateSubGroupText] = true;
    mAssetContextMenuItemEnabled [mActionCreateAssetText] = true;
    mAssetContextMenuItemEnabled [mActionImportAssetText] = true;
    mAssetContextMenuItemEnabled [mActionDuplicateAssetText] = true;
    mAssetContextMenuItemEnabled [mActionDeleteResourceText] = true;

    // Create signal/slot connection
    connect(mResourceTree, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(handleItemChanged(QTreeWidgetItem*,int)));

    // Create edit
    mSearchLayout = new QHBoxLayout;
    mSearchLine = new QLineEdit();
    connect(mSearchLine, SIGNAL(textChanged(QString)), this, SLOT(searchLineTextChanged(QString)));

    // Create findbutton
    QImage imageSearch(mIconDir + TOOL_RESOURCETREE_ICON_SEARCH);
    QPixmap pixMapSearch = QPixmap::fromImage(imageSearch).scaled(TOOL_RESOURCETREE_DEFAULT_ICON_WIDTH, TOOL_RESOURCETREE_DEFAULT_ICON_HEIGHT);
    mSearchLabel = new QLabel();
    mSearchLabel->setPixmap(pixMapSearch);
    mSearchLabel->setContentsMargins(-8, -8, -8, -8);

    // Create clearbutton
    QImage imageClear(mIconDir + TOOL_RESOURCETREE_ICON_CLOSE);
    QPixmap pixMapClear = QPixmap::fromImage(imageClear).scaled(TOOL_RESOURCETREE_DEFAULT_ICON_WIDTH, TOOL_RESOURCETREE_DEFAULT_ICON_HEIGHT);
    mSearchClearButton = new QPushButton();
    mSearchClearButton->setStyleSheet(QString("QPushButton {border: none; background: transparent;} QPushButton:hover {background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #565656, stop:1 #8a8a8a);}"));
    mSearchClearButton->setIcon(QIcon(pixMapClear));
    mSearchClearButton->setIconSize(QSize(TOOL_RESOURCETREE_DEFAULT_ICON_WIDTH, TOOL_RESOURCETREE_DEFAULT_ICON_HEIGHT));
    mSearchClearButton->setContentsMargins(-8, -8, -8, -8);
    connect(mSearchClearButton, SIGNAL(clicked()), this, SLOT(clearSearchLine()));

    // Contextmenu
    mContextMenu = 0;
    mToplevelGroupSubMenu = 0;
    mSubGroupSubMenu = 0;
    mContextMenuEnabled = true;
    mCreateTopLevelGroupContextMenuItemEnabled = true;
    mCreateSubGroupContextMenuItemEnabled = true;
    mInheritSubGroupIconFromParent = true;
    mCreateAssetContextMenuItemEnabled = true;
    mImportAssetContextMenuItemEnabled = true;
    mDuplicateAssetContextMenuItemEnabled = true;
    mDeleteResourceContextMenuItemEnabled = true;
    mCollapseExpandContextMenuItemEnabled = true;
    buildContextMenu();

    // Layout
    mSearchLayout->addWidget(mSearchLabel, 1);
    mSearchLayout->addWidget(mSearchLine, 2000);
    mSearchLayout->addWidget(mSearchClearButton, 1);
    mainLayout->addLayout(mSearchLayout, 1);
    mainLayout->addWidget(mResourceTree, 2000);
    setLayout(mainLayout);
}

//****************************************************************************/
QtResourceTreeWidget::~QtResourceTreeWidget(void)
{
    clearRegisteredResources();
    clearResourceInfoVec();
}

//****************************************************************************/
void QtResourceTreeWidget::setMaxDepth(int maxDepth)
{
    mMaxDepth = maxDepth;
}

//****************************************************************************/
int QtResourceTreeWidget::getMaxDepth(void) const
{
    return mMaxDepth;
}

//****************************************************************************/
bool QtResourceTreeWidget::eventFilter(QObject* object, QEvent* event)
{
    QMouseEvent* mouseEvent = (QMouseEvent*) event;
    switch ((int) event->type())
    {
        case QEvent::MouseButtonPress:
            mouseClickHandler(mouseEvent);
            emit resourceSelected();
        break;

        case QEvent::MouseButtonDblClick:
            mouseDoubleClickHandler(mouseEvent);
        break;

        case QEvent::MouseMove:
            mouseMoveHandler(mouseEvent);
        break;

        case QEvent::Drop:
            dropHandler(object, event);
        break;
    }

    event->accept();
    return QObject::eventFilter(object, event);
}

//****************************************************************************/
void QtResourceTreeWidget::mouseClickHandler(QMouseEvent* event)
{
    switch ((int) event->button())
    {
        case Qt::LeftButton:
        {
            selectResourceFromCursor();
        }
        break;

        case Qt::RightButton:
        {
            if (mContextMenuEnabled)
            {
                QPoint pos;
                pos.setX(event->screenPos().x());
                pos.setY(event->screenPos().y());
                enableContextMenuItemsForSelectedResource();
                mContextMenu->popup(pos);
            }
        }
        break;
    }
}

//****************************************************************************/
void QtResourceTreeWidget::mouseDoubleClickHandler(QMouseEvent* event)
{
    switch ((int) event->button())
    {
        case Qt::LeftButton:
        {
            doubleClickResourceFromCursor();
        }
        break;
    }
}

//****************************************************************************/
void QtResourceTreeWidget::enableContextMenuItemForTopLevelGroup(const QString& menuItemText, bool enabled)
{
    mToplevelGroupContextMenuItemEnabled [menuItemText] = enabled;
}

//****************************************************************************/
void QtResourceTreeWidget::enableContextMenuItemForSubGroup(const QString& menuItemText, bool enabled)
{
    mSubGroupContextMenuItemEnabled [menuItemText] = enabled;
}

//****************************************************************************/
void QtResourceTreeWidget::enableContextMenuItemForAsset(const QString& menuItemText, bool enabled)
{
    mAssetContextMenuItemEnabled [menuItemText] = enabled;
}

//****************************************************************************/
void QtResourceTreeWidget::enableContextMenuItemsForSelectedResource(void)
{
    QTreeWidgetItem* item = mResourceTree->currentItem();
    if (!item)
        return;

    if (isItemAsset(item))
    {
        QMap<QString, bool>::iterator it;
        QMap<QString, bool>::iterator itStart = mAssetContextMenuItemEnabled.begin();
        QMap<QString, bool>::iterator itEnd = mAssetContextMenuItemEnabled.end();
        for (it = itStart; it != itEnd; ++it)
            enableContextMenuItem(it.key(), it.value());
    }
    else if (isItemSubGroup(item))
    {
        QMap<QString, bool>::iterator it;
        QMap<QString, bool>::iterator itStart = mSubGroupContextMenuItemEnabled.begin();
        QMap<QString, bool>::iterator itEnd = mSubGroupContextMenuItemEnabled.end();
        for (it = itStart; it != itEnd; ++it)
            enableContextMenuItem(it.key(), it.value());
    }
    else if (isItemToplevelGroup(item))
    {
        QMap<QString, bool>::iterator it;
        QMap<QString, bool>::iterator itStart = mToplevelGroupContextMenuItemEnabled.begin();
        QMap<QString, bool>::iterator itEnd = mToplevelGroupContextMenuItemEnabled.end();
        for (it = itStart; it != itEnd; ++it)
            enableContextMenuItem(it.key(), it.value());
    }
}

//****************************************************************************/
void QtResourceTreeWidget::enableContextMenuItem(const QString& menuItemText, bool enabled)
{
    QAction* action = getContextMenuAction(menuItemText);
    if (action)
    {
        action->setEnabled(enabled);
        QFont font;
        font.setItalic(!enabled); // Italic when no selected items available
        action->setFont(font);
    }
}

//****************************************************************************/
QAction* QtResourceTreeWidget::getContextMenuAction(const QString& menuItemText)
{
    QList<QAction*> actions = mContextMenu->actions();
    foreach (QAction* action, actions)
        if (action->text() == menuItemText)
            return action;

    return 0;
}

//****************************************************************************/
void QtResourceTreeWidget::dropHandler (QObject* object, QEvent* event)
{
    event->accept();

    // Determine whether the data was from this object
    QDropEvent* dropEvent = static_cast<QDropEvent*>(event);
    const QMimeData* mimeData = dropEvent->mimeData();
    if (mimeData->parent() != this)
        QMessageBox::information(0, "Warning", TOOL_RESOURCETREE_WARNING_5);

    // Get dragged item
    QTreeWidgetItem* itemDragged = mResourceTree->currentItem();
    int parentId = getParentIdFromItem(itemDragged);
    if (parentId == 0)
    {
        QMessageBox::information(0, "Warning", TOOL_RESOURCETREE_WARNING_1);
        return; // Toplevelitems may not be moved
    }

    // Get item to be dropped on
    QPoint pos = mResourceTree->mapFromGlobal(QCursor::pos());
    QTreeWidgetItem* itemDropped = mResourceTree->itemAt(pos);
    if (!itemDropped)
        return;

    int toplevelParentIdDragged = getToplevelParentIdFromItem(itemDragged);
    int toplevelParentIdDropped = getToplevelParentIdFromItem(itemDropped);

    // Items may only be moved within the same toplevel item
    // Assets cannot be attached to another asset
    // Subgroups cannot be attached to an asset
    if (toplevelParentIdDragged == toplevelParentIdDropped)
    {
        if (isItemSubGroup(itemDragged) && isItemAsset(itemDropped))
            QMessageBox::information(0, "Warning", TOOL_RESOURCETREE_WARNING_2);
        else if (isItemAsset(itemDragged) && isItemAsset(itemDropped))
            QMessageBox::information(0, "Warning", TOOL_RESOURCETREE_WARNING_3);
        else
        {
            int resourceId = getResourceIdFromItem(itemDragged);
            int parentId = getResourceIdFromItem(itemDropped);
            moveResource (resourceId, parentId);
        }
    }
    else
        QMessageBox::information(0, "Warning", TOOL_RESOURCETREE_WARNING_4);
}

//****************************************************************************/
void QtResourceTreeWidget::mouseMoveHandler (QMouseEvent* event)
{
    // If not left button - return
    if (!(event->buttons() & Qt::LeftButton))
        return;

    // This is the dragged item; store it in mDraggedResourceName, otherwise it cannot be determined later on
    QTreeWidgetItem* item = mResourceTree->currentItem();
    mDraggedResourceName = getFullQualifiedNameFromItem(item);
}

//****************************************************************************/
const QString& QtResourceTreeWidget::getDraggedFullQualifiedResourceName(void)
{
    return mDraggedResourceName;
}

//****************************************************************************/
void QtResourceTreeWidget::registerResource (int resourceId,
                                             const QString& resourceName,
                                             const QString& fullQualifiedName,
                                             const QString& iconName)
{
    QtResourceInfo* info = new QtResourceInfo();
    info->topLevelId = 0;
    info->resourceId = resourceId;
    info->resourceName = resourceName;
    info->fullQualifiedName = fullQualifiedName;
    info->iconName = iconName;
    mRegisteredResources.append(info);

    // Add also to submenu toplevel group...
    if (mToplevelGroupSubMenu)
    {
        QAction* actionToplevel;
        actionToplevel = new QAction(resourceName, mToplevelGroupSubMenu);
        actionToplevel->setCheckable(true);
        QIcon icon(mIconDir + iconName);
        actionToplevel->setIcon(icon);
        actionToplevel->setData(QVariant(resourceId));
        mToplevelGroupSubMenu->addAction(actionToplevel);
    }

    // ... and to the submenu subgroup
    if (mSubGroupSubMenu)
    {
        QAction* actionSubGroup;
        actionSubGroup = new QAction(resourceName + QString(" subgroup"), mSubGroupSubMenu);
        actionSubGroup->setCheckable(true);
        QIcon icon(mIconDir + iconName);
        actionSubGroup->setIcon(icon);
        actionSubGroup->setData(QVariant(resourceId));
        mSubGroupSubMenu->addAction(actionSubGroup);
    }
}

//****************************************************************************/
void QtResourceTreeWidget::clearRegisteredResources (void)
{
    foreach (QtResourceInfo* info, mRegisteredResources)
        delete info;

    mRegisteredResources.clear();
}

//****************************************************************************/
QtResourceInfo* QtResourceTreeWidget::getRegisteredResourceInfo (int resourceId)
{
    foreach (QtResourceInfo* info, mRegisteredResources)
        if (info->resourceId == resourceId)
            return info;

    return 0;
}

//****************************************************************************/
QVector<QtResourceInfo*>& QtResourceTreeWidget::getRegisteredResources (void)
{
    return mRegisteredResources;
}

//****************************************************************************/
void QtResourceTreeWidget::setContextMenuEnabled(bool enabled)
{
    mContextMenuEnabled = enabled;
}

//****************************************************************************/
bool QtResourceTreeWidget::isContextMenuEnabled(void)
{
    return mContextMenuEnabled;
}

//****************************************************************************/
void QtResourceTreeWidget::setCreateTopLevelGroupContextMenuItemEnabled(bool enabled)
{
    mCreateTopLevelGroupContextMenuItemEnabled = enabled;
    buildContextMenu();
}

//****************************************************************************/
bool QtResourceTreeWidget::isCreateTopLevelGroupContextMenuItemEnabled(void)
{
    return mCreateTopLevelGroupContextMenuItemEnabled;
}

//****************************************************************************/
void QtResourceTreeWidget::setTopLevelGroupItemEditable(bool editable)
{
    // Don't bother to iterate through the already existing toplevel items; only newly added resources are taken into account
    mTopLevelGroupItemEditable = editable;
}

//****************************************************************************/
bool QtResourceTreeWidget::isTopLevelGroupItemEditable(void)
{
    return mTopLevelGroupItemEditable;
}

//****************************************************************************/
void QtResourceTreeWidget::setDeleteTopLevelGroupEnabled (bool enabled)
{
    mDeleteTopLevelGroupEnabled = enabled;
}

//****************************************************************************/
bool QtResourceTreeWidget::isDeleteTopLevelGroupEnabled (void)
{
    return mDeleteTopLevelGroupEnabled;
}

//****************************************************************************/
void QtResourceTreeWidget::setCreateSubGroupContextMenuItemEnabled(bool enabled)
{
    mCreateSubGroupContextMenuItemEnabled = enabled;
    buildContextMenu();
}

//****************************************************************************/
bool QtResourceTreeWidget::isCreateSubGroupContextMenuItemEnabled(void)
{
    return mCreateSubGroupContextMenuItemEnabled;
}

//****************************************************************************/
void QtResourceTreeWidget::setSubLevelGroupItemEditable(bool editable)
{
    // Don't bother to iterate through the already existing sublevel items; only newly added resources are taken into account
    mSubLevelGroupItemEditable = editable;
}

//****************************************************************************/
bool QtResourceTreeWidget::isSubLevelGroupItemEditable(void)
{
    return mSubLevelGroupItemEditable;
}

//****************************************************************************/
void QtResourceTreeWidget::setCreateAssetContextMenuItemEnabled(bool enabled)
{
    mCreateAssetContextMenuItemEnabled = enabled;
    buildContextMenu();
}

//****************************************************************************/
bool QtResourceTreeWidget::isCreateAssetContextMenuItemEnabled(void)
{
    return mCreateAssetContextMenuItemEnabled;
}

//****************************************************************************/
void QtResourceTreeWidget::setImportAssetContextMenuItemEnabled(bool enabled)
{
    mImportAssetContextMenuItemEnabled = enabled;
    buildContextMenu();
}

//****************************************************************************/
bool QtResourceTreeWidget::isImportAssetContextMenuItemEnabled(void)
{
    return mImportAssetContextMenuItemEnabled;
}

//****************************************************************************/
void QtResourceTreeWidget::setDuplicateAssetContextMenuItemEnabled(bool enabled)
{
    mDuplicateAssetContextMenuItemEnabled = enabled;
    buildContextMenu();
}

//****************************************************************************/
bool QtResourceTreeWidget::isDuplicateAssetContextMenuItemEnabled(void)
{
    return mDuplicateAssetContextMenuItemEnabled;
}

//****************************************************************************/
void QtResourceTreeWidget::setDeleteResourceContextMenuItemEnabled(bool enabled)
{
    mDeleteResourceContextMenuItemEnabled = enabled;
    buildContextMenu();
}

//****************************************************************************/
void QtResourceTreeWidget::setCollapseExpandContextMenuItemEnabled(bool enabled)
{
    mCollapseExpandContextMenuItemEnabled = enabled;
    buildContextMenu();
}

//****************************************************************************/
bool QtResourceTreeWidget::isDeleteResourceContextMenuItemEnabled(void)
{
    return mDeleteResourceContextMenuItemEnabled;
}

//****************************************************************************/
bool QtResourceTreeWidget::isCollapseExpandContextMenuItemEnabled(void)
{
    return mCollapseExpandContextMenuItemEnabled;
}

//****************************************************************************/
void QtResourceTreeWidget::setAssetItemEditable(bool editable)
{
    // Don't bother to iterate through the already existing asset items; only newly added resources are taken into account
    mAssetItemEditable = editable;
}

//****************************************************************************/
bool QtResourceTreeWidget::isAssetItemEditable(void)
{
    return mAssetItemEditable;
}

//****************************************************************************/
void QtResourceTreeWidget::setInheritSubGroupIconFromParent(bool inherit)
{
    mInheritSubGroupIconFromParent = inherit;
}

//****************************************************************************/
bool QtResourceTreeWidget::isInheritSubGroupIconFromParent(void)
{
    return mInheritSubGroupIconFromParent;
}

//****************************************************************************/
void QtResourceTreeWidget::buildContextMenu(void)
{
    if (!mContextMenu)
    {
        mContextMenu = new QMenu(mResourceTree);
        connect(mContextMenu, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuItemSelected(QAction*)));
    }
    mContextMenu->clear();
    mSubGroupSubMenu = 0;
    mToplevelGroupSubMenu = 0;

    // Custom menu items
    QString menuItemText;
    if (mCustomContextMenuList.size() != 0)
        foreach (menuItemText, mCustomContextMenuList)
            if (!menuItemText.isEmpty())
                mContextMenu->addAction(new QAction(menuItemText, mResourceTree));

    // Menu item for creating a toplevel
    if (mCreateTopLevelGroupContextMenuItemEnabled)
        mToplevelGroupSubMenu = mContextMenu->addMenu(mActionCreateToplevelGroupText);

    // Menu item for creating a subgroup
    if (mCreateSubGroupContextMenuItemEnabled)
    {
        if (mInheritSubGroupIconFromParent)
            mContextMenu->addAction(new QAction(mActionCreateSubGroupText, mResourceTree));
        else
            mSubGroupSubMenu = mContextMenu->addMenu(mActionCreateSubGroupText);
    }

    // Menu item for creating an asset
    if (mCreateAssetContextMenuItemEnabled)
        mContextMenu->addAction(new QAction(mActionCreateAssetText, mResourceTree));

    // Menu item for importing an asset
    if (mImportAssetContextMenuItemEnabled)
        mContextMenu->addAction(new QAction(mActionImportAssetText, mResourceTree));

    // Menu item for duplicating the asset
    if (mDuplicateAssetContextMenuItemEnabled)
        mContextMenu->addAction(new QAction(mActionDuplicateAssetText, mResourceTree));

    // Menu item for deleting an item from the resource tree
    if (mDeleteResourceContextMenuItemEnabled)
        mContextMenu->addAction(new QAction(mActionDeleteResourceText, mResourceTree));

    // Menu item for collapse/expand
    if (mCollapseExpandContextMenuItemEnabled)
        mContextMenu->addAction(new QAction(mActionCollapseExpandText, mResourceTree));
}

//****************************************************************************/
void QtResourceTreeWidget::addResource (int topLevelId,
                                        int resourceId,
                                        int parentId,
                                        const QString& resourceName,
                                        const QString& fullQualifiedName,
                                        const QString& iconName,
                                        bool isAsset,
                                        bool suppressSignal)
{
    // First, check existence in the resource tree; do not processed if the resourceId is already available
    if (getResourceItem(resourceId))
        return;

    // Second, check depth
    if (getDepth(parentId) >= mMaxDepth)
        return;

    // Third, the parent may not be an asset
    QTreeWidgetItem* parentItem = getResourceItem(parentId);
    if (isItemAsset(parentItem))
        return;

    // Create the item
    QTreeWidgetItem* resourceItem = new QTreeWidgetItem();

    // Set icon
    if (!iconName.isEmpty())
    {
        QImage imageIcon(mIconDir + iconName);
        //QPixmap pixMapIcon = QPixmap::fromImage(imageIcon).scaled(TOOL_RESOURCETREE_ICON_WIDTH, TOOL_RESOURCETREE_ICON_WIDTH);
        QPixmap pixMapIcon = QPixmap::fromImage(imageIcon).scaled(assetIconWidth, assetIconHeight);
        resourceItem->setData(0, Qt::DecorationRole, QVariant(pixMapIcon));
    }

    // Set name
    resourceItem->setText(0, resourceName);

    // Set resourceId, parentId, iconName and fullQualifiedName
    resourceItem->setData(TOOL_RESOURCETREE_KEY_RESOURCEID, Qt::UserRole, QVariant(resourceId));
    resourceItem->setData(TOOL_RESOURCETREE_KEY_PARENTID, Qt::UserRole, QVariant(parentId));
    resourceItem->setData(TOOL_RESOURCETREE_KEY_ICONNAME, Qt::UserRole, QVariant(iconName));
    resourceItem->setData(TOOL_RESOURCETREE_KEY_FULLNAME, Qt::UserRole, QVariant(fullQualifiedName));

    int type;
    if (isAsset)
    {
        type = TOOL_RESOURCETREE_KEY_TYPE_ASSET;
        setFlagsResourceItem (resourceItem, mAssetItemEditable);
    }
    else if(parentId == 0)
    {
        type = TOOL_RESOURCETREE_KEY_TYPE_TOPLEVEL_GROUP;
        setFlagsResourceItem (resourceItem, mTopLevelGroupItemEditable);
    }
    else
    {
        type = TOOL_RESOURCETREE_KEY_TYPE_GROUP;
        setFlagsResourceItem (resourceItem, mSubLevelGroupItemEditable);
    }
    resourceItem->setData(TOOL_RESOURCETREE_KEY_TYPE, Qt::UserRole, QVariant(type));

    // Add it to the tree
    if (parentId == 0)
      mResourceTree->addTopLevelItem(resourceItem);
    else if (parentItem)
    {
        parentItem->addChild(resourceItem);
        parentItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    }

    // Emit signal
    if (!suppressSignal)
        emit resourceAdded(resourceId);
}

//****************************************************************************/
void QtResourceTreeWidget::setFlagsResourceItem(QTreeWidgetItem* resourceItem, bool editable)
{
    if (editable)
        resourceItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled | Qt::ItemIsDragEnabled);
    else
        resourceItem->setFlags(resourceItem->flags() & ~Qt::ItemIsEditable);
}

//****************************************************************************/
int QtResourceTreeWidget::addResource (int parentId,
                                       const QString& resourceName,
                                       const QString& fullQualifiedName,
                                       const QString& iconName,
                                       bool isAsset,
                                       bool suppressSignal)
{
    int resourceId = generateUniqueResourceId();
    int topLevelId = getToplevelParentId(resourceId);
    addResource (topLevelId, resourceId, parentId, resourceName, fullQualifiedName, iconName, isAsset, suppressSignal);
    return resourceId;
}

//****************************************************************************/
QVector<QtResourceInfo*>& QtResourceTreeWidget::getResources (void)
{
    clearResourceInfoVec();
    QTreeWidgetItemIterator it(mResourceTree);
    while (*it)
    {
        QtResourceInfo* info = new QtResourceInfo();
        info->topLevelId = getToplevelParentIdFromItem(*it);
        info->resourceId = getResourceIdFromItem(*it);
        info->parentId = getParentIdFromItem(*it);
        info->resourceName = (*it)->text(0);
        info->fullQualifiedName = getFullQualifiedNameFromItem(*it);
        info->iconName = getIconNameFromItem(*it);
        info->resourceType = getTypeFromItem(*it);
        mResourceInfoVec.append(info);
        ++it;
    }

    return mResourceInfoVec;
}

//****************************************************************************/
void QtResourceTreeWidget::setResources(const QVector<QtResourceInfo*>& resources, bool suppressSignal)
{
    clear();
    QVectorIterator<QtResourceInfo*> it(resources);
    it.toFront();
    QtResourceInfo* info;
    bool isAsset;

    // The order of the resources is important a resource cannot be added if its parent does not exist.
    // If the resources were aquired by means of getResources, the order is ok.
    while (it.hasNext())
    {
        info = it.next();
        isAsset = (info->resourceType == TOOL_RESOURCETREE_KEY_TYPE_ASSET);
        addResource (info->topLevelId,
                     info->resourceId,
                     info->parentId,
                     info->resourceName,
                     info->fullQualifiedName,
                     info->iconName,
                     isAsset,
                     suppressSignal);
    }
}

//****************************************************************************/
QVector<QtResourceInfo*>& QtResourceTreeWidget::getAssets (void)
{
    clearResourceInfoVec();
    QTreeWidgetItemIterator it(mResourceTree);
    int type;
    while (*it)
    {
        QtResourceInfo* info = new QtResourceInfo();
        type = getTypeFromItem(*it);
        if (type == TOOL_RESOURCETREE_KEY_TYPE_ASSET)
        {
            info->topLevelId = getToplevelParentIdFromItem(*it);
            info->resourceId = getResourceIdFromItem(*it);
            info->parentId = getParentIdFromItem(*it);
            info->resourceName = (*it)->text(0);
            info->fullQualifiedName = getFullQualifiedNameFromItem(*it);
            info->iconName = getIconNameFromItem(*it);
            info->resourceType = getTypeFromItem(*it);
            mResourceInfoVec.append(info);
            ++it;
        }
    }

    return mResourceInfoVec;
}

//****************************************************************************/
QVector<QtResourceInfo*>& QtResourceTreeWidget::getGroups (void)
{
    clearResourceInfoVec();
    QTreeWidgetItemIterator it(mResourceTree);
    int type;
    while (*it)
    {
        QtResourceInfo* info = new QtResourceInfo();
        type = getTypeFromItem(*it);
        if (type == TOOL_RESOURCETREE_KEY_TYPE_TOPLEVEL_GROUP || type == TOOL_RESOURCETREE_KEY_TYPE_TOPLEVEL_GROUP)
        {
            info->resourceId = getToplevelParentIdFromItem(*it);
            info->resourceId = getResourceIdFromItem(*it);
            info->parentId = getParentIdFromItem(*it);
            info->resourceName = (*it)->text(0);
            info->fullQualifiedName = getFullQualifiedNameFromItem(*it);
            info->iconName = getIconNameFromItem(*it);
            info->resourceType = getTypeFromItem(*it);
            mResourceInfoVec.append(info);
            ++it;
        }
    }

    return mResourceInfoVec;
}

//****************************************************************************/
void QtResourceTreeWidget::clearResourceInfoVec (void)
{
    foreach (QtResourceInfo* info, mResourceInfoVec)
        delete info;

    mResourceInfoVec.clear();
}

//****************************************************************************/
void QtResourceTreeWidget::deleteResource (int resourceId, bool suppressSignal)
{
    // Toplevel item may not be deleted if mDeleteTopLevelGroupEnabled is  'false'
    if (!mDeleteTopLevelGroupEnabled && getParentId(resourceId) == 0)
        return;

    QTreeWidgetItem* item = getResourceItem(resourceId);
    int index = mResourceTree->indexOfTopLevelItem(item);
    mResourceTree->takeTopLevelItem(index);
    delete item;

    // Signal
    if (!suppressSignal)
        emit resourceDeleted(resourceId);
}

//****************************************************************************/
void QtResourceTreeWidget::deleteResource (const QString& name, bool suppressSignal)
{
    int resourceId = getResourceIdFromName(name);
    deleteResource(resourceId, suppressSignal);
}

//****************************************************************************/
void QtResourceTreeWidget::deleteResource (int toplevelId, const QString& name, bool suppressSignal)
{
    int resourceId = getResourceIdFromToplevelIdAndName(toplevelId, name);
    deleteResource(resourceId, suppressSignal);
}

//****************************************************************************/
void QtResourceTreeWidget::clear (void)
{
    mResourceTree->clear();
}

//****************************************************************************/
void QtResourceTreeWidget::moveResource (int resourceId, int parentId)
{
    if (parentId == 0)
        return;

    int existingParentId = getParentId(resourceId);
    QTreeWidgetItem* item = getResourceItem(resourceId);
    if (item)
    {
        QTreeWidgetItem* newParentItem = getResourceItem(parentId);
        if (newParentItem)
        {
            QTreeWidgetItem* existingParentItem = getResourceItem(existingParentId);
            if (existingParentItem)
            {
                existingParentItem->removeChild(item);
                item->setData(TOOL_RESOURCETREE_KEY_PARENTID, Qt::UserRole, QVariant(parentId));
                newParentItem->addChild(item);
                expand(parentId);
                emit resourceMoved(resourceId);
            }
        }
    }
}

//****************************************************************************/
void QtResourceTreeWidget::selectResource (int resourceId, bool emitSignal)
{
    QTreeWidgetItem* item = getResourceItem(resourceId);
    if (item)
    {
        mResourceTree->setCurrentItem(item);

        if (emitSignal)
            emit resourceSelected(resourceId);
    }
}

//****************************************************************************/
void QtResourceTreeWidget::selectResourceFromCursor (bool emitSignal)
{
    QPoint pos = mResourceTree->mapFromGlobal(QCursor::pos());
    QTreeWidgetItem* item = mResourceTree->itemAt(pos);
    if (item)
    {
        mResourceTree->setCurrentItem(item);

        if (emitSignal)
            emit resourceSelected(getResourceIdFromItem(item));
    }
}

//****************************************************************************/
void QtResourceTreeWidget::doubleClickResourceFromCursor (bool emitSignal)
{
    QPoint pos = mResourceTree->mapFromGlobal(QCursor::pos());
    QTreeWidgetItem* item = mResourceTree->itemAt(pos);
    if (item)
    {
        mResourceTree->setCurrentItem(item);

        if (emitSignal)
            emit resourceDoubleClicked(getResourceIdFromItem(item));
    }
}

//****************************************************************************/
int QtResourceTreeWidget::getSelectedResource (void)
{
    QTreeWidgetItem* item = mResourceTree->currentItem();
    if (item)
        return getResourceIdFromItem(item);

    return 0;
}

//****************************************************************************/
QTreeWidgetItem* QtResourceTreeWidget::getSelectedResourceItem (void)
{
    return mResourceTree->currentItem();
}

//****************************************************************************/
int QtResourceTreeWidget::getFirstInParent (int parentId)
{
    QTreeWidgetItemIterator it(mResourceTree);
    while (*it)
    {
        if (parentId == getParentIdFromItem(*it))
            return getResourceIdFromItem(*it);
        ++it;
    }

    return 0;
}

//****************************************************************************/
bool QtResourceTreeWidget::isFirstInParent (int resourceId, int parentId)
{
    return resourceId == getFirstInParent(parentId);
}

//****************************************************************************/
int QtResourceTreeWidget::getLastInParent(int parentId)
{
    QTreeWidgetItemIterator it(mResourceTree);
    QTreeWidgetItem* childItem = 0;
    while (*it)
    {
        if (parentId == getParentIdFromItem(*it))
            childItem = *it;

        ++it;
    }

    return getResourceIdFromItem(childItem);
}

//****************************************************************************/
bool QtResourceTreeWidget::isLastInParent (int resourceId, int parentId)
{
    return resourceId == getLastInParent(parentId);
}

//****************************************************************************/
int QtResourceTreeWidget::getPrevious (int resourceId)
{
    QTreeWidgetItemIterator it(mResourceTree);
    int previousId = 0;
    int currentId = 0;
    while (*it)
    {
        currentId = getResourceIdFromItem(*it);
        if (resourceId == currentId)
            return previousId;

        previousId = currentId;
        ++it;
    }

    return previousId;
}

//****************************************************************************/
int QtResourceTreeWidget::getNext (int resourceId)
{
    QTreeWidgetItemIterator it(mResourceTree);
    bool next = false;
    while (*it)
    {
        if (next)
            return getResourceIdFromItem(*it);

        if (resourceId == getResourceIdFromItem(*it))
            next = true;

        ++it;
    }

    return 0;
}

//****************************************************************************/
QVector<int> QtResourceTreeWidget::getChildIdVector (int parentId)
{
    mChildIdVector.clear();
    QTreeWidgetItemIterator it(mResourceTree);
    while (*it)
    {
        if (parentId == getParentIdFromItem(*it))
            mChildIdVector.append(getResourceIdFromItem(*it));
        ++it;
    }

    return mChildIdVector;
}

//****************************************************************************/
void QtResourceTreeWidget::expand (int resourceId)
{
    QTreeWidgetItem* item = getResourceItem(resourceId);
    if (item)
    {
        mResourceTree->expandItem(item);
        emit resourceExpanded(resourceId);
    }
}

//****************************************************************************/
void QtResourceTreeWidget::expandAll (void)
{
    mResourceTree->expandAll();
    mCollapsed = false;
}

//****************************************************************************/
void QtResourceTreeWidget::collapse (int resourceId)
{
    QTreeWidgetItem* item = getResourceItem(resourceId);
    if (item)
    {
        mResourceTree->collapseItem(item);
        emit resourceCollapsed(resourceId);
    }
}

//****************************************************************************/
void QtResourceTreeWidget::collapseAll (void)
{
    mResourceTree->collapseAll();
    mCollapsed = true;
}

//****************************************************************************/
QTreeWidgetItem* QtResourceTreeWidget::getResourceItem(int resourceId)
{
    QTreeWidgetItemIterator it(mResourceTree);
    while (*it)
    {
        if (getResourceIdFromItem(*it) == resourceId)
            return *it;

            ++it;
    }

    return 0;
}

//****************************************************************************/
int QtResourceTreeWidget::getResourceIdFromItem (QTreeWidgetItem* item)
{
    int id = -1;
    if (item)
    {
        id = item->data(TOOL_RESOURCETREE_KEY_RESOURCEID, Qt::UserRole).toInt();
    }

    return id;
}

//****************************************************************************/
int QtResourceTreeWidget::getResourceIdFromName (const QString& name)
{
    QTreeWidgetItemIterator it(mResourceTree);
    QString fullQualifiedResourceName;
    while (*it)
    {
        fullQualifiedResourceName = getFullQualifiedNameFromItem(*it);
        if (name == fullQualifiedResourceName)
            return getResourceIdFromItem(*it);

        ++it;
    }

    return -1;
}

//****************************************************************************/
int QtResourceTreeWidget::getResourceIdFromToplevelIdAndName (int topLevelId, const QString& name)
{
    QTreeWidgetItemIterator it(mResourceTree);
    QString fullQualifiedResourceName;
    int top;
    while (*it)
    {
        top = getToplevelParentIdFromItem(*it);
        if (top == topLevelId)
        {
            fullQualifiedResourceName = getFullQualifiedNameFromItem(*it);
            if (name == fullQualifiedResourceName)
                return getResourceIdFromItem(*it);
        }

        ++it;
    }

    return -1;
}

//****************************************************************************/
int QtResourceTreeWidget::getParentIdFromItem (QTreeWidgetItem* item)
{
    int id = -1;
    if (item)
        id = item->data(TOOL_RESOURCETREE_KEY_PARENTID, Qt::UserRole).toInt();

    return id;
}

//****************************************************************************/
int QtResourceTreeWidget::getToplevelParentIdFromItem (QTreeWidgetItem* item)
{
    int id = getResourceIdFromItem(item);
    int resourceId = 0;
    while (id != 0)
    {
        resourceId = id;
        id = getParentId(id);
    }

    return resourceId;
}

//****************************************************************************/
int QtResourceTreeWidget::getToplevelParentId (int resourceId)
{
    QTreeWidgetItem* item = getResourceItem(resourceId);
    return getToplevelParentIdFromItem(item);
}

//****************************************************************************/
QTreeWidgetItem* QtResourceTreeWidget::getToplevelParentItemFromItem (QTreeWidgetItem* item)
{
    int toplevelParentId = getToplevelParentIdFromItem(item);
    return getResourceItem(toplevelParentId);
}

//****************************************************************************/
int QtResourceTreeWidget::getParentId (int resourceId)
{
    QTreeWidgetItem* resourceItem = getResourceItem(resourceId);
    if (resourceItem)
        return getParentIdFromItem(resourceItem);

    return 0;
}

//****************************************************************************/
const QString& QtResourceTreeWidget::getIconNameFromItem (QTreeWidgetItem* item)
{
    mTempString = QString("");
    if (item)
        mTempString = item->data(TOOL_RESOURCETREE_KEY_ICONNAME, Qt::UserRole).toString();

    return mTempString;
}

//****************************************************************************/
const QString& QtResourceTreeWidget::getResourceNameFromItem(QTreeWidgetItem* item)
{
    mTempString = QString("");
    if (item)
        mTempString = item->text(0);

    return mTempString;
}

//****************************************************************************/
const QString& QtResourceTreeWidget::getFullQualifiedNameFromItem(QTreeWidgetItem* item)
{
    mTempString = QString("");
    if (item)
        mTempString = item->data(TOOL_RESOURCETREE_KEY_FULLNAME, Qt::UserRole).toString();

    return mTempString;
}

//****************************************************************************/
const QString& QtResourceTreeWidget::getResourceName(int resourceId)
{
    QTreeWidgetItem* item = getResourceItem(resourceId);
    return getResourceNameFromItem(item);
}

//****************************************************************************/
const QString& QtResourceTreeWidget::getFullQualifiedName(int resourceId)
{
    QTreeWidgetItem* item = getResourceItem(resourceId);
    return getFullQualifiedNameFromItem(item);
}

//****************************************************************************/
int QtResourceTreeWidget::getTypeFromItem (QTreeWidgetItem* item)
{
    int type = 0;
    if (item)
        type = item->data(TOOL_RESOURCETREE_KEY_TYPE, Qt::UserRole).toInt();

    return type;
}

//****************************************************************************/
int QtResourceTreeWidget::getTypeFromResourceId (int resourceId)
{
    QTreeWidgetItem* item = getResourceItem(resourceId);
    return getTypeFromItem(item);
}

//****************************************************************************/
bool QtResourceTreeWidget::isItemToplevelGroup(QTreeWidgetItem* item)
{
    return getTypeFromItem(item) == TOOL_RESOURCETREE_KEY_TYPE_TOPLEVEL_GROUP;
}

//****************************************************************************/
bool QtResourceTreeWidget::isResourceToplevelGroup(int resourceId)
{
    QTreeWidgetItem* item = getResourceItem(resourceId);
    return isItemToplevelGroup(item);
}

//****************************************************************************/
bool QtResourceTreeWidget::isItemSubGroup(QTreeWidgetItem* item)
{
    return getTypeFromItem(item) == TOOL_RESOURCETREE_KEY_TYPE_GROUP;
}

//****************************************************************************/
bool QtResourceTreeWidget::isResourceSubGroup(int resourceId)
{
    QTreeWidgetItem* item = getResourceItem(resourceId);
    return isItemSubGroup(item);
}

//****************************************************************************/
bool QtResourceTreeWidget::isItemAsset(QTreeWidgetItem* item)
{
    return getTypeFromItem(item) == TOOL_RESOURCETREE_KEY_TYPE_ASSET;
}

//****************************************************************************/
bool QtResourceTreeWidget::isResourceAsset(int resourceId)
{
    QTreeWidgetItem* item = getResourceItem(resourceId);
    return isItemAsset(item);
}

//****************************************************************************/
void QtResourceTreeWidget::contextMenuItemSelected(QAction* action)
{
    if (mToplevelGroupSubMenu &&
            action->parent() == mToplevelGroupSubMenu &&
            mCreateTopLevelGroupContextMenuItemEnabled)
    {
        // ---------- Create toplevel group ----------
        QtResourceInfo* info = getRegisteredResourceInfo (action->data().toInt());
        if (info)
        {
            // Add toplevel group and disable it
            addResource (info->resourceId, info->resourceId, 0, info->resourceName, info->fullQualifiedName, info->iconName);
            action->setCheckable(false);
            action->setEnabled(false);
        }
        return;
    }
    else if (mSubGroupSubMenu &&
            action->parent() == mSubGroupSubMenu &&
            mCreateSubGroupContextMenuItemEnabled)
    {
        // ---------- Create subgroup, with the icon from the submenu ----------
        QtResourceInfo* info = getRegisteredResourceInfo (action->data().toInt());
        if (info)
        {
            // Add sublevel group
            int parentId = getSelectedResource();
            int topLevelId = getToplevelParentId(parentId);
            if (parentId != 0)
            {
                // A subgroup may not be added to the toplevel
                int resourceId = generateUniqueResourceId();
                QString name = QString("<") + info->resourceName + QString(" subgroup>");
                addResource (topLevelId,
                             resourceId,
                             parentId,
                             name,
                             name,
                             info->iconName);
                expand(parentId);
            }
        }
        return;
    }
    else if (action->text() == mActionCreateSubGroupText &&
             mCreateSubGroupContextMenuItemEnabled)
    {
        // ---------- Create subgroup, with the icon from the toplevel parent ----------
        QTreeWidgetItem* item = mResourceTree->currentItem();
        if (isItemAsset(item))
            return;

        if (item)
        {
            int parentId = getResourceIdFromItem(item);
            if (parentId != 0)
            {
                // A subgroup may not be added to the toplevel
                int resourceId = generateUniqueResourceId();
                int topLevelId = getToplevelParentId(resourceId);
                if (mSubgroupIconName.isEmpty())
                    mSubgroupIconName = getIconNameFromItem(item);
                QString name = QString("<") + item->text(0) + QString(" subgroup>");
                addResource (topLevelId,
                             resourceId,
                             parentId,
                             name,
                             name,
                             mSubgroupIconName);
                expand(parentId);
            }
        }
        return;
    }
    else if (action->text() == mActionCreateAssetText)
    {
        // ---------- Create asset ----------
        QTreeWidgetItem* item = mResourceTree->currentItem();
        if (isItemAsset(item))
            return;

        if (item)
        {
            int parentId = getResourceIdFromItem(item);
            int toplevelId = getToplevelParentIdFromItem(item);
            if (parentId != 0)
            {
                // A subgroup may not be added to the toplevel
                int resourceId = generateUniqueResourceId();
                addResource (toplevelId,
                             resourceId,
                             parentId,
                             QString("<Asset>"),
                             QString(""),
                             QString(""),
                             true);
                expand(parentId);
            }
        }

        return;
    }
    else if (action->text() == mActionImportAssetText)
    {
        QTreeWidgetItem* item = mResourceTree->currentItem();
        if (isItemAsset(item))
            return;

        if (item)
        {
            int resourceId = getResourceIdFromItem(item);
            emit resourceImported(resourceId);
        }

        return;
    }
    else if (action->text() == mActionDuplicateAssetText)
    {
        QTreeWidgetItem* item = mResourceTree->currentItem();
        if (!isItemAsset(item))
            return;

        if (item && mAddAssetAfterDuplicateAssetSelected)
        {
            int parentId = getParentIdFromItem(item);
            int toplevelId = getToplevelParentIdFromItem(item);
            QString fullName = getFullQualifiedNameFromItem(item);
            int duplicateResourceId = generateUniqueResourceId();
            addResource (toplevelId,
                         duplicateResourceId,
                         parentId,
                         QString("Copy of ") + item->text(0),
                         fullName,
                         QString(""),
                         true);
            emit assetDuplicated(duplicateResourceId);
        }
        else if (item && !mAddAssetAfterDuplicateAssetSelected)
        {
            int resourceId = getResourceIdFromItem(item);
            emit assetDuplicated(resourceId);
        }


        return;
    }
    else if (action->text() == mActionDeleteResourceText)
    {
        QTreeWidgetItem* item = mResourceTree->currentItem();
        if (item)
        {
            int resourceId = getResourceIdFromItem(item);
            int parentId = getParentIdFromItem(item);
            deleteResource(resourceId);
            if (parentId == 0)
            {
                // Enable the context menu again
                if (mToplevelGroupSubMenu && mToplevelGroupSubMenu->isEnabled())
                {
                    QList<QAction*> actions = mToplevelGroupSubMenu->actions();
                    foreach (QAction* action, actions)
                    {
                        if (action->data().toInt() == resourceId)
                        {
                            action->setCheckable(true);
                            action->setEnabled(true);
                        }
                    }
                }
            }
        }
        return;
    }
    else if (action->text() == mActionCollapseExpandText)
    {
        // Collapse/expand item is selected
        if (mCollapsed)
            expandAll();
        else
            collapseAll();
    }
    else
    {
        // Check whether a custom item is selected
        QString menuItem;
        int resourceId = 0;
        QTreeWidgetItem* item = mResourceTree->currentItem();
        resourceId = getResourceIdFromItem(item);
        foreach (menuItem, mCustomContextMenuList)
        {
            if (action->text() == menuItem)
                emit customContextMenuItemSelected(menuItem, resourceId);
        }
    }
}

//****************************************************************************/
int QtResourceTreeWidget::generateUniqueResourceId(void)
{
    // Check the already added resources in the tree
    int id = 0;
    int max = 0;
    QTreeWidgetItemIterator it(mResourceTree);
    while (*it)
    {
        max = getResourceIdFromItem(*it);
        if (max > id)
            id = max;

        ++it;
    }

    // Also check that it is the highest of the registered resources
    foreach (QtResourceInfo* info, mRegisteredResources)
        if (info->resourceId > id)
            id = info->resourceId;

    id++; // Make it highest value in the tree and in the registered resources

    return id;
}

//****************************************************************************/
int QtResourceTreeWidget::getDepth(int resourceId)
{
    int parentId = getParentId(resourceId);
    if (parentId == 0)
        return 1;
    else
        return getDepth(parentId) + 1;
}

//****************************************************************************/
void QtResourceTreeWidget::searchLineTextChanged(QString text)
{
    findAndShowItems (text);
}

//****************************************************************************/
void QtResourceTreeWidget::findAndShowItems(const QString& searchPattern)
{
    resetSearch();
    emit resourceSearched(searchPattern);

    if (searchPattern.isEmpty())
        return;

    QString pattern = searchPattern.toLower();
    QTreeWidgetItemIterator it(mResourceTree);
    QTreeWidgetItem* parentItem;
    int parentId;
    QString name;
    while (*it)
    {
        if (isItemAsset(*it))
        {
            name = (*it)->text(0);
            name = name.toLower();
            if (!name.contains(pattern))
                (*it)->setHidden(true);
        }

        parentId = getParentIdFromItem(*it);
        hideParentIfChildrenAreHidden(parentId);

        ++it;
    }
    mResourceTree->expandAll();
}

//****************************************************************************/
void QtResourceTreeWidget::hideParentIfChildrenAreHidden(int parentId)
{
    QTreeWidgetItem* parentItem = getResourceItem(parentId);
    if (!parentItem)
        return;

    if (isItemToplevelGroup(parentItem))
        return;

    if (allChildrenHidden(parentId))
    {
        parentItem->setHidden(true);
        hideParentIfChildrenAreHidden(getParentIdFromItem(parentItem));
    }
}

//****************************************************************************/
void QtResourceTreeWidget::clearSearchLine(void)
{
    mSearchLine->clear();
    resetSearch();
    emit resourceSearchReset();
}

//****************************************************************************/
void QtResourceTreeWidget::resetSearch(void)
{
    QTreeWidgetItemIterator it(mResourceTree);
    while (*it)
    {
        (*it)->setHidden(false);
        ++it;
    }
}

//****************************************************************************/
bool QtResourceTreeWidget::allChildrenHidden(int parentId)
{
    // Iterate through the resource tree and check if there is any child not hidden
    QTreeWidgetItemIterator it(mResourceTree);
    QTreeWidgetItem* item;
    int parentIdOfIterator;
    while (*it)
    {
        parentIdOfIterator = getParentIdFromItem(*it);
        if (parentId == parentIdOfIterator && !(*it)->isHidden())
            return false;

        ++it;
    }
    return true;
}

//****************************************************************************/
void QtResourceTreeWidget::handleItemChanged (QTreeWidgetItem * item, int column)
{
    int id = getResourceIdFromItem(item);
    emit resourceChanged (id);
}

//****************************************************************************/
void QtResourceTreeWidget::addCustomContextMenuItem (const QString& menuItemText)
{
    mCustomContextMenuList.append(menuItemText);
    buildContextMenu();
}

//****************************************************************************/
void QtResourceTreeWidget::clearSelection (void)
{
    mResourceTree->clearSelection();
    mResourceTree->setCurrentItem(0);
}

//****************************************************************************/
void QtResourceTreeWidget::setIcon (int resourceId, const QString& iconName)
{
    QTreeWidgetItem* item = getResourceItem(resourceId);
    if (!item)
        return;

    // Set icon
    if (!iconName.isEmpty())
    {
        QImage imageIcon(mIconDir + iconName);
        QPixmap pixMapIcon = QPixmap::fromImage(imageIcon).scaled(assetIconWidth, assetIconHeight);
        item->setData(0, Qt::DecorationRole, QVariant(pixMapIcon));
        item->setData(TOOL_RESOURCETREE_KEY_ICONNAME, Qt::UserRole, QVariant(iconName));
    }
}
