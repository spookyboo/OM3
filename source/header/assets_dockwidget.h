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
class _OgamExport AssetsDockWidget : public QDockWidget
{
	Q_OBJECT

	public:
        AssetsDockWidget (QString title, MainWindow* parent, Qt::WindowFlags flags = 0);
        ~AssetsDockWidget (void);

        /** Add a new tab to the AssetsDockWidget and set the widget, passed as an
         * argument, in the tab.
         */
        void addWidget (QWidget* widget,
                        QString fullQualifiedIconName,
                        QString tabName);

        /** Returns an instance of a MediaWidget, based on the (file) extension of the asset/resource.
         * A specific MediaWidget plugin is used to actually create the MediaWidget
         * This MediaListWidget is not managed by the AssetsDockWidget.
         */
        MediaWidget* createMediaWidget (const AssetMetaData& assetMetaData);

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

