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
#include <QString>
#include <QFile>
#include <QMenuBar>
#include <QSettings>
#include "constants.h"
#include "mainwindow.h"
#include "workspace_widget.h"
#include "plugin_resource_provider_interface.h"     // Only needed to force creation of 'plugin_resource_interface.obj', otherwise the plugins get a linking error
#include "plugin_media_widget_interface.h"          // Only needed to force creation of 'plugin_media_widget_interface.obj', otherwise the plugins get a linking error

typedef PluginInterface* (*PLUGIN_CREATE)(AssetsDockWidget* assetsDockWidget);
typedef void (*PLUGIN_DELETE)(void);

//****************************************************************************/
MainWindow::MainWindow (void) : mIsClosing(false)
{
	// Perform standard functions
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWindows();

    // Add the workspace;  OM3 iself is the 'resource provider' and adds an instance of WorkspaceWidget to the tab
    mWorkspaceWidget = new WorkspaceWidget();
    mWorkspaceWidget->addContextMenuItem(CONTEXT_MENU_ACTION_DELETE_FROM_WORKSPACE, WorkspaceWidget::CONTEXT_MEDIA_ITEMS_SELECTED);
    mWorkspaceWidget->addContextMenuItem(CONTEXT_MENU_ACTION_CATEGORIZE, WorkspaceWidget::CONTEXT_MEDIA_ITEMS_SELECTED);
    mAssetsDockWidget->addResourceProviderWidget (mWorkspaceWidget, ICON_PATH + ICON_WORKSPACE, "Workspace");
    connect(mWorkspaceWidget, SIGNAL(contextMenuItemSelected(QAction*)), this, SLOT(handleContextMenuItemSelected(QAction*)));

    // Load the plugins and add them to the mPluginVector
    mDynLibManager = new DynLibManager();
    QSettings settings(OM3_PLUGINS_CONFIG, QSettings::IniFormat);

    // Loop through the plugins config file
    mNumberOfLibraries = settings.value(PLUGINS_CONFIG_PROPERTY_NUMBER_OF_PLUGINS).toInt();
    QString libraryName;
    DynLib* library;
    PluginInterface* plugin;
    PluginMediaWidgetInterface* pluginMediaWidget;
    PluginMediaWidgetInterface::SupportedExtensions extensions;
    std::string extension;
    PLUGIN_CREATE pFunc;
    for (unsigned int i = 0; i < mNumberOfLibraries; ++i)
    {
        // Load all libraries defined in the plugins config file
        libraryName = settings.value(PLUGINS_CONFIG_PROPERTY_PLUGINS_PREFIX + QVariant(i+1).toString()).toString();
        library = DynLibManager::getSingleton().load(libraryName.toStdString()); // Loads the library
        pFunc = reinterpret_cast<PLUGIN_CREATE>(library->getSymbol("createPlugin")); // Reference to the function in the library that creates the plugin
        plugin = pFunc(mAssetsDockWidget); // Create the plugin in the library
        plugin->install(); // Only for resource plugins: Install adds a new tab to the mAssetsDockWidget
        mPluginVector.append(plugin);

        // Add all extension/plugin combinations to the mMediaWidgetExtensionMap
        if (plugin->getType() == PLUGIN_TYPE_MEDIA_WIDGET)
        {
            pluginMediaWidget = static_cast<PluginMediaWidgetInterface*>(plugin);
            extensions.clear();
            extensions = pluginMediaWidget->getSupportedExtensions();
            PluginMediaWidgetInterface::SupportedExtensions::iterator it = extensions.begin();
            PluginMediaWidgetInterface::SupportedExtensions::iterator itEnd = extensions.end();
            while (it != itEnd)
            {
                extension = *it;
                mMediaWidgetExtensionMap [extension] = pluginMediaWidget;
                ++it;
            }
        }
        mPluginVector.append(plugin);
    }

    // Set the title
    setWindowTitle(QString("Open Game Asset Manager"));

    // Set the stylesheet of the application
    QFile File(QString("dark.qss"));
    File.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(File.readAll());
    setStyleSheet(styleSheet);
	showMaximized();
}

