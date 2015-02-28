
COMPILER_IS_ARM = $$find(COMPILER, arm-oe.*)

count(COMPILER_IS_ARM, 1) {
    BUILD = trik
} else {
    BUILD = desktop
}

CONFIG(debug, debug | release) {
    CONFIGURATION = $$BUILD-debug
    CONFIGURATION_SUFFIX = -$$BUILD-d
} else {
    CONFIGURATION = $$BUILD-release
    equals(BUILD, "trik") {
        CONFIGURATION_SUFFIX =
    } else {
        CONFIGURATION_SUFFIX = -$$BUILD
    }
}

DESTDIR = $$PWD/bin/$$CONFIGURATION

PROJECT_BASENAME = $$basename(_PRO_FILE_)
PROJECT_NAME = $$section(PROJECT_BASENAME, ".", 0, 0)

TARGET = $$PROJECT_NAME$$CONFIGURATION_SUFFIX

OBJECTS_DIR = .build/$$CONFIGURATION/.obj
MOC_DIR = .build/$$CONFIGURATION/.moc
RCC_DIR = .build/$$CONFIGURATION/.rcc
UI_DIR = .build/$$CONFIGURATION/.ui

INCLUDEPATH += $$_PRO_FILE_PWD_ \
               $$_PRO_FILE_PWD_/include/$$PROJECT_NAME \
                $$_PRO_FILE_PWD_/include/internal \

LIBS += -L$$DESTDIR

QMAKE_CXXFLAGS += -std=c++11 -g -Wall -Wextra

GLOBAL_PWD = $$PWD

defineTest(uses) {
    LIBS += -L$$DESTDIR
    PROJECTS = $$1
    for(PROJECT, PROJECTS) {
        LIBS += -l$$PROJECT$$CONFIGURATION_SUFFIX
        INCLUDEPATH += $$GLOBAL_PWD/$$PROJECT/include $$GLOBAL_PWD/$$PROJECT/include/internal
    }
    export(LIBS)
    export(INCLUDEPATH)
}
