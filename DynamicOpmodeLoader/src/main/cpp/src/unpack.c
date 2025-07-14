#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "utils.h"

#include <jni.h>
#include <sys/stat.h>
#include <sys/types.h>

#define unpack Java_com_minerkid08_dynamicopmodeloader_FileServer_unzip

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

void writeFile(const FileDirectoryEntry* entry, const char* path, const char* name, const char* ext);

FILE* file;
int offset = 0;

const char* prefix;

JNIEXPORT void JNICALL unpack(JNIEnv* env2, jobject this, jobject stdlib, jstring path)
{
  env = env2;
	initUtils(stdlib);
	prefix = (*env)->GetStringUTFChars(env, path, 0);
	char* filePath = malloc(strlen(prefix) + 10);
	strcpy(filePath, prefix);
	strcat(filePath, "/data.pak");
	file = fopen(filePath, "rb");
	free(filePath);
	if (file == 0)
	{
		print("cant open file");
		(*env)->ReleaseStringUTFChars(env, path, prefix);
		return;
	}

	FileHeader header;
	fread(&header, sizeof(FileHeader), 1, file);
	offset = header.treeSize + sizeof(FileHeader);

	char* directoriesBase = malloc(header.treeSize);
	fread(directoriesBase, 1, header.treeSize, file);

	char* directories = directoriesBase;

	while (1)
	{
		int len = strlen(directories);
		const char* ext = directories;
		directories += len + 1;
		while (1)
		{
			int len = strlen(directories);
			const char* path = directories;
			directories += len + 1;
			while (1)
			{
				int len = strlen(directories);
				const char* name = directories;
				directories += len + 1;
				FileDirectoryEntry entry;

				memcpy(&entry, directories, sizeof(FileDirectoryEntry));
				writeFile(&entry, path, name, ext);

				directories += sizeof(FileDirectoryEntry);
				if (*directories == 0)
				{
					directories++;
					break;
				}
			}
			if (*directories == 0)
			{
				directories++;
				break;
			}
		}
		if (*directories == 0)
		{
			directories++;
			break;
		}
	}
	free(directoriesBase);
	fclose(file);
	(*env)->ReleaseStringUTFChars(env, path, prefix);
}

void writeFile(const FileDirectoryEntry* entry, const char* path, const char* name, const char* ext)
{
	int len = strlen(path);
	int off = strlen(prefix) + 1;
	char* str = malloc(len + 1 + off);
	struct stat st;
	strcpy(str, prefix);
	strcat(str, "/");
	for (int i = 0; i < len; i++)
	{
		if (path[i] == '/')
		{
			str[i + off] = 0;
			if (stat(str, &st) == -1)
				mkdir(str, 0777);
		}
		str[i + off] = path[i];
	}
	str[len + off] = 0;
	if (stat(str, &st) == -1)
		mkdir(str, 0777);
	free(str);

	// prefix + / + path + / + name + . + ext + \0
	int filenameLen = strlen(prefix) + 1 + strlen(path) + 1 + strlen(name) + 1 + strlen(ext) + 1;
	char* filename = malloc(filenameLen);
	strcpy(filename, prefix);
	strcat(filename, "/");
	strcat(filename, path);
	strcat(filename, "/");
	strcat(filename, name);
	strcat(filename, ".");
	strcat(filename, ext);

	print("writing file '%s'", filename);
	fseek(file, entry->entryOffset + offset, SEEK_SET);
	void* contents = malloc(entry->entryLength);
	fread(contents, entry->entryLength, 1, file);

	FILE* outfile = fopen(filename, "wb");
	fwrite(contents, entry->entryLength, 1, outfile);

	fclose(outfile);
	free(filename);
	free(contents);
}
