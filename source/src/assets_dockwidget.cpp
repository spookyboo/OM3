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
#include "plugin_media_widget_interface.h"

//****************************************************************************/
AssetsDockWidget::AssetsDockWidget (QString title, MainWindow* parent, Qt::WindowFlags flags) :
	QDockWidget (title, parent, flags), 
    mParent (parent),
    mResourcesAdded (false),
    mDefaultMediaWidgetPlugin (0),
    mDefaultMediaWidgetPluginMissingWarning (false)
{
    // Create widgets
    mInnerMain = new QMainWindow();
    setWidget(mInnerMain);
    mTabWidget = new QTabWidget(mInnerMain);
    mInnerMain->setCentralWidget(mTabWidget);
    setWindowTitle(QString(""));

    // TODO: Read the global properties config file
}

//****************************************************************************/
AssetsDockWidget::~AssetsDockWidget (void)
{
}

//****************************************************************************/
void AssetsDockWidget::addResourceProviderWidget (QWidget* widget,
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
MediaWidget* AssetsDockWidget::createMediaWidget (AssetMetaData* assetMetaData, QWidget* parent)
{
    // The plugins are registered in mParent
    PluginMediaWidgetInterface* plugin = mParent->findPluginMediaWidgetByExtension (assetMetaData->extension);
    if (plugin)
    {
        // The plugin is found, so it creates the appropriate widget
        return plugin->createMediaWidget(assetMetaData, parent);
    }
    else if (mDefaultMediaWidgetPlugin)
    {
        // The specific plugin is not found, so use the default
        return mDefaultMediaWidgetPlugin->createMediaWidget(assetMetaData, parent);
    }
    else
    {
        // The default plugin was not yet created, so create it first
        mDefaultMediaWidgetPlugin = mParent->findPluginMediaWidgetByExtension ("default");
        if (mDefaultMediaWidgetPlugin)
        {
            return mDefaultMediaWidgetPlugin->createMediaWidget(assetMetaData, parent);
        }
        else
        {
            if (!mDefaultMediaWidgetPluginMissingWarning)
            {
                // Warning; Once is enough
                QMessageBox::warning (0, QString("Warning"), QString("The default media widget plugin is missing; did you add it to the plugins.cfg file?"));
                mDefaultMediaWidgetPluginMissingWarning = true;
            }
        }
    }
}

/****************************************************************************/
void AssetsDockWidget::addResourceToWorkbench (const AssetMetaData& assetMetaData)
{
    // Todo:
    // - Copy the resource from fullQualifiedFileNameImport to fullQualifiedFileNameLocal
    // - Add it to the workbench
    QMessageBox::information(0, "addResourceToWorkbench", "Done");
}

//****************************************************************************/
const QVariant& AssetsDockWidget::getGlobalConfigPropertyValue (const QString& propertyName)
{
    // TODO
    return QVariant();
}

//****************************************************************************/
void AssetsDockWidget::tabSelected (int index)
{
    // Replace the code in this function with your own code.
    //QString message = QString("Tab with index <") + QVariant(index).toString() + QString("> selected");
    //QMessageBox::information(this, QString("QTabWidget"), message);
}

