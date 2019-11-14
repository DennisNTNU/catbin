#include <stdio.h>	// for printf();
#include <stdlib.h>	// fot atoi()
#include <fcntl.h>	// for open()
#include <unistd.h>	// for read(), close()


void printHex(unsigned char* buffer, unsigned int size, unsigned char columnCount, unsigned char endianness);
char makePrintable(char byte);
unsigned char endiannessTerm(unsigned char endianness, int k, unsigned int bytesInWord);

int main (int argc, char** argv)
{
	if (argc < 2) return printf("No files given\n") - 1;

	unsigned char colCount = 3;
	unsigned char endianness = 0; // 0 := big endian

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
				printf("options:\n\t-h for help\n\t-c XX for column count of print out\n\t-l print little endian, instead of big endian\n");
				return 0;
				break;
			case 'c':
				if (i+1 < argc)
				{
					colCount = atoi(argv[++i]);
					//printf("\n\nDebug: %i\n\n", colCount);
					if (colCount == 0)
					{
						return printf("Invalid Column Count\n") - 2;
					}
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

	if (filepath == NULL) return printf("No files given\n") - 3;

	int fd = open(filepath, O_RDONLY);
	if (fd < 0) return printf("Could not open file\n") - 4;

	unsigned int size = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);

	printf("Size: %iB | Column Count: %i | ", size, colCount);
	if (endianness)
		printf("Little Endian\n");
	else
		printf("Big Endian\n");

	if (size > 99999) return printf("file too large!\n") - 5;

	unsigned char buffer[99999] = {0};
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
	const unsigned int restBytes = size - lines * bytesInLine - restWords * bytesInWord;

	for (int line = 0; line < lines; line++)
	{
		printf("\n%i\t0x%.6x:  ", line*bytesInLine, line*bytesInLine);
		for (int word = 0; word < wordsInLine; word++)
		{
			printf(" ");
			for (int k = 0; k < bytesInWord; k++)
			{
				unsigned int index = line*bytesInLine + word*bytesInWord + endiannessTerm(endianness, k, bytesInWord);
				printf("%.2x ", buffer[index]);
			}
		}

		printf("  ");
		for (int word = 0; word < wordsInLine; word++)
		{
			for (int k = 0; k < bytesInWord; k++)
			{
				unsigned int index = line*bytesInLine + word*bytesInWord + endiannessTerm(endianness, k, bytesInWord);
				printf("%c", makePrintable(buffer[index]));
			}
		}
	}

	//printf("restBytes: %i\n", restBytes);

	if (restWords | restBytes)
	{
		printf("\n%i\t0x%.6x:  ", lines*bytesInLine, lines*bytesInLine);
		int word;
		for (word = 0; word < restWords; word++)
		{
			printf(" ");
			for (int k = 0; k < bytesInWord; k++)
			{
				unsigned int index = lines*bytesInLine + word*bytesInWord + endiannessTerm(endianness, k, bytesInWord);
				printf("%.2x ", buffer[index]);
			}
		}

		printf(" ");
		for (int k = 0; k < restBytes; k++)
		{
			unsigned int index = lines*bytesInLine + word*bytesInWord + endiannessTerm(endianness, k, bytesInWord);
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

unsigned char endiannessTerm(unsigned char endianness, int k, unsigned int bytesInWord)
{
	if (endianness) // little endian
		return (bytesInWord - 1 - k)*endianness;
	else // big endian
		return k;
}
