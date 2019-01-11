# use normal make for this Makefile
#
# Makefile for building user programs to run on top of Nachos
#
# Several things to be aware of:
#
#    Nachos assumes that the location of the program startup routine (the
# 	location the kernel jumps to when the program initially starts up)
#       is at location 0.  This means: start.o must be the first .o passed 
# 	to ld, in order for the routine "Start" to be loaded at location 0
#

# if you are cross-compiling, you need to point to the right executables
# and change the flags to ld and the build procedure for as

GCCDIR = ../../../gnu-decstation-ultrix/decstation-ultrix/2.95.3/

LDFLAGS = -T script -N
ASFLAGS = -mips2
CPPFLAGS = $(INCDIR)

# if you aren't cross-compiling:
# GCCDIR =
# LDFLAGS = -N -T 0
# ASFLAGS =
# CPPFLAGS = -P $(INCDIR)

PATH = $(GCCDIR):/lib:/usr/bin:/bin

CC = $(GCCDIR)gcc -B../../../gnu-decstation-ultrix/
AS = $(GCCDIR)as
LD = $(GCCDIR)ld

CPP = gcc -E
INCDIR =-I../userprog -I../threads
CFLAGS = -G 0 -c $(INCDIR)

all: halt shell matmult sort echo createfile cat copy ping pong scheduler sema_test philosopher1 philosopher2 philosopher3 philosopher4 philosopher5 processA processB processAB make_chassis make_tire assemble vehicle_assembly

start.o: start.s ../userprog/syscall.h
	$(CPP) $(CPPFLAGS) start.c > strt.s
	$(AS) $(ASFLAGS) -o start.o strt.s
	rm strt.s

halt.o: halt.c
	$(CC) $(CFLAGS) -c halt.c
halt: halt.o start.o
	$(LD) $(LDFLAGS) start.o halt.o -o halt.coff
	../bin/coff2noff halt.coff halt

shell.o: shell.c
	$(CC) $(CFLAGS) -c shell.c
shell: shell.o start.o
	$(LD) $(LDFLAGS) start.o shell.o -o shell.coff
	../bin/coff2noff shell.coff shell

sort.o: sort.c
	$(CC) $(CFLAGS) -c sort.c
sort: sort.o start.o
	$(LD) $(LDFLAGS) start.o sort.o -o sort.coff
	../bin/coff2noff sort.coff sort

matmult.o: matmult.c
	$(CC) $(CFLAGS) -c matmult.c
matmult: matmult.o start.o
	$(LD) $(LDFLAGS) start.o matmult.o -o matmult.coff
	../bin/coff2noff matmult.coff matmult

createfile.o: createfile.c
	$(CC) $(CFLAGS) -c createfile.c
createfile: createfile.o start.o
	$(LD) $(LDFLAGS) start.o createfile.o -o createfile.coff
	../bin/coff2noff createfile.coff createfile

echo.o: echo.c
	$(CC) $(CFLAGS) -c echo.c
echo: echo.o start.o
	$(LD) $(LDFLAGS) start.o echo.o -o echo.coff
	../bin/coff2noff echo.coff echo

cat.o: cat.c
	$(CC) $(CFLAGS) -c cat.c
cat: cat.o start.o
	$(LD) $(LDFLAGS) start.o cat.o -o cat.coff
	../bin/coff2noff cat.coff cat

copy.o: copy.c
	$(CC) $(CFLAGS) -c copy.c
copy: copy.o start.o
	$(LD) $(LDFLAGS) start.o copy.o -o copy.coff
	../bin/coff2noff copy.coff copy

ping.o: ping.c
	$(CC) $(CFLAGS) -c ping.c
ping: ping.o start.o
	$(LD) $(LDFLAGS) start.o ping.o -o ping.coff
	../bin/coff2noff ping.coff ping

pong.o: pong.c
	$(CC) $(CFLAGS) -c pong.c
pong: pong.o start.o
	$(LD) $(LDFLAGS) start.o pong.o -o pong.coff
	../bin/coff2noff pong.coff pong

