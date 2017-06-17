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
#include "media_widget.h"

//****************************************************************************/
MediaWidget::MediaWidget (AssetMetaData* assetMetaData, QWidget* parent) :
    QWidget (parent)
{
    /* The pointer to the assetMetaData is volatile; the MediaWidget will keep its
     * own copy (also to enrich it with its own data), using a copy constructor.
     */
    mAssetMetaData = AssetMetaData(*assetMetaData);
}

//****************************************************************************/
MediaWidget::~MediaWidget(void)
{
}

//****************************************************************************/
void MediaWidget::addTag (const QString& tag)
{
    std::vector<std::string> v;
    v.push_back(tag.toStdString());
    mAssetMetaData.tags.push_back(v);
}

//****************************************************************************/
void MediaWidget::addTagHierarchy (const QVector<QString>& tagHierarchy)
{
    if (tagHierarchy.empty())
        return;

    // First convert it to a version with std::vector and std::string
    AssetMetaData::TagHierarchy th;
    foreach (QString tag, tagHierarchy)
    {
        th.push_back(tag.toStdString());
    }

    mAssetMetaData.tags.push_back(th);
}

//****************************************************************************/
void MediaWidget::deleteTag (const QString& tag)
{
    AssetMetaData::Tags::iterator itTags = mAssetMetaData.tags.begin();
    AssetMetaData::Tags::iterator itTagsEnd = mAssetMetaData.tags.end();
    AssetMetaData::TagHierarchy tagHierarchy;
    std::string tagInHierarchy;
    std::string tagToCompare = tag.toStdString();
    bool deleteHierarchy;
    while (itTags != itTagsEnd)
    {
        tagHierarchy = *itTags;
        AssetMetaData::TagHierarchy::iterator itHierarchy = tagHierarchy.begin();
        AssetMetaData::TagHierarchy::iterator itHierarchyEnd = tagHierarchy.end();
        deleteHierarchy = false;
        while (itHierarchy != itHierarchyEnd)
        {
            tagInHierarchy = *itHierarchy;
            if (tagInHierarchy == tagToCompare)
            {
                // The tag is part of this hierarchy; delete this vector
                deleteHierarchy = true;
                break;
            }

            ++itHierarchy;
        }

        if (deleteHierarchy)
        {
            itTags = mAssetMetaData.tags.erase(itTags);
        }
        else
        {
            ++itTags;
        }
    }
}

//****************************************************************************/
bool MediaWidget::filterOnTag (const QString& tag)
{
    if (mAssetMetaData.tags.empty())
    {
        // Fast check
        setVisible(false);
        return false;
    }

    AssetMetaData::Tags::iterator itTags = mAssetMetaData.tags.begin();
    AssetMetaData::Tags::iterator itTagsEnd = mAssetMetaData.tags.end();
    AssetMetaData::TagHierarchy tagHierarchy;
    std::string tagInHierarchy;
    std::string tagToCompare = tag.toStdString();
    while (itTags != itTagsEnd)
    {
        tagHierarchy = *itTags;
        AssetMetaData::TagHierarchy::iterator itHierarchy = tagHierarchy.begin();
        AssetMetaData::TagHierarchy::iterator itHierarchyEnd = tagHierarchy.end();
        while (itHierarchy != itHierarchyEnd)
        {
            tagInHierarchy = *itHierarchy;
            if (tagInHierarchy == tagToCompare)
            {
                // The tag is part of this hierarchy; make the widget visible
                setVisible(true);
                return true;
            }

            ++itHierarchy;
        }

        ++itTags;
    }

    setVisible(false);
    return false;
}

//****************************************************************************/
bool MediaWidget::filterOnTags (const QVector<QString>& tags)
{
    foreach (QString tag, tags)
    {
        if (filterOnTag(tag))
            return true;
    }

    setVisible(false);
    return false;
}

//****************************************************************************/
void MediaWidget::resetFilter (void)
{
    setVisible(true);
}
