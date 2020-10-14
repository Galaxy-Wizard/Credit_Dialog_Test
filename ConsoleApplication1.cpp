// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <windows.h>
#include <iostream>
#include <fstream>



/*/====================/*/
//  Умные указатели    //
//  Начало             //
/*/====================/*/

namespace galaxy_group
{
    class smart_array_pointer
    {
    public:
        explicit smart_array_pointer(void* p) :stored_array_pointer(p)  { }
        smart_array_pointer(smart_array_pointer& p) = delete; //{ stored_array_pointer = p.stored_array_pointer; }
        smart_array_pointer& operator=(void* p) = delete;// { stored_array_pointer = p; }
        
        ~smart_array_pointer() { if (stored_array_pointer != nullptr) { delete[] stored_array_pointer; } }

        void* get() { return stored_array_pointer; }
    protected:
        void* stored_array_pointer;
    };

    class smart_pointer
    {
    public:
        explicit smart_pointer(void* p) :stored_pointer(p) { }
        smart_pointer(smart_pointer& p) = delete;// { stored_pointer = p.stored_pointer; }
        smart_pointer& operator=(void* p) = delete;// { stored_pointer = p; }

        ~smart_pointer() { if (stored_pointer != nullptr) { delete stored_pointer; } }

        void* get() { return stored_pointer; }
    protected:
        void* stored_pointer;
    };
}

/*/====================/*/
//  Умные указатели    //
//  Конец              //
/*/====================/*/

