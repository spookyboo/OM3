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
A Media widgets is used to show a media file as a widget (e.g. in the workbench
***************************************************************************/
class MediaWidget : public QWidget
{
    public:
        MediaWidget (const AssetMetaData& assetMetaData, QWidget* parent = 0)
        {
            mAssetMetaData = assetMetaData;
        }
        virtual ~MediaWidget (void) {}

        /**
          Returns the information of this media widget
          */
        const AssetMetaData& getAssetMetaData (void) const {return mAssetMetaData;}

    private:
        AssetMetaData mAssetMetaData;
};

#endif
