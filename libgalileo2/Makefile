TARGET=libgalileo2.a

SRCDIR=src
SRCS=$(SRCDIR)/galileo2io.c $(SRCDIR)/i2c.c $(SRCDIR)/eeprom.c $(SRCDIR)/lcd.c $(SRCDIR)/util.c $(SRCDIR)/adc.c $(SRCDIR)/pwm.c

FLAGS=-O2 -Wall -MMD
INCLUDE=-I. -I./include
LIBDIR=
LIBS=

CC=$(CROSS_COMPILE)gcc
CCAR=$(CROSS_COMPILE)ar
CFLAGS=$(FLAGS) $(INCLUDE)
LDFLAGS=$(LIBDIR) $(LIBS)
CCARFLAGS=-crvs

all: $(TARGET)

$(TARGET): $(SRCS:.c=.o)
	$(CCAR) $(CCARFLAGS) $@ $^ 

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

-include $(SRCS:.c=.d)

clean:
	rm -f $(SRCDIR)/*~ $(SRCDIR)/*.bak $(SRCDIR)/*.o $(SRCDIR)/*.d

distclean: clean
	rm -f $(TARGET)