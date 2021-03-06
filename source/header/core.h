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

#ifndef CORE_H
#define CORE_H

#include <QTreeWidget>
#include <QMimeData>
#include <QStringList>
#include <QFileInfo>
#include <QMap>

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

static QString SUPPORTED_IMAGE_FORMATS[] = {"bmp", "ico", "jpg", "jif", "jpeg", "jpe", "jng", "koa", "iff", "lbm", "mng", "pbm", "pbm", "pcd", "pcx", "pgm", "pgm", "png", "ppm", "ppm", "ras", "tga", "targa", "tif", "tiff", "wap", "wbmp", "wbm", "psd", "cut", "xbm", "xpm", "gif", "hdr", "g3", "sgi", "exr", "j2k", "j2c", "jp2", "pfm", "pct", "pict", "pic", "dds"};
static int SUPPORTED_IMAGE_FORMATS_LENGTH = 45;
static QString SUPPORTED_AUDIO_FORMATS[] = {"mp3", "wav", "flac", "aac", "m4a", "3ga"};
static int SUPPORTED_AUDIO_FORMATS_LENGTH = 6;

/****************************************************************************
* Represent the origin of a source (i.e. sound, texture, mesh). Currently, only
* file types are available. This excludes sources from a database, sources
* from within a zip file, etc.
***************************************************************************/
enum Source {SOURCE_FILE};

/****************************************************************************
* Util function to determine whether a file is of a certain type, based on
* the file extenstion
***************************************************************************/
static bool isTypeBasedOnExtension(const QString& fileName, QString ext[], int arrayLength)
{
    int lastPoint = fileName.lastIndexOf(".");
    if (lastPoint > 0)
    {
        QString extension = fileName.right(fileName.length() - lastPoint - 1);
        extension = extension.toLower();
        for (int i = 0; i < arrayLength; ++i)
            if (extension == ext[i])
                return true;
    }

    return false;
}

/****************************************************************************
* Strip leading slashes and backslashes
***************************************************************************/
static void stripLeadingSlashes(QString& str)
{
    bool repeat = str.startsWith(QString("/")) || str.startsWith(QString("\\"));
    while (repeat)
    {
        str = str.remove(0, 1);
        repeat = str.startsWith(QString("/")) || str.startsWith(QString("\\"));
    }
}

/****************************************************************************
* Strip leading slashes and backslashes
***************************************************************************/
static bool fileExist(const QString& fileName)
{
    QFileInfo checkFile(fileName);
    if (checkFile.exists() && checkFile.isFile())
        return true;
    else
        return false;
}

#endif
