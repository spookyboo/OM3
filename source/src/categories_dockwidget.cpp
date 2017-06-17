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
#include "constants.h"
#include "mainwindow.h"
#include "Categories_dockwidget.h"
#include "icons_dialog.h"

//****************************************************************************/
CategoriesDockWidget::CategoriesDockWidget(QString title, MainWindow* parent, Qt::WindowFlags flags) : 
	QDockWidget (title, parent, flags), 
    mParent(parent),
    mFilterAssets(false)
{
    mResourceTreeWidget = new QtResourceTreeWidget (ICON_PATH);
    mResourceTreeWidget->generateUniqueResourceId(); // Start with 0

    // Custom actions
    mResourceTreeWidget->addCustomContextMenuItem(CONTEXT_MENU_ACTION_ADD_CATEGORY, false);
    mResourceTreeWidget->addCustomContextMenuItem(CONTEXT_MENU_ACTION_REMOVE_CATEGORY, false);
    mResourceTreeWidget->addCustomContextMenuItem(CONTEXT_MENU_ACTION_SET_ICON, false);
    mResourceTreeWidget->addCustomContextSubMenu(CONTEXT_MENU_ACTION_FILTER_ON_OFF, false);
    mResourceTreeWidget->addCustomContextSubMenuItem(CONTEXT_MENU_ACTION_FILTER_ON_OFF, CONTEXT_MENU_ACTION_FILTER_OFF, false);
    mResourceTreeWidget->addCustomContextSubMenuItem(CONTEXT_MENU_ACTION_FILTER_ON_OFF, CONTEXT_MENU_ACTION_FILTER_ON, false); // Add it reverse because of using a multimap
    mResourceTreeWidget->setDeleteResourceContextMenuItemEnabled(false, true); // Rebuild the contextmenu

    // Toplevel group settings
    mResourceTreeWidget->setCreateTopLevelGroupContextMenuItemEnabled(false);
    mResourceTreeWidget->setTopLevelGroupItemEditable(true);

    // Sub group settings
    mResourceTreeWidget->setCreateSubGroupContextMenuItemEnabled(false);
    mResourceTreeWidget->setSubLevelGroupItemEditable(true);
    mResourceTreeWidget->setSubgroupIconName(ICON_CATEGORY_DEFAULT);

    // Assets settings
    mResourceTreeWidget->setCreateAssetContextMenuItemEnabled(false);
    mResourceTreeWidget->setImportAssetContextMenuItemEnabled(false);
    mResourceTreeWidget->setDuplicateAssetContextMenuItemEnabled(false);
    mResourceTreeWidget->setAssetItemEditable(false);

    // Handlers
    connect(mResourceTreeWidget, SIGNAL(customContextMenuItemSelected(QString, int)), this, SLOT(handleCustomContextMenuItemSelected(QString, int)));
    connect(mResourceTreeWidget, SIGNAL(resourceSelected(int)), this, SLOT(handleCategorySelected(int)));

    // Perform standard functions
    setWidget(mResourceTreeWidget);
}

//****************************************************************************/
CategoriesDockWidget::~CategoriesDockWidget(void)
{
}

//****************************************************************************/
QVector<QString> CategoriesDockWidget::getCategoryHierarchy (void)
{
    // Build the tree based on the selected resource
    mCategoryTree.clear();
    int resourceId = mResourceTreeWidget->getSelectedResource();
    while (resourceId != 0)
    {
        mCategoryTree.push_back(mResourceTreeWidget->getResourceName(resourceId));
        resourceId = mResourceTreeWidget->getParentId(resourceId);
    }

    return mCategoryTree;
}

//****************************************************************************/
void CategoriesDockWidget::handleCustomContextMenuItemSelected (const QString& menuItemText, int resourceId)
{
    if (menuItemText == CONTEXT_MENU_ACTION_ADD_CATEGORY)
    {
        if (!mResourceTreeWidget->isResourceToplevelGroup(resourceId) &&
                !mResourceTreeWidget->isResourceSubGroup(resourceId) &&
                !mResourceTreeWidget->isResourceAsset(resourceId))
        {
            // If there is no toplevel add a toplevel category
            int id = mResourceTreeWidget->generateUniqueResourceId();
            mResourceTreeWidget->addResource (0,
                                              id,
                                              0,
                                              QString("Category"),
                                              QString("Category"),
                                              ICON_CATEGORY_DEFAULT);
            mResourceTreeWidget->clearSelection();
        }
        else if (mResourceTreeWidget->isResourceToplevelGroup(resourceId) ||
                 mResourceTreeWidget->isResourceSubGroup(resourceId))
        {
            // If the category is a toplevel or a sublevel, add the category to it
            mResourceTreeWidget->addResource (resourceId,
                                              QString("Category"),
                                              QString("Category"),
                                              ICON_CATEGORY_DEFAULT);
            mResourceTreeWidget->expand(resourceId);
            mResourceTreeWidget->clearSelection();
        }
    }
    else if (menuItemText == CONTEXT_MENU_ACTION_REMOVE_CATEGORY)
    {
        mResourceTreeWidget->deleteResource(resourceId, true);
    }
    else if (menuItemText == CONTEXT_MENU_ACTION_SET_ICON)
    {
        if (!mResourceTreeWidget->isResourceToplevelGroup(resourceId) &&
                !mResourceTreeWidget->isResourceSubGroup(resourceId) &&
                !mResourceTreeWidget->isResourceAsset(resourceId))
            return;

        IconsDialog iconsDialog(this);
        if (iconsDialog.exec())
        {
            mResourceTreeWidget->setIcon(resourceId, iconsDialog.getSelectedIconBasename());
        }
    }
    else if (menuItemText == CONTEXT_MENU_ACTION_FILTER_ON)
    {
        mFilterAssets = true;
        int resourceId = mResourceTreeWidget->getSelectedResource();
        QString categoryName = mResourceTreeWidget->getResourceName(resourceId);
        mParent->filterOnTag(categoryName);
    }
    else if (menuItemText == CONTEXT_MENU_ACTION_FILTER_OFF)
    {
        mFilterAssets = false;
        mParent->resetFilter ();
    }
}

//****************************************************************************/
void CategoriesDockWidget::handleCategorySelected (int resourceId)
{
    QString categoryName = mResourceTreeWidget->getResourceName(resourceId);
    if (mFilterAssets)
        mParent->filterOnTag(categoryName);
}
