﻿#include "calculations.h"

calculations::calculations()
{
    input_vector.clear();
    output_vector.clear();
    start ();
}
VectorXd calculations::constants_xd(VectorXd)
{
    VectorXd beta = alpha.unaryExpr([](double x) {
        return x/abs(200*x);
    });
    return beta ;
}
VectorXd calculations::abs_xd(VectorXd)
{
    file_data.write_mes("Модуль вектора alpha");
    /*VectorXd beta = alpha;
for (int i =0; i < beta.size(); i ++)
beta(i) = abs(alpha(i));*/
    VectorXd beta = alpha.unaryExpr([](double x) {
        if (x >= 0)
            return x;
        else
            return -x;
    });
    vector<double> double_beta = convert_to_double(beta);
    return beta;
}
void calculations::start () // функция начала вычислений
{
    /* функция начала вычислений
вызывается при старте программы
1) читает входные данные
2) если данные прочитать удалось
3) возводит флаг готовности начала вычислений
4) вызывает функцию предварительной подготовки данных */
    if (file_data.get_success_read ())
        prepar_data ();
    else
        set_state(false);
}
void calculations::set_state (bool flag) // функция запуска остановки алгоритма
{
    state =flag;
    if (!state)
    {
        file_result ();
        file_data.write_mes("Алгоритм остановлен");
    }
    else
        file_data.write_mes("Алгоритм запущен");
}
bool calculations::get_state () // функция получения состояния алгоритма
{
    return state;
}
void calculations::prepar_data () // предварительная подготовка данных
{
    // для ввода количества итераций, точности рассчета, начальных значений переменных
    file_data.write_mes("готовим данные для вычислений");
    input_vector = file_data.get_read_data();
    Map<VectorXd> temp_d_e_f (input_vector.data(), 6);
    Map<VectorXd> temp_sub_ (input_vector.data() +6, 9);
    d_e_f = temp_d_e_f;
    sub_ =temp_sub_ ;
    a_b_c.resize(d_e_f.size());
    a_b_c.setZero();
    e =0.05;
    n =50;
    step =0;
    wcout << L"Инициализация начальных значений";
    set_state(true);
    file_data.write_mes("Запускаем вычисления");
    calculate_work ();
}
void calculations::form_funktion() // формирование рассчетной функции
{
    // написание формул для вычисления
    if (get_state())
    {
        file_data.write_mes("Вычисляем градиент");
        form_grad ();
        file_data.write_mes("Проверяем точность полученного решения");
        check_grad ();
    }
    if (get_state())
    {
        file_data.write_mes("Вычисление шагов градиентного спуска");
        work_calculate ();
        file_data.write_mes("формирование новых параметров");
        form_new_param();
        file_data.write_mes("Проверка на количество итераций");
        check_iteration ();
    }
}

void calculations::form_grad () // формирование градиента
{
    /* Функция формирования градиента
вызывается итеративно
Коэффициент рассчитывается по всем функциям, затем суммируется */
    file_data.write_mes("Предполагаемые параметры");
    vector<double> temp = convert_to_double(a_b_c);
    ostringstream temp_convert;
    temp_convert << fixed << funck(); // значение функции
    string mes = "значение функции " + temp_convert.str() + " ";
    file_data.write_mes(mes);
    file_data.write_mes("Градиент равен ");
    grad = funck_grad ();
    vector<double> double_grad = convert_to_double(grad);
}
void calculations::check_grad () // функция проверки градиента
{
    /* вызывается итеративно. Проверяет
1) градиент стремится к нулю
2) градиент не изменяется */
    norm_grad = grad.norm();
    ostringstream temp_convert;
    temp_convert << fixed << norm_grad ;
    string mes = "норма вектора градиента " + temp_convert.str() + " ";
    file_data.write_mes(mes);
    if (norm_grad <=e)
        set_state(false);
}

