a.out:	swlist.c
	$(CC) $(CFLAGS) $<

.DUMMY:	clean
clean:
	rm a.out
