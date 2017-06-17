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

#ifndef CategoriesDOCWIDGET_H
#define CategoriesDOCWIDGET_H

#include "prerequisites.h"
#include "constants.h"
#include <QtWidgets>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QToolBar>
#include <QTabWidget>
#include "tool_resourcetree_widget.h"

QT_BEGIN_NAMESPACE
class QDockWidget;
QT_END_NAMESPACE

class MainWindow;

/****************************************************************************
 This class represents a DockWidget, containing the category tree. Assets/
 resources in the Workspace can be rearranged by adding them to a category.
 ***************************************************************************/
class _OM3Export CategoriesDockWidget : public QDockWidget
{
	Q_OBJECT

	public:
		CategoriesDockWidget(QString title, MainWindow* parent, Qt::WindowFlags flags = 0);
		~CategoriesDockWidget(void);

        /** Returns the current catagory. Because the category can be part of a hierarchy, the whole
         * tree is returned.
         */
        QVector<QString> getCategoryHierarchy (void);

        /** Determines wether filtering must be set on or off
         */
        const bool isFilterAssets (void) const {return mFilterAssets;}

    private slots:
        void handleCustomContextMenuItemSelected (const QString& menuItemText, int resourceId);
        void handleCategorySelected (int resourceId);

	private:
		MainWindow* mParent;
        QtResourceTreeWidget* mResourceTreeWidget;
        QVector<QString> mCategoryTree;
        bool mFilterAssets;
};

#endif

