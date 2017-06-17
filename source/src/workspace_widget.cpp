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
#include <QMouseEvent>
#include "workspace_widget.h"

//****************************************************************************/
WorkspaceWidget::WorkspaceWidget (QWidget* parent) :
    MediaListWidget (parent)
{
    connect(mContextMenu, SIGNAL(triggered(QAction*)), this, SLOT(handleContextMenuItemSelected(QAction*)));
}

//****************************************************************************/
WorkspaceWidget::~WorkspaceWidget (void)
{
}

//****************************************************************************/
void WorkspaceWidget::mousePressEvent( QMouseEvent* e )
{
    if (e->button() == Qt::RightButton)
    {
        QPoint pos;
        pos.setX(e->screenPos().x());
        pos.setY(e->screenPos().y());
        buildContextmenu (); // (Re)build the context menu, because it is very dynamic
        mContextMenu->popup(pos); // Open context menu
    }
    MediaListWidget::mousePressEvent(e); // Passthrough, so selecting an item still works
}

//****************************************************************************/
void WorkspaceWidget::handleContextMenuItemSelected(QAction* action)
{
    emit contextMenuItemSelected (action);
}
