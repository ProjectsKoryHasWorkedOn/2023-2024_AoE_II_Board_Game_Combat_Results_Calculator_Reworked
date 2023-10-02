QT       += core gui
# Updated QT to a newer version (6.5.2)


# Set icon
RC_ICONS = swords_clashing.ico


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets core multimedia

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutwindow.cpp \
    aliases.cpp \
    backend/combatCalculator.cpp \
    backend/entity.cpp \
    backend/fileImporter.cpp \
    backend/modifiersCalculator.cpp \
    backend/run_game.cpp \
    csv/csv_entity.cpp \
    csv/csv_event.cpp \
    csv/csv_player_name.cpp \
    csv/csv_technology.cpp \
    csv/entities.cpp \
    csv/player_names.cpp \
    main.cpp \
    mainwindow.cpp \
    soundEffects.cpp \
    stream_buffer.cpp

HEADERS += \
    aboutwindow.h \
    aliases.h \
    backend/combatCalculator.h \
    backend/entity.h \
    backend/fileImporter.h \
    backend/modifiersCalculator.h \
    backend/run_game.h \
    csv/csv_entity.h \
    csv/csv_event.h \
    csv/csv_player_name.h \
    csv/csv_technology.h \
    csv/entities.h \
    csv/player_names.h \
    mainwindow.h \
    mainwindow.hpp \
    soundEffects.h \
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
