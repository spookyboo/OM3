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

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

//************************************ Icons *******************************************/
static const QString ICON_PATH = QString("../common/icons/");
static const QString ICON_CATEGORY_DEFAULT = QString("category_default.png");
static const QString ICON_FOLDER = QString("folder.png");
static const QString ICON_WORKSPACE = QString("workspace.png");
static const QString ICON_ASSET_DEFAULT = QString("asset_default.png");

//************************************ Contextmenu actions Category widget *******************************************/
static const QString CONTEXT_MENU_ACTION_ADD_CATEGORY = QString("Add category");
static const QString CONTEXT_MENU_ACTION_REMOVE_CATEGORY = QString("Remove category");
static const QString CONTEXT_MENU_ACTION_SET_ICON = QString("Set icon");
static const QString CONTEXT_MENU_ACTION_FILTER_ON_OFF = QString("Filter on category");
static const QString CONTEXT_MENU_ACTION_FILTER_ON = QString("On");
static const QString CONTEXT_MENU_ACTION_FILTER_OFF = QString("Off");


//************************************ Contextmenu actions Workspace *******************************************/
static const QString CONTEXT_MENU_ACTION_DELETE_FROM_WORKSPACE = QString("Delete selected asset(s) from the workspace");
static const QString CONTEXT_MENU_ACTION_CATEGORIZE = QString("Assign selected asset(s) to the current category");

//************************************ Misc *******************************************/
static const QString OM3_PLUGINS_CONFIG = QString("plugins.cfg");
static const QString PLUGINS_CONFIG_PROPERTY_NUMBER_OF_PLUGINS = QString("numberOfPlugins");
static const QString PLUGINS_CONFIG_PROPERTY_PLUGINS_PREFIX = QString("plugin_");

#endif
