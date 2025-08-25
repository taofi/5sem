#define MAX_STUDENTS 100

struct Student {
    char Name[64];
    char Surname[128];
    unsigned char Course;
    unsigned char Group;
    char ID[8];
};


HANDLE hFileMapping, hFile;
struct Student* pStudents;
int studentCount = MAX_STUDENTS;
struct Student students[MAX_STUDENTS];