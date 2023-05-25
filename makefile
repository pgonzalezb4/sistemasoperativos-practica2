CC=gcc
CFLAGS=-g -Wall

preprocess_dir = ./src/preprocess/
source_dir = ./src/
build_dir = ./bin/


run_server: server 
	clear
	$(build_dir)server

run_client: client 
	clear
	$(build_dir)client

data: preprocess_data 
	$(preprocess_dir)transform
	$(preprocess_dir)index
	rm $(preprocess_dir)transform $(preprocess_dir)index

server: $(source_dir)p2-server.c
	$(CC) $(CFLAGS) -o $(build_dir)server $(source_dir)p2-server.c

client: $(source_dir)p2-client.c
	$(CC) $(CFLAGS) -o $(build_dir)client $(source_dir)p2-client.c 

preprocess_data: transform index

index: $(preprocess_dir)index_data.c
	$(CC) $(CFLAGS) -o $(preprocess_dir)index $(preprocess_dir)index_data.c

transform: $(preprocess_dir)csv2bin.c
	$(CC) $(CFLAGS) -o $(preprocess_dir)transform $(preprocess_dir)csv2bin.c

clean:
	rm ./data/processed/rides.bin ./data/processed/source_id_table.bin
	rm $(build_dir)client $(build_dir)server 
