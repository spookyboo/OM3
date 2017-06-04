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

#ifndef ICONS_DIALOG_H
#define ICONS_DIALOG_H

#include <QDialog>
#include "prerequisites.h"
#include "constants.h"
#include "tool_default_texturewidget.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class _OgamExport IconsDialog : public QDialog
{
    Q_OBJECT

    public:
        IconsDialog (QWidget* parent);
        ~IconsDialog (void);

        const QString& getSelectedIconFilename (void) const {return mSelectedIconFilename;}
        const QString& getSelectedIconBasename (void) const {return mSelectedIconBasename;}

    private slots:
        void handleOkAndAccept(void);
        void handleCancel(void);
        void handleAssetSelected (const QString& name, const QString& baseName);
        void handleAssetDoubleClicked (const QString& name, const QString& baseName);

    protected:
        void addTextureFile (const QString& fileName);

    private:
        QtDefaultTextureWidget* mTextureWidget;
        QString mSelectedIconFilename;
        QString mSelectedIconBasename;
};

#endif
