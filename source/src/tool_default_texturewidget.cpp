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
#include <QDir>
#include <QLabel>
#include <QImageReader>
#include <QListWidgetItem>
#include <QDropEvent>
#include <QProcess>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include "core.h"
#include "tool_default_texturewidget.h"

//****************************************************************************/
QtDefaultTextureAndText::QtDefaultTextureAndText(const QPixmap& pixmap,
                                                 const QString& name,
                                                 const QString& baseName,
                                                 const QSize& size,
                                                 QWidget* parent) : QWidget(parent)
{
    setContentsMargins(-8, -8, -8, -8);
    setMinimumSize(size);
    setMaximumSize(size);
    QHBoxLayout* mainLayout = new QHBoxLayout;
    QVBoxLayout* textureAndNameLayout = new QVBoxLayout;

    mName = name;
    mBaseName = baseName;
    mBaseNameEdit = new QLineEdit;
    mBaseNameEdit->setText(mBaseName);
    mBaseNameEdit->setEnabled(false);
    mBaseNameEdit->hide();

    mTextureLabel = new QLabel();
    mTextureLabel->setPixmap(pixmap);
    mTextureLabel->setScaledContents(true);
    setMouseTracking(true);

    // Layout
    textureAndNameLayout->addWidget(mTextureLabel, 1000);
    textureAndNameLayout->addWidget(mBaseNameEdit, 1);
    mainLayout->addLayout(textureAndNameLayout);
    setLayout(mainLayout);
    setStyleSheet("QWidget{background-color : #333333;}");
}

//****************************************************************************/
QtDefaultTextureAndText::~QtDefaultTextureAndText(void)
{
}

//****************************************************************************/
void QtDefaultTextureAndText::mouseMoveEvent(QMouseEvent *event)
{
    // If not left button - return
    if (!(event->buttons() & Qt::LeftButton))
        return;

    // Start drag
    QDrag* drag = new QDrag(this);
    QMimeData* mimeData = new QMimeData;
    mimeData->setText(mName);
    mimeData->setObjectName("QtDefaultTextureAndText");
    drag->setMimeData(mimeData);
    drag->exec(Qt::CopyAction | Qt::MoveAction);
}

//****************************************************************************/
const QPixmap* QtDefaultTextureAndText::getPixmap (void)
{
    return mTextureLabel->pixmap();
}

//****************************************************************************/
//****************************************************************************/
//****************************************************************************/
QtDefaultTextureListWidget::QtDefaultTextureListWidget(QWidget* parent) : QListWidget(parent)
{
    setMouseTracking(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
}

//****************************************************************************/
QtDefaultTextureListWidget::~QtDefaultTextureListWidget(void)
{
}

//****************************************************************************/
void QtDefaultTextureListWidget::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
        case Qt::Key_Delete:
        {
            if (count() > 0)
            {
                QListWidgetItem* item = currentItem();
                if (item)
                {
                    QWidget* widget = itemWidget(item);
                    if (widget)
                    {
                        int r = row(item);
                        QtDefaultTextureAndText* textureAndText = static_cast<QtDefaultTextureAndText*>(widget);
                        QString name = textureAndText->mName;
                        QString baseName = textureAndText->mBaseName;
                        removeItemWidget(item);
                        takeItem(r);
                        emit assetDeleted(name, baseName);
                    }
                }
            }
        }
        break;
    }

    event->accept();
}

//****************************************************************************/
void QtDefaultTextureListWidget::dropEvent(QDropEvent* event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls())
    {
        QList<QUrl> urlList = mimeData->urls();
        for (int i = 0; i < urlList.size(); ++i)
        {
            QString baseName = urlList.at(i).fileName();
            if (isTypeBasedOnExtension(baseName, SUPPORTED_IMAGE_FORMATS, SUPPORTED_IMAGE_FORMATS_LENGTH))
            {
                QString name = urlList.at(i).path();
                stripLeadingSlashes(name);
                emit textureFileDropped(name, baseName);
            }
        }
    }
    event->acceptProposedAction();
}

//****************************************************************************/
void QtDefaultTextureListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

//****************************************************************************/
void QtDefaultTextureListWidget::dragMoveEvent(QDragMoveEvent *event)
{
    QListWidgetItem* item = currentItem();
    if (item)
    {
        QWidget* widget = itemWidget(item);
        if (widget)
        {
            QtDefaultTextureAndText* textureAndText = static_cast<QtDefaultTextureAndText*>(widget);
            QString name = textureAndText->mName;
            QString baseName = textureAndText->mBaseName;
            emit assedDragMoved(name, baseName);
        }
    }

    event->acceptProposedAction();
}

