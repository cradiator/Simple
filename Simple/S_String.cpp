#include "Common.h"
#include "S_String.h"
#include "DBG.h"
#include "MM.h"

#define DEFAULT_BUFFER_SIZE (0x100)

struct StringBuffer
{
	int  total_size;
	int  current_size;
	char buf[1];	// char buf[size];
};

void S_OpenString(struct S_Interpreter* interpreter)
{
	DCHECK((interpreter->Flag & INTERPRETER_FLAG_OPENSTRING) == 0);
	DCHECK(interpreter->PendingString == 0);

	// mark flag and create a StrinBuffer for interpreter.
	interpreter->Flag |= INTERPRETER_FLAG_OPENSTRING;

	int init_size = sizeof(struct StringBuffer) - sizeof(char) + DEFAULT_BUFFER_SIZE;
	struct StringBuffer* sb = (struct StringBuffer*)malloc(init_size);
	DCHECK(sb != 0);

	memset(sb, 0, init_size);
	sb->total_size   = DEFAULT_BUFFER_SIZE;
	sb->current_size = 0;
	interpreter->PendingString = sb;
}

void S_AddStringChar(struct S_Interpreter* interpreter, char c)
{
	DCHECK((interpreter->Flag & INTERPRETER_FLAG_OPENSTRING) != 0);
	DCHECK(interpreter->PendingString != 0);

	struct StringBuffer* sb = (struct StringBuffer*)interpreter->PendingString;

	// if remain buffer is not enough, allocate an new one and replace the old one.
	if (sb->current_size >= sb->total_size)
	{
		int new_size = sb->total_size * 2 + sizeof(struct StringBuffer) - sizeof(char);
		struct StringBuffer* new_sb = (struct StringBuffer*)malloc(new_size);
		DCHECK(new_sb != 0);

		memset(new_sb, 0, new_size);
		new_sb->total_size   = sb->total_size * 2;
		new_sb->current_size = sb->current_size;
		memcpy(new_sb->buf, sb->buf, sb->current_size);

		free(sb);
		sb = new_sb;
		interpreter->PendingString = sb;
	}

	// buffer the char
	sb->buf[sb->current_size] = c;
	sb->current_size++;
}

char* S_CloseString(struct S_Interpreter* interpreter)
{
	DCHECK(interpreter != 0);
	DCHECK((interpreter->Flag & INTERPRETER_FLAG_OPENSTRING) != 0);
	DCHECK(interpreter->PendingString != 0);

	// Copy string into ParsingStorage.
	struct StringBuffer* sb = (struct StringBuffer*)interpreter->PendingString;
	int str_size = sb->current_size + 1;

	char* string = (char*)MM_AllocateStorage(interpreter->ParsingStorage, str_size);
	DCHECK(string != 0);
	memcpy(string, sb->buf, sb->current_size);
	string[sb->current_size] = '\0';

	// Free StringBuffer
	free(sb);
	interpreter->PendingString = 0;
	interpreter->Flag &= ~INTERPRETER_FLAG_OPENSTRING;

	return string;
}

char* S_AllocateStringInParsingStorage(struct S_Interpreter* interpreter, const char* string)
{
    DCHECK(interpreter != 0);
    if (string == 0 || string[0] == '\0')
    {
        return "";
    }

    int str_len = strlen(string);
    char* s = (char*)MM_AllocateStorage(interpreter->ParsingStorage, str_len + 1);
    memcpy(s, string, str_len);
    s[str_len] = '\0';

    return s;
}

