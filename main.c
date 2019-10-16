#include <stdio.h>
#include <fcntl.h>	// for open(), ioctl();
#include <unistd.h>	// for close(), read(), write()


int main ()
{
	int fd = open("main.bin", O_RDONLY);
	if (fd < 0)
	{
		printf("Could not open file\n");
		return -1;
	}
	unsigned int size = lseek(fd, 0, SEEK_END);

	if (size > 99999)
	{
		printf("file too large!\n");
		return -2;
	}

	printf("Size: %i\n", size);
	lseek(fd, 0, SEEK_SET);

	unsigned char buffer[2048] = {0};

	read(fd, buffer, size);

	close(fd);



	const unsigned int bytesInLine = 20;
	const unsigned int wordsInLine = bytesInLine / 4;
	const unsigned int lines = size / bytesInLine;

	const unsigned int rest = size - lines * bytesInLine;
	const unsigned int wordsInRest = rest / 4;

	

	for (int line = 0; line < lines; line++)
	{

		printf("\n%i\t%.8x:  ", line*bytesInLine, line*bytesInLine);
		for (int word = 0; word < wordsInLine; word++)
		{
			printf(" ");
			for (int k = 0; k < 4; k++)
			{
				unsigned int index = line*bytesInLine + word*4 + 3 - k;
				printf("%.2x ", buffer[index]);
			}
		}
	}	

	printf("\n%i\t%.8x:  ", lines*bytesInLine, lines*bytesInLine);
	for (int word = 0; word < wordsInRest; word++)
	{
		printf(" ");
		for (int k = 0; k < 4; k++)
		{
			unsigned int index = lines*bytesInLine + word*4 + 3 - k;
			printf("%.2x ", buffer[index]);
		}
	}
	printf("\n");

	return 0;
}