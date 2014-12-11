/* -*-C-*-
 *
 * $Revision: 1.1 $
 *   $Author: bgat $
 *     $Date: 2003/05/03 02:23:35 $
 *
 * Copyright (c) 2000 ARM, INC.
 * All Rights Reserved.
 *
 *   Project: BootStrap Loader
 *
 *   bsl module definitions
 */

#ifndef MODULE_H
#define MODULE_H

#ifdef	__ARMCC_VERSION
extern unsigned Image$$RO$$Base[];
extern unsigned Image$$RO$$Limit[];
extern unsigned Image$$RW$$Base[];
extern unsigned Image$$ZI$$Base[];
extern unsigned Image$$ZI$$Limit[];
#endif

typedef struct ModuleHeader ModuleHeader;
typedef struct CmdTable CmdTable;
typedef struct SWIRegs SWIRegs;
typedef struct ModuleInfo *ModuleHandle;
typedef char *TitleString;
typedef char *HelpString;
typedef unsigned SWIBase;
typedef unsigned ARMWord;

typedef void (*CallBack)(void);

typedef struct ServiceBlock ServiceBlock;

struct ServiceBlock {
    CallBack r0;
    unsigned r1;
    unsigned r2;
    unsigned r3;
};

typedef void (*StartCode)(char *cmd);
typedef ModuleHandle (*InitCode)(void);
typedef void (*FinalCode)(void);
typedef ServiceBlock (*ServiceCode)(ServiceBlock sb);
typedef CallBack (*CommandCode)(char *cmd);
typedef CallBack (*SWICode)(unsigned swino, SWIRegs *regs);

struct SWIRegs {
	unsigned	r[13];
};

struct CmdTable {
	char 		*command;
	CommandCode	code;
	unsigned	flags;
	char		*syntax;
	char		*help;
};

#define MODULE_MAGIC	0x4d484944	/* 'MHID' */

#define UNPLUGGED_FLAG	0x0001
#define AUTOSTART_FLAG	0x0002
#define FACTORY_FLAG	0x0004

#define AEB_MINOR_VERSION	(0)
#define EV7T_MINOR_VERSION	(1)

#define MAJOR_VERSION	1
#define MINOR_VERSION	EV7T_MINOR_VERSION

struct ModuleHeader {
	unsigned	magic;
	unsigned short	flags;
	unsigned char	major;
	unsigned char	minor;
	unsigned	checksum;
	ARMWord		*ro_base;
	ARMWord		*ro_limit;
	ARMWord		*rw_base;
	ARMWord		*zi_base;
	ARMWord		*zi_limit;
	const ModuleHeader	*self;
	StartCode	start;
	InitCode	init;
	FinalCode	final;
	ServiceCode	service;
	TitleString	title;
	HelpString	help;
	const CmdTable	*cmdtbl;
	SWIBase		swi_base;
	SWICode		swi_handler;
};

#endif
