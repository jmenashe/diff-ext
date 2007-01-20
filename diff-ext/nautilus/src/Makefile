PACKAGE = diff-ext
PKG_CONFIG = pkg-config
CFLAGS = $(shell $(PKG_CONFIG) --cflags pango atk cairo glib libnautilus-extension)  -Wall -g \
	-DIMAGEDIR='"/usr/local/share/pixmaps/$(PACKAGE)"' -DLOCALEDIR='"/usr/local/share/locale"' \
	-DGETTEXT_PACKAGE='"$(PACKAGE)"'

LDLIBS = $(shell $(PKG_CONFIG) --libs libnautilus-extension)
LDFLAGS = -shared
        
all: libdiff-ext.so

.c.o: 
	$(CC) $(CFLAGS) -c -o $@ $<
        
libdiff-ext.so: main.o
	$(CC) $(LDFLAGS) -o $@ $< $(LDLIBS)
