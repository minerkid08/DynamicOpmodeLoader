#include "defFile.h"
#include "dynList.h"
#include "global.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	const char* name;
	const char* signature;
	char builder;
} function_t;

typedef struct
{
	const char* name;
	function_t* functions;
} class_t;

#define TYPE_INT 0
#define TYPE_DOUBLE 1
#define TYPE_BOOL 2
#define TYPE_STRING 3
#define TYPE_TABLE 4
#define TYPE_OBJECT 5
#define TYPE_FUNCTION 6

typedef struct tableElement_t tableElement_t;

struct tableElement_t
{
	const char* name;
	char type;
	union {
		int i;
		double d;
		char b;
		const char* s;
		tableElement_t* t;
	};
};

tableElement_t** stack[30];

tableElement_t* table;
tableElement_t** curTable;
static int tableLevel = 0;

class_t* classes;

const char* copyStr(const char* c)
{
	int len = strlen(c);
	char* str = malloc(len + 1);
	strcpy(str, c);
	return str;
}

void initDefFile()
{
	classes = dynList_new(0, sizeof(class_t));
	table = dynList_new(0, sizeof(tableElement_t));
	curTable = &table;
}

void freeOpmodes()
{
	int len = dynList_size(classes);
	for (int i = 0; i < len; i++)
	{
		class_t* class = classes + i;
		free((void*)class->name);
		int len2 = dynList_size(class->functions);
		for (int i = 0; i < len2; i++)
		{
			function_t* fun = class->functions + i;
			free((void*)fun->name);
			free((void*)fun->signature);
		}
	}
}

void freeTable(tableElement_t* table)
{
	int len = dynList_size(table);
	for (int i = 0; i < len; i++)
	{
		tableElement_t* t = table + i;
		free((void*)t->name);
		switch (t->type)
		{
		case TYPE_INT:
		case TYPE_DOUBLE:
		case TYPE_BOOL:
			break;
		case TYPE_TABLE:
			freeTable(t->t);
			dynList_free(t->t);
			break;
		case TYPE_STRING:
		case TYPE_OBJECT:
		case TYPE_FUNCTION:
			free((void*)t->s);
			break;
		}
	}
}

int genAnnotations(const char* sig, FILE* file, const char* className)
{
	int arg = 0;
	const char* annotation = "param";
	for (int i = 1; i < strlen(sig); i++)
	{
		char c = sig[i];
		if (c == ')')
		{
			annotation = "return";
			continue;
		}
		if (c == 'V' && className == 0)
			break;
		if (strcmp(annotation, "param") == 0)
			fprintf(file, "---@param a%d ", arg++);
		else
		{
			fprintf(file, "---@return ");
			if (className)
			{
				fprintf(file, "%s\n", className);
				break;
			}
		}
		if (c == 'Z')
			fprintf(file, "boolean\n");
		if (c == 'L')
		{
			if (strStartsWith(sig + i + 1, "java/lang/String"))
			{
				while (sig[++i] != ';')
					;
				fprintf(file, "string\n");
			}
			else if (strStartsWith(sig + i + 1, "com/minerkid08/dynamicopmodeloader/LuaCallback"))
			{
				while (sig[++i] != ';')
					;
				fprintf(file, "function\n");
			}
			else
			{
				int ind = 0;
				while (sig[++i] != ';')
				{
					if (sig[i] == '/')
						ind = i;
				}
				char* s = malloc(i - ind);
				strncpy(s, sig + ind + 1, i - ind - 1);
				s[i - ind - 1] = 0;
				fprintf(file, "%s\n", s);
				free(s);
			}
		}
		if (c == 'D')
			fprintf(file, "number\n");
		if (c == 'I')
			fprintf(file, "number\n");
		if (c == 'F')
			fprintf(file, "number\n");
	}

	return arg;
}

