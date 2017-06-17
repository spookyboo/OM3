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

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QImageReader>
#include <QFileInfo>
#include <QImage>
#include <QPixmap>
#include <QException>
#include "constants.h"
#include "core.h"
#include "icons_dialog.h"

//****************************************************************************/
IconsDialog::IconsDialog(QWidget* parent) :
    QDialog(parent)
{
    mSelectedIconFilename = "";
    mSelectedIconBasename = "";
    setWindowTitle(QString("Icon selection"));
    QVBoxLayout* mainLayout = new QVBoxLayout;

    // Texture widget
    mTextureWidget = new QtDefaultTextureWidget(this);
    mTextureWidget->setTextureSize(QSize(72, 64)); // Add 8 pixels to the width to compensate the frame width
    connect(mTextureWidget, SIGNAL(selected(QString,QString)), this, SLOT(handleAssetSelected(QString,QString)));
    connect(mTextureWidget, SIGNAL(doubleClicked(QString,QString)), this, SLOT(handleAssetDoubleClicked(QString,QString)));

    // Fill the texture widget
    addTextureFile (ICON_PATH + ICON_FOLDER); // TEST
    addTextureFile (ICON_PATH + ICON_WORKSPACE); // TEST
    addTextureFile (ICON_PATH + ICON_CATEGORY_DEFAULT); // TEST
    addTextureFile (ICON_PATH + ICON_ASSET_DEFAULT); // TEST

    // Close and Cancel button
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QPushButton* button = buttonBox->button(QDialogButtonBox::Ok);
    button->setText("Ok");
    button = buttonBox->button(QDialogButtonBox::Cancel);
    button->setText("Cancel");
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(handleOkAndAccept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(handleCancel()));

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(buttonBox);

    // Layout
    mainLayout->addWidget(mTextureWidget);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);
}

//****************************************************************************/
IconsDialog::~IconsDialog(void)
{
}

//****************************************************************************/
void IconsDialog::handleOkAndAccept (void)
{
    accept();
}

//****************************************************************************/
void IconsDialog::handleCancel (void)
{
    reject();
}

//****************************************************************************/
void IconsDialog::handleAssetSelected (const QString& name, const QString& baseName)
{
    mSelectedIconFilename = name;
    mSelectedIconBasename = baseName;
}

//****************************************************************************/
void IconsDialog::handleAssetDoubleClicked (const QString& name, const QString& baseName)
{
    mSelectedIconFilename = name;
    mSelectedIconBasename = baseName;
    accept();
}

//****************************************************************************/
void IconsDialog::addTextureFile (const QString& fileName)
{
    QFileInfo info (fileName);
    QString baseName = info.baseName();
    QImage image;
    QPixmap pixmap;
    try
    {
        if (fileExist(fileName))
        {
            // Decrease the texture, otherwise it cannot be loaded by the pixmap
            QImageReader reader(fileName);
            reader.setScaledSize(mTextureWidget->getTextureSize()); // Prevents from reading to much data in memory
            image = reader.read();
            pixmap.convertFromImage(image);
        }
        else
        {
            pixmap.load(ICON_ASSET_DEFAULT);
        }

        mTextureWidget->addTexture(pixmap, fileName, baseName);
    }
    catch (QException e){}
}
