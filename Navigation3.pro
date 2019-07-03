QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        CFileIO.cpp \
        CGPSSensor.cpp \
        CJsonPersistence.cpp \
        CJsonScanner.cpp \
        CJsonToken.cpp \
        CNavigationSystem.cpp \
        CPOI.cpp \
        CPoiDatabase.cpp \
        CRoute.cpp \
        CWaypoint.cpp \
        CWpDatabase.cpp \
        InvalidCharacterException.cpp \
        OverloadedOperator.cpp \
        ReadWriteUtils.cpp \
        lex.json.cc \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Database-poi.txt \
    Database-wp.txt \
    Navigation3.pro.user \
    json2.txt \
    json3.txt \
    jsonError.txt \
    jsonErrorC.txt \
    jsonInput.txt \
    jsonOutput.txt \
    jsonWarning.txt \
    jsonWarningC.txt \
    test2-poi.txt \
    test2-wp.txt \
    userInput-poi.txt \
    userInput-wp.txt \
    userOutput-poi.txt \
    userOutput-wp.txt \
    userOutput.txt

HEADERS += \
    CDatabase.h \
    CFileIO.h \
    CGPSSensor.h \
    CJsonPersistence.h \
    CJsonScanner.h \
    CJsonToken.h \
    CNavigationSystem.h \
    CPOI.h \
    CPersistentStorage.h \
    CPoiDatabase.h \
    CRoute.h \
    CWaypoint.h \
    CWpDatabase.h \
    FlexLexer.h \
    InvalidCharacterException.h \
    OverloadedOperator.h \
    ReadWriteUtils.h \
    lex.json.h \
    main.cpp.autosave
