C_SOURCES := $(wildcard *.c)

ASM_SOURCES := $(wildcard *.s)

PROGRAM = shooter.prg

SPRITES := $(wildcard *.spr)

DISKIMAGE = shooter.d64

CC = cl65

CCOPTS = -t c64 -O -Oi -Or -Os

DEBUGOPTS = -l shooter.asm -T -g -Wl -Ln -Wl shooter.lbl

EMULATOR_APP = /usr/bin/x64

DISKIMAGE_APP = /usr/bin/c1541


.PHONY: all
all:
	$(CC) $(CCOPTS) $(C_SOURCES) $(ASM_SOURCES) -o $(PROGRAM)

.PHONY: assembler
assembler:
	$(CC) $(CCOPTS) $(DEBUGOPTS) $(C_SOURCES)

image:
	$(DISKIMAGE_APP) -format shooter,1 d64 $(DISKIMAGE)
	$(DISKIMAGE_APP) $(DISKIMAGE) -write $(PROGRAM) $(PROGRAM)
	for sprite in $(SPRITES) ; do \
		$(DISKIMAGE_APP) $(DISKIMAGE) -write $$sprite $$sprite ; done

clean:
	rm *.o

emulator:
	$(EMULATOR_APP) -autostart "$(DISKIMAGE):shooter.prg"
