all:
	gcc ccas.c -o ccas.exe -lm

debug:
	gcc ccas.c -o ccas.exe -lm -g