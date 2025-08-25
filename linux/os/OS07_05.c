#include <stdio.h>
#include <time.h>

int main()
{
    // Получение текущего времени
    time_t t = time(NULL);
    struct tm* localTime = localtime(&t);

    if (localTime == NULL)
    {
        perror("Ошибка получения локального времени");
        return 1;
    }

    // Вывод текущей даты и времени в формате дд.мм.гггг чч:мин:сек
    printf("%02d.%02d.%04d %02d:%02d:%02d\n",
        localTime->tm_mday,
        localTime->tm_mon + 1,  // Месяцы начинаются с 0
        localTime->tm_year + 1900, // Год с 1900
        localTime->tm_hour,
        localTime->tm_min,
        localTime->tm_sec);

    return 0;
}
