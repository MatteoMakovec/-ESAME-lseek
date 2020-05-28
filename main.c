#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>


off_t show_current_file_offset(int fd);


int main(int argc, char * argv[]) {
	char * file_name = "output.txt";
	int res;
	off_t file_offset;


	// apro un file già esistente in modalità lettura/scrittura
	int fd = open(file_name, O_RDWR); 
	if (fd == -1) { 
		perror("open()");
		exit(EXIT_FAILURE);
	}

	// all'apertura del file, il file offset vale 0
	// non occorre impostare il file offset
	show_current_file_offset(fd);

	// spostiamo il file offset alla posizione 256 (dall'inizio del file)
	file_offset = lseek(fd, 256, SEEK_SET); // SEEK_SET: posizione rispetto all'inizio del file
	if (file_offset == -1) {
		perror("lseek()");
		exit(EXIT_FAILURE);
	}

	// leggiamo 16 bytes
	char buf[16];
	res = read(fd, buf, sizeof(buf) / sizeof(char));
	if (res == -1) {
		perror("read()");
		exit(EXIT_FAILURE);
	}

	buf[15] = 0;
	printf("read: %s\n", buf);

	// poi spostiamo il file offset all'inizio del file e scriviamo qualcosa
	file_offset = lseek(fd, 0, SEEK_SET);
	if (file_offset == -1) {
		perror("lseek()");
		exit(EXIT_FAILURE);
	}

	char * msg = "ciao!";
	res = write(fd, msg, strlen(msg));
	if (res == -1) {
		perror("write()");
		exit(EXIT_FAILURE);
	} 

	if (close(fd) == -1) {
		perror("close()");
	}

	return 0;
}


/*
 * Questa funzione prende come parametro un descrittore di file aperto
 * e restituisce il file offset di quel descrittore di file
 */
off_t show_current_file_offset(int fd) {
	off_t file_offset;
	file_offset = lseek(fd, 0, SEEK_CUR); // SEEK_CUR: posizione relativa rispetto alla posizione corrente
	if (file_offset == -1) {
		perror("lseek()");
		exit(EXIT_FAILURE);
	}

	printf("current file offset: %ld\n", file_offset);

	return EXIT_SUCCESS;
}