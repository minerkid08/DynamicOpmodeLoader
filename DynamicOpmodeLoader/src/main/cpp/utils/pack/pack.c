#include "buffer.h"
#include "dynList.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <sys/types.h>

#pragma pack(1)
typedef struct
{
	unsigned int signature;
	unsigned int version;
	unsigned int treeSize;
} FileHeader;

#pragma pack(1)
typedef struct
{
	unsigned int crc;
	unsigned short preloadBytes;
	unsigned short fileIndex;
	unsigned int entryOffset;
	unsigned int entryLength;
	unsigned short terminator;
} FileDirectoryEntry;

typedef struct
{
	char* name;
	char** list;
} DirEntry;

typedef struct
{
	char* name;
	DirEntry* list;
} ExtEntry;

ExtEntry* files;

void scan(const char* directory)
{
	DIR* dir = opendir(directory);
	struct dirent* entry = 0;

	while (entry = readdir(dir))
	{
		if (entry->d_name[0] == '.')
			continue;

		char* fileName = (char*)malloc(strlen(directory) + strlen(entry->d_name) + 2);
		strcpy(fileName, directory);
		strcat(fileName, "/");
		strcat(fileName, entry->d_name);

		struct stat st;
		stat(fileName, &st);

		if (st.st_mode & 0100000)
		{
			printf("%s\n", fileName);
			const char* ext = entry->d_name;
			for (int i = 0; i < strlen(entry->d_name); i++)
			{
				if (entry->d_name[i] == '.')
				{
					ext = entry->d_name + i + 1;
				}
			}

			int extLen = dynList_size(files);
			char foundExt = 0;
			for (int i = 0; i < extLen; i++)
			{
				ExtEntry* extEntry = files + i;
				if (strcmp(extEntry->name, ext) == 0)
				{
					foundExt = 1;

					int dirLen = dynList_size(extEntry->list);
					char foundDir = 0;
					for (int i = 0; i < dirLen; i++)
					{
						DirEntry* dirEntry = extEntry->list + i;
						if (strcmp(dirEntry->name, directory) == 0)
						{
							foundDir = 1;

							int fileLen = dynList_size(dirEntry->list);
							dynList_resize((void**)&dirEntry->list, fileLen + 1);

							int buflen = strlen(entry->d_name) - strlen(ext);
							char* filename = malloc(buflen);
							memcpy(filename, entry->d_name, buflen - 1);
							filename[buflen - 1] = 0;
							dirEntry->list[fileLen] = filename;

							break;
						}
					}
					if (foundDir == 0)
					{
						dynList_resize((void**)&extEntry->list, dirLen + 1);
						DirEntry* dirEntry = &extEntry->list[dirLen];
						dirEntry->name = malloc(strlen(directory) + 1);
						strcpy(dirEntry->name, directory);
						dirEntry->list = dynList_new(1, sizeof(char*));

						int buflen = strlen(entry->d_name) - strlen(ext);
						char* filename = malloc(buflen);
						memcpy(filename, entry->d_name, buflen - 1);
						filename[buflen - 1] = 0;
						dirEntry->list[0] = filename;
					}

					break;
				}
			}
			if (foundExt == 0)
			{
				dynList_resize((void**)&files, extLen + 1);
				ExtEntry* extEntry = &files[extLen];
				extEntry->name = malloc(strlen(ext) + 1);
				strcpy(extEntry->name, ext);
				extEntry->list = dynList_new(1, sizeof(ExtEntry));

				DirEntry* dirEntry = &extEntry->list[0];
				dirEntry->name = malloc(strlen(directory) + 1);
				strcpy(dirEntry->name, directory);
				dirEntry->list = dynList_new(1, sizeof(char*));

				int buflen = strlen(entry->d_name) - strlen(ext);
				char* filename = malloc(buflen);
				memcpy(filename, entry->d_name, buflen - 1);
				filename[buflen - 1] = 0;
				dirEntry->list[0] = filename;
			}
		}
		else
		{
			scan(fileName);
		}
		free(fileName);
	}
	closedir(dir);
}

int main(int argc, const char** argv)
{
	if (argc < 3)
	{
		printf("need 3 arg\n");
		return 1;
	}
	files = dynList_new(0, sizeof(ExtEntry));
	scan(argv[1]);

	Buffer buffer;
	Buffer dataBuffer;

	bufferInit(&buffer);
	bufferInit(&dataBuffer);

	char zero = 0;

	int extLen = dynList_size(files);
	for (int i = 0; i < extLen; i++)
	{
		ExtEntry* extEntry = &files[i];
		printf("%s\n", extEntry->name);
		bufferWrite(&buffer, "%s", extEntry->name);
		bufferWriteData(&buffer, &zero, 1);

		int dirLen = dynList_size(extEntry->list);
		for (int i = 0; i < dirLen; i++)
		{
			DirEntry* dirEntry = &extEntry->list[i];
			printf("  %s\n", dirEntry->name);
			bufferWrite(&buffer, "%s", dirEntry->name);
			bufferWriteData(&buffer, &zero, 1);

			int fileLen = dynList_size(dirEntry->list);
			for (int i = 0; i < fileLen; i++)
			{
				printf("    %s\n", dirEntry->list[i]);
				bufferWrite(&buffer, "%s", dirEntry->list[i]);
				bufferWriteData(&buffer, &zero, 1);

				char* filename =
					malloc(strlen(dirEntry->name) + 1 + strlen(dirEntry->list[i]) + 1 + strlen(extEntry->name) + 1);

				strcpy(filename, dirEntry->name);
				strcat(filename, "/");
				strcat(filename, dirEntry->list[i]);
				strcat(filename, ".");
				strcat(filename, extEntry->name);

				FILE* file = fopen(filename, "rb");

				fseek(file, 0, SEEK_END);
				int len = ftell(file);
				fseek(file, 0, SEEK_SET);

				void* data = malloc(len);
				fread(data, 1, len, file);

				FileDirectoryEntry entry;
				entry.crc = 0;
				entry.entryLength = len;
				entry.entryOffset = dataBuffer.len;
				entry.fileIndex = 0x7fff;
				entry.terminator = 0xffff;
				entry.preloadBytes = 0;

				bufferWriteData(&dataBuffer, data, len);
				bufferWriteData(&buffer, &entry, sizeof(FileDirectoryEntry));
				free(data);
				free(filename);
				fclose(file);
			}
			bufferWriteData(&buffer, &zero, 1);
		}
		bufferWriteData(&buffer, &zero, 1);
	}
	bufferWriteData(&buffer, &zero, 1);

	FileHeader header;
	header.signature = 0x55aa1234;
	header.version = 1;
	header.treeSize = buffer.len;
	FILE* file = fopen(argv[2], "wb");
	fwrite(&header, 1, sizeof(FileHeader), file);
	fwrite(buffer.data, 1, buffer.len, file);
	fwrite(dataBuffer.data, 1, dataBuffer.len, file);
	fclose(file);
}
