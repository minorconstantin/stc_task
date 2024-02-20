#ifndef READ_RITE_FILES_H
#define READ_RITE_FILES_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

class read_rite_files
{
public:
    read_rite_files(); // конструктор
short get_error_read (); // получения статуса чтения файла
vector<double>  get_read_data(); // функция получения входных данных
void read_settings (); // функция чтения входных данных
void write_answer (vector<double>  ); // функция записи результата
private:
short error_read; // маркер успешности прочитанного файла
vector<double>  read_data; // вектор, хранящий входные данные
void set_error_read(short); // сохранение ошибки чтения
void set_read_data(double); //  добавление прочитанного в вектор
string convert_to_text (short); // функция формирования текстового описания ошибки
};
#endif // READ_RITE_FILES_H
