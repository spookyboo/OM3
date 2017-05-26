QT += widgets

DEFINES += OGAM_IMPORT_EXPORT_PLUGIN_EXPORTS

HEADERS       = \
    source/header/magus_core.h \
    source/header/mainwindow.h \
    source/header/categories_dockwidget.h \ 
    source/header/assets_dockwidget.h \ 
    source/header/tool_resourcetree_widget.h \
    source/header/magus_treewidget.h \
    source/header/constants.h \
    source/header/icons_dialog.h \
    source/header/tool_default_texturewidget.h \
    source/header/dynlib.h \
    source/header/dynlib_manager.h \
    source/header/singleton.h \
    source/header/plugin_interface.h


SOURCES       = \
    source/src/main.cpp \
    source/src/mainwindow.cpp \
    source/src/categories_dockwidget.cpp \ 
    source/src/assets_dockwidget.cpp \ 
    source/src/tool_resourcetree_widget.cpp \
    source/src/magus_treewidget.cpp \
    source/src/icons_dialog.cpp \
    source/src/tool_default_texturewidget.cpp \
    source/src/dynlib.cpp \
    source/src/dynlib_manager.cpp


INCLUDEPATH += "../Ogam/source/header/"




Release:DESTDIR = ../Ogam/bin
Debug:DESTDIR = ../Ogam/bin
target.path = $$[QTDIR]/
INSTALLS += target

