TARGET = tst_client
CONFIG += testcase

QT += serviceframework testlib
QT -= gui

DESTDIR = ./

# Increase the stack size on MSVC to 4M to avoid a stack overflow
win32-msvc*:QMAKE_LFLAGS += /STACK:4194304

CONFIG -= app_bundle

!mtlib:!contains(config_test_mtlib, yes):DEFINES += TESTDATA_DIR=\\\"$$PWD/\\\"

jsondb|contains(QT_CONFIG, jsondb): {
    mtlib|contains(config_test_mtlib, yes): {
        DEFINES += QT_JSONDB
    }
}

!contains(DEFINES, QT_JSONDB):contains(QT_CONFIG,dbus): {
    QT += dbus
    DEFINES+=SFW_USE_DBUS_BACKEND
}

SOURCES += tst_qservicemanager_ipc.cpp

symbian {
    TARGET.CAPABILITY = ALL -TCB
}
