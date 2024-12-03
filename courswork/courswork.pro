QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    application.cpp \
    database.cpp \
    groupwork.cpp \
    main.cpp \
    authorization.cpp \
    managementsubjects.cpp \
    objectwork.cpp \
    register.cpp \
    stipwork.cpp \
    studentwork.cpp

HEADERS += \
    application.h \
    authorization.h \
    database.h \
    groupwork.h \
    managementsubjects.h \
    objectwork.h \
    register.h \
    stipwork.h \
    studentwork.h

FORMS += \
    application.ui \
    groupwork.ui \
    managementsubjects.ui \
    objectwork.ui \
    register.ui \
    authorization.ui \
    stipwork.ui \
    studentwork.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
