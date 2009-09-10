/* radare - LGPL - Copyright 2008 nibble<.ds@gmail.com> */

#ifndef _INCLUDE_R_BIN_H_
#define _INCLUDE_R_BIN_H_

#include <r_types.h>
#include <r_io.h>
#include <list.h>

#define R_BIN_SCN_EXECUTABLE(x) x & 0x1
#define R_BIN_SCN_WRITABLE(x)   x & 0x2
#define R_BIN_SCN_READABLE(x)   x & 0x4
#define R_BIN_SCN_SHAREABLE(x)  x & 0x8

#define R_BIN_DBG_STRIPPED(x) x & 0x01
#define R_BIN_DBG_STATIC(x)   x & 0x02
#define R_BIN_DBG_LINENUMS(x) x & 0x04
#define R_BIN_DBG_SYMS(x)     x & 0x08
#define R_BIN_DBG_RELOCS(x)   x & 0x10

#define R_BIN_SIZEOF_STRINGS 256

/* types */
struct r_bin_t {
	const char *file;
	int fd;
	int rw;
	void *bin_obj;
	void *user;
	struct r_io_bind_t iob;
	struct r_bin_handle_t *cur;
	struct list_head bins;
};

struct r_bin_handle_t {
	char *name;
	char *desc;
	int (*init)(void *user);
	int (*fini)(void *user);
	int (*open)(struct r_bin_t *bin);
	int (*close)(struct r_bin_t *bin);
	int (*check)(struct r_bin_t *bin);
	ut64 (*baddr)(struct r_bin_t *bin);
	struct r_bin_entry_t* (*entry)(struct r_bin_t *bin);
	struct r_bin_section_t* (*sections)(struct r_bin_t *bin);
	struct r_bin_symbol_t* (*symbols)(struct r_bin_t *bin);
	struct r_bin_import_t* (*imports)(struct r_bin_t *bin);
	struct r_bin_string_t* (*strings)(struct r_bin_t *bin);
	struct r_bin_info_t* (*info)(struct r_bin_t *bin);
	struct r_bin_field_t* (*fields)(struct r_bin_t *bin);
	ut64 (*resize_section)(struct r_bin_t *bin, char *name, ut64 size);
	struct list_head list;
};

struct r_bin_entry_t {
	ut64 rva;
	ut64 offset;
};

struct r_bin_section_t {
	char name[R_BIN_SIZEOF_STRINGS];
	ut64 size;
	ut64 vsize;
	ut64 rva;
	ut64 offset;
	ut64 characteristics;
	int last;
};

struct r_bin_symbol_t {
	char name[R_BIN_SIZEOF_STRINGS];
	char forwarder[R_BIN_SIZEOF_STRINGS];
	char bind[R_BIN_SIZEOF_STRINGS];
	char type[R_BIN_SIZEOF_STRINGS];
	ut64 rva;
	ut64 offset;
	ut64 size;
	ut64 ordinal;
	int last;
};

struct r_bin_import_t {
	char name[R_BIN_SIZEOF_STRINGS];
	char bind[R_BIN_SIZEOF_STRINGS];
	char type[R_BIN_SIZEOF_STRINGS];
	ut64 rva;
	ut64 offset;
	ut64 ordinal;
	ut64 hint;
	int last;
};

struct r_bin_string_t {
	char string[R_BIN_SIZEOF_STRINGS];
	ut64 rva;
	ut64 offset;
	ut64 ordinal;
	ut64 size;
	int last;
};

struct r_bin_info_t {
	char type[R_BIN_SIZEOF_STRINGS];
	char class[R_BIN_SIZEOF_STRINGS];
	char rclass[R_BIN_SIZEOF_STRINGS];
	char arch[R_BIN_SIZEOF_STRINGS];
	char machine[R_BIN_SIZEOF_STRINGS];
	char os[R_BIN_SIZEOF_STRINGS];
	char subsystem[R_BIN_SIZEOF_STRINGS];
	int big_endian;
	ut64 dbg_info;
};

struct r_bin_field_t {
	char name[R_BIN_SIZEOF_STRINGS];
	ut64 rva;
	ut64 offset;
	int last;
};

/* bin.c */
R_API struct r_bin_t *r_bin_new();
R_API struct r_bin_t *r_bin_free(struct r_bin_t *bin);
R_API int r_bin_init(struct r_bin_t *bin);
R_API void r_bin_set_user_ptr(struct r_bin_t *bin, void *user);
R_API int r_bin_add(struct r_bin_t *bin, struct r_bin_handle_t *foo);
R_API int r_bin_list(struct r_bin_t *bin);
R_API int r_bin_open(struct r_bin_t *bin, const char *file, int rw, char *plugin_name);
R_API int r_bin_close(struct r_bin_t *bin);
R_API ut64 r_bin_get_baddr(struct r_bin_t *bin);
R_API struct r_bin_entry_t* r_bin_get_entry(struct r_bin_t *bin);
R_API struct r_bin_section_t* r_bin_get_sections(struct r_bin_t *bin);
R_API struct r_bin_symbol_t* r_bin_get_symbols(struct r_bin_t *bin);
R_API struct r_bin_import_t* r_bin_get_imports(struct r_bin_t *bin);
R_API struct r_bin_string_t* r_bin_get_strings(struct r_bin_t *bin);
R_API struct r_bin_info_t* r_bin_get_info(struct r_bin_t *bin);
R_API struct r_bin_field_t* r_bin_get_fields(struct r_bin_t *bin);
R_API ut64 r_bin_get_section_offset(struct r_bin_t *bin, char *name);
R_API ut64 r_bin_get_section_rva(struct r_bin_t *bin, char *name);
R_API ut64 r_bin_get_section_size(struct r_bin_t *bin, char *name);

/* plugin pointers */
extern struct r_bin_handle_t r_bin_plugin_elf;
extern struct r_bin_handle_t r_bin_plugin_elf64;
extern struct r_bin_handle_t r_bin_plugin_pe;
extern struct r_bin_handle_t r_bin_plugin_pe64;
extern struct r_bin_handle_t r_bin_plugin_java;
extern struct r_bin_handle_t r_bin_plugin_dummy;

#endif
