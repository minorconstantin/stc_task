#ifndef CALCULATIONS_H
#define CALCULATIONS_H
#include "read_rite_files.h"
#include "Eigen/Dense"
using namespace Eigen;
class calculations
{
public:
    calculations();
private:
read_rite_files file_data; // класс для чтения и записи данных
bool state; // флаг состояния алгоритма запущен остановлен
void start (); // функция начала вычислений
bool get_state() ; // получить состояние алгоритма
void set_state(bool); //  установка состояния алгоритма
void  prepar_data  (); // потготовка даных
void form_funktion(); // формирование функции невязки
void form_grad ();  // формирование градиента
void check_grad (); // проверка условия окончания алгоритма по градиенту
void work_calculate (); // функция вычислений
void form_new_param(); // формирование новых значений параметров
void check_new_param (); // проверка условия завершения алгоритма по разности функций 
void  substitution(); // замена начальных значений
void check_iteration (); // проверка условия выхода из алгоритма  по количеству итераций
void calculate_work (); // цикл вычислений
void file_result (); // запись результатов 
double e; // точность алгоритма
double A; // шаг градиентного спуска
unsigned  int n; //  количество итераций
};

#endif // CALCULATIONS_H