#pragma once
#include "file_controller.h"

#define BUFFER_SIZE 2048

HANDLE file_handle = NULL;

char *read_buffer = NULL;
char *write_buffer = NULL;

int read_bytes = 0;
int write_bytes = 0;

struct Error open_file(char *filePath) 
{
	close_file();
	file_handle = CreateFileA(
		filePath, 
		GENERIC_READ | GENERIC_WRITE,
		0, 
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (file_handle == INVALID_HANDLE_VALUE) {
		file_handle = NULL;
		read_buffer = NULL;
		write_buffer = NULL;
		read_bytes = 0;
		return ENABLED_TO_OPEN_FILE;
	}

	read_buffer = calloc(BUFFER_SIZE, sizeof(char));
	if (read_buffer == NULL) return MEMORY_ALLOCATION;

	write_buffer = calloc(BUFFER_SIZE, sizeof(char));
	if (write_buffer == NULL) return MEMORY_ALLOCATION;

	return SUCCSESS;
}

struct Error add_row(char* row, int pos) 
{
	if (row == NULL)
		return ADDED_ROW_IS_NULL;

	if (file_handle == NULL)
		return FILE_HANDLE_NOT_OPEN;

	int row_count = get_row_count();

	if (pos < -1)
		return INVALID_ADD_POSITION;

	if (pos > row_count + 1)
		return INVALID_ADD_POSITION;

	struct Error result;

	if (pos == 0) 
		result = add_begin(row);
	else if (pos == -1) 
		result = add_end(row);
	else 
		result = add_instead(row, pos);
	
	return result;
}

struct Error add_begin(char* row)
{
	SetFilePointer(file_handle, 0, NULL, FILE_BEGIN);

	if (!ReadFile(file_handle, read_buffer, BUFFER_SIZE, &read_bytes, NULL)) {
		return READ_FILE_ERROR;
	}

	strcpy(write_buffer, row);
	strcat(write_buffer, "\n");
	strcat(write_buffer, read_buffer);

	SetFilePointer(file_handle, 0, NULL, FILE_BEGIN);

	if (!WriteFile(file_handle, write_buffer, strlen(write_buffer) + 1, &write_bytes, NULL)) {
		return WRITE_FILE_ERROR;
	}

	return SUCCSESS;
}

struct Error add_end(char* row) {
	SetFilePointer(file_handle, 0, NULL, FILE_BEGIN);

	if (!ReadFile(file_handle, read_buffer, BUFFER_SIZE, &read_bytes, NULL)) {
		return READ_FILE_ERROR;
	}

	strcpy(write_buffer, read_buffer);
	strcat(write_buffer, row);
	strcat(write_buffer, "\n");

	SetFilePointer(file_handle, 0, NULL, FILE_BEGIN);

	if (!WriteFile(file_handle, write_buffer, strlen(write_buffer) + 1, &write_bytes, NULL)) {
		return WRITE_FILE_ERROR;
	}

	return SUCCSESS;
}

struct Error add_instead(char* row, int pos) {
	int row_index = find_nth_row(pos - 1);

	if (row_index == -1) {
		return INVALID_ADD_POSITION;
	}

	memset(write_buffer, 0, BUFFER_SIZE);

	memcpy(write_buffer, read_buffer, row_index * sizeof(char));

	if(row_index != 0)
		strcat(write_buffer, "\n");
	strcat(write_buffer, row);
	if(row_index == 0)
	strcat(write_buffer, "\n");
	strcat(write_buffer, read_buffer + row_index * sizeof(char));

	SetFilePointer(file_handle, 0, NULL, FILE_BEGIN);

	if (!WriteFile(file_handle, write_buffer, strlen(write_buffer) + 1, &write_bytes, NULL)) {
		return WRITE_FILE_ERROR;
	}

	return SUCCSESS;
}

struct Error print_rows()
{
	SetFilePointer(file_handle, 0, NULL, FILE_BEGIN);

	if (!ReadFile(file_handle, read_buffer, BUFFER_SIZE, &read_bytes, NULL)) {
		return READ_FILE_ERROR;
	}

	char* token;

	token = strtok(read_buffer, "\n");
	while (token != NULL) {
		printf("%s\n", token);
		token = strtok(NULL, "\n");
	}

	return SUCCSESS;
}

struct Error close_file() 
{
	CloseHandle(file_handle);

	file_handle = NULL;

	//free(read_buffer);
	//free(write_buffer);

	read_buffer = NULL;
	write_buffer = NULL;

	read_bytes = 0;

	return SUCCSESS;
}

struct Error print_row(int pos)
{
	if (file_handle == NULL)
		return FILE_HANDLE_NOT_OPEN;

	struct Error result = SUCCSESS;

	if (pos == 0 || pos == 1)
		result = get_first_row();
	else if (pos == -1)
		result = get_last_row();
	else
		result = get_row(pos);

