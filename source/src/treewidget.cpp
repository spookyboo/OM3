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
#include "treewidget.h"
#include <QMessageBox>

//****************************************************************************/
QtTreeWidget::QtTreeWidget(QWidget* parent) : QTreeWidget(parent)
{
    mTypes.append(QString("test"));
}

//****************************************************************************/
QtTreeWidget::~QtTreeWidget(void)
{
}

//****************************************************************************/
void QtTreeWidget::addIntValueToMimeData (int key, int value)
{
    mKeyValueMap[key] = value;
}

//****************************************************************************/
QMimeData* QtTreeWidget::mimeData(const QList<QTreeWidgetItem *> items) const
{
    QMimeData* mimeData = new QMimeData();
    QMap<int, int> map = mKeyValueMap;
    QByteArray encodedData;
    mimeData->setObjectName(objectName());
    mimeData->setParent(parent());

    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    QMap<int, int>::iterator i;
    for (i = map.begin(); i != map.end(); ++i)
        stream << i.key() << i.value();

    mimeData->setData(QString("test"), encodedData);

    return mimeData;
}

//****************************************************************************/
Qt::DropActions	QtTreeWidget::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

//****************************************************************************/
QStringList	QtTreeWidget::mimeTypes() const
{
    return mTypes;
}
