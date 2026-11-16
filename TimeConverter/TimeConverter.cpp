// TimeConverter.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <windows.h>
#include <sstream>
#include <iomanip>
#include "Converter.h"

int main()
{   
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Converter converter;

    while (true)
    {
        std::cout << "Input Date (YYYY-MM-DD HH:MM:SS) or 'stop':\n";

        std::string input;
        if (!std::getline(std::cin, input))
            break;

        if (input == "stop")
            break;

        std::tm t = {};
        std::stringstream iss(input);
        iss >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");

        if (iss.fail())
        {
            std::cout << "ERROR: Incorrect format. Expected YYYY-MM-DD HH:MM:SS.\n";
            continue;
        }

        int year = t.tm_year + 1900;
        int month = t.tm_mon + 1;
        int day = t.tm_mday;
        int hour = t.tm_hour;
        int minute = t.tm_min;
        int second = t.tm_sec;

        if (!converter.ifValidDateTime(year, month, day, hour, minute, second))
        {
            std::cout << "ERROR: Current date/time is not exists.\n";
            continue;
        }

		std::cout << converter.currentDateTime(year, month, day, hour, minute, second) << "\n";
		std::cout << converter.formatRelative(t) << "\n";
    }

    std::cout << "Bye!\n";
    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
