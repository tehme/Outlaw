QT += core network
CONFIG += c++11
TARGET = NetworkClient
TEMPLATE = lib

# Alternative: put everything to the same dir, but add 'd' postfix to debug libs.
CONFIG(debug, debug|release) {
    BUILDMODE = debug
} else {
    BUILDMODE = release
}

DESTDIR = ../../built/$${BUILDMODE}/Libs

DEFINES += NETWORK_CLIENT_LIBRARY

HEADERS += \
    NetworkClientGeneral.hpp \
    VarInt.hpp \
    MessageBuffer.hpp \
    TcpClient.hpp \
    BaseGameState.hpp


SOURCES += \
    VarInt.cpp \
    MessageBuffer.cpp \
    TcpClient.cpp \
    BaseGameState.cpp
