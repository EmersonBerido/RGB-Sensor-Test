LASTNAME=JUAN
FIRSTNAME=JOSHUA
HW = 5

CC = gcc
CFLAGS = -Wall -g
LIBS = -lpigpio -lrt -pthread -lm

ROOTNAME = $(LASTNAME)_$(FIRSTNAME)_HW$(HW)_main.c
ADDOBJ = RGB.c Hexcode.c

EXE = assignment5

all: $(EXE)

$(EXE): $(ROOTNAME)
	$(CC) $(CFLAGS) $(ROOTNAME) $(ADDOBJ) -o $(EXE) $(LIBS)

clean:
	rm -f $(EXE)

run: $(EXE)
	sudo ./$(EXE)

