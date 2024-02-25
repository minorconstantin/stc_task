#include "read_rite_files.h"

read_rite_files::read_rite_files()
{
read_data.clear(); // на всякий случай очищаем файл с данными
set_success_read (false);
read_settings ();
}
void read_rite_files::set_success_read(bool state) // функция сохранения ошибки
{
success_read = state;
}
bool read_rite_files::get_success_read() // функция проверки успешности чтения
{
return success_read;
}
void read_rite_files::set_error_read(short error) // функция сохранения ошибки
{
error_read = error;
}
void read_rite_files::set_read_data(double new_data) // функция добавления данных
{
read_data.push_back(new_data);
}
short read_rite_files::get_error_read ()
{
return  error_read ;
}
vector<double>  read_rite_files::get_read_data ()
{
return read_data ;
}
void read_rite_files::read_settings ()
{
ifstream setings_file ("setings.txt");
if (!setings_file.is_open()) 
{
set_error_read (-1);
return;
}
string string_temp; // строка для чтения файла
while (getline(setings_file, string_temp))
{
istringstream for_convert(string_temp);
double new_data;
for_convert >> new_data;
set_read_data(new_data);
set_error_read(read_data.size());
if (get_error_read()   ==15)
set_success_read (true);
else
set_success_read (false);
}
setings_file.close();
}
string read_rite_files::convert_to_text (short error)
{
string text_error ;
switch (error)
{
case -1:
text_error  = "не удалось открыть файл";
break;
case 0:
text_error   ="не удалось прочитать файл";
break;
case 15:
text_error   ="данные успешно прочитаны";
break;
default:
ostringstream temp_convert;
temp_convert << fixed << error;
text_error    = "не соответствие данных  прочитано " + temp_convert.str() + " а должно быть 15";
}
return text_error   ;
}
void read_rite_files::write_answer (vector<double>   result)
{
ofstream file_result ("coords.txt");
if (!file_result .is_open()) 
return;
if (result.size() !=6)
file_result << convert_to_text (get_error_read());
else
{
file_result <<  "координата a по x =" << result.at(0) << endl;
file_result <<  "координата a по y =" << result.at(1) << endl;
file_result <<  "координата b по x =" << result.at(2) << endl;
file_result <<  "координата b по y =" << result.at(3) << endl;
file_result <<  "координата c по x =" << result.at(4) << endl;
file_result <<  "координата c по y =" << result.at(5) << endl;
}
file_result.close();
}
