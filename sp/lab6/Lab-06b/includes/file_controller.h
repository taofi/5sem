#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Windows.h"
#include "error.h"
#include "stdio.h"

struct Error open_file(char *filePath);
struct Error add_row(char *row, int pos);
struct Error rem_row(int pos);
struct Error print_row(int pos);
struct Error print_rows();
struct Error close_file();

struct Error get_first_row();
struct Error get_last_row();
struct Error get_row(int pos);

struct Error add_begin(char* row);
struct Error add_end(char* row);
struct Error add_instead(char* row, int pos);

struct Error rem_begin();
struct Error rem_end();
struct Error rem_current(int pos);

int get_row_count();
int find_nth_row(int n);
