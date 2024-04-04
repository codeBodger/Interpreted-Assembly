TARGET := interpreter

$(TARGET): $(TARGET).c
	gcc -o $(TARGET).out $(TARGET).c -L ~/lib -I ~/include -lsense
