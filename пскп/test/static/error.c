#include "error.h"

struct Error SUCCSESS = { SUCCSESS_CODE , "SUCCSESS" };
struct Error INVALID_COMMAND = { INVALID_COMMAND_CODE, "invalid command" };
struct Error FILE_HANDLE_NOT_OPEN = { FILE_HANDLE_NOT_OPEN_CODE, "file handle is not opened" };
struct Error ENABLED_TO_OPEN_FILE = { ENABLED_TO_OPEN_FILE_CODE , "enabled to open file" };
struct Error INVALID_ADD_POSITION = { INVALID_ADD_POSITION_CODE , "add row is incorrect" };
struct Error ADDED_ROW_IS_NULL = { ADDED_ROW_IS_NULL_CODE , "added row is null" };
struct Error INVALID_REMOVE_POSITION = { INVALID_REMOVE_POSITION_CODE , "remove row is incorrect" };
struct Error ROW_NOT_FOUND = { ROW_NOT_FOUND_CODE, "row not found" };
struct Error MEMORY_ALLOCATION = { MEMORY_ALLOCATION_CODE, "memory allocation error"};
struct Error READ_FILE_ERROR = { READ_FILE_ERROR_CODE, "error reading file"};
struct Error WRITE_FILE_ERROR = { WRITE_FILE_ERROR_CODE, "error writing file"};