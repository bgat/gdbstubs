/* -*-C-*-
 *
 * $Revision: 1.1 $
 *   $Author: bgat $
 *     $Date: 2003/05/02 20:59:34 $
 *
 * Copyright (c) 2000 ARM, INC.
 * All Rights Reserved.
 *
 *   Project: BootStrap Loader
 *
 *   Host side check sum generator for modules
 */

#include <stdio.h>
#include <stdlib.h>

#include "module.h"

#ifndef SEEK_SET
#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2
#endif

#define REAL_ADDRESS(head, address) real_address(head, (void *)(address))

int byte_reversing;

static unsigned byte_x(unsigned v)
{
    return (v << 24) | ((v << 8) & 0x00ff0000) | ((v >> 8) & 0x0000ff00) | (v >> 24);
}

#define tv(a) (byte_reversing ? (unsigned *)byte_x((unsigned)a) : (unsigned *)(a))

void *real_address(ModuleHeader *head, void *address)
{
    return (char *)address + ((char *)head - (char *)tv(head->self));
}

unsigned cksum(unsigned *base, unsigned *limit)
{
    unsigned sum = 0;
 
    while (base < limit) sum ^= *base++;
    return sum;
}

unsigned cksum_modules(unsigned *buff, unsigned size)
{
    ModuleHeader *head;
    unsigned i;
    unsigned *base, *limit;
    unsigned nmodules = 0;
    unsigned rm;

    rm = byte_x(MODULE_MAGIC);
    for (i = 0; i < size; i++) {
	if (buff[i] == MODULE_MAGIC || buff[i] == rm) {
	    byte_reversing = 0;
	    if (buff[i] == rm) byte_reversing = 1;
	    head = (ModuleHeader *)&buff[i];
	    base = (unsigned *)REAL_ADDRESS(head, tv(head->ro_base));
	    limit = (unsigned *)REAL_ADDRESS(head, tv(head->ro_limit))+(tv(head->zi_base) - tv(head->rw_base));
	    /* EOR checksum is the same either endianness */
	    head->checksum = 0;		/* Prevent foolishness */
	    head->checksum = cksum(base, limit);
	    printf("Found module %s, header at offset 0x%x, checksum = 0x%x\n",
		   REAL_ADDRESS(head, (char *)tv(head->title)), i * 4, (unsigned)tv(head->checksum));
	    printf("head = 0x%x, base = 0x%x, limit = 0x%x (size = 0x%x)\n"
		   "ro_base = 0x%x\n"
		   "ro_limit = 0x%x\n"
		   "rw_base = 0x%x\n"
		   "zi_base = 0x%x\n"
		   "zi_limit = 0x%x\n",
		   head, base, limit, (char*)limit - (char*)base,
		   tv(head->ro_base),
		   tv(head->ro_limit),
		   tv(head->rw_base),
		   tv(head->zi_base),
		   tv(head->zi_limit));
	    nmodules++;
	}
    }
    return nmodules;
}

int main(int argc, char **argv)
{
    FILE *f;
    unsigned size;
    unsigned nmodules;
    char *infile;
    unsigned *buff;

    if (argc != 2) {
	fprintf(stderr, "Usage: cksum <module>\n");
	exit(1);
    }
    infile = argv[1];
    f = fopen(infile, "r");
    if (!f) {
	fprintf(stderr, "Error opening '%s' with read access. Check filename and permissions\n", infile);
	exit(1);
    }
    if (fseek(f, 0, SEEK_END)) {
	fprintf(stderr, "Error performing seek on '%s'. Is it a rgular file\n", infile);
	exit(1);
    }
    size = ftell(f);
    if (fseek(f, 0, SEEK_SET)) {
	fprintf(stderr, "Error performing seek on '%s'. Is it a rgular file\n", infile);
	exit(1);
    }
    buff = malloc((size + 3) & ~3);
    buff[(size-1)/4] = 0;
    if (fread(buff, 1, size, f) != size || fclose(f)) {
	fprintf(stderr, "Error reading '%s'\n", infile);
	exit(1);
    }
    nmodules = cksum_modules(buff, (size + 3)/4);
    if (!nmodules) {
	fprintf(stderr, "No modules found in '%s'. Check module header has been linked in.\n", infile);
	exit(1);
    }
    printf("Checksum performed on %d module(s). Rewriting file '%s'.\n", nmodules, infile);
    f = fopen(infile, "w");
    if (!f) {
        fprintf(stderr, "Error opening '%s' with write access. Check permissions\n", infile);
        exit(1);
    }
    if (fwrite(buff, 1, size, f) != size || fclose(f)) {
	fprintf(stderr, "SERIOUS ERROR writing '%s', file may be corrupted.\n", infile);
	exit(1);
    }
    return 0;
}