scheduler.o: scheduler.c
	$(CC) $(CFLAGS) -c scheduler.c
scheduler: scheduler.o start.o
	$(LD) $(LDFLAGS) start.o scheduler.o -o scheduler.coff
	../bin/coff2noff scheduler.coff scheduler

sema_test.o: sema_test.c
	$(CC) $(CFLAGS) -c sema_test.c
sema_test: sema_test.o start.o
	$(LD) $(LDFLAGS) start.o sema_test.o -o sema_test.coff
	../bin/coff2noff sema_test.coff sema_test

philosopher1.o: philosopher1.c
	$(CC) $(CFLAGS) -c philosopher1.c
philosopher1: philosopher1.o start.o
	$(LD) $(LDFLAGS) start.o philosopher1.o -o philosopher1.coff
	../bin/coff2noff philosopher1.coff philosopher1

philosopher2.o: philosopher2.c
	$(CC) $(CFLAGS) -c philosopher2.c
philosopher2: philosopher2.o start.o
	$(LD) $(LDFLAGS) start.o philosopher2.o -o philosopher2.coff
	../bin/coff2noff philosopher2.coff philosopher2

philosopher3.o: philosopher3.c
	$(CC) $(CFLAGS) -c philosopher3.c
philosopher3: philosopher3.o start.o
	$(LD) $(LDFLAGS) start.o philosopher3.o -o philosopher3.coff
	../bin/coff2noff philosopher3.coff philosopher3

philosopher4.o: philosopher4.c
	$(CC) $(CFLAGS) -c philosopher4.c
philosopher4: philosopher4.o start.o
	$(LD) $(LDFLAGS) start.o philosopher4.o -o philosopher4.coff
	../bin/coff2noff philosopher4.coff philosopher4

philosopher5.o: philosopher5.c
	$(CC) $(CFLAGS) -c philosopher5.c
philosopher5: philosopher5.o start.o
	$(LD) $(LDFLAGS) start.o philosopher5.o -o philosopher5.coff
	../bin/coff2noff philosopher5.coff philosopher5

processA.o: processA.c
	$(CC) $(CFLAGS) -c processA.c
processA: processA.o start.o
	$(LD) $(LDFLAGS) start.o processA.o -o processA.coff
	../bin/coff2noff processA.coff processA

processB.o: processB.c
	$(CC) $(CFLAGS) -c processB.c
processB: processB.o start.o
	$(LD) $(LDFLAGS) start.o processB.o -o processB.coff
	../bin/coff2noff processB.coff processB

processAB.o: processAB.c
	$(CC) $(CFLAGS) -c processAB.c
processAB: processAB.o start.o
	$(LD) $(LDFLAGS) start.o processAB.o -o processAB.coff
	../bin/coff2noff processAB.coff processAB

make_chassis.o: make_chassis.c
	$(CC) $(CFLAGS) -c make_chassis.c
make_chassis: make_chassis.o start.o
	$(LD) $(LDFLAGS) start.o make_chassis.o -o make_chassis.coff
	../bin/coff2noff make_chassis.coff make_chassis

make_tire.o: make_tire.c
	$(CC) $(CFLAGS) -c make_tire.c
make_tire: make_tire.o start.o
	$(LD) $(LDFLAGS) start.o make_tire.o -o make_tire.coff
	../bin/coff2noff make_tire.coff make_tire

assemble.o: assemble.c
	$(CC) $(CFLAGS) -c assemble.c
assemble: assemble.o start.o
	$(LD) $(LDFLAGS) start.o assemble.o -o assemble.coff
	../bin/coff2noff assemble.coff assemble

vehicle_assembly.o: vehicle_assembly.c
	$(CC) $(CFLAGS) -c vehicle_assembly.c
vehicle_assembly: vehicle_assembly.o start.o
	$(LD) $(LDFLAGS) start.o vehicle_assembly.o -o vehicle_assembly.coff
	../bin/coff2noff vehicle_assembly.coff vehicle_assembly