//****************************************************************************/
//****************************************************************************/
//****************************************************************************/
QtDefaultTextureWidget::QtDefaultTextureWidget(QWidget* parent) : QWidget(parent)
{
    setWindowTitle(QString("Texture selection"));
    mNameTexture = QString("");
    mBaseNameTexture = QString("");
    mSystemCommandEditAsset = QString("");
    mTextureSize = QSize(128, 218);
    mOriginIsFile = true;
    QHBoxLayout* mainLayout = new QHBoxLayout;
    QVBoxLayout* textureSelectionLayout = new QVBoxLayout;

    // Contextmenu
    installEventFilter(this);
    setContextMenuPolicy(Qt::CustomContextMenu);
    mContextMenu = new QMenu(this);
    connect(mContextMenu, SIGNAL(triggered(QAction*)), this, SLOT(handleContextMenuAction(QAction*)));

    // Define selection widget (QtDefaultTextureListWidget)
    mSelectionList = new QtDefaultTextureListWidget();
    mSelectionList->setViewMode(QListView::ListMode);
    mSelectionList->setWrapping(true);
    mSelectionList->setWordWrap(true);
    mSelectionList->setSpacing(0);
    mSelectionList->setUniformItemSizes(true);
    mSelectionList->setMovement(QListView::Snap);
    mSelectionList->setFlow(QListView::LeftToRight);
    connect(mSelectionList, SIGNAL(textureFileDropped(QString,QString)), this, SLOT(handleTextureFileDropped(QString,QString)));
    connect(mSelectionList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(handleSelected(QListWidgetItem*)));
    connect(mSelectionList, SIGNAL(assedDragMoved(QString,QString)), this, SLOT(handleSelected(QString,QString)));
    connect(mSelectionList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(handleDoubleClicked(QListWidgetItem*)));
    connect(mSelectionList, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(handleSelected(QListWidgetItem*)));
    //connect(mSelectionList, SIGNAL(itemEntered(QListWidgetItem*)), this, SLOT(handleMouseOver(QListWidgetItem*)));
    connect(mSelectionList, SIGNAL(assetDeleted(QString,QString)), this, SLOT(handleAssetDeleted(QString,QString)));

    // Layout
    textureSelectionLayout->addWidget(mSelectionList);
    mainLayout->addLayout(textureSelectionLayout);
    setLayout(mainLayout);
}

//****************************************************************************/
QtDefaultTextureWidget::~QtDefaultTextureWidget(void)
{
}

//****************************************************************************/
const QPixmap* QtDefaultTextureWidget::getCurrentPixmap (void)
{
    QListWidgetItem* item = mSelectionList->currentItem();
    if (item)
    {
        QWidget* widget = mSelectionList->itemWidget(item);
        if (widget)
        {
            QtDefaultTextureAndText* textureAndText = static_cast<QtDefaultTextureAndText*>(widget);
            return textureAndText->getPixmap();
        }
    }

    return 0;
}

//****************************************************************************/
void QtDefaultTextureWidget::addTexture(const QPixmap& pixmap, const QString& name, const QString& baseName, bool replace)
{
    if (replace)
    {
        deleteTexture(name);
    }

    if (!isTextureExisting(name))
    {
        QtDefaultTextureAndText* textureAndText = new QtDefaultTextureAndText(pixmap, name, baseName, mTextureSize, this);
        QListWidgetItem* item = new QListWidgetItem();
        item->setSizeHint(mTextureSize); // Must be present, otherwise the widget is not shown
        mSelectionList->addItem(item);
        mSelectionList->setItemWidget(item, textureAndText);
    }
}

//****************************************************************************/
void QtDefaultTextureWidget::deleteTexture(const QString& name, bool nameIsFullName)
{
    QtDefaultTextureAndText* textureAndText;
    QWidget* widget;
    int row;
    QList<QListWidgetItem*> list = mSelectionList->findItems(QString("*"), Qt::MatchWildcard);
    foreach (QListWidgetItem* item, list)
    {
        widget = mSelectionList->itemWidget(item);
        if (widget)
        {
            textureAndText = static_cast<QtDefaultTextureAndText*>(widget);
            if ((textureAndText->mName == name && nameIsFullName) ||
                (textureAndText->mBaseName == name && !nameIsFullName))
            {
                row = mSelectionList->row(item);
                mSelectionList->removeItemWidget(item);
                mSelectionList->takeItem(row);
                return;
            }
        }
    }
}

