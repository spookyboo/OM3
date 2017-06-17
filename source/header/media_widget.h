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

#ifndef MEDIA_WIDGET_H
#define MEDIA_WIDGET_H

#include <QWidget>
#include "plugin_interface.h"

/****************************************************************************
A Media widgets is used to show a media file as a widget (e.g. in the workspace)
MediaWidget is supposed to be subclassed in a plugin that implements the
PluginMediaWidgetInterface.
The MediaWidget class as such does not contain any functionality to make it
extendable when used in a plugin.
***************************************************************************/
class MediaWidget : public QWidget
{
    public:
        MediaWidget (AssetMetaData* assetMetaData, QWidget* parent);
        virtual ~MediaWidget (void);

        /** Add a tag to this MediaWidget; tags are used for filtering
         * Duplicates are ignored
         */
        virtual void addTag (const QString& tag);

        /** When a tag is part of a hierarchy with one or more parent tags, the
         * whole tree must be added. This keeps the hierarchy structure of the tags,
         * so when one of the tags in the tree is deleted, the whole tree must be deleted.
          */
        virtual void addTagHierarchy (const QVector<QString>& tagHierarchy);

        /** Delete a tag from this MediaWidget. If the tag is part of a tree, the
         * whole tree is deleted.
         * Ignored, if the tag is not found.
         */
        virtual void deleteTag (const QString& tag);

        /** Make the MediaWidget invisible if it does not comply to the tag.
         * Make it visible if it does comply.
         * @return bool, indicating that the widget is visible or not
         */
        virtual bool filterOnTag (const QString& tag);

        /** Make the MediaWidget invisible if it does not comply to the vector with tags.
         * Make it visible if it does comply.
         * @remark The tags do not have to be part of one tree.
         * @return bool, indicating that the widget is visible or not
         */
        virtual bool filterOnTags (const QVector<QString>& tags);

        /** Make the MediaWidget visible
         */
        virtual void resetFilter (void);

        /** Returns the information of this media widget
         */
        const AssetMetaData& getAssetMetaData (void) const {return mAssetMetaData;}

        /** Returns the information of this media widget
         */
        virtual void delegateActionByText (const QString& actionText) = 0;

    protected:
        AssetMetaData mAssetMetaData;
};

#endif
