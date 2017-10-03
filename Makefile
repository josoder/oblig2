CC=gcc
CFLAGS=-I.
DEPS = md5.h
OBJ = md5.o md5_file.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

md5: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	$(RM) *.obj *.o *.sln *.pdb *.ilk *.exe $(TARGET)$(EXT)
