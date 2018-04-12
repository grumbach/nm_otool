# nm_otool

```rtfm
man nm; man otool
```

# Description

|Program       |Load commands |Segment Name |
|--------------|--------------|-------------|
|```nm```      |LC_SYMTAB     |             |
|```otool -t```|LC_SEGMENT_64 | __TEXT      |


### ```magic``` number
```c
#define MH_MAGIC    0xfeedface	/* the mach magic number */
#define MH_CIGAM    0xcefaedfe	/* NXSwapInt(MH_MAGIC) */
#define MH_MAGIC_64 0xfeedfacf	/* the 64-bit mach magic number */
#define MH_CIGAM_64 0xcffaedfe	/* NXSwapInt(MH_MAGIC_64) */
#define FAT_MAGIC   0xcafebabe	/* the fat magic number */
#define FAT_CIGAM   0xbebafeca	/* NXSwapLong(FAT_MAGIC) */
```

### ```mach_header``` and ```load_command```
```c
struct mach_header:             /* struct mach_header_64 for 64 */
    uint32_t        magic;      /* mach magic number identifier */
    cpu_type_t      cputype;    /* cpu specifier */
    cpu_subtype_t   cpusubtype; /* machine specifier */
    uint32_t        filetype;   /* type of file */
    uint32_t        ncmds;      /* number of load commands */
    uint32_t        sizeofcmds; /* the size of all the load commands */
    uint32_t        flags;      /* flags */
_64 uint32_t        reserved;   /* reserved */

struct load_command:            /* load_command header */
    uint32_t        cmd;        /* type of load command */
    uint32_t        cmdsize;    /* total size of command in bytes */
```

### ```symtab_command```
```c
struct symtab_command:
    uint32_t        cmd;        /* LC_SYMTAB */
    uint32_t        cmdsize;    /* sizeof(struct symtab_command) */
    uint32_t        symoff;     /* symbol table offset */
    uint32_t        nsyms;      /* number of symbol table entries */
    uint32_t        stroff;     /* string table offset */
    uint32_t        strsize;    /* string table size in bytes */
```

### Symbol ```nlist```
```c
struct nlist:                   /* struct nlist_64 for 64 */
    union           n_un:       /* union with char *n_name in _32 */
        uint32_t    n_strx;     /* index into the string table (union n_un) */
    uint8_t         n_type;     /* type flag, see below */
    uint8_t         n_sect;     /* section number or NO_SECT */
    int16_t         n_desc;     /* see <mach-o/stab.h> */
_32 uint32_t        n_value;    /* value of this symbol (or stab offset) */
_64 uint64_t        n_value;    /* value of this symbol (or stab offset) */
```

### ```nlist.n_type```
```c
/*
** The n_type field really contains four fields:
** unsigned char N_STAB:3
**               N_PEXT:1
**               N_TYPE:3
**               N_EXT:1
** which are used via the following masks.
*/
#define N_STAB  0xe0            /* if any of these bits set, a symbolic debugging entry */
#define N_PEXT  0x10            /* private external symbol bit */
#define N_TYPE  0x0e            /* mask for the type bits */
#define N_EXT   0x01            /* external symbol bit, set for external symbols */
/*
** Values for N_TYPE bits of the n_type field.
*/
#define N_UNDF  0x0             /* undefined, n_sect == NO_SECT */
#define N_ABS   0x2             /* absolute, n_sect == NO_SECT */
#define N_SECT  0xe             /* defined in section number n_sect */
#define N_PBUD  0xc             /* prebound undefined (defined in a dylib) */
#define N_INDR  0xa             /* indirect */
```
