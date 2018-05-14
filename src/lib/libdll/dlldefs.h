/***********************************************************************
 *                                                                      *
 *               This software is part of the ast package               *
 *          Copyright (c) 1997-2018 AT&T Intellectual Property          *
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
 *                                                                      *
 ***********************************************************************/

// : : generated by proto : :
// : : generated from ast/src/lib/libdll/features/dll by iffe version 2013-11-14 : :

#ifndef _def_dll_dll
#define _def_dll_dll 1

#define _hdr_dlfcn 1  // #include <dlfcn.h> ok
#define _lib_dlopen 1 // dlopen() in default lib(s)

#if defined(__MVS__) && !defined(__SUSV3)
#define __SUSV3 1
#endif
#include <dlfcn.h>

#define RTLD_PARENT 0

#define DLL_INFO_PREVER 0x0001 // pre-suffix style version
#define DLL_INFO_DOTVER 0x0002 // post-suffix style version

typedef unsigned long (*Dll_plugin_version_f)(void);
typedef int (*Dllerror_f)(void *, void *, int, ...);

typedef struct Dllinfo_s {
    char **sibling; // sibling dirs on $PATH
    char *prefix;   // library name prefix
    char *suffix;   // library name suffix
    char *env;      // library path env var
    int flags;      // DLL_INFO_* flags
#ifdef _DLLINFO_PRIVATE_
    _DLLINFO_PRIVATE_
#endif
} Dllinfo_t;

typedef struct Dllnames_s {
    char *id;
    char *name;
    char *base;
    char *type;
    char *opts;
    char *path;
    char data[1024];
} Dllnames_t;

typedef struct Dllent_s {
    char *path;
    char *name;
#ifdef _DLLENT_PRIVATE_
    _DLLENT_PRIVATE_
#endif
} Dllent_t;

typedef struct Dllscan_s {
    void *pad;
#ifdef _DLLSCAN_PRIVATE_
    _DLLSCAN_PRIVATE_
#endif
} Dllscan_t;

extern Dllinfo_t *dllinfo(void);
extern void *dllplugin(const char *, const char *, const char *, unsigned long, unsigned long *,
                       int, char *, size_t);
extern void *dllplug(const char *, const char *, const char *, int, char *, size_t);
extern void *dllfind(const char *, const char *, int, char *, size_t);
extern Dllnames_t *dllnames(const char *, const char *, Dllnames_t *);
extern void *dll_lib(Dllnames_t *, unsigned long, Dllerror_f, void *);
extern void *dllmeth(const char *, const char *, unsigned long);
extern void *dllopen(const char *, int);
extern void *dllnext(int);
extern void *dlllook(void *, const char *);
extern int dllcheck(void *, const char *, unsigned long, unsigned long *);
extern unsigned long dllversion(void *, const char *);
extern char *dllerror(int);

extern Dllscan_t *dllsopen(const char *, const char *, const char *);
extern Dllent_t *dllsread(Dllscan_t *);
extern int dllsclose(Dllscan_t *);

#endif
