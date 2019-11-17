COMPILER = gcc
FLAG = -std=c99

disassembler: 
	$(COMPILER) $(FLAG) disassembler.c -o disassembler.out 

clean:
	rm *.out