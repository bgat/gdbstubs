
# prefix for installed gcc tools
TARGET=h8300-hms

# build tools definition
CC=$(TARGET)-gcc

DEPS 		= $(h8s_2633evb_DEPS)

# Hitachi h8s architecture, 2633 evb (using hdi)

ASMDUMP	= -Wa,-alh,-L -fverbose-asm  > $*.dmp
# use -O0 if you would like to debug this stub in some debugger ;)
CFLAGS	= -DGDB_STATUS_EXPEDITED -W -Wall -g -ms -Os
LFLAGS	= -g -ms

h8s_2633evb_c_files	:= gdb h8s-2633 h8s-2633evb h8s-2633-sci
h8s_2633evb_s_files	:= h8s-2633-vects
h8s_2633evb_MODULES 	:= $(h8s_2633evb_c_files) $(h8s_2633evb_s_files)
h8s_2633evb_OBJS	:= $(h8s_2633evb_MODULES:%=%.o)
h8s_2633evb_DEPS	:= $(h8s_2633evb_MODULES:%=%.d)
h8s_2633evb_LIBS	:=

h8s-2633evb.out: $(h8s_2633evb_OBJS) Makefile h8s-2633evb.cmd
	$(CC) -nostartfiles -o $*.out $(LFLAGS) \
		$(h8s_2633evb_OBJS) $(h8s_2633evb_LIBS) \
		-Wl,-Th8s-2633evb.cmd \
		-Wl,-M -Wl,"-Map",$*.map \

# default rule for *.c files
%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@ $(ASMDUMP)

# gcc -MM has problems with subdirs, use this woraround
%.d : %.c
	@$(CC) -MM $< > $*.d-tmp
	@echo -n $(*D)/ > $*.d && cat $*.d-tmp >> $*.d && rm -f $*.d-tmp

# default rule for *.s files
%.o : %.s
	$(CC) -g -c -W -Wall $< -o $@

%.d : %.s
	$(CC) -MM $< > $*.d


# supply some delault rules for other output formats

%.bin: %.out
	$(OBJCOPY) --output-target binary --strip-all $< $@

%.srec: %.out
	$(OBJCOPY) --output-target srec --strip-all $< $@

include $(DEPS)

.PHONY : clean
clean :
	rm -f *.o
	rm -f *.dmp 
	rm -f *.d 
	rm -f *.out *.bin *.srec
	rm -f *.map
