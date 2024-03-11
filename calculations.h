#ifndef CALCULATIONS_H
#define CALCULATIONS_H
#include <cmath>
#include <algorithm>
#include "read_rite_files.h"
#include "eigen/Eigen/Dense"
#include "eigen/nonlinearoptimization"
#include "eigen/NumericalDiff"
#include "eigen/AutoDiff"
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
void set_state(bool); // установка состояния алгоритма
void prepar_data (); // потготовка даных
void form_funktion(); // формирование функции невязки
void form_grad (); // формирование градиента
void check_grad (); // проверка условия окончания алгоритма по градиенту
void work_calculate (); // функция вычислений
void form_new_param(); // формирование новых значений параметров
void check_iteration (); // проверка условия выхода из алгоритма по количеству итераций
void calculate_work (); // цикл вычислений
void file_result (); // запись результатов 
double e; // точность алгоритма
double norm_grad ; // норма вектора градиента
unsigned int n; // максимальное количество итераций
unsigned int step; // счетчик итераций
vector<double> input_vector, output_vector; // входные данные
VectorXd a_b_c; // выходные данные
VectorXd d_e_f; // координаты точек d e f
VectorXd sub_; // разности векторов
VectorXd grad; // градиенты
VectorXd alpha; // шаг спуска
double funck (const VectorXd &, VectorXd &, VectorXd &); // исходная функция
VectorXd funck_grad (const VectorXd &) ; // функция градиентов
vector<double> convert_to_double(VectorXd ); // функция конвертирования из VectorXd в double
VectorXd abs_xd(VectorXd) ; // модуль координат градиентов
VectorXd constants_xd(VectorXd) ; // функция создания шага спуска градиента

};

#endif // CALCULATIONS_H