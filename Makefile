LFLAGS := -lm # `pkg-config gtk+-3.0 --libs`
CFLAGS := -Wall -Wextra -Wpedantic -ggdb # `pkg-config gtk+-3.0 --cflags`
INC_DIR := include/

pesquisa-tcc: main.o patricia.o leitura.o
	$(CC) $^ $(LFLAGS) -o $@

main.o: src/main.c include/patricia.h
	$(CC) $(CFLAGS) -I $(INC_DIR) -c $<

patricia.o: src/patricia.c include/patricia.h
	$(CC) $(CFLAGS) -I $(INC_DIR) -c $<

leitura.o: src/leitura.c include/leitura.h include/patricia.h
	$(CC) $(CFLAGS) -I $(INC_DIR) -c $<

clean:
	rm *.o pesquisa-tcc
