CFLAGS := -Wall -Wextra -Wpedantic -ggdb
INC_DIR := include/

pesquisa-tcc: main.o
	$(CC) $^ -o $@

main.o: src/main.c
	$(CC) $(CFLAGS) -I $(INC_DIR) -c $<

clean:
	rm *.o pesquisa-tcc
