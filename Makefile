a.out:	main.c swlist.h
	$(CC) $(CFLAGS) $(filter %.c,$^)

.DUMMY:	clean
clean:
	rm a.out