//****************************************************************************/
void QtDefaultTextureWidget::deleteTexture(const QString& name, const QString& baseName)
{
    QtDefaultTextureAndText* textureAndText;
    QWidget* widget;
    int row;
    QList<QListWidgetItem*> list = mSelectionList->findItems(QString("*"), Qt::MatchWildcard);
    foreach (QListWidgetItem* item, list)
    {
        widget = mSelectionList->itemWidget(item);
        if (widget)
        {
            textureAndText = static_cast<QtDefaultTextureAndText*>(widget);
            if (textureAndText->mName == name && textureAndText->mBaseName == baseName)
            {
                row = mSelectionList->row(item);
                mSelectionList->removeItemWidget(item);
                mSelectionList->takeItem(row);
                return;
            }
        }
    }
}

//****************************************************************************/
void QtDefaultTextureWidget::setTextureSelected(const QString& name, bool isFullQualifiedName)
{
    QtDefaultTextureAndText* textureAndText;
    QWidget* widget;
    QList<QListWidgetItem*> list = mSelectionList->findItems(QString("*"), Qt::MatchWildcard);
    foreach (QListWidgetItem* item, list)
    {
        widget = mSelectionList->itemWidget(item);
        if (widget)
        {
            textureAndText = static_cast<QtDefaultTextureAndText*>(widget);
            if (isFullQualifiedName)
            {
                if (textureAndText->mName == name)
                {
                    mSelectionList->setFocus();
                    item->setSelected(true);
                    return;
                }
            }
            else
            {
                if (textureAndText->mBaseName == name)
                {
                    mSelectionList->setFocus();
                    item->setSelected(true);
                    return;
                }
            }
        }
    }
}

//****************************************************************************/
const QString& QtDefaultTextureWidget::getNameTexture(void)
{
    return mNameTexture;
}

//****************************************************************************/
const QString& QtDefaultTextureWidget::getBaseNameTexture(void)
{
    return mBaseNameTexture;
}

//****************************************************************************/
void QtDefaultTextureWidget::setOriginIsFile(bool originIsFile)
{
    mOriginIsFile = originIsFile;
}

//****************************************************************************/
void QtDefaultTextureWidget::clearContent(void)
{
    mSelectionList->clear();
}

//****************************************************************************/
void QtDefaultTextureWidget::handleSelected(QListWidgetItem* item)
{
    QWidget* widget = mSelectionList->itemWidget(item);
    if (widget)
    {
        QtDefaultTextureAndText* textureAndText = static_cast<QtDefaultTextureAndText*>(widget);
        mNameTexture = textureAndText->mName;
        mBaseNameTexture = textureAndText->mBaseName;

        Qt::MouseButtons buttons = QApplication::mouseButtons();
        if (buttons & Qt::RightButton && mContextMenu->actions().size() > 0)
        {
            QPoint pos = QCursor::pos();
            mContextMenu->popup(pos);
        }
        emit selected(textureAndText->mName, textureAndText->mBaseName);
    }
}

//****************************************************************************/
void QtDefaultTextureWidget::handleSelected(const QString& name, const QString& baseName)
{
    emit selected(name, baseName);
}

//****************************************************************************/
void QtDefaultTextureWidget::handleDoubleClicked(QListWidgetItem* item)
{
    QWidget* widget = mSelectionList->itemWidget(item);
    if (widget)
    {
        QtDefaultTextureAndText* textureAndText = static_cast<QtDefaultTextureAndText*>(widget);
        if (!mSystemCommandEditAsset.isEmpty())
        {
            QProcess p;
            QStringList sl;
            sl.append(textureAndText->mName);
            p.startDetached(mSystemCommandEditAsset, sl);
        }

        emit doubleClicked(textureAndText->mName, textureAndText->mBaseName);
    }
}

//****************************************************************************/
void QtDefaultTextureWidget::handleTextureFileDropped(const QString& name, const QString& baseName)
{
    QPixmap pixmap(name);
    addTexture(pixmap, name, baseName);
    emit textureFileDropped(name, baseName);
}

