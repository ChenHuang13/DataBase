//
// Created by huangsy13 on 12/21/15.
//

#ifndef DATABASE_TOOL_H
#define DATABASE_TOOL_H

#include "define.h"

void setMin(uchar* a, int dt, int cl) {
    switch (dt) {
        case LL_TYPE:
            memcpy(a, &llmin, 8);
            memset(a + 8, 0xff, 8);
        case DB_TYPE:
            //todo
        default:
            a[0] = 0;
            memset(a + cl, 0xff, 8);
    }
}

void setMax(uchar* b, int dt, int cl) {
    switch (dt) {
        case LL_TYPE:
            memcpy(b, &llmax, 8);
            memset(b + 8, 0xff, 8);
        case DB_TYPE:
            //todo
        default:
            b[0] = 255;
            b[1] = 0;
            memset(b + cl, 0xff, 8);
    }
}

void getRange(void* src, int sl, int cl, int dt, int cp) {
    switch (cp) {
        case L:
            cr.rt = RANGE;
            memcpy(cr.b, src, sl);
            memset(cr.b + cl, 0xff, 8);
            setMin(cr.a, dt, cl);
            break;
        case LE:
            cr.rt = RANGE;
            memcpy(cr.b, src, sl);
            memset(cr.b + cl, 0x7f, 8);
            setMin(cr.a, dt, cl);
            break;
        case G:
            cr.rt = RANGE;
            memcpy(cr.a, src, sl);
            memset(cr.a + cl, 0x7f, 8);
            setMax(cr.b, dt, cl);
            break;
        case GE:
            cr.rt = RANGE;
            memcpy(cr.a, src, sl);
            memset(cr.a + cl, 0xff, 8);
            setMax(cr.b, dt, cl);
            break;
        case E:
            cr.rt = RANGE;
            memcpy(cr.a, src, sl);
            memset(cr.a + cl, 0xff, 8);
            memcpy(cr.b, src, sl);
            memset(cr.b + cl, 0x7f, 8);
            break;
        case IS:
            cr.rt = ISNULL;
    }
}


#endif //DATABASE_TOOL_H