void printTable(tableElement_t* table, FILE* file, int level)
{
	char* indent = malloc(level * 2 + 1);
	for (int i = 0; i < 2 * level; i++)
		indent[i] = ' ';
	indent[2 * level] = 0;
	int len = dynList_size(table);
	char c[2];
	c[0] = ',';
	c[1] = 0;
	if (level == 0)
		c[0] = 0;
	for (int i = 0; i < len; i++)
	{
		if (i == len - 1)
			c[0] = 0;
		tableElement_t* t = table + i;
		switch (t->type)
		{
		case TYPE_INT:
			fprintf(file, "%s%s = %d%s\n", indent, t->name, t->i, c);
			break;
		case TYPE_DOUBLE:
			fprintf(file, "%s%s = %f%s\n", indent, t->name, t->d, c);
			break;
		case TYPE_BOOL:
			if (t->b)
				fprintf(file, "%s%s = true%s\n", indent, t->name, c);
			else
				fprintf(file, "%s%s = false%s\n", indent, t->name, c);
			break;
		case TYPE_STRING:
			fprintf(file, "%s%s = \"%s\"%s\n", indent, t->name, t->s, c);
			break;
		case TYPE_OBJECT:
			fprintf(file, "---@type %s\n", t->s);
			fprintf(file, "%s%s = {}%s\n", indent, t->name, c);
			break;
		case TYPE_FUNCTION: {
			int argc = genAnnotations(t->s, file, 0);
			fprintf(file, "%s = function(", t->name);
			for (int i = 0; i < argc; i++)
			{
				if (i == argc - 1)
					fprintf(file, "a%d", i);
				else
					fprintf(file, "a%d, ", i);
			}
			fprintf(file, ") end%s\n", c);
			break;
		}
		case TYPE_TABLE:
			fprintf(file, "%s%s = {\n", indent, t->name);
			printTable(t->t, file, level + 1);
			fprintf(file, "%s}\n", indent);
			break;
		}
	}
	free(indent);
}

void closeDefFile()
{
	assert(*curTable == table);

#ifdef ANDROID
	FILE* file = fopen("/sdcard/def.lua", "wb");
#else
	FILE* file = fopen("def.lua", "wb");
#endif

	int len = dynList_size(classes);
	for (int i = 0; i < len; i++)
	{
		class_t* class = classes + i;
		fprintf(file, "---@class %s\n%s = {}\n", class->name, class->name);
		int len2 = dynList_size(class->functions);
		for (int j = 0; j < len2; j++)
		{
			function_t* fun = class->functions + j;
			int argc = genAnnotations(fun->signature, file, fun->builder ? class->name : 0);
			fprintf(file, "function %s:%s(", class->name, fun->name);
			for (int i = 0; i < argc; i++)
			{
				if (i == argc - 1)
					fprintf(file, "a%d", i);
				else
					fprintf(file, "a%d, ", i);
			}
			fprintf(file, ") end\n");
		}
		fprintf(file, "\n");
	}

	printTable(table, file, 0);

	fprintf(file, "---@class Dir\n");
	fprintf(file, "---@field name string\n");
	fprintf(file, "---@field file boolean\n\n");

	fprintf(file, "---@param path string\n");
	fprintf(file, "---@return Dir[]\n");
	fprintf(file, "function io.list(path) end\n\n");

	fprintf(file, "---@enum OpmodeType\n");
	fprintf(file, "OpmodeType = {Telop = 0, Auto = 1};\n\n");

	fprintf(file, "---@class Opmode\n");
	fprintf(file, "---@field name string\n");
	fprintf(file, "---@field type OpmodeType\n");
	fprintf(file, "---@field group string\n");
	fprintf(file, "---@field order string\n\n");

	fprintf(file, "---@param opmode Opmode\n");
	fprintf(file, "function addOpmode(opmode) end");

	fclose(file);

	freeOpmodes();
	dynList_free(classes);
	freeTable(table);
	dynList_free(table);
}

