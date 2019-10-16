#include <stdio.h>	// for printf();
#include <stdlib.h>	// fot atoi()
#include <fcntl.h>	// for open()
#include <unistd.h>	// for read(), close()


// options:
//	-h for help
//	-c XX for column count of print out
//  -l print little endian, instead of big endian

void printHex(unsigned char* buffer, unsigned int size, unsigned char columnCount, unsigned char endianness);
char makePrintable(char byte);

int main (int argc, char** argv)
{

	if (argc < 2) return printf("No files given\n") - 1;

	unsigned char colCount = 3;
	unsigned char endianness = 0; // 0 little endian

	char* filepath = NULL;
	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			// parse options
			switch (argv[i][1])
			{
			case 'h':
				// print help
				return 0;
				break;
			case 'c':
				if (i+1 < argc)
				{
					colCount = atoi(argv[++i]);
				}
				break;
			case 'l':
				endianness = 1;
				break;
			default:
				break;
			}
		}
		else
		{
			filepath = argv[i];
		}
	}

	if (filepath == NULL) return printf("No files given\n") - 2;

	int fd = open(filepath, O_RDONLY);
	if (fd < 0) return printf("Could not open file\n") - 3;

	unsigned int size = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);

	printf("Size: %i | Column Count: %i | ", size, colCount);
	if (endianness)
		printf("Little Endian\n");
	else
		printf("Big Endian\n");

	if (size > 99999) return printf("file too large!\n") - 4;

	unsigned char buffer[2048] = {0}; //Bug: buffer smaller than possible filesize TODO: fix this bug
	read(fd, buffer, size);
	close(fd);

	
	printHex(buffer, size, colCount, endianness);

	return 0;
}

void printHex(unsigned char* buffer, unsigned int size, unsigned char columnCount, unsigned char endianness)
{



	const unsigned int bytesInWord = 4;
	const unsigned int bytesInLine = columnCount * bytesInWord;
	const unsigned int wordsInLine = bytesInLine / bytesInWord;
	const unsigned int lines = size / bytesInLine;

	const unsigned int restWords = (size - lines * bytesInLine) / bytesInWord;
	const unsigned int restBytes = size - lines * bytesInLine - restWords*bytesInWord;

	for (int line = 0; line < lines; line++)
	{
		printf("\n%i\t0x%.8x:  ", line*bytesInLine, line*bytesInLine);
		for (int word = 0; word < wordsInLine; word++)
		{
			printf(" ");
			for (int k = 0; k < bytesInWord; k++)
			{
				unsigned int index = line*bytesInLine + word*bytesInWord + 3 - k;
				printf("%.2x ", buffer[index]);
			}
		}

		printf("  ");
		for (int word = 0; word < wordsInLine; word++)
		{
			for (int k = 0; k < bytesInWord; k++)
			{
				unsigned int index = line*bytesInLine + word*bytesInWord + 3 - k;
				printf("%c", makePrintable(buffer[index]));
			}
		}		
	}

	//printf("restBytes: %i\n", restBytes);

	if (restWords | restBytes)
	{
		printf("\n%i\t0x%.8x:  ", lines*bytesInLine, lines*bytesInLine);
		int word;
		for (word = 0; word < restWords; word++)
		{
			printf(" ");
			for (int k = 0; k < bytesInWord; k++)
			{
				unsigned int index = lines*bytesInLine + word*bytesInWord + 3 - k;
				printf("%.2x ", buffer[index]);
			}
		}
		printf(" ");
		for (int k = 0; k < restBytes; k++)
		{
			unsigned int index = lines*bytesInLine + word*bytesInWord + 3 - k;
			printf("%.2x ", buffer[index]);
		}
	}
	printf("\n");
}

char makePrintable(char byte)
{
	if (byte < 32 || byte > 126)
	{
		return 46;
	}
	return byte;
}