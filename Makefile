CFLAGS := -Wall -Wextra -Wpedantic -ggdb
INC_DIR := include/

pesquisa-tcc: main.o patricia.o
	$(CC) $^ -o $@

main.o: src/main.c
	$(CC) $(CFLAGS) -I $(INC_DIR) -c $<

patricia.o: src/patricia.c include/patricia.h
	$(CC) $(CFLAGS) -I $(INC_DIR) -c $<

leitura.o: src/leitura.c include/leitura.h
	$(cc) $(CFLAGS) -I $(INC_DIR) -c $<

clean:
	rm *.o pesquisa-tcc
