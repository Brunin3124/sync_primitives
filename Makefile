CC = gcc
CFLAGS = -Wall -pthread
OBJ = sync_mutex.o sync_sem.o sync_condvar.o sync_banker.o

# Alvo principal: compila o teste do banqueiro
banqueiro: $(OBJ) teste_banker.c
	$(CC) $(CFLAGS) teste_banker.c $(OBJ) -o teste_banker

# Alvo para os testes antigos (opcional)
performance: $(OBJ) perfomance_test.c
	$(CC) $(CFLAGS) perfomance_test.c $(OBJ) -o perfomance_test

# Regra genérica para transformar .c em .o
%.o: %.c sync_lib.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o teste_banker perfomance_test