	return result;
}

struct Error get_row(int pos)
{
	if (pos == get_row_count())
		return get_last_row();

	char print_buffer[256] = { 0 };

	int start_index = find_nth_row(pos);
	int end_index = find_nth_row(pos + 1);

	if (start_index == -1 || end_index == -1)
		return ROW_NOT_FOUND;

	int length = end_index - start_index;

	memcpy(print_buffer, read_buffer + start_index + 1, (length - 1) * sizeof(char));

	printf("%d row: %s\n", pos, print_buffer);

	return SUCCSESS;
}

struct Error get_first_row()
{
	char print_buffer[256] = { 0 };

	int index = find_nth_row(1);

	if (index == -1)
		return ROW_NOT_FOUND;

	memcpy(print_buffer, read_buffer, index);

	printf("first row: %s\n", print_buffer);

	return SUCCSESS;
}

struct Error get_last_row()
{
	char print_buffer[256] = { 0 };

	int index = find_nth_row(get_row_count() - 1);

	if (index == -1)
		return ROW_NOT_FOUND;

	strcpy(print_buffer, read_buffer + index * sizeof(char) + 1);
	print_buffer[strlen(print_buffer) - 1] = '\0';

	printf("last row: %s\n", print_buffer);

	return SUCCSESS;
}

int get_row_count() {
	SetFilePointer(file_handle, 0, NULL, FILE_BEGIN);

	if (!ReadFile(file_handle, read_buffer, BUFFER_SIZE, &read_bytes, NULL)) {
		return -1;
	}

	int count = 0;

	while (*read_buffer != '\0') {
		if (*read_buffer == '\n') {
			count++;
		}
		read_buffer++;
	}

	return count;
}

int find_nth_row(int n) {
	
	SetFilePointer(file_handle, 0, NULL, FILE_BEGIN);

	if (!ReadFile(file_handle, read_buffer, BUFFER_SIZE, &read_bytes, NULL)) {
		return -1;
	}
	if (n == 0)
		return 0;
	int count = 0;
	int index = -1;

	for (int i = 0; read_buffer[i] != '\0'; i++) {
		if (read_buffer[i] == '\n') {
			count++;
			if (count == n) {
				index = i;
				break;
			}
		}
	}

	return index;
}

struct Error rem_row(int pos) {
	if (file_handle == NULL)
		return FILE_HANDLE_NOT_OPEN;

	int row_count = get_row_count();

	if (pos < -1)
		return INVALID_REMOVE_POSITION;

	if (pos > row_count + 1)
		return INVALID_REMOVE_POSITION;

	struct Error result;

	if (pos == 0)
		result = rem_begin();
	else if (pos == -1)
		result = rem_end();
	else
		result = rem_current(pos);

	return result;
}

struct Error rem_begin() {
	int second_row_index = find_nth_row(1);

	if (second_row_index == -1)
		return ROW_NOT_FOUND;

	memset(write_buffer, 0, BUFFER_SIZE);

	strcpy(write_buffer, read_buffer + (1 + second_row_index) * sizeof(char));

	SetFilePointer(file_handle, 0, NULL, FILE_BEGIN);

	if (!WriteFile(file_handle, write_buffer, strlen(write_buffer) + 1, &write_bytes, NULL)) {
		return WRITE_FILE_ERROR;
	}

	return SUCCSESS;
}

struct Error rem_end() {
	int pred_last_row_index = find_nth_row(get_row_count() - 1);

	if (pred_last_row_index == -1)
		return ROW_NOT_FOUND;

	memset(write_buffer, 0, BUFFER_SIZE);

	memcpy(write_buffer, read_buffer, (pred_last_row_index + 1) * sizeof(char));

	SetFilePointer(file_handle, 0, NULL, FILE_BEGIN);

	if (!WriteFile(file_handle, write_buffer, strlen(write_buffer) + 1, &write_bytes, NULL)) {
		return WRITE_FILE_ERROR;
	}

	return SUCCSESS;
}
struct Error rem_current(int pos) {
	int before_deleted_row_index = find_nth_row(pos - 1);
	int after_deleted_row_index = find_nth_row(pos);

	if (before_deleted_row_index == -1 || after_deleted_row_index == -1)
		return ROW_NOT_FOUND;

	memset(write_buffer, 0, BUFFER_SIZE);

	memcpy(write_buffer, read_buffer, before_deleted_row_index);
	strcat(write_buffer, read_buffer + after_deleted_row_index);

	SetFilePointer(file_handle, 0, NULL, FILE_BEGIN);

	if (!WriteFile(file_handle, write_buffer, strlen(write_buffer) + 1, &write_bytes, NULL)) {
		return WRITE_FILE_ERROR;
	}

	return SUCCSESS;
}