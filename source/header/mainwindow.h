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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCloseEvent>
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QToolBar>
#include "categories_dockwidget.h"
#include "assets_dockwidget.h"
#include "dynlib_manager.h"

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

/****************************************************************************
 MainWindow is the main container window
 ***************************************************************************/
class PluginMediaWidgetInterface;
class _OM3Export MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
        typedef QVector<PluginInterface*> PluginVector;

        /** The MediaWidgetExtensionMap contains all extensions and pointers to the MediaWidget plugins
         * that creates the MediaWidget for this extension.
         * @remarks A QMap is used (instead of a multimap) to allow overwriting extension; this way it becomes
         * possible that an extension/plugin combination is overwritten, because a better solution is available.
         * This can be tweaked by changing the plugin order in the plugins config file.
         */
        typedef QMap<std::string, PluginMediaWidgetInterface*> MediaWidgetExtensionMap;
		MainWindow(void);
		~MainWindow(void);
		void update(void);
		bool mIsClosing;

        /** This function returns a pointer to a plugin that is able to ceate a MediaWidget for
         * a certain extension. E.g. if the extension is "jpg", the OM3MediaWidgetTexturePlugin
         * is returned (unless the plugin is surpassed by another plugin).
         */
        PluginMediaWidgetInterface* findPluginMediaWidgetByExtension (const std::string& extension);

	private slots:
        void doNewMenuAction(void);
        void doOpenMenuAction(void);
        void doSaveMenuAction(void);
        void doQuitMenuAction(void);
        void doResetWindowLayoutMenuAction(void);


	private:
		void createActions(void);
		void createMenus(void);
		void createToolBars(void);
		void createStatusBar(void);
		void createDockWindows(void);
		void closeEvent(QCloseEvent* event);

        QMenu* mWorkspaceMenu;
        QMenu* mSettingsMenu;
        QMenu* mWindowMenu;
        QAction* mNewMenuAction;
        QAction* mOpenMenuAction;
        QAction* mSaveMenuAction;
        QAction* mQuitMenuAction;
        QAction* mResetWindowLayoutMenuAction;
        CategoriesDockWidget* mCategoriesDockWidget;
        AssetsDockWidget* mAssetsDockWidget;
        DynLibManager* mDynLibManager;
        PluginVector mPluginVector;
        unsigned int mNumberOfLibraries;
        MediaWidgetExtensionMap mMediaWidgetExtensionMap;
};

#endif

