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

#ifndef AssetsDOCWIDGET_H
#define AssetsDOCWIDGET_H

#include "prerequisites.h"
#include "constants.h"
#include <QtWidgets>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QToolBar>
#include <QTabWidget>
#include "media_listwidget.h"
#include "media_widget.h"

QT_BEGIN_NAMESPACE
class QDockWidget;
QT_END_NAMESPACE

class MainWindow;
class ResourceWidget;

/****************************************************************************
 This class represents a DockWidget
 ***************************************************************************/
class PluginMediaWidgetInterface;
class _OM3Export AssetsDockWidget : public QDockWidget
{
	Q_OBJECT

	public:
        AssetsDockWidget (QString title, MainWindow* parent, Qt::WindowFlags flags = 0);
        ~AssetsDockWidget (void);

        /** Add a new tab to the AssetsDockWidget and set the widget, created by the resource
         * provider, in the tab. This widget can be any kind of QWidget. OM3 does not enforce how
         * a resource provider is implemented. It only expects a widget that can be displayed in the
         * tab.
         * * This function is typically called by  a resource provider plugin.
         */
        void addResourceProviderWidget (QWidget* widget,
                                        QString fullQualifiedIconName,
                                        QString tabName);

        /** Creates and returns an instance of a MediaWidget, based on the (file) extension of the
         * asset/resource. Specific MediaWidget plugins are used to actually create MediaWidgets.
         * This MediaListWidget is not managed by the AssetsDockWidget.
         */
        MediaWidget* createMediaWidget (AssetMetaData* assetMetaData, QWidget* parent = 0);

        /** A resource managed by a resource provider plugin can be added to the OM3 workbench,
         * making it a local resource. A copy of the resource is made and used within the workbench.
         * This function is typically called by  a resource provider plugin.
         */
        void addResourceToWorkbench (const AssetMetaData& assetMetaData);

    private slots:
        void tabSelected (int index);

	private:
		MainWindow* mParent;
        QMainWindow* mInnerMain;
        QTabWidget* mTabWidget;
        PluginMediaWidgetInterface* mDefaultMediaWidgetPlugin;
        bool mResourcesAdded; // Determines wether a ResourceWidget was added
        bool mDefaultMediaWidgetPluginMissingWarning; // Used to inform that a plugin is missing
};

#endif

