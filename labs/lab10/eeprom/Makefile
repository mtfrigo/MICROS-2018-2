TARGETS=eeprom
FLAGS=-Wall -MMD -g
INCLUDE=-I. -I../include -I ../../lib/include
LIBDIR=-L../../lib
LIBS=-lgalileo2

CC=$(CROSS_COMPILE)gcc
CFLAGS=$(FLAGS) $(INCLUDE)
LDFLAGS=$(LIBDIR) $(LIBS)

all: $(TARGETS)

$(TARGETS) : % : %.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

-include $(SRCS:.c=.d)

clean:
	rm -f *~ *.bak *.o *.d

distclean: clean
	rm -f $(TARGETS)