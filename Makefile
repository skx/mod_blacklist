
all: mod_blacklist.c Makefile
	apxs2 -c mod_blacklist.c

clean:
	rm -f *.la *.lo *.slo || true
	rm -rf .libs/ || true

indent:
	astyle --style=allman -A1 --indent=spaces=4   --break-blocks --pad-oper --pad-header --unpad-paren --max-code-length=200  mod_blacklist.c
