#define _CRT_SECURE_NO_WARNINGS
#include "locale.h"
#include "Windows.h"
#include "stdio.h"
#include "error.h"
#include "menu.h"
#include "file_controller.h"

struct Error execute_command(int command);

int main() 
{
	setlocale(LC_ALL, "");

	while (TRUE) {
		print_menu();

		int command = get_menu_command();

		struct Error result = execute_command(command);

		if (result.code != SUCCSESS_CODE) {
					system("cls");;
			print_error(result);
		}

		wait_key();

		system("cls");
	}
}

struct Error execute_command(int command)
{
			system("cls");;

	switch (command) {
	case 1: 
	{
		char pathname[128];/* = "D:\\C++Codes\\Lab-06b\\file.txt";*/

		print("enter path:\n");
		
		get_str(pathname);

		struct Error result = open_file(pathname);

		if (result.code == SUCCSESS_CODE) 
			print("\nfile openned...");

		return result;
	}
	case 2: 
	{
		char row[128];

		print("enter row:\n");

		fgets(row, sizeof(row), stdin);
		row[strcspn(row, "\n")] = '\0';

		print("\nenter pos:\n");

		int pos = get_pos();

		struct Error result = add_row(row, pos);

		if (result.code == SUCCSESS_CODE)
			print("\nrow added...");

		return result;
	}
	case 3:
		print("enter pos:\n");

		int pos = get_pos();

		struct Error result = rem_row(pos);

		if (result.code == SUCCSESS_CODE)
			print("\nrow removed...");

		return result;
	case 4:
	{
		print("enter pos:\n");

		int pos = get_pos();

		struct Error result = print_row(pos);

		return result;
	}
	case 5:
	{
		struct Error result = print_rows();

		return result;
	}
	case 6: 
	{
		struct Error result = close_file();

		if (result.code == SUCCSESS_CODE)
			print("file closed...");

		return result;
	}
	case 0:
		ExitProcess(0);
		break;
	default:
		return INVALID_COMMAND;
	}
}
