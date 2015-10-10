CONFIG(debug, debug|release) {
    BUILDMODE = debug
} else {
    BUILDMODE = release
}

BUILTDIR = built/$${BUILDMODE}