//****************************************************************************/
MainWindow::~MainWindow (void)
{
    QSettings settings(OM3_PLUGINS_CONFIG, QSettings::IniFormat);
    PLUGIN_DELETE pFunc;
    QString libraryName;
    std::string stdLibraryName;
    DynLib* library;
    for (unsigned int i = 0; i < mNumberOfLibraries; ++i)
    {
        libraryName = settings.value(PLUGINS_CONFIG_PROPERTY_PLUGINS_PREFIX).toString() + QVariant(i).toString();
        stdLibraryName = libraryName.toStdString();
        library = DynLibManager::getSingleton().getDynLib(stdLibraryName);
        if (library)
        {
            // TODO: uninstall the plugins

            pFunc = reinterpret_cast<PLUGIN_DELETE>(library->getSymbol("deletePlugin"));
            pFunc(); // Deletes the plugin instance in the library
            DynLibManager::getSingleton().unload(library); // Unloads the library
        }
    }
}

//****************************************************************************/
void MainWindow::closeEvent(QCloseEvent* event)
{
    mIsClosing = true;
}

//****************************************************************************/
void MainWindow::createActions (void)
{
    mNewMenuAction = new QAction(QString("New"), this);
    connect(mNewMenuAction, SIGNAL(triggered()), this, SLOT(doNewMenuAction()));
    mOpenMenuAction = new QAction(QString("Open"), this);
    connect(mOpenMenuAction, SIGNAL(triggered()), this, SLOT(doOpenMenuAction()));
    mSaveMenuAction = new QAction(QString("Save"), this);
    connect(mSaveMenuAction, SIGNAL(triggered()), this, SLOT(doSaveMenuAction()));
    mQuitMenuAction = new QAction(QString("Quit"), this);
    connect(mQuitMenuAction, SIGNAL(triggered()), this, SLOT(doQuitMenuAction()));
    mResetWindowLayoutMenuAction = new QAction(QString("Reset Window Layout"), this);
    connect(mResetWindowLayoutMenuAction, SIGNAL(triggered()), this, SLOT(doResetWindowLayoutMenuAction()));

}

//****************************************************************************/
void MainWindow::createMenus (void)
{
    mWorkspaceMenu = menuBar()->addMenu(QString("&Workspace"));
    mWorkspaceMenu->addAction(mNewMenuAction);
    mWorkspaceMenu->addAction(mOpenMenuAction);
    mWorkspaceMenu->addAction(mSaveMenuAction);
    mWorkspaceMenu->addAction(mQuitMenuAction);
    mSettingsMenu = menuBar()->addMenu(QString("Settings"));
    mWindowMenu = menuBar()->addMenu(QString("Window"));
    mWindowMenu->addAction(mResetWindowLayoutMenuAction);

}

//****************************************************************************/
void MainWindow::createToolBars (void)
{

}

//****************************************************************************/
void MainWindow::createStatusBar (void)
{

}

//****************************************************************************/
void MainWindow::createDockWindows (void)
{
    mCategoriesDockWidget = new CategoriesDockWidget("Categories", this);
    addDockWidget(Qt::LeftDockWidgetArea, mCategoriesDockWidget);
    mAssetsDockWidget = new AssetsDockWidget("Assets", this);
    setCentralWidget(mAssetsDockWidget);
}

//****************************************************************************/
void MainWindow::doNewMenuAction (void)
{
    // Replace the code in this function with your own code.
    QMessageBox::information(this, QString("QAction"), QString("Menu item <New> selected;\nMainWindow::doNewMenuAction() called"));
}

//****************************************************************************/
void MainWindow::doOpenMenuAction (void)
{
    // Replace the code in this function with your own code.
    QMessageBox::information(this, QString("QAction"), QString("Menu item <Open> selected;\nMainWindow::doOpenMenuAction() called"));
}

