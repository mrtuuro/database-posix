TARGET = bin/dbview
SRC =  $(wildcard src/*.c)
OBJ =  $(patsubst src/%.c, obj/%.o, $(SRC))

run: clean default
	./$(TARGET) -f ./mynewdb.db -n 
	./$(TARGET) -f ./mynewdb.db -a "Tunc Ozay, Istanbul, 12"
	./$(TARGET) -f ./mynewdb.db -a "Ayca Akman, Tekirdag, 20"
	./$(TARGET) -f ./mynewdb.db -a "Ceto, Istanbul, 55"
	./$(TARGET) -f ./mynewdb.db -a "Ecem Karan, Kadıkoy, 31"
	./$(TARGET) -f ./mynewdb.db -a "Test Deneem, Bahcelievler, 221"


default: $(TARGET)

clean:
	@rm -f obj/*.o
	@rm -f bin/*
	@rm -f *.db

$(TARGET): $(OBJ)
	@clang -o $@ $?

obj/%.o : src/%.c
	@clang -c $< -o $@ -Iinclude
