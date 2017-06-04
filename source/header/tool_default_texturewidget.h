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

#ifndef TOOL_DEF_TEXTURE_WIDGET_H
#define TOOL_DEF_TEXTURE_WIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QLabel>
#include <QMenu>

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

/****************************************************************************
Helper class to combine the texture name and the texture
***************************************************************************/
class QtDefaultTextureAndText : public QWidget
{
    Q_OBJECT

    public:
        QString mBaseName;
        QString mName;
        QtDefaultTextureAndText(const QPixmap& pixmap,
                         const QString& name,
                         const QString& baseName,
                         const QSize& size,
                         QWidget* parent = 0);
        virtual ~QtDefaultTextureAndText(void);
        virtual void mouseMoveEvent(QMouseEvent *event);

        // Returns the pixmap
        const QPixmap* getPixmap (void);

    private:
        QLabel* mTextureLabel;
        QLineEdit* mBaseNameEdit;
};

//****************************************************************************/
//****************************************************************************/
/****************************************************************************
/****************************************************************************
The QtDefaultTextureListWidget is a QListWidget with convenience functions regarding
drag and drop.
***************************************************************************/
class QtDefaultTextureListWidget : public QListWidget
{
    Q_OBJECT

    public:
        QtDefaultTextureListWidget(QWidget* parent = 0);
        virtual ~QtDefaultTextureListWidget(void);

    signals:
        // Emitted when a texture file is dropped
        void textureFileDropped(const QString& name, const QString& baseName);

        // Emitted when an asset is deleted
        void assetDeleted(const QString& name, const QString& baseName);

        // Emitted when a texture file is moved
        void textureFileMoved(const QString& name, const QString& baseName);

        // Emitted when the asset is moved
        void assedDragMoved(const QString& name, const QString& baseName);

    protected:
        virtual void keyPressEvent(QKeyEvent* event);
        virtual void dropEvent(QDropEvent* event);
        virtual void dragEnterEvent(QDragEnterEvent *event);
        virtual void dragMoveEvent(QDragMoveEvent *event);
};

//****************************************************************************/
//****************************************************************************/
/****************************************************************************
Main class for default texture selection widget
***************************************************************************/
class QtDefaultTextureWidget : public QWidget
{
    Q_OBJECT

    public:
        QtDefaultTextureWidget(QWidget* parent = 0);
        virtual ~QtDefaultTextureWidget(void);

        // Add a pixmap to this widget. The name is a (fully qualified) filename for example.
        // E.g. name = "c:/temp/Tools/common/icons/info.png"
        // E.g. baseName = "info.png"
        void addTexture(const QPixmap& pixmap, const QString& name, const QString& baseName, bool replace = false);

        // Delete an item from the QtDefaultTextureWidget. Use the full qualified name as search criterium if nameIsFullName = true;
        // else use the baseName as search criterium.
        void deleteTexture(const QString& name, bool nameIsFullName = true);

        // Set a texture selected (based on name (fully qualified) or the basebame
        void setTextureSelected(const QString& name, bool isFullQualifiedName = false);

        // Delete an item from the QtDefaultTextureWidget. Both name (full qualified name) and baseName must match
        void deleteTexture(const QString& name, const QString& baseName);

        // Return the name of the selected texture
        // E.g. "c:/temp/Tools/common/icons/info.png"
        const QString& getNameTexture(void);

        // Return the base name of the selected texture
        // E.g. "info.png" in case the name of the texture is a full qualified filename.
        const QString& getBaseNameTexture(void);

        // If a texture is originated from a file, setOriginIsFile must be set to 'true'
        void setOriginIsFile(bool originIsFile);

        // Clear the content of the widget
        void clearContent(void);

        // Define the width and height of a texture in the selection box
        void setTextureSize (QSize size);
        QSize getTextureSize (void) {return mTextureSize;}

        // Apply filtering; only the items that meet the pattern are displayed
        void filter(const QString& pattern);

        // Apply filtering based on a StringVector with names
        void filter(const QStringList& names);

        // Reset the filtering
        void resetFilter(void);

        // Hide all items
        void clearAll(void);

        // Determine whether dropping texture files from the file explorer is allowed
        void setDropFilesAllowed(bool allowed);

        // Executes a system command when doubleclicked
        void setSystemCommandEditAsset(const QString& systemCommand);

        // Checks for a duplicate; added for HLMS Editor
        bool isTextureExisting(const QString& name);

        // Gets the currently selected pixmap
        const QPixmap* getCurrentPixmap (void);

        // Add an item to the context menu
        void addContextMenuActionText(const QString& actionText);

    signals:
        // Emitted when the mouse is pressed
        void mousePressed(const QString& name, const QString& baseName);

        // Emitted when a texture is selected (via the mouse)
        void selected(const QString& name, const QString& baseName);

        // Emitted when a texture is doubleclicked (via the mouse)
        void doubleClicked(const QString& name, const QString& baseName);

        // Emitted when a texture file is dropped
        void textureFileDropped(const QString& name, const QString& baseName);

        // Emitted when an asset is deleted
        void assetDeleted(const QString& name, const QString& baseName);

        // Emitted when an action of the context menu is selected
        void contextMenuSelected(QAction* action, const QString& name, const QString& baseName);

    protected slots:
        void handleSelected(QListWidgetItem* item);
        void handleSelected(const QString& name, const QString& baseName);
        void handleDoubleClicked(QListWidgetItem* item);
        void handleTextureFileDropped (const QString& name, const QString& baseName);
        void handleMouseOver(QListWidgetItem* item);
        void handleAssetDeleted(const QString& name, const QString& baseName);
        void handleContextMenuAction(QAction* action);

    private:
        QString mNameTexture; // In case of a filename, this is the fully qualified filename (path + filename)
        QString mBaseNameTexture; // If mNameTexture is a filename, this is the basename.
        QString mSystemCommandEditAsset;
        QtDefaultTextureListWidget* mSelectionList;
        QSize mTextureSize;
        bool mOriginIsFile;
        QMenu* mContextMenu;
};

#endif
