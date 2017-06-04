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
#include "constants.h"
#include "mainwindow.h"
typedef PluginInterface* (*PLUGIN_CREATE)(AssetsDockWidget* assetsDockWidget);
typedef void (*PLUGIN_DELETE)(void);

//****************************************************************************/
MainWindow::MainWindow(void) : mIsClosing(false)
{
	// Perform standard functions
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWindows();

    // Add the workbench
    mAssetsDockWidget->addWidget(new QWidget(), ICON_PATH + ICON_WORKBENCH, "Workbench");
    // TODO: Change 'new QWidget()'

    // Load the plugins and add them to the AssesDockWidget
    mDynLibManager = new DynLibManager();
    mFileResourceLibrary = DynLibManager::getSingleton().load("OgamFileResourcePlugin"); // Loads the 'OgamFileResourcePlugin' library
    PLUGIN_CREATE pFunc = reinterpret_cast<PLUGIN_CREATE>(mFileResourceLibrary->getSymbol("createPlugin")); // Reference to the function in the library that creates the plugin
    PluginInterface* plugin = pFunc(mAssetsDockWidget); // Create the plugin in the library
    plugin->install(); // Install adds a new tab to the mAssetsDockWidget (in case of the 'OgamFileResourcePlugin' library at least)
    mPluginVector.append(plugin);

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
MainWindow::~MainWindow(void)
{
    // TODO: uninstall the plugins

    PLUGIN_DELETE pFunc = reinterpret_cast<PLUGIN_DELETE>(mFileResourceLibrary->getSymbol("deletePlugin"));
    pFunc(); // Deletes the plugin instance in the library
    DynLibManager::getSingleton().unload(mFileResourceLibrary); // Unloads the library
}

//****************************************************************************/
void MainWindow::closeEvent(QCloseEvent* event)
{
    mIsClosing = true;
}

//****************************************************************************/
void MainWindow::createActions(void)
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
void MainWindow::createMenus(void)
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
void MainWindow::createToolBars(void)
{

}

//****************************************************************************/
void MainWindow::createStatusBar(void)
{

}

//****************************************************************************/
void MainWindow::createDockWindows(void)
{
    mCategoriesDockWidget = new CategoriesDockWidget("Categories", this);
    addDockWidget(Qt::LeftDockWidgetArea, mCategoriesDockWidget);
    mAssetsDockWidget = new AssetsDockWidget("Assets", this);
    setCentralWidget(mAssetsDockWidget);
}

//****************************************************************************/
void MainWindow::doNewMenuAction(void)
{
    // Replace the code in this function with your own code.
    QMessageBox::information(this, QString("QAction"), QString("Menu item <New> selected;\nMainWindow::doNewMenuAction() called"));
}

//****************************************************************************/
void MainWindow::doOpenMenuAction(void)
{
    // Replace the code in this function with your own code.
    QMessageBox::information(this, QString("QAction"), QString("Menu item <Open> selected;\nMainWindow::doOpenMenuAction() called"));
}

//****************************************************************************/
void MainWindow::doSaveMenuAction(void)
{
    // Replace the code in this function with your own code.
    QMessageBox::information(this, QString("QAction"), QString("Menu item <Save> selected;\nMainWindow::doSaveMenuAction() called"));
}

//****************************************************************************/
void MainWindow::doQuitMenuAction(void)
{
    close();
}

//****************************************************************************/
void MainWindow::doResetWindowLayoutMenuAction(void)
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
void MainWindow::update(void)
{

}
