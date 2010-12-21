HEADERS += vibra/xqvibra.h \
    vibra/xqvibra_p.h

SOURCES += vibra/xqvibra.cpp \
    vibra/xqvibra_p.cpp

symbian:LIBS += -lhwrmvibraclient
