QT       += core gui
# Updated QT to a newer version (6.5.2)
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Would rather avoid using this multimedia library, doesn't seem very reliable
# greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutwindow.cpp \
    backend/combatCalculator.cpp \
    backend/entity.cpp \
    backend/fileImporter.cpp \
    backend/modifiersCalculator.cpp \
    backend/run_game.cpp \
    main.cpp \
    mainwindow.cpp \
    stream_buffer.cpp

HEADERS += \
    aboutwindow.h \
    backend/combatCalculator.h \
    backend/entity.h \
    backend/fileImporter.h \
    backend/modifiersCalculator.h \
    backend/run_game.h \
    mainwindow.h \
    mainwindow.hpp \
    stream_buffer.h

FORMS += \
    aboutwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    CMakeLists.txt
