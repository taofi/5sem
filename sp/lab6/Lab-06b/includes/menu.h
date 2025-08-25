#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "error.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <conio.h>

void wait_key();
void print(char *message);

void print_menu();
void print_error(struct Error error);

int get_menu_command();
int get_pos();
void get_str(char out_str[]);