//****************************************************************************/
void MainWindow::doSaveMenuAction (void)
{
    // Replace the code in this function with your own code.
    QMessageBox::information(this, QString("QAction"), QString("Menu item <Save> selected;\nMainWindow::doSaveMenuAction() called"));
}

//****************************************************************************/
void MainWindow::doQuitMenuAction (void)
{
    close();
}

//****************************************************************************/
void MainWindow::doResetWindowLayoutMenuAction (void)
{
    mCategoriesDockWidget->show();
    addDockWidget(Qt::LeftDockWidgetArea, mCategoriesDockWidget);
    mAssetsDockWidget->show();
    setCentralWidget(mAssetsDockWidget);

    // Delegate also to the plugins
    foreach (PluginInterface* plugin, mPluginVector)
    {
        plugin->resetWindowLayout();
    }
}


//****************************************************************************/
void MainWindow::update (void)
{

}

//****************************************************************************/
PluginMediaWidgetInterface* MainWindow::findPluginMediaWidgetByExtension (const std::string& extension)
{
    PluginMediaWidgetInterface* plugin = 0;
    MediaWidgetExtensionMap::iterator i = mMediaWidgetExtensionMap.find(extension);
    if (i != mMediaWidgetExtensionMap.end())
    {
        plugin = i.value();
    }

    return plugin;
}

//****************************************************************************/
void MainWindow::handleContextMenuItemSelected(QAction* action)
{
    QList<QListWidgetItem*> list = mWorkspaceWidget->selectedItems();
    MediaWidget* widget;
    QListWidgetItem* item;
    if (action->text() == CONTEXT_MENU_ACTION_DELETE_FROM_WORKSPACE)
    {
        /* It was a menu item added by the MainWindow, so handle it
         */
        if (!list.empty())
        {
            foreach (item, list)
            {
                widget = static_cast<MediaWidget*>(mWorkspaceWidget->itemWidget(item));
                mWorkspaceWidget->deleteItem(item);
            }
        }
    }
    else if (action->text() == CONTEXT_MENU_ACTION_CATEGORIZE)
    {
        if (!list.empty())
        {
            // Get the category vector from the CategoriesDockWidget
            QVector<QString> categories = mCategoriesDockWidget->getCategoryHierarchy ();
            if (!categories.empty())
            {
                // Run through each selected mediawidget and add the tags
                foreach (item, list)
                {
                    {
                        widget = static_cast<MediaWidget*>(mWorkspaceWidget->itemWidget(item));

                        // Add the elements from the vector to each selected MediaWidget, as tags
                        widget->addTagHierarchy(categories);
                    }
                }
            }
        }
    }
    else
    {
        /* It was not a menu item added by the CentralDockWidget, so it is probably a menu item associated
         * with a MediaWidget; delegate it
         */
        if (!list.empty())
        {
            foreach (item, list)
            {
                widget = static_cast<MediaWidget*>(mWorkspaceWidget->itemWidget(item));
                widget->delegateActionByText (action->text());
            }
        }
    }
}

//****************************************************************************/
void MainWindow::filterOnTag (const QString& tag)
{
    QList<QListWidgetItem*> list = mWorkspaceWidget->findItems(QString("*"), Qt::MatchWildcard);
    MediaWidget* widget;
    QListWidgetItem* item;
    foreach (item, list)
    {
        widget = static_cast<MediaWidget*>(mWorkspaceWidget->itemWidget(item));
        widget->filterOnTag(tag);
    }
}

//****************************************************************************/
void MainWindow::resetFilter (void)
{
    QList<QListWidgetItem*> list = mWorkspaceWidget->findItems(QString("*"), Qt::MatchWildcard);
    MediaWidget* widget;
    QListWidgetItem* item;
    foreach (item, list)
    {
        widget = static_cast<MediaWidget*>(mWorkspaceWidget->itemWidget(item));
        widget->resetFilter();
    }
}