//****************************************************************************/
void QtDefaultTextureWidget::handleMouseOver(QListWidgetItem* item)
{
    QWidget* widget = mSelectionList->itemWidget(item);
    if (widget)
    {
        QtDefaultTextureAndText* textureAndText = static_cast<QtDefaultTextureAndText*>(widget);
        QString name = textureAndText->mName;
        mSelectionList->setToolTip(name);
    }
}

//****************************************************************************/
void QtDefaultTextureWidget::handleAssetDeleted(const QString& name, const QString& baseName)
{
    emit assetDeleted(name, baseName);
}

//****************************************************************************/
void QtDefaultTextureWidget::setTextureSize (QSize size)
{
    mTextureSize = size;
}

//****************************************************************************/
void QtDefaultTextureWidget::filter(const QString& pattern)
{
    QtDefaultTextureAndText* textureAndText;
    QWidget* widget;
    QString name;
    QList<QListWidgetItem*> list = mSelectionList->findItems(QString("*"), Qt::MatchWildcard);
    foreach (QListWidgetItem* item, list)
    {
        widget = mSelectionList->itemWidget(item);
        if (widget)
        {
            textureAndText = static_cast<QtDefaultTextureAndText*>(widget);
            name = textureAndText->mBaseName;
            name = name.toLower();
            if (!name.contains(pattern))
                item->setHidden(true);
        }
    }
}

//****************************************************************************/
void QtDefaultTextureWidget::filter(const QStringList& names)
{
    clearAll();
    QtDefaultTextureAndText* textureAndText;
    QWidget* widget;
    QList<QListWidgetItem*> list = mSelectionList->findItems(QString("*"), Qt::MatchWildcard);
    foreach (QListWidgetItem* item, list)
    {
        widget = mSelectionList->itemWidget(item);
        foreach(QString name, names)
        {
            textureAndText = static_cast<QtDefaultTextureAndText*>(widget);
            if (name == textureAndText->mName)
                item->setHidden(false);
        }
    }
}

//****************************************************************************/
void QtDefaultTextureWidget::resetFilter(void)
{
    QList<QListWidgetItem*> list = mSelectionList->findItems(QString("*"), Qt::MatchWildcard);
    foreach (QListWidgetItem* item, list)
        item->setHidden(false);
}

//****************************************************************************/
void QtDefaultTextureWidget::clearAll(void)
{
    QList<QListWidgetItem*> list = mSelectionList->findItems(QString("*"), Qt::MatchWildcard);
    foreach (QListWidgetItem* item, list)
        item->setHidden(true);
}

//****************************************************************************/
void QtDefaultTextureWidget::setDropFilesAllowed(bool allowed)
{
    mSelectionList->setMouseTracking(allowed);
    mSelectionList->setAcceptDrops(allowed);
    mSelectionList->setDropIndicatorShown(allowed);
}

//****************************************************************************/
void QtDefaultTextureWidget::setSystemCommandEditAsset(const QString& systemCommand)
{
    mSystemCommandEditAsset = systemCommand;
}

//****************************************************************************/
bool QtDefaultTextureWidget::isTextureExisting(const QString& name)
{
    QtDefaultTextureAndText* textureAndText;
    QWidget* widget;
    QList<QListWidgetItem*> list = mSelectionList->findItems(QString("*"), Qt::MatchWildcard);
    foreach (QListWidgetItem* item, list)
    {
        widget = mSelectionList->itemWidget(item);
        if (widget)
        {
            textureAndText = static_cast<QtDefaultTextureAndText*>(widget);
            if (textureAndText->mName == name)
                return true;
        }
    }
    return false;
}

//****************************************************************************/
void QtDefaultTextureWidget::handleContextMenuAction(QAction* action)
{
    QListWidgetItem* item = mSelectionList->currentItem();
    if (item)
    {
        QWidget* widget = mSelectionList->itemWidget(item);
        if (widget)
        {
            QtDefaultTextureAndText* textureAndText = static_cast<QtDefaultTextureAndText*>(widget);
            QString name = textureAndText->mName;
            QString baseName = textureAndText->mBaseName;
            emit contextMenuSelected(action, name, baseName);
        }
    }
}

//****************************************************************************/
void QtDefaultTextureWidget::addContextMenuActionText(const QString& actionText)
{
    mContextMenu->addAction(new QAction(actionText));
}
