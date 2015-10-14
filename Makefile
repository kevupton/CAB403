LIBS=Client/lib
INCLUDE_PATH=libs/

cli_exec: libraries
	gcc Client/main.c $(wildcard libraries/*) -o cli_exec -lpthread

libraries: 
	mkdir libraries/
	for dir in $(LIBS); do \
		cd $$dir; \
		gcc -c *.c; \
		mv *.o ../../libraries; \
		cd -; \
	done

clean:
	rm -rf libraries/	

