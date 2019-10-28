CFLAGS	:=$(CFLAGS) -Wall -Wextra -std=c99 -pedantic -ggdb
LDFLAGS	:=$(LDFLAGS) #-lm -pthread

PROGRAM	=main

SRCDIR	=source
INCDIR	=include
OUTDIR	=out

SRCS	:=$(wildcard $(SRCDIR)/*.c)
OBJS	:=$(SRCS:$(SRCDIR)/%.c=$(OUTDIR)/%.o)

CFLAGS	:=$(CFLAGS) -I$(INCDIR) -I$(OUTDIR)

all: link

link: $(OUTDIR) gengetopt $(OBJS)
	$(CC) $(LDFLAGS) $(OUTDIR)/arguments.o $(OBJS) -o $(OUTDIR)/$(PROGRAM)

$(OUTDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $(@:$(OUTDIR)/%.o=$(SRCDIR)/%.c) -o $@

gengetopt:
	gengetopt --file-name arguments --output-dir $(OUTDIR) --unamed-opts -i $(SRCDIR)/arguments.ggo
	$(CC) $(CFLAGS) -c $(OUTDIR)/arguments.c -o $(OUTDIR)/arguments.o

$(OUTDIR):
	mkdir -p $(OUTDIR)

clean:
	rm -rf $(OUTDIR)

.PHONY: all link clean
