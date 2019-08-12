a.out:	main.c m-swlist.h
	$(CC) $(CFLAGS) $(filter %.c,$^)

.DUMMY:	clean
clean:
	rm a.out
