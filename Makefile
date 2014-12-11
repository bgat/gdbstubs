H8300_H8S2148EDK_STUB = main.c gdb.c h8300.c h8300.S
H8300_H8S2148EDK_TEST = test.c
H8300_PREFIX = h8300-elf-
H8300_CFLAGS = -Wall -g -O2 -ms -mint32

ARM7TDMI_PREFIX = arm-elf-
ARM7TDMI_E7T_STUB = arm7tdmi.c arm7tdmi.S arm7tdmi-ks32c50100.c gdb.c
ARM7TDMI_CFLAGS = -Wall -g -O0 -v

MIPS_PREFIX = mips-elf-
MIPS_CSB250_STUB = mips.c mips-a.S csb250.c gdb.c
MIPS_CFLAGS =  -Wall -g -O0 -mips1 -mno-crt0 -T mips.ld


.PHONY: h8300-stub


h8300-stub: h8300-h8s2148edk-stub.elf h8300-h8s2148edk-stub.mot

h8300-test: h8300-h8s2148edk-test.elf

h8300-h8s2148edk-stub.mot: h8300-h8s2148edk-stub.elf
	$(H8300_PREFIX)objcopy -O srec $< $@

h8300-h8s2148edk-stub.elf: $(H8300_H8S2148EDK_STUB)
	$(H8300_PREFIX)gcc $(H8300_CFLAGS) -o $@ \
		-nostartfiles -nodefaultlibs \
		-DCRT0 -DH8300_H8S2148EDK $(H8300_H8S2148EDK_STUB) \
		-Wl,--script=h8300-h8s2148edk-stub.x 
	$(H8300_PREFIX)objdump --syms $@ | grep " _start"

h8300-h8s2148edk-test.elf: $(H8300_H8S2148EDK_TEST)
	$(H8300_PREFIX)gcc $(CFLAGS) -o $@ \
		$(H8300_H8S2148EDK_TEST) \
		-Wl,--script=h8300-h8s2148edk.x 



.PHONY: e7t-stub

e7t-stub: e7t-stub.elf 

e7t-stub.uue: e7t-stub.bin ./arm-bsl-cksum
	./arm-bsl-cksum e7t-stub.bin
	uuencode $< $< | tr '`' ' ' > $@

e7t-stub.bin: e7t-stub.elf
	$(ARM7TDMI_PREFIX)objcopy -O binary $< $@

e7t-stub.elf: $(ARM7TDMI_E7T_STUB)
	$(ARM7TDMI_PREFIX)gcc -DCRT0 -DKS32C50100 \
		$(ARM7TDMI_CFLAGS) -o $@ \
		-nostartfiles -nodefaultlibs \
		-Wl,--script="arm7tdmi-e7t-stub.x" \
		$(ARM7TDMI_E7T_STUB)
	$(ARM7TDMI_PREFIX)objdump --syms $@ | grep " _start"


.PHONY: e7t

e7t: e7t.elf 

e7t.uue: e7t.bin ./arm-bsl-cksum
	./arm-bsl-cksum $<
	uuencode $< $< | tr '`' ' ' > $@

e7t.bin: e7t.elf
	$(ARM7TDMI_PREFIX)objcopy -O binary $< $@

e7t.elf: e7t.c
	$(ARM7TDMI_PREFIX)gcc -g  -o $@ $<


.PHONY: e7t-irq

e7t-irq: e7t-irq.elf 

e7t-irq.uue: e7t-irq.elf ./arm-bsl-cksum
#	$(ARM7TDMI_PREFIX)objcopy -O binary --only-section=.text $< e7t-irq.bin
	$(ARM7TDMI_PREFIX)objcopy -O binary  $< e7t-irq.bin
#	./arm-bsl-cksum e7t-irq.bin
	uuencode e7t-irq.bin e7t-irq.bin | tr '`' ' ' > $@
	rm e7t-irq.bin

e7t-irq.bin: e7t-irq.elf
	$(ARM7TDMI_PREFIX)objcopy -O binary $< $@

e7t-irq.elf: e7t-irq.c
	$(ARM7TDMI_PREFIX)gcc -g -DCRT0 -nostartfiles -Wl,--script="arm7tdmi-e7t-stub.x" -o $@ $<
#	$(ARM7TDMI_PREFIX)gcc -g -DCRT0 -nostartfiles -o $@ $<


.PHONY: mips-csb250

mips-csb250: mips-csb250.elf

mips-csb250.elf: $(MIPS_CSB250_STUB)
	$(MIPS_PREFIX)gcc -DCRT0 -DCSB250 \
		$(MIPS_CFLAGS) -o $@ \
		-nostartfiles -nodefaultlibs \
		$(MIPS_CSB250_STUB)
	$(MIPS_PREFIX)objdump --syms $@ | grep " _start"




clean:
	rm -f *.mot *.elf *.bin *.uue
