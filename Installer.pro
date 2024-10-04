QT += core gui uitools network

CONFIG += c++11

SOURCES += main.cpp \
           dialog/OptionsDialog.cpp \
           dialog/Splash.cpp \
           dialog/download.cpp \
           functions/DataShop/DataShop.cpp \
           functions/GetSF.cpp \
           functions/IO_File.cpp \
           functions/argumentParser.cpp \
           functions/devicedetector.cpp \
           dialog/devNotes.cpp \
           functions/getFiles.cpp \
           functions/getFriendly.cpp \
           functions/interface/Phone.cpp \
           functions/interface/RecoveryInterface.cpp \
           functions/interface/adbI.cpp \
           functions/jsonparse.cpp \
           functions/log/loggingSystem.cpp \
           functions/parseDevices.cpp \
           functions/parseRedirects.cpp \
           functions/system.cpp \
           get/get_codename.cpp \
           get/get_device.cpp

HEADERS += \
    dialog/OptionsDialog.hpp \
    dialog/Splash.h \
    dialog/download.hpp \
    functions/DataShop/DataShop.hpp \
    functions/GetSF.h \
    functions/IO_File.h \
    functions/argumentParser.hpp \
    functions/devicedetector.h \
    dialog/devNotes.h \
    functions/getFiles.h \
    functions/getFriendly.h \
    functions/interface/Phone.hpp \
    functions/interface/RecoveryInterface.hpp \
    functions/interface/adbI.h \
    functions/jsonparse.h \
    functions/log/loggingSystem.hpp \
    functions/parseDevices.h \
    functions/parseRedirects.h \
    functions/system.h \
    get/get_codename.h \
    get/get_device.h

RESOURCES += resources.qrc

LIBS += -lcurl

QMAKE_CXXFLAGS += -frtti

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