void calculations::work_calculate () // функция формирования шага градиента
{
    /* вызывается в цикле
рассчитывает шаг градиента для всех переменных */
    file_data.write_mes("Коэффициент градиентного спуска");
    alpha = grad/(norm_grad +e);
    // alpha = alpha.cwiseProduct((1./abs_xd(alpha)));
    vector<double> double_alpha= convert_to_double(alpha);
}
void calculations::form_new_param () // функция формирования новых значений параметров
{
    /* вызывается итеративно
Рассчитывает приближенное значение параметров с учетом градиентов */
    file_data.write_mes("Новое значение параметров");
    a_b_c = a_b_c -grad.cwiseProduct(abs_xd(alpha));
    vector<double> double_a_b_c = convert_to_double(a_b_c);
}
void calculations::check_iteration () // проверка на количество итераций
{
    /* функция вызывается итеративно.
Если количество итераций больше заданного останавливаем алгоритм*/
    ++step;
    if (step >=n)
        set_state(false);
}
void calculations::calculate_work () // функция цикла вычислений
{
    // вызываются другие функции алгоритма вычислений
    wcout << L"Запуск вычислений";
    ostringstream max_iter;
    string mes;
    max_iter << fixed << n;
    while (get_state())
    {
        ostringstream current_iter;
        current_iter << fixed << step;
        mes = "номер текущей итерации " + current_iter.str() + " имаксимальное количество итераций " +max_iter.str() + ".";
        file_data.write_mes(mes);
        form_funktion();
    }
}
void calculations::file_result () // функция записи результатов и ошибок
{
    output_vector.resize(a_b_c.size());
    Map<VectorXd>(output_vector.data(), output_vector.size()) = a_b_c;
    file_data.write_answer(output_vector);
}
double calculations::funck()
{
    VectorXd funk =VectorXd::Zero(9); // вектор, в котором хранится значение функции
    funk(0) =sqrt(pow((sub_(0) + sqrt(pow(d_e_f(0) - a_b_c(2), 2) + pow(d_e_f(1) - a_b_c(3), 2)) -sqrt(pow(d_e_f(0) - a_b_c(0), 2) + pow(d_e_f(1) - a_b_c(1), 2))), 2)) ;
    funk(1) =sqrt(pow((sub_(1) + sqrt(pow(d_e_f(0) - a_b_c(4), 2) + pow(d_e_f(1) - a_b_c(5), 2)) -sqrt(pow(d_e_f(0) - a_b_c(0), 2) + pow(d_e_f(1) - a_b_c(1), 2))), 2)) ;
    funk(2) =sqrt(pow((sub_(2) + sqrt(pow(d_e_f(0) - a_b_c(4), 2) + pow(d_e_f(1) - a_b_c(5), 2)) -sqrt(pow(d_e_f(0) - a_b_c(2), 2) + pow(d_e_f(1) - a_b_c(3), 2))), 2)) ;
    funk(3) =sqrt(pow((sub_(3) + sqrt(pow(d_e_f(2) - a_b_c(2), 2) + pow(d_e_f(3) - a_b_c(3), 2)) -sqrt(pow(d_e_f(2) - a_b_c(0), 2) + pow(d_e_f(3) - a_b_c(1), 2))), 2)) ;
    funk(4) =sqrt(pow((sub_(4) + sqrt(pow(d_e_f(2) - a_b_c(4), 2) + pow(d_e_f(3) - a_b_c(5), 2)) -sqrt(pow(d_e_f(2) - a_b_c(0), 2) + pow(d_e_f(3) - a_b_c(1), 2))), 2)) +sqrt(pow((sub_(5) + sqrt(pow(d_e_f(2) - a_b_c(4), 2) + pow(d_e_f(3) - a_b_c(5), 2)) -sqrt(pow(d_e_f(2) - a_b_c(2), 2) + pow(d_e_f(3) - a_b_c(3), 2))), 2));
    funk(5) = sqrt(pow((sub_(5) + sqrt(pow(d_e_f(2) - a_b_c(4), 2) + pow(d_e_f(3) - a_b_c(5), 2)) -sqrt(pow(d_e_f(2) - a_b_c(2), 2) + pow(d_e_f(3) - a_b_c(3), 2))), 2)) ;
    funk(6) =sqrt(pow((sub_(6) + sqrt(pow(d_e_f(4) - a_b_c(2), 2) + pow(d_e_f(5) - a_b_c(3), 2)) -sqrt(pow(d_e_f(4) - a_b_c(0), 2) + pow(d_e_f(5) - a_b_c(1), 2))), 2));
    funk(7) =sqrt(pow((sub_(7) + sqrt(pow(d_e_f(4) - a_b_c(4), 2) + pow(d_e_f(5) - a_b_c(5), 2)) -sqrt(pow(d_e_f(4) - a_b_c(0), 2) + pow(d_e_f(5) - a_b_c(1), 2))), 2));
    funk(8) =sqrt(pow((sub_(8) + sqrt(pow(d_e_f(4) - a_b_c(4), 2) + pow(d_e_f(5) - a_b_c(5), 2)) -sqrt(pow(d_e_f(4) - a_b_c(2), 2) + pow(d_e_f(5) - a_b_c(3), 2))), 2));
    return funk.array().sum();
}
VectorXd calculations::funck_grad ()
{
    VectorXd temp_a_b_c =VectorXd::Zero(a_b_c.size());
VectorXd temp_summ =VectorXd::Zero(a_b_c.size());
AutoDiffScalar<Vector2d> x_0(a_b_c(0), Vector2d(1,0)), y_0;
AutoDiffScalar<Vector2d> x_1(a_b_c(1), Vector2d(1,0)), y_1;
AutoDiffScalar<Vector2d> x_2(a_b_c(2), Vector2d(1,0)), y_2;
AutoDiffScalar<Vector2d> x_3(a_b_c(3), Vector2d(1,0)), y_3;
AutoDiffScalar<Vector2d> x_4(a_b_c(4), Vector2d(1,0)), y_4;
AutoDiffScalar<Vector2d> x_5(a_b_c(5), Vector2d(1,0)), y_5;
    y_0 =sqrt(pow((sub_(0) + sqrt(pow(d_e_f(0) - a_b_c(2), 2) + pow(d_e_f(1) - a_b_c(3), 2)) -sqrt(pow(d_e_f(0) - x_0, 2) + pow(d_e_f(1) - a_b_c(1), 2))), 2)) ;
temp_summ(0)  = y_0.derivatives()[0];
    y_1 =sqrt(pow((sub_(0) + sqrt(pow(d_e_f(0) - a_b_c(2), 2) + pow(d_e_f(1) - a_b_c(3), 2)) -sqrt(pow(d_e_f(0) - a_b_c(0), 2) + pow(d_e_f(1) - x_1, 2))), 2)) ;
temp_summ(1)  = y_1.derivatives()[0];
    y_2 =sqrt(pow((sub_(0) + sqrt(pow(d_e_f(0) - x_2, 2) + pow(d_e_f(1) - a_b_c(3), 2)) -sqrt(pow(d_e_f(0) - a_b_c(0), 2) + pow(d_e_f(1) - a_b_c(1), 2))), 2)) ;
temp_summ(2)  = y_2.derivatives()[0];
    y_3 =sqrt(pow((sub_(0) + sqrt(pow(d_e_f(0) - a_b_c(2), 2) + pow(d_e_f(1) - x_3, 2)) -sqrt(pow(d_e_f(0) - a_b_c(0), 2) + pow(d_e_f(1) - a_b_c(1), 2))), 2)) ;
temp_summ(3)  = y_3.derivatives()[0];
temp_a_b_c = temp_a_b_c + temp_summ;
temp_summ.setZero(); 
    y_0 =sqrt(pow((sub_(1) + sqrt(pow(d_e_f(0) - a_b_c(4), 2) + pow(d_e_f(1) - a_b_c(5), 2)) -sqrt(pow(d_e_f(0) - x_0, 2) + pow(d_e_f(1) - a_b_c(1), 2))), 2)) ;
temp_summ(0)  = y_0.derivatives()[0];
    y_1 =sqrt(pow((sub_(1) + sqrt(pow(d_e_f(0) - a_b_c(4), 2) + pow(d_e_f(1) - a_b_c(5), 2)) -sqrt(pow(d_e_f(0) - a_b_c(0), 2) + pow(d_e_f(1) - x_1, 2))), 2)) ;
temp_summ(1)  = y_1.derivatives()[0];
    y_4 =sqrt(pow((sub_(1) + sqrt(pow(d_e_f(0) - x_4, 2) + pow(d_e_f(1) - a_b_c(5), 2)) -sqrt(pow(d_e_f(0) - a_b_c(0), 2) + pow(d_e_f(1) - a_b_c(1), 2))), 2)) ;
temp_summ(4)  = y_4.derivatives()[0];
    y_5 =sqrt(pow((sub_(1) + sqrt(pow(d_e_f(0) - a_b_c(4), 2) + pow(d_e_f(1) - x_5, 2)) -sqrt(pow(d_e_f(0) - a_b_c(0), 2) + pow(d_e_f(1) - a_b_c(1), 2))), 2)) ;
temp_summ(5)  = y_5.derivatives()[0];
temp_a_b_c = temp_a_b_c + temp_summ; 
temp_summ.setZero();
    y_2 =sqrt(pow((sub_(2) + sqrt(pow(d_e_f(0) - a_b_c(4), 2) + pow(d_e_f(1) - a_b_c(5), 2)) -sqrt(pow(d_e_f(0) - x_2, 2) + pow(d_e_f(1) - a_b_c(3), 2))), 2)) ;
temp_summ(2)  = y_2.derivatives()[0];
    y_3 =sqrt(pow((sub_(2) + sqrt(pow(d_e_f(0) - a_b_c(4), 2) + pow(d_e_f(1) - a_b_c(5), 2)) -sqrt(pow(d_e_f(0) - a_b_c(2), 2) + pow(d_e_f(1) - x_3, 2))), 2)) ;
temp_summ(3)  = y_3.derivatives()[0];
    y_4 =sqrt(pow((sub_(2) + sqrt(pow(d_e_f(0) - x_4, 2) + pow(d_e_f(1) - a_b_c(5), 2)) -sqrt(pow(d_e_f(0) - a_b_c(2), 2) + pow(d_e_f(1) - a_b_c(3), 2))), 2)) ;
temp_summ(4)  = y_4.derivatives()[0];
    y_5 =sqrt(pow((sub_(2) + sqrt(pow(d_e_f(0) - a_b_c(4), 2) + pow(d_e_f(1) - x_5, 2)) -sqrt(pow(d_e_f(0) - a_b_c(2), 2) + pow(d_e_f(1) - a_b_c(3), 2))), 2)) ;
temp_summ(5)  = y_5.derivatives()[0];
temp_a_b_c = temp_a_b_c + temp_summ;
temp_summ.setZero();
    y_0 =sqrt(pow((sub_(3) + sqrt(pow(d_e_f(2) - a_b_c(2), 2) + pow(d_e_f(3) - a_b_c(3), 2)) -sqrt(pow(d_e_f(2) - x_0, 2) + pow(d_e_f(3) - a_b_c(1), 2))), 2)) ;
temp_summ(0)  = y_0.derivatives()[0];
    y_1 =sqrt(pow((sub_(3) + sqrt(pow(d_e_f(2) - a_b_c(2), 2) + pow(d_e_f(3) - a_b_c(3), 2)) -sqrt(pow(d_e_f(2) - a_b_c(0), 2) + pow(d_e_f(3) - x_1, 2))), 2)) ;
temp_summ(1)  = y_1.derivatives()[0];
    y_2 =sqrt(pow((sub_(3) + sqrt(pow(d_e_f(2) - x_2, 2) + pow(d_e_f(3) - a_b_c(3), 2)) -sqrt(pow(d_e_f(2) - a_b_c(0), 2) + pow(d_e_f(3) - a_b_c(1), 2))), 2)) ;
temp_summ(2)  = y_2.derivatives()[0];
    y_3 =sqrt(pow((sub_(3) + sqrt(pow(d_e_f(2) - a_b_c(2), 2) + pow(d_e_f(3) - x_3, 2)) -sqrt(pow(d_e_f(2) - a_b_c(0), 2) + pow(d_e_f(3) - a_b_c(1), 2))), 2)) ;
temp_summ(3)  = y_3.derivatives()[0];
temp_a_b_c = temp_a_b_c + temp_summ;
temp_summ.setZero(); 
    y_0 =sqrt(pow((sub_(4) + sqrt(pow(d_e_f(2) - a_b_c(4), 2) + pow(d_e_f(3) - a_b_c(5), 2)) -sqrt(pow(d_e_f(2) - x_0, 2) + pow(d_e_f(3) - a_b_c(1), 2))), 2)) ;
temp_summ(0)  = y_0.derivatives()[0];
    y_1 =sqrt(pow((sub_(4) + sqrt(pow(d_e_f(2) - a_b_c(4), 2) + pow(d_e_f(3) - a_b_c(5), 2)) -sqrt(pow(d_e_f(2) - a_b_c(0), 2) + pow(d_e_f(3) - x_1, 2))), 2)) ;
temp_summ(1)  = y_1.derivatives()[0];
    y_4 =sqrt(pow((sub_(4) + sqrt(pow(d_e_f(2) - x_4, 2) + pow(d_e_f(3) - a_b_c(5), 2)) -sqrt(pow(d_e_f(2) - a_b_c(0), 2) + pow(d_e_f(3) - a_b_c(1), 2))), 2)) ;
temp_summ(4)  = y_4.derivatives()[0];
    y_5 =sqrt(pow((sub_(4) + sqrt(pow(d_e_f(2) - a_b_c(4), 2) + pow(d_e_f(3) - x_5, 2)) -sqrt(pow(d_e_f(2) - a_b_c(0), 2) + pow(d_e_f(3) - a_b_c(1), 2))), 2)) ;
temp_summ(5)  = y_5.derivatives()[0];
temp_a_b_c = temp_a_b_c + temp_summ; 
temp_summ.setZero();
    y_2 =sqrt(pow((sub_(5) + sqrt(pow(d_e_f(2) - a_b_c(4), 2) + pow(d_e_f(3) - a_b_c(5), 2)) -sqrt(pow(d_e_f(2) - x_2, 2) + pow(d_e_f(3) - a_b_c(3), 2))), 2)) ;
temp_summ(2)  = y_2.derivatives()[0];
    y_3 =sqrt(pow((sub_(5) + sqrt(pow(d_e_f(2) - a_b_c(4), 2) + pow(d_e_f(3) - a_b_c(5), 2)) -sqrt(pow(d_e_f(2) - a_b_c(2), 2) + pow(d_e_f(3) - x_3, 2))), 2)) ;
temp_summ(3)  = y_3.derivatives()[0];
    y_4 =sqrt(pow((sub_(5) + sqrt(pow(d_e_f(2) - x_4, 2) + pow(d_e_f(3) - a_b_c(5), 2)) -sqrt(pow(d_e_f(2) - a_b_c(2), 2) + pow(d_e_f(3) - a_b_c(3), 2))), 2)) ;
temp_summ(4)  = y_4.derivatives()[0];
    y_5 =sqrt(pow((sub_(5) + sqrt(pow(d_e_f(2) - a_b_c(4), 2) + pow(d_e_f(3) - x_5, 2)) -sqrt(pow(d_e_f(2) - a_b_c(2), 2) + pow(d_e_f(3) - a_b_c(3), 2))), 2)) ;
temp_summ(5)  = y_5.derivatives()[0];
temp_a_b_c = temp_a_b_c + temp_summ;
temp_summ.setZero();
    y_0 =sqrt(pow((sub_(6) + sqrt(pow(d_e_f(4) - a_b_c(2), 2) + pow(d_e_f(5) - a_b_c(3), 2)) -sqrt(pow(d_e_f(4) - x_0, 2) + pow(d_e_f(5) - a_b_c(1), 2))), 2)) ;
temp_summ(0)  = y_0.derivatives()[0];
    y_1 =sqrt(pow((sub_(6) + sqrt(pow(d_e_f(4) - a_b_c(2), 2) + pow(d_e_f(5) - a_b_c(3), 2)) -sqrt(pow(d_e_f(4) - a_b_c(0), 2) + pow(d_e_f(5) - x_1, 2))), 2)) ;
temp_summ(1)  = y_1.derivatives()[0];
    y_2 =sqrt(pow((sub_(6) + sqrt(pow(d_e_f(4) - x_2, 2) + pow(d_e_f(5) - a_b_c(3), 2)) -sqrt(pow(d_e_f(4) - a_b_c(0), 2) + pow(d_e_f(5) - a_b_c(1), 2))), 2)) ;
temp_summ(2)  = y_2.derivatives()[0];
    y_3 =sqrt(pow((sub_(6) + sqrt(pow(d_e_f(4) - a_b_c(2), 2) + pow(d_e_f(5) - x_3, 2)) -sqrt(pow(d_e_f(4) - a_b_c(0), 2) + pow(d_e_f(5) - a_b_c(1), 2))), 2)) ;
temp_summ(3)  = y_3.derivatives()[0];
temp_a_b_c = temp_a_b_c + temp_summ;
temp_summ.setZero(); 
    y_0 =sqrt(pow((sub_(7) + sqrt(pow(d_e_f(4) - a_b_c(4), 2) + pow(d_e_f(5) - a_b_c(5), 2)) -sqrt(pow(d_e_f(4) - x_0, 2) + pow(d_e_f(5) - a_b_c(1), 2))), 2)) ;
temp_summ(0)  = y_0.derivatives()[0];
    y_1 =sqrt(pow((sub_(7) + sqrt(pow(d_e_f(4) - a_b_c(4), 2) + pow(d_e_f(5) - a_b_c(5), 2)) -sqrt(pow(d_e_f(4) - a_b_c(0), 2) + pow(d_e_f(5) - x_1, 2))), 2)) ;
temp_summ(1)  = y_1.derivatives()[0];
    y_4 =sqrt(pow((sub_(7) + sqrt(pow(d_e_f(4) - x_4, 2) + pow(d_e_f(5) - a_b_c(5), 2)) -sqrt(pow(d_e_f(4) - a_b_c(0), 2) + pow(d_e_f(5) - a_b_c(1), 2))), 2)) ;
temp_summ(4)  = y_4.derivatives()[0];
    y_5 =sqrt(pow((sub_(7) + sqrt(pow(d_e_f(4) - a_b_c(4), 2) + pow(d_e_f(5) - x_5, 2)) -sqrt(pow(d_e_f(4) - a_b_c(0), 2) + pow(d_e_f(5) - a_b_c(1), 2))), 2)) ;
temp_summ(5)  = y_5.derivatives()[0];
temp_a_b_c = temp_a_b_c + temp_summ; 
temp_summ.setZero();
    y_2 =sqrt(pow((sub_(8) + sqrt(pow(d_e_f(4) - a_b_c(4), 2) + pow(d_e_f(5) - a_b_c(5), 2)) -sqrt(pow(d_e_f(4) - x_2, 2) + pow(d_e_f(5) - a_b_c(3), 2))), 2)) ;
temp_summ(2)  = y_2.derivatives()[0];
    y_3 =sqrt(pow((sub_(8) + sqrt(pow(d_e_f(4) - a_b_c(4), 2) + pow(d_e_f(5) - a_b_c(5), 2)) -sqrt(pow(d_e_f(4) - a_b_c(2), 2) + pow(d_e_f(5) - x_3, 2))), 2)) ;
temp_summ(3)  = y_3.derivatives()[0];
    y_4 =sqrt(pow((sub_(8) + sqrt(pow(d_e_f(4) - x_4, 2) + pow(d_e_f(5) - a_b_c(5), 2)) -sqrt(pow(d_e_f(4) - a_b_c(2), 2) + pow(d_e_f(5) - a_b_c(3), 2))), 2)) ;
temp_summ(4)  = y_4.derivatives()[0];
    y_5 =sqrt(pow((sub_(8) + sqrt(pow(d_e_f(4) - a_b_c(4), 2) + pow(d_e_f(5) - x_5, 2)) -sqrt(pow(d_e_f(4) - a_b_c(2), 2) + pow(d_e_f(5) - a_b_c(3), 2))), 2)) ;
temp_summ(5)  = y_5.derivatives()[0];
temp_a_b_c = temp_a_b_c + temp_summ;
temp_summ.setZero();

    return temp_a_b_c;

/*    temp_summ(0) = ((d_e_f(0) - a_b_c(0))*(-sqrt(pow(d_e_f(0) - a_b_c(0),2) + pow(d_e_f(1) - a_b_c(1),2)) + sqrt(pow(d_e_f(0) - a_b_c(2),2) + pow(d_e_f(1) - a_b_c(3),2)) + sub_(0)))/(sqrt(pow(d_e_f(0) - a_b_c(0),2) + pow(d_e_f(1) - a_b_c(1),2))*sqrt(pow(-sqrt(pow(d_e_f(0) - a_b_c(0),2) + pow(d_e_f(1) - a_b_c(1),2)) + sqrt(pow(d_e_f(0) - a_b_c(2),2) + pow(d_e_f(1) - a_b_c(3),2)) + sub_(0),2))) ;
    temp_summ(1) =((d_e_f(0) - a_b_c(0))*(-sqrt(pow(d_e_f(0) - a_b_c(0),2) + pow(d_e_f(1) - a_b_c(1),2)) + sqrt(pow(d_e_f(0) - a_b_c(4),2) + pow(d_e_f(1) - a_b_c(5),2)) + sub_(1)))/(sqrt(pow(d_e_f(0) - a_b_c(0),2) + pow(d_e_f(1) - a_b_c(1),2))*sqrt(pow(-sqrt(pow(d_e_f(0) - a_b_c(0),2) + pow(d_e_f(1) - a_b_c(1),2)) + sqrt(pow(d_e_f(0) - a_b_c(4),2) + pow(d_e_f(1) - a_b_c(5),2)) + sub_(1),2)));
    temp_summ(2) =((d_e_f(2) - a_b_c(0))*(-sqrt(pow(d_e_f(2) - a_b_c(0),2) + pow(d_e_f(3) - a_b_c(1),2)) + sqrt(pow(d_e_f(2) - a_b_c(2),2) + pow(d_e_f(3) - a_b_c(3),2)) + sub_(3)))/(sqrt(pow(d_e_f(2) - a_b_c(0),2) + pow(d_e_f(3) - a_b_c(1),2))*sqrt(pow(-sqrt(pow(d_e_f(2) - a_b_c(0),2) + pow(d_e_f(3) - a_b_c(1),2)) + sqrt(pow(d_e_f(2) - a_b_c(2),2) + pow(d_e_f(3) - a_b_c(3),2)) + sub_(3),2)));
    temp_summ(3) =((d_e_f(2) - a_b_c(0))*(-sqrt(pow(d_e_f(2) - a_b_c(0),2) + pow(d_e_f(3) - a_b_c(1),2)) + sqrt(pow(d_e_f(2) - a_b_c(4),2) + pow(d_e_f(3) - a_b_c(5),2)) + sub_(4)))/(sqrt(pow(d_e_f(2) - a_b_c(0),2) + pow(d_e_f(3) - a_b_c(1),2))*sqrt(pow(-sqrt(pow(d_e_f(2) - a_b_c(0),2) + pow(d_e_f(3) - a_b_c(1),2)) + sqrt(pow(d_e_f(2) - a_b_c(4),2) + pow(d_e_f(3) - a_b_c(5),2)) + sub_(4),2)));
    temp_summ(4) =((d_e_f(4) - a_b_c(0))*(-sqrt(pow(d_e_f(4) - a_b_c(0),2) + pow(d_e_f(5) - a_b_c(1),2)) + sqrt(pow(d_e_f(4) - a_b_c(2),2) + pow(d_e_f(5) - a_b_c(3),2)) + sub_(6)))/(sqrt(pow(d_e_f(4) - a_b_c(0),2) + pow(d_e_f(5) - a_b_c(1),2))*sqrt(pow(-sqrt(pow(d_e_f(4) - a_b_c(0),2) + pow(d_e_f(5) - a_b_c(1),2)) + sqrt(pow(d_e_f(4) - a_b_c(2),2) + pow(d_e_f(5) - a_b_c(3),2)) + sub_(6),2)));
    temp_summ(5) =((d_e_f(4) - a_b_c(0))*(-sqrt(pow(d_e_f(4) - a_b_c(0),2) + pow(d_e_f(5) - a_b_c(1),2)) + sqrt(pow(d_e_f(4) - a_b_c(4),2) + pow(d_e_f(5) - a_b_c(5),2)) + sub_(7)))/(sqrt(pow(d_e_f(4) - a_b_c(0),2) + pow(d_e_f(5) - a_b_c(1),2))*sqrt(pow(-sqrt(pow(d_e_f(4) - a_b_c(0),2) + pow(d_e_f(5) - a_b_c(1),2)) + sqrt(pow(d_e_f(4) - a_b_c(4),2) + pow(d_e_f(5) - a_b_c(5),2)) + sub_(7),2)));
    temp_a_b_c (0) =temp_summ.array().sum();
    temp_summ(0) =((d_e_f(1) - a_b_c(1))*(-sqrt(pow(d_e_f(0) - a_b_c(0),2) + pow(d_e_f(1) - a_b_c(1),2)) + sqrt(pow(d_e_f(0) - a_b_c(2),2) + pow(d_e_f(1) - a_b_c(3),2)) + sub_(0)))/(sqrt(pow(d_e_f(0) - a_b_c(0),2) + pow(d_e_f(1) - a_b_c(1),2))*sqrt(pow(-sqrt(pow(d_e_f(0) - a_b_c(0),2) + pow(d_e_f(1) - a_b_c(1),2)) + sqrt(pow(d_e_f(0) - a_b_c(2),2) + pow(d_e_f(1) - a_b_c(3),2)) + sub_(0),2)));
    temp_summ(1) =((d_e_f(1) - a_b_c(1))*(-sqrt(pow(d_e_f(0) - a_b_c(0),2) + pow(d_e_f(1) - a_b_c(1),2)) + sqrt(pow(d_e_f(0) - a_b_c(4),2) + pow(d_e_f(1) - a_b_c(5),2)) + sub_(1)))/(sqrt(pow(d_e_f(0) - a_b_c(0),2) + pow(d_e_f(1) - a_b_c(1),2))*sqrt(pow(-sqrt(pow(d_e_f(0) - a_b_c(0),2) + pow(d_e_f(1) - a_b_c(1),2)) + sqrt(pow(d_e_f(0) - a_b_c(4),2) + pow(d_e_f(1) - a_b_c(5),2)) + sub_(1),2)));
    temp_summ(2) =((d_e_f(3) - a_b_c(1))*(-sqrt(pow(d_e_f(2) - a_b_c(0),2) + pow(d_e_f(3) - a_b_c(1),2)) + sqrt(pow(d_e_f(2) - a_b_c(2),2) + pow(d_e_f(3) - a_b_c(3),2)) + sub_(3)))/(sqrt(pow(d_e_f(2) - a_b_c(0),2) + pow(d_e_f(3) - a_b_c(1),2))*sqrt(pow(-sqrt(pow(d_e_f(2) - a_b_c(0),2) + pow(d_e_f(3) - a_b_c(1),2)) + sqrt(pow(d_e_f(2) - a_b_c(2),2) + pow(d_e_f(3) - a_b_c(3),2)) + sub_(3),2)));
    temp_summ(3) =((d_e_f(3) - a_b_c(1))*(-sqrt(pow(d_e_f(2) - a_b_c(0),2) + pow(d_e_f(3) - a_b_c(1),2)) + sqrt(pow(d_e_f(2) - a_b_c(4),2) + pow(d_e_f(3) - a_b_c(5),2)) + sub_(4)))/(sqrt(pow(d_e_f(2) - a_b_c(0),2) + pow(d_e_f(3) - a_b_c(1),2))*sqrt(pow(-sqrt(pow(d_e_f(2) - a_b_c(0),2) + pow(d_e_f(3) - a_b_c(1),2)) + sqrt(pow(d_e_f(2) - a_b_c(4),2) + pow(d_e_f(3) - a_b_c(5),2)) + sub_(4),2)));
    temp_summ(4) =((d_e_f(5) - a_b_c(1))*(-sqrt(pow(d_e_f(4) - a_b_c(0),2) + pow(d_e_f(5) - a_b_c(1),2)) + sqrt(pow(d_e_f(4) - a_b_c(2),2) + pow(d_e_f(5) - a_b_c(3),2)) + sub_(6)))/(sqrt(pow(d_e_f(4) - a_b_c(0),2) + pow(d_e_f(5) - a_b_c(1),2))*sqrt(pow(-sqrt(pow(d_e_f(4) - a_b_c(0),2) + pow(d_e_f(5) - a_b_c(1),2)) + sqrt(pow(d_e_f(4) - a_b_c(2),2) + pow(d_e_f(5) - a_b_c(3),2)) + sub_(6),2))) ;
    temp_summ(5) =((d_e_f(5) - a_b_c(1))*(-sqrt(pow(d_e_f(4) - a_b_c(0),2) + pow(d_e_f(5) - a_b_c(1),2)) + sqrt(pow(d_e_f(4) - a_b_c(4),2) + pow(d_e_f(5) - a_b_c(5),2)) + sub_(7)))/(sqrt(pow(d_e_f(4) - a_b_c(0),2) + pow(d_e_f(5) - a_b_c(1),2))*sqrt(pow(-sqrt(pow(d_e_f(4) - a_b_c(0),2) + pow(d_e_f(5) - a_b_c(1),2)) + sqrt(pow(d_e_f(4) - a_b_c(4),2) + pow(d_e_f(5) - a_b_c(5),2)) + sub_(7),2)));
    temp_a_b_c (1) =temp_summ.array().sum();
    temp_summ(0) =temp_a_b_c (2) = ((a_b_c(2) - d_e_f(0))*(-sqrt(pow(d_e_f(0) - a_b_c(0),2) + pow(d_e_f(1) - a_b_c(1),2)) + sqrt(pow(d_e_f(0) - a_b_c(2),2) + pow(d_e_f(1) - a_b_c(3),2)) + sub_(0)))/(sqrt(pow(d_e_f(0) - a_b_c(2),2) + pow(d_e_f(1) - a_b_c(3),2))*sqrt(pow(-sqrt(pow(d_e_f(0) - a_b_c(0),2) + pow(d_e_f(1) - a_b_c(1),2)) + sqrt(pow(d_e_f(0) - a_b_c(2),2) + pow(d_e_f(1) - a_b_c(3),2)) + sub_(0),2)));
    temp_summ(1) = ((d_e_f(0) - a_b_c(2))*(-sqrt(pow(d_e_f(0) - a_b_c(2),2) + pow(d_e_f(1) - a_b_c(3),2)) + sqrt(pow(d_e_f(0) - a_b_c(4),2) + pow(d_e_f(1) - a_b_c(5),2)) + sub_(2)))/(sqrt(pow(d_e_f(0) - a_b_c(2),2) + pow(d_e_f(1) - a_b_c(3),2))*sqrt(pow(-sqrt(pow(d_e_f(0) - a_b_c(2),2) + pow(d_e_f(1) - a_b_c(3),2)) + sqrt(pow(d_e_f(0) - a_b_c(4),2) + pow(d_e_f(1) - a_b_c(5),2)) + sub_(2),2))) ;
    temp_summ(2) =((a_b_c(2) - d_e_f(2))*(-sqrt(pow(d_e_f(2) - a_b_c(0),2) + pow(d_e_f(3) - a_b_c(1),2)) + sqrt(pow(d_e_f(2) - a_b_c(2),2) + pow(d_e_f(3) - a_b_c(3),2)) + sub_(3)))/(sqrt(pow(d_e_f(2) - a_b_c(2),2) + pow(d_e_f(3) - a_b_c(3),2))*sqrt(pow(-sqrt(pow(d_e_f(2) - a_b_c(0),2) + pow(d_e_f(3) - a_b_c(1),2)) + sqrt(pow(d_e_f(2) - a_b_c(2),2) + pow(d_e_f(3) - a_b_c(3),2)) + sub_(3),2)));
    temp_summ(3) =((d_e_f(2) - a_b_c(2))*(-sqrt(pow(d_e_f(2) - a_b_c(2),2) + pow(d_e_f(3) - a_b_c(3),2)) + sqrt(pow(d_e_f(2) - a_b_c(4),2) + pow(d_e_f(3) - a_b_c(5),2)) + sub_(5)))/(sqrt(pow(d_e_f(2) - a_b_c(2),2) + pow(d_e_f(3) - a_b_c(3),2))*sqrt(pow(-sqrt(pow(d_e_f(2) - a_b_c(2),2) + pow(d_e_f(3) - a_b_c(3),2)) + sqrt(pow(d_e_f(2) - a_b_c(4),2) + pow(d_e_f(3) - a_b_c(5),2)) + sub_(5),2)));
    temp_summ(4) =((a_b_c(2) - d_e_f(4))*(-sqrt(pow(d_e_f(4) - a_b_c(0),2) + pow(d_e_f(5) - a_b_c(1),2)) + sqrt(pow(d_e_f(4) - a_b_c(2),2) + pow(d_e_f(5) - a_b_c(3),2)) + sub_(6)))/(sqrt(pow(d_e_f(4) - a_b_c(2),2) + pow(d_e_f(5) - a_b_c(3),2))*sqrt(pow(-sqrt(pow(d_e_f(4) - a_b_c(0),2) + pow(d_e_f(5) - a_b_c(1),2)) + sqrt(pow(d_e_f(4) - a_b_c(2),2) + pow(d_e_f(5) - a_b_c(3),2)) + sub_(6),2)));
    temp_summ(5) =((d_e_f(4) - a_b_c(2))*(-sqrt(pow(d_e_f(4) - a_b_c(2),2) + pow(d_e_f(5) - a_b_c(3),2)) + sqrt(pow(d_e_f(4) - a_b_c(4),2) + pow(d_e_f(5) - a_b_c(5),2)) + sub_(8)))/(sqrt(pow(d_e_f(4) - a_b_c(2),2) + pow(d_e_f(5) - a_b_c(3),2))*sqrt(pow(-sqrt(pow(d_e_f(4) - a_b_c(2),2) + pow(d_e_f(5) - a_b_c(3),2)) + sqrt(pow(d_e_f(4) - a_b_c(4),2) + pow(d_e_f(5) - a_b_c(5),2)) + sub_(8),2)));
    temp_a_b_c (2) =temp_summ.array().sum();
    temp_summ(0) =((a_b_c(3) - d_e_f(1))*(-sqrt(pow(d_e_f(0) - a_b_c(0),2) + pow(d_e_f(1) - a_b_c(1),2)) + sqrt(pow(d_e_f(0) - a_b_c(2),2) + pow(d_e_f(1) - a_b_c(3),2)) + sub_(0)))/(sqrt(pow(d_e_f(0) - a_b_c(2),2) + pow(d_e_f(1) - a_b_c(3),2))*sqrt(pow(-sqrt(pow(d_e_f(0) - a_b_c(0),2) + pow(d_e_f(1) - a_b_c(1),2)) + sqrt(pow(d_e_f(0) - a_b_c(2),2) + pow(d_e_f(1) - a_b_c(3),2)) + sub_(0),2)));
    temp_summ(1) =((d_e_f(1) - a_b_c(3))*(-sqrt(pow(d_e_f(0) - a_b_c(2),2) + pow(d_e_f(1) - a_b_c(3),2)) + sqrt(pow(d_e_f(0) - a_b_c(4),2) + pow(d_e_f(1) - a_b_c(5),2)) + sub_(2)))/(sqrt(pow(d_e_f(0) - a_b_c(2),2) + pow(d_e_f(1) - a_b_c(3),2))*sqrt(pow(-sqrt(pow(d_e_f(0) - a_b_c(2),2) + pow(d_e_f(1) - a_b_c(3),2)) + sqrt(pow(d_e_f(0) - a_b_c(4),2) + pow(d_e_f(1) - a_b_c(5),2)) + sub_(2),2)));
    temp_summ(2) =((a_b_c(3) - d_e_f(3))*(-sqrt(pow(d_e_f(2) - a_b_c(0),2) + pow(d_e_f(3) - a_b_c(1),2)) + sqrt(pow(d_e_f(2) - a_b_c(2),2) + pow(d_e_f(3) - a_b_c(3),2)) + sub_(3)))/(sqrt(pow(d_e_f(2) - a_b_c(2),2) + pow(d_e_f(3) - a_b_c(3),2))*sqrt(pow(-sqrt(pow(d_e_f(2) - a_b_c(0),2) + pow(d_e_f(3) - a_b_c(1),2)) + sqrt(pow(d_e_f(2) - a_b_c(2),2) + pow(d_e_f(3) - a_b_c(3),2)) + sub_(3),2)));
    temp_summ(3) =((d_e_f(3) - a_b_c(3))*(-sqrt(pow(d_e_f(2) - a_b_c(2),2) + pow(d_e_f(3) - a_b_c(3),2)) + sqrt(pow(d_e_f(2) - a_b_c(4),2) + pow(d_e_f(3) - a_b_c(5),2)) + sub_(5)))/(sqrt(pow(d_e_f(2) - a_b_c(2),2) + pow(d_e_f(3) - a_b_c(3),2))*sqrt(pow(-sqrt(pow(d_e_f(2) - a_b_c(2),2) + pow(d_e_f(3) - a_b_c(3),2)) + sqrt(pow(d_e_f(2) - a_b_c(4),2) + pow(d_e_f(3) - a_b_c(5),2)) + sub_(5),2)));
    temp_summ(4) =((a_b_c(3) - d_e_f(5))*(-sqrt(pow(d_e_f(4) - a_b_c(0),2) + pow(d_e_f(5) - a_b_c(1),2)) + sqrt(pow(d_e_f(4) - a_b_c(2),2) + pow(d_e_f(5) - a_b_c(3),2)) + sub_(6)))/(sqrt(pow(d_e_f(4) - a_b_c(2),2) + pow(d_e_f(5) - a_b_c(3),2))*sqrt(pow(-sqrt(pow(d_e_f(4) - a_b_c(0),2) + pow(d_e_f(5) - a_b_c(1),2)) + sqrt(pow(d_e_f(4) - a_b_c(2),2) + pow(d_e_f(5) - a_b_c(3),2)) + sub_(6),2)));
    temp_summ(5) =((d_e_f(5) - a_b_c(3))*(-sqrt(pow(d_e_f(4) - a_b_c(2),2) + pow(d_e_f(5) - a_b_c(3),2)) + sqrt(pow(d_e_f(4) - a_b_c(4),2) + pow(d_e_f(5) - a_b_c(5),2)) + sub_(8)))/(sqrt(pow(d_e_f(4) - a_b_c(2),2) + pow(d_e_f(5) - a_b_c(3),2))*sqrt(pow(-sqrt(pow(d_e_f(4) - a_b_c(2),2) + pow(d_e_f(5) - a_b_c(3),2)) + sqrt(pow(d_e_f(4) - a_b_c(4),2) + pow(d_e_f(5) - a_b_c(5),2)) + sub_(8),2)));
    temp_a_b_c (3) =temp_summ.array().sum();
    temp_summ(0) =((a_b_c(4) - d_e_f(0))*(-sqrt(pow(d_e_f(0) - a_b_c(0),2) + pow(d_e_f(1) - a_b_c(1),2)) + sqrt(pow(d_e_f(0) - a_b_c(4),2) + pow(d_e_f(1) - a_b_c(5),2)) + sub_(1)))/(sqrt(pow(d_e_f(0) - a_b_c(4),2) + pow(d_e_f(1) - a_b_c(5),2))*sqrt(pow(-sqrt(pow(d_e_f(0) - a_b_c(0),2) + pow(d_e_f(1) - a_b_c(1),2)) + sqrt(pow(d_e_f(0) - a_b_c(4),2) + pow(d_e_f(1) - a_b_c(5),2)) + sub_(1),2)));
    temp_summ(1) =((a_b_c(4) - d_e_f(0))*(-sqrt(pow(d_e_f(0) - a_b_c(2),2) + pow(d_e_f(1) - a_b_c(3),2)) + sqrt(pow(d_e_f(0) - a_b_c(4),2) + pow(d_e_f(1) - a_b_c(5),2)) + sub_(2)))/(sqrt(pow(d_e_f(0) - a_b_c(4),2) + pow(d_e_f(1) - a_b_c(5),2))*sqrt(pow(-sqrt(pow(d_e_f(0) - a_b_c(2),2) + pow(d_e_f(1) - a_b_c(3),2)) + sqrt(pow(d_e_f(0) - a_b_c(4),2) + pow(d_e_f(1) - a_b_c(5),2)) + sub_(2),2)));
    temp_summ(2) =((a_b_c(4) - d_e_f(2))*(-sqrt(pow(d_e_f(2) - a_b_c(0),2) + pow(d_e_f(3) - a_b_c(1),2)) + sqrt(pow(d_e_f(2) - a_b_c(4),2) + pow(d_e_f(3) - a_b_c(5),2)) + sub_(4)))/(sqrt(pow(d_e_f(2) - a_b_c(4),2) + pow(d_e_f(3) - a_b_c(5),2))*sqrt(pow(-sqrt(pow(d_e_f(2) - a_b_c(0),2) + pow(d_e_f(3) - a_b_c(1),2)) + sqrt(pow(d_e_f(2) - a_b_c(4),2) + pow(d_e_f(3) - a_b_c(5),2)) + sub_(4),2)));
    temp_summ(3) = ((a_b_c(4) - d_e_f(2))*(-sqrt(pow(d_e_f(2) - a_b_c(2),2) + pow(d_e_f(3) - a_b_c(3),2)) + sqrt(pow(d_e_f(2) - a_b_c(4),2) + pow(d_e_f(3) - a_b_c(5),2)) + sub_(5)))/(sqrt(pow(d_e_f(2) - a_b_c(4),2) + pow(d_e_f(3) - a_b_c(5),2))*sqrt(pow(-sqrt(pow(d_e_f(2) - a_b_c(2),2) + pow(d_e_f(3) - a_b_c(3),2)) + sqrt(pow(d_e_f(2) - a_b_c(4),2) + pow(d_e_f(3) - a_b_c(5),2)) + sub_(5),2)));
    temp_summ(4) =((a_b_c(4) - d_e_f(4))*(-sqrt(pow(d_e_f(4) - a_b_c(0),2) + pow(d_e_f(5) - a_b_c(1),2)) + sqrt(pow(d_e_f(4) - a_b_c(4),2) + pow(d_e_f(5) - a_b_c(5),2)) + sub_(7)))/(sqrt(pow(d_e_f(4) - a_b_c(4),2) + pow(d_e_f(5) - a_b_c(5),2))*sqrt(pow(-sqrt(pow(d_e_f(4) - a_b_c(0),2) + pow(d_e_f(5) - a_b_c(1),2)) + sqrt(pow(d_e_f(4) - a_b_c(4),2) + pow(d_e_f(5) - a_b_c(5),2)) + sub_(7),2)));
    temp_summ(5) =((a_b_c(4) - d_e_f(4))*(-sqrt(pow(d_e_f(4) - a_b_c(2),2) + pow(d_e_f(5) - a_b_c(3),2)) + sqrt(pow(d_e_f(4) - a_b_c(4),2) + pow(d_e_f(5) - a_b_c(5),2)) + sub_(8)))/(sqrt(pow(d_e_f(4) - a_b_c(4),2) + pow(d_e_f(5) - a_b_c(5),2))*sqrt(pow(-sqrt(pow(d_e_f(4) - a_b_c(2),2) + pow(d_e_f(5) - a_b_c(3),2)) + sqrt(pow(d_e_f(4) - a_b_c(4),2) + pow(d_e_f(5) - a_b_c(5),2)) + sub_(8),2)));
    temp_a_b_c (4) =temp_summ.array().sum();
    temp_summ(0) =((a_b_c(5) - d_e_f(1))*(-sqrt(pow(d_e_f(0) - a_b_c(0),2) + pow(d_e_f(1) - a_b_c(1),2)) + sqrt(pow(d_e_f(0) - a_b_c(4),2) + pow(d_e_f(1) - a_b_c(5),2)) + sub_(1)))/(sqrt(pow(d_e_f(0) - a_b_c(4),2) + pow(d_e_f(1) - a_b_c(5),2))*sqrt(pow(-sqrt(pow(d_e_f(0) - a_b_c(0),2) + pow(d_e_f(1) - a_b_c(1),2)) + sqrt(pow(d_e_f(0) - a_b_c(4),2) + pow(d_e_f(1) - a_b_c(5),2)) + sub_(1),2)));
    temp_summ(1) =((a_b_c(5) - d_e_f(1))*(-sqrt(pow(d_e_f(0) - a_b_c(2),2) + pow(d_e_f(1) - a_b_c(3),2)) + sqrt(pow(d_e_f(0) - a_b_c(4),2) + pow(d_e_f(1) - a_b_c(5),2)) + sub_(2)))/(sqrt(pow(d_e_f(0) - a_b_c(4),2) + pow(d_e_f(1) - a_b_c(5),2))*sqrt(pow(-sqrt(pow(d_e_f(0) - a_b_c(2),2) + pow(d_e_f(1) - a_b_c(3),2)) + sqrt(pow(d_e_f(0) - a_b_c(4),2) + pow(d_e_f(1) - a_b_c(5),2)) + sub_(2),2)));
    temp_summ(2) =((a_b_c(5) - d_e_f(3))*(-sqrt(pow(d_e_f(2) - a_b_c(0),2) + pow(d_e_f(3) - a_b_c(1),2)) + sqrt(pow(d_e_f(2) - a_b_c(4),2) + pow(d_e_f(3) - a_b_c(5),2)) + sub_(4)))/(sqrt(pow(d_e_f(2) - a_b_c(4),2) + pow(d_e_f(3) - a_b_c(5),2))*sqrt(pow(-sqrt(pow(d_e_f(2) - a_b_c(0),2) + pow(d_e_f(3) - a_b_c(1),2)) + sqrt(pow(d_e_f(2) - a_b_c(4),2) + pow(d_e_f(3) - a_b_c(5),2)) + sub_(4),2)));
    temp_summ(3) =((a_b_c(5) - d_e_f(3))*(-sqrt(pow(d_e_f(2) - a_b_c(2),2) + pow(d_e_f(3) - a_b_c(3),2)) + sqrt(pow(d_e_f(2) - a_b_c(4),2) + pow(d_e_f(3) - a_b_c(5),2)) + sub_(5)))/(sqrt(pow(d_e_f(2) - a_b_c(4),2) + pow(d_e_f(3) - a_b_c(5),2))*sqrt(pow(-sqrt(pow(d_e_f(2) - a_b_c(2),2) + pow(d_e_f(3) - a_b_c(3),2)) + sqrt(pow(d_e_f(2) - a_b_c(4),2) + pow(d_e_f(3) - a_b_c(5),2)) + sub_(5),2)));
    temp_summ(4) =((a_b_c(5) - d_e_f(5))*(-sqrt(pow(d_e_f(4) - a_b_c(0),2) + pow(d_e_f(5) - a_b_c(1),2)) + sqrt(pow(d_e_f(4) - a_b_c(4),2) + pow(d_e_f(5) - a_b_c(5),2)) + sub_(7)))/(sqrt(pow(d_e_f(4) - a_b_c(4),2) + pow(d_e_f(5) - a_b_c(5),2))*sqrt(pow(-sqrt(pow(d_e_f(4) - a_b_c(0),2) + pow(d_e_f(5) - a_b_c(1),2)) + sqrt(pow(d_e_f(4) - a_b_c(4),2) + pow(d_e_f(5) - a_b_c(5),2)) + sub_(7),2)));
    temp_summ(5) =((a_b_c(5) - d_e_f(5))*(-sqrt(pow(d_e_f(4) - a_b_c(2),2) + pow(d_e_f(5) - a_b_c(3),2)) + sqrt(pow(d_e_f(4) - a_b_c(4),2) + pow(d_e_f(5) - a_b_c(5),2)) + sub_(8)))/(sqrt(pow(d_e_f(4) - a_b_c(4),2) + pow(d_e_f(5) - a_b_c(5),2))*sqrt(pow(-sqrt(pow(d_e_f(4) - a_b_c(2),2) + pow(d_e_f(5) - a_b_c(3),2)) + sqrt(pow(d_e_f(4) - a_b_c(4),2) + pow(d_e_f(5) - a_b_c(5),2)) + sub_(8),2)));
    temp_a_b_c (5) =temp_summ.array().sum(); */
}
vector<double> calculations::convert_to_double(VectorXd calc_vector)
{
    vector<double> temp_double;
    temp_double.resize(calc_vector.size());
    Map<VectorXd>(temp_double.data(), temp_double.size()) = calc_vector;
    file_data.write_mes(temp_double);
    return temp_double;
}
