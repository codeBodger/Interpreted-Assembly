TARGET := interpreter
TAGS := -L ~/lib -I ~/include -lsense

$(TARGET): $(TARGET).c
	gcc -o $(TARGET).out $(TARGET).c $(TAGS)

g: $(TARGET).c
	gcc -g -o $(TARGET).g $(TARGET).c $(TAGS)