int main()
{
    const std::wstring code_fail_message = std::wstring(L"Задача провалена.");
    const std::wstring code_success_message = std::wstring(L"Задача выполнена успешно.");

    const std::wstring code_fail_memory_resevation_message = std::wstring(L"Обнаружена проблема резервирования памяти.");
    const std::wstring code_fail_input_file_size_message = std::wstring(L"Обнаружена проблема нулевого размера входного файла.");

    const std::wstring running_state_file_read_message = std::wstring(L"Файл прочитан в память.");

    const std::wstring report_syntax_error_message = std::wstring(L"Обнаружена синтаксическая ошибка комментариев в коде в исходном файле.");
    const std::wstring report_resulting_file_name_message = std::wstring(L"Название файла с результатом работы: ");


    const std::wstring enter_file_name_message = std::wstring(L"Введите название файла для обработки (файл должен находиться в каталоге программы)\n (принимаются символы из local(\"rus_rus.866\")\n (пробелы в названиях файлов не принимаются)):");
    const std::wstring file_is_selected_message = std::wstring(L"Выбран файл ");
    const std::wstring file_is_selected_mark_message = std::wstring(L"\"");

    std::wcout.imbue(std::locale("rus_rus.866"));
    std::wcin.imbue(std::locale("rus_rus.866"));


    std::wstring file_name;
    std::wstring file_path;

    DWORD buffer_directory_length = 10000;
    galaxy_group::smart_array_pointer buffer_directory(new wchar_t[buffer_directory_length]);

    if (buffer_directory.get() != nullptr)
    {
        ZeroMemory(buffer_directory.get(), buffer_directory_length*sizeof(wchar_t));

        int bytes = GetModuleFileNameW(NULL, (wchar_t*)buffer_directory.get(), buffer_directory_length);
        if (bytes != 0)
        {
            for (size_t counter = wcslen((wchar_t*)buffer_directory.get()); counter > 0 && ((wchar_t*)(buffer_directory.get()))[counter] != wchar_t(L'\\'); counter--)
            {
                ((wchar_t*)(buffer_directory.get()))[counter] = wchar_t(0);
            }

            for (size_t counter = 0; counter < buffer_directory_length && ((wchar_t*)(buffer_directory.get()))[counter] != wchar_t(0); counter++)
            {
                file_path += ((wchar_t*)(buffer_directory.get()))[counter];
            }
        }
    }
    else
    {
        std::wcout << std::endl << std::endl << code_fail_memory_resevation_message << std::endl;
        std::wcout << std::endl << std::endl << code_fail_message << std::endl;

        return 1;
    }

    std::wcout << (const wchar_t *)(enter_file_name_message.c_str()) << std::endl;

    std::wcin >> file_name;

    std::wcout << std::endl << (const wchar_t*)(file_is_selected_message.c_str()) << (const wchar_t*)(file_is_selected_mark_message.c_str())
        << (const wchar_t*)(file_path.c_str()) << (const wchar_t*)(file_name.c_str())
        << (const wchar_t*)(file_is_selected_mark_message.c_str()) << std::endl;

    std::wstring file_path_name = file_path + file_name;

    std::ifstream file_istream(file_path_name);

    std::string file_data;

    const DWORD file_data_buffer_size = 10000;
    galaxy_group::smart_pointer file_data_buffer(new char[file_data_buffer_size]);

    ZeroMemory((char*)(file_data_buffer.get()), file_data_buffer_size * sizeof(char));

    file_istream.read((char*)(file_data_buffer.get()), file_data_buffer_size);

    if (strlen((char*)(file_data_buffer.get())) == 0)
    {
        std::wcout << std::endl << std::endl << code_fail_input_file_size_message << std::endl;
        std::wcout << std::endl << std::endl << code_fail_message << std::endl;

        return 1;
    }

    file_data += std::string((char*)((file_data_buffer.get())));

    for (;;)
    {
        ZeroMemory((char*)(file_data_buffer.get()), file_data_buffer_size * sizeof(char));

        file_istream.read((char*)(file_data_buffer.get()), file_data_buffer_size);

        if (strlen((char*)(file_data_buffer.get())) == 0)
        {
            break;
        }

        file_data += std::string((char*)((file_data_buffer.get())));
    }

    std::wcout << std::endl << running_state_file_read_message << std::endl;

    const std::wstring file_extension_new = std::wstring(L".new.txt");

    std::wstring file_path_name_out = file_path_name + file_extension_new;

  
    std::string file_resulting_data;

    for (auto data_iterator = file_data.begin(); data_iterator != file_data.end(); data_iterator++)
    {
        auto data_iterator_plus_one = data_iterator + 1;

        if (data_iterator_plus_one == file_data.end())
        {
            break;
        }

        if (
            *data_iterator == char('/')
            &&
            *data_iterator_plus_one == char('/')
            )
        {
            auto data_commentary_start_iterator = data_iterator;
            auto data_commentary_end_iterator = data_iterator_plus_one;

            for (; data_iterator != file_data.end(); data_iterator++)
            {
                if (
                    *data_iterator == char('\n')
                    )
                {
                    data_commentary_end_iterator = data_iterator - 1;
                    break;
                }
            }

            if (data_iterator != file_data.end())
            {
                file_resulting_data += *data_iterator;
            }
        }
        else
        {
            if (
                *data_iterator == char('/')
                &&
                *data_iterator_plus_one == char('*')
                )
            {
                auto data_commentary_start_iterator = data_iterator;
                auto data_commentary_end_iterator = data_iterator_plus_one;

                data_iterator++;
                data_iterator++;

                for (; data_iterator != file_data.end(); data_iterator++)
                {
                    data_iterator_plus_one = data_iterator + 1;

                    if (data_iterator_plus_one == file_data.end())
                    {
                        break;
                    }

                    if (
                        *data_iterator == char('*')
                        &&
                        *data_iterator_plus_one == char('/')
                        )
                    {
                        data_commentary_end_iterator = data_iterator_plus_one;
                        data_iterator++;
                        data_iterator++;
                        break;
                    }
                }

                if (data_commentary_start_iterator == data_commentary_end_iterator + 1)
                {
                    std::wcout << std::endl << std::endl << report_syntax_error_message << std::endl;
                }
                else
                {
                    if (data_iterator != file_data.end())
                    {
                        file_resulting_data += *data_iterator;
                    }
                }
            }
            else
            {
                if (data_iterator != file_data.end())
                {
                    file_resulting_data += *data_iterator;
                }
            }
        }
    }

    std::ofstream file_ostream(file_path_name_out);

    file_ostream.write(file_resulting_data.c_str(), file_resulting_data.size());

    std::wcout << std::endl << report_resulting_file_name_message.c_str() << file_path_name_out << std::endl;

    std::wcout << std::endl << std::endl << code_success_message << std::endl;

    /*/
    std::cout << std::endl << "====================================================================================================" << std::endl;
    std::cout << file_data;
    std::cout << std::endl << "====================================================================================================" << std::endl;

    std::cout << std::endl << "====================================================================================================" << std::endl;
    std::cout << file_resulting_data;
    std::cout << std::endl << "====================================================================================================" << std::endl;

    std::wcout << std::endl << std::endl << code_success_message << std::endl;
    /*/


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
