#pragma once

#define SAFE_DELETE(p) { \
    if (p) { \
        delete(p); \
        (p) = nullptr; \
    } \
}

#define SAFE_DELETE_ARRAY(ary) { \
    if (ary) { \
        delete[](ary); \
        ary = nullptr; \
    } \
}

/**
*   アプリの続行が不可能となるエラーが発生した場合、呼び出す
*/
#define HALT() { \
    PostQuitMessage(0); \
}