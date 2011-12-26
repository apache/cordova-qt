# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
wwwDir.source = www
xmlDir.source = xml

DEPLOYMENTFOLDERS = wwwDir xmlDir # file1 dir1

symbian:TARGET.UID3 = 0xE3522943

# Smart Installer package's UID
# This UID is from the protected range 
# and therefore the package will fail to install if self-signed
# By default qmake uses the unprotected range value if unprotected UID is defined for the application
# and 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable.
CONFIG += mobility
MOBILITY += feedback location systeminfo

SOURCES += main.cpp mainwindow.cpp \
    src/plugins/notification.cpp \
    src/plugins/geolocation.cpp \
    src/plugins/fileapi.cpp \
    src/plugins/device.cpp \
    src/pluginregistry.cpp \
    src/plugins/console.cpp \
    src/phonegap.cpp \
    src/pgplugin.cpp \
    src/pgwebpage.cpp \
    src/plugins/connection.cpp
HEADERS += mainwindow.h \
    src/plugins/notification.h \
    src/plugins/geolocation.h \
    src/plugins/fileapi.h \
    src/plugins/device.h \
    src/pluginregistry.h \
    src/plugins/console.h \
    src/phonegap.h \
    src/pgplugin.h \
    src/pgwebpage.h \
    src/plugins/connection.h
FORMS += mainwindow.ui

QT += webkit

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

OTHER_FILES +=






























































