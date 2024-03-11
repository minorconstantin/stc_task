#ifndef READ_RITE_FILES_H
#define READ_RITE_FILES_H
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

class read_rite_files
{
public:
    read_rite_files(); // конструктор
    ~read_rite_files(); // диструктор
    vector<double> get_read_data(); // функция получения входных данных
    void write_answer (vector<double> ); // функция записи результата
    bool get_success_read (); // получения статуса прочитанных файлов
    void write_mes (string); // запись строки
    void write_mes (vector<double>); // запись вектора
private:
    ofstream output_work ; // файл для документирования работы программы
    short error_read; // маркер успешности прочитанного файла
    vector<double> read_data; // вектор, хранящий входные данные
    bool success_read; // успешность чтения
    void read_settings (); // функция чтения входных данных
    void set_success_read (bool); // установка статуса успешности прочтения входных данных
    void set_error_read(short); // сохранение ошибки чтения
    void set_read_data(double); // добавление прочитанного в вектор
    string convert_to_text (short); // функция формирования текстового описания ошибки
    short get_error_read (); // получения статуса чтения файла
    void work_open(); // функция открытия файла
    void work_close(); // функция закрытия файла
};
#endif // READ_RITE_FILES_H
