#include "menu.h"


void wait_key() 
{
	_getch();
}

void print(char* message) 
{
	printf("%s", message);
}

void print_menu()
{
	printf("Choose operation:\n");
	printf("1. open file\n");
	printf("2. add row\n");
	printf("3. remove row\n");
	printf("4. print row\n");
	printf("5. print all\n");
	printf("6. close file\n");
	printf("0. exit\n");
}

void print_error(struct Error error) 
{
	printf("======================   ERROR   ======================\n");
	printf("code: %d\n", error.code);
	printf("message: %s\n", error.message);
	printf("=======================================================\n");
}

int get_menu_command() 
{
	int command;
	scanf("%d", &command);

	getchar();

	return command;
}

void get_str(char out_str[]) 
{
	scanf("%[^\n]s", out_str);
}

int get_pos() 
{
	int pos;
	scanf("%d", &pos);

	getchar();

	return pos;
}