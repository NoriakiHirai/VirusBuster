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
*   �A�v���̑��s���s�\�ƂȂ�G���[�����������ꍇ�A�Ăяo��
*/
#define HALT() { \
    PostQuitMessage(0); \
}