void addClass(const char* name)
{
	if (mode != MODE_GENFILE)
		return;
	char c = 0;
	int len = dynList_size(classes);
	for (int i = 0; i < len; i++)
	{
		class_t* class = classes + i;
		if (strcmp(name, class->name) == 0)
		{
			c = 1;
			break;
		}
	}
	if (c == 1)
		return;

	dynList_resize((void**)&classes, len + 1);

	class_t* class = classes + len;

	class->name = copyStr(name);
	class->functions = dynList_new(0, sizeof(function_t));
}

void addClassFun(const char* className, const char* funName, const char* funSig, char builder)
{
	if (mode != MODE_GENFILE)
		return;
	addClass(className);
	class_t* class = 0;
	int len = dynList_size(classes);
	for (int i = 0; i < len; i++)
	{
		class_t* c = classes + i;
		if (strcmp(className, c->name) == 0)
		{
			class = c;
			break;
		}
	}

	len = dynList_size(class->functions);
	dynList_resize((void**)&class->functions, len + 1);

	function_t* fun = class->functions + len;

	fun->name = copyStr(funName);
	fun->signature = copyStr(funSig);
	fun->builder = builder;
}

void addTable(const char* name)
{
	if (mode != MODE_GENFILE)
		return;
	int len = dynList_size(*curTable);
	dynList_resize((void**)curTable, len + 1);
	tableElement_t* element = (*curTable) + len;
	element->name = copyStr(name);
	element->type = TYPE_TABLE;

	element->t = dynList_new(0, sizeof(tableElement_t));
	stack[tableLevel] = curTable;
	curTable = &element->t;

	tableLevel++;
}

void removeTable()
{
	if (mode != MODE_GENFILE)
		return;
	tableLevel--;
	curTable = stack[tableLevel];
}

void addi(const char* name, int value)
{
	if (mode != MODE_GENFILE)
		return;
	int len = dynList_size(*curTable);
	dynList_resize((void**)curTable, len + 1);
	tableElement_t* element = (*curTable) + len;
	element->name = copyStr(name);
	element->type = TYPE_INT;
	element->i = value;
}
void addd(const char* name, double value)
{
	if (mode != MODE_GENFILE)
		return;
	int len = dynList_size(*curTable);
	dynList_resize((void**)curTable, len + 1);
	tableElement_t* element = (*curTable) + len;
	element->name = copyStr(name);
	element->type = TYPE_DOUBLE;
	element->d = value;
}
void addb(const char* name, char value)
{
	if (mode != MODE_GENFILE)
		return;
	int len = dynList_size(*curTable);
	dynList_resize((void**)curTable, len + 1);
	tableElement_t* element = (*curTable) + len;
	element->name = copyStr(name);
	element->type = TYPE_BOOL;
	element->b = value;
}
void adds(const char* name, const char* value)
{
	if (mode != MODE_GENFILE)
		return;
	int len = dynList_size(*curTable);
	dynList_resize((void**)curTable, len + 1);
	tableElement_t* element = (*curTable) + len;
	element->name = copyStr(name);
	element->type = TYPE_STRING;
	element->s = copyStr(value);
}
void addo(const char* name, const char* objName)
{
	if (mode != MODE_GENFILE)
		return;
	int len = dynList_size(*curTable);
	dynList_resize((void**)curTable, len + 1);
	tableElement_t* element = (*curTable) + len;
	element->name = copyStr(name);
	element->type = TYPE_OBJECT;
	element->s = copyStr(objName);
}
void addf(const char* name, const char* signature)
{
	if (mode != MODE_GENFILE)
		return;
	int len = dynList_size(*curTable);
	dynList_resize((void**)curTable, len + 1);
	tableElement_t* element = (*curTable) + len;
	element->name = copyStr(name);
	element->type = TYPE_FUNCTION;
	element->s = copyStr(signature);
}
