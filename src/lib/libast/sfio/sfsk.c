/***********************************************************************
 *                                                                      *
 *               This software is part of the ast package               *
 *          Copyright (c) 1985-2012 AT&T Intellectual Property          *
 *                      and is licensed under the                       *
 *                 Eclipse Public License, Version 1.0                  *
 *                    by AT&T Intellectual Property                     *
 *                                                                      *
 *                A copy of the License is available at                 *
 *          http://www.eclipse.org/org/documents/epl-v10.html           *
 *         (with md5 checksum b35adb5213ca9657e911e9befb180842)         *
 *                                                                      *
 *              Information and Software Systems Research               *
 *                            AT&T Research                             *
 *                           Florham Park NJ                            *
 *                                                                      *
 *               Glenn Fowler <glenn.s.fowler@gmail.com>                *
 *                    David Korn <dgkorn@gmail.com>                     *
 *                     Phong Vo <phongvo@gmail.com>                     *
 *                                                                      *
 ***********************************************************************/
#include "config_ast.h"  // IWYU pragma: keep

#include <sys/types.h>
#include <unistd.h>

#include "sfhdr.h"
#include "sfio.h"

/*      Seek function that knows discipline
**
**      Written by Kiem-Phong Vo.
*/
Sfoff_t sfsk(Sfio_t *f, Sfoff_t addr, int type, Sfdisc_t *disc) {
    Sfoff_t p;
    Sfdisc_t *dc;
    ssize_t s;
    int local, mode;
    SFMTXDECL(f)

    SFMTXENTER(f, (Sfoff_t)(-1))

    GETLOCAL(f, local);
    if (!local && !(f->bits & SF_DCDOWN)) {
        if ((mode = f->mode & SF_RDWR) != (int)f->mode && _sfmode(f, mode, 0) < 0)
            SFMTXRETURN(f, -1)
        if (SFSYNC(f) < 0) SFMTXRETURN(f, -1)
        if (f->mode == SF_READ && (f->bits & SF_MMAP) && f->data) {
            SFMUNMAP(f, f->data, f->endb - f->data);
            f->data = NULL;
        }
        f->next = f->endb = f->endr = f->endw = f->data;
    }

    if ((type &= (SEEK_SET | SEEK_CUR | SEEK_END)) > SEEK_END) SFMTXRETURN(f, -1)

    for (;;) {
        dc = disc;
        if (f->flags & SF_STRING) {
            SFSTRSIZE(f);
            if (type == SEEK_SET) {
                s = (ssize_t)addr;
            } else if (type == SEEK_CUR) {
                s = (ssize_t)(addr + f->here);
            } else {
                s = (ssize_t)(addr + f->extent);
            }
        } else {
            SFDISC(f, dc, seekf);
            if (dc && dc->seekf) {
                SFDCSK(f, addr, type, dc, p);
            } else {
                p = lseek(f->file, (sfoff_t)addr, type);
            }
            if (p >= 0) SFMTXRETURN(f, p)
            s = -1;
        }

        if (local) SETLOCAL(f);
        switch (_sfexcept(f, SF_SEEK, s, dc)) {
            case SF_EDISC:
            case SF_ECONT:
                if (f->flags & SF_STRING) SFMTXRETURN(f, (Sfoff_t)s)
                goto do_continue;
            default:
                SFMTXRETURN(f, (Sfoff_t)-1)
        }

    do_continue:
        for (dc = f->disc; dc; dc = dc->disc) {
            if (dc == disc) break;
        }
        disc = dc;
    }
}
