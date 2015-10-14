LIBS_CLI=Client/lib
LIBS_SERV=Server/lib
INCLUDE_PATH=libs/
EXEC_FOLDER=bin/

all: clean serv_exec cli_exec

serv_exec: serv_libraries
	mkdir -p $(EXEC_FOLDER)
	gcc Server/main.c $(wildcard libs/*) -o $(EXEC_FOLDER)/server -lpthread
	make clean_serv

cli_exec: cli_libraries
	mkdir -p $(EXEC_FOLDER)
	gcc Client/main.c $(wildcard libc/*) -o $(EXEC_FOLDER)/client -lpthread
	make clean_cli

cli_libraries: 
	mkdir libc/
	for dir in $(LIBS_CLI); do \
		cd $$dir; \
		gcc -c *.c; \
		mv *.o ../../libc; \
		cd -; \
	done

serv_libraries: 
	mkdir libs/
	for dir in $(LIBS_SERV); do \
		cd $$dir; \
		gcc -c *.c; \
		mv *.o ../../libs; \
		cd -; \
	done

clean: clean_serv clean_cli

clean_serv:
	rm -rf libs/

clean_cli: 
	rm -rf libc/