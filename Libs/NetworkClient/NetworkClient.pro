QT += core network
CONFIG += c++11
TARGET = NetworkClient
TEMPLATE = lib

include(../../BuiltDir.pri)

# Alternative: put everything to the same dir, but add 'd' postfix to debug libs.
DESTDIR = ../../$${BUILTDIR}/Libs


DEFINES += NETWORK_CLIENT_LIBRARY

HEADERS += \
    NetworkClientGeneral.hpp \
    VarInt.hpp \
    MessageBuffer.hpp \
    TcpClient.hpp \
    BaseGameState.hpp \
    PacketsV47.hpp


SOURCES += \
    VarInt.cpp \
    MessageBuffer.cpp \
    TcpClient.cpp \
    BaseGameState.cpp \
    PacketsV47.cpp
