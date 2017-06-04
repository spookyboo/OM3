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
#include "constants.h"
#include "mainwindow.h"
#include "Assets_dockwidget.h"

//****************************************************************************/
AssetsDockWidget::AssetsDockWidget(QString title, MainWindow* parent, Qt::WindowFlags flags) : 
	QDockWidget (title, parent, flags), 
    mParent(parent),
    mResourcesAdded(false)
{
    mInnerMain = new QMainWindow();
    setWidget(mInnerMain);
    mTabWidget = new QTabWidget(mInnerMain);
    mInnerMain->setCentralWidget(mTabWidget);
    setWindowTitle(QString(""));
}

//****************************************************************************/
AssetsDockWidget::~AssetsDockWidget(void)
{
}

//****************************************************************************/
void AssetsDockWidget::addWidget (QWidget* widget,
                                  QString fullQualifiedIconName,
                                  QString tabName)
{
    // Ad a new tab with the resourceWidget to the AssetsDockWidget
    mTabWidget->addTab(widget, QIcon(fullQualifiedIconName), tabName);

    // Show the tab and establish singal/slot only once
    if (!mResourcesAdded)
    {
        mTabWidget->show();
        connect(mTabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabSelected(int)));
    }

    mResourcesAdded = true;
}


//****************************************************************************/
void AssetsDockWidget::tabSelected(int index)
{
    // Replace the code in this function with your own code.
    //QString message = QString("Tab with index <") + QVariant(index).toString() + QString("> selected");
    //QMessageBox::information(this, QString("QTabWidget"), message);
}

MediaListWidget* AssetsDockWidget::createMediaListWidget(void)
{
    return new MediaListWidget();
}
