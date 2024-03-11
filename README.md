﻿# stc_task
Тестовое задание на позицию математик программист в ООО СТЦ
Данный репазиторий содержит программу вычисления координат фокусов пересекающихся гипербол. Фокусы обозначаются  латинскими буквами a, b,c. 
Входные данные: координаты точек пересечения d, e, f;
разность ad - bd;
разность ad - cd;
разность bd - cd;
разность ae - be;
разность  ae - ce;
разность  be - ce;
разность af - bf;
разность af - cf;
разность  bf - cf.
все входные данные находятся в файле input.txt, который заполняется вручную. Файл находится в папке с программой.
выходные данные:
координаты фокусов ax, bx, cx, ay, by, cy. выходные  данные находятся в файле  output.txt, который заполняется в программе после произведенных вычислений. файл находится в папке с программой.
структура файла входных данных:
В файле перечисленны все входные данные как ряд вещественных чисел. файл состоит из шшести строк:
координаты точки d dx dy
координаты точки e ex euy
координаты точки f fx fy
(ad - bd )  (ad - cd)  (bd - cd)
(ae - be)   (ae - ce)   (be - ce)
(af - bf) (af - cf)  (bf - cf), где выражение в скобках - разность между векторами.
структура программы:
структура выходного файла:
файл состоит из 3 строк:
координаты точки a - ax ay
координаты точки b - bx , by
координаты точки c - cx cy

программа состоит из двух классов: 
calculations -  класс для математических вычислений;
- read_rite_files - класс  для чтения входных и формирования файла выходных данных.
Инструментарий написания программы:
программа написана на языке программирования C++. чтение и запись файлов производились с помощью стандартной библиотеки C++. математические вычисления производились с помощью библиотеки работы с матрицами - Eigen.
алгоритм работы программы:
1)  запуск функции start класса calculations из функции main;
2) внутри функции start класса calculations вызов функции read_settings  класса read_rite_files для чтения настроек;
3) если настройки не удалось прочитать? вызов функции errors класса read_rite_files для формирования и записи ошибки в выходной файл.
4) если настройки прочитаны? запуск функции  prepare класса calculation для формирования начальных данных:
- точности рассчета;
- шага градиентного спуска;
- начальных значений искомого вектора;
- формирование структур для хранения векторов;
- формирование целевой функции задачи.
5) запуск циклического алгоритма.
- запуск функции grad_ вычисления градиентов по всем переменным (ax ... cy) в точках начальных значений этих переменных.
6) запуск функции grad_morm для вычисления нормы градиента. 
7) если сумма модулей градиента <= точности  запуск функции stop остановки алгоритма.
8) если норма градиента > точности запуск функции xy_calculate для всех искомых координат по формуле: x1 = x0 -A*x0, где A-  шаг градиентного спуска. 
9) если сумма по модулю разности x1 -x0 координат всех искомых точек (ax, cy) < точности? вызов функции stop.
10) если сумма по модулю разности x1 -x0 координат всех искомых точек (ax, cy) >  точности?  вызыв функции change_x для подстановки новых приближенных значений: x1 = x0 для всех координат: (ax...cy).
11) вызов функции count подсчета количества итераций. если количество итераций > заданного n? вызов функции stop.
12) если количество итераций < заданного n, запись нового значения итераций.
13) если алгоритм остановлен? вызов функции write_answer класса read_rite_files для формирования выходного файла.
состав проекта:
- входной файл setings.txt;
- выходной файл  coords.txt;
- каталог Eigen с библиотекой;
- calculations.h объявление класса вычислений;
- calculations.cpp - исходнй код класса вычислений;
- CMakeLists - файл настроек проекта;
- Flowchart.png - блок-схема алгоритма;
- grad_defend.exe - файл запуска;
- main.cpp точка входа в программу;
- read_rite_files.h объявление класса работы с файлом;
read_rite_files.cpp - реализация класса работы с файлом.
Обозначения:
sqr(x) - квадратный корень числа x
x^y - число x возведенное в степень y.
Математическое описание задачи:
a_x - координата точки a по x
a_y - координата точки a по y
Для нахождения координат точек A, B, C необходимо решить систему уравнений
(ad -bd) +(sqr((d_y - b_y) ^2 + (d_x - b_x)^2)) -(sqr((d_y - a_y) ^2 + (d_x - a_x)^2)) =0
(ad -cd) +(sqr((d_y - c_y) ^2 + (d_x - c_x)^2)) -(sqr((d_y - a_y) ^2 + (d_x - a_x)^2)) =0
(bd -cd) +(sqr((d_y - c_y) ^2 + (d_x - c_x)^2)) -(sqr((d_y - b_y) ^2 + (d_x - b_x)^2)) =0
(ae -be) +(sqr((e_y - b_y) ^2 + (e_x - b_x)^2)) -(sqr((e_y - a_y) ^2 + (e_x - a_x)^2)) =0
(ae -ce) +(sqr((e_y - c_y) ^2 + (e_x - c_x)^2)) -(sqr((e_y - a_y) ^2 + (e_x - a_x)^2)) =0
(be -ce) +(sqr((e_y - c_y) ^2 + (e_x - c_x)^2)) -(sqr((e_y - b_y) ^2 + (e_x - b_x)^2)) =0
(af -bf) +(sqr((f_y - b_y) ^2 + (f_x - b_x)^2)) -(sqr((f_y - a_y) ^2 + (f_x - a_x)^2)) =0
(af -cf) +(sqr((f_y - c_y) ^2 + (f_x - c_x)^2)) -(sqr((f_y - a_y) ^2 + (f_x - a_x)^2)) =0
(bf -cf) +(sqr((f_y - c_y) ^2 + (f_x - c_x)^2)) -(sqr((f_y - b_y) ^2 + (f_x - b_x)^2)) =0
Далее необходимо найти градиент - частные производные по ay, ax, by, bx, cy, cx.
Для этого необходимо взять производную для каждого уравнения, входящего в систему уравнений, и сложить полученные производные (производная сууммы равна сумме производных)
Уравнение 1:
(ad -bd) +(sqr((d_y - b_y) ^2 + (d_x - b_x)^2)) -(sqr((d_y - a_y) ^2 + (d_x - a_x)^2)) =0
производная по b_y
-(d_y - b_y)/sqrt((d_y - b_y)^2)
производная по b_x
-(d_x - b_x)/sqrt((d_x - b_x)^2)
производная по a_y
(d_y - a_y)/sqrt((d_y - a_y)^2)
производная по a_x
(d_x - a_x)/sqrt((d_x - a_x)^2)
Уравнение 2:
(ad -cd) +(sqr((d_y - c_y) ^2 + (d_x - c_x)^2)) -(sqr((d_y - a_y) ^2 + (d_x - a_x)^2)) =0
производная по c_y
-(d_y - c_y)/sqrt((d_y - c_y)^2)
производная по c_x
-(d_x - c_x)/sqrt((d_x - c_x)^2)
производная по a_y
(d_y - a_y)/sqrt((d_y - a_y)^2)
производная по a_x
(d_x - a_x)/sqrt((d_x - a_x)^2)
Уравнение 3:
(bd -cd) +(sqr((d_y - c_y) ^2 + (d_x - c_x)^2)) -(sqr((d_y - b_y) ^2 + (d_x - b_x)^2)) =0
производная по c_y
-(d_y - c_y)/sqrt((d_y - c_y)^2)
производная по c_x
-(d_x - c_x)/sqrt((d_x - c_x)^2)
производная по b_y
(d_y - b_y)/sqrt((d_y - b_y)^2)
производная по b_x
(d_x - b_x)/sqrt((d_x - b_x)^2)
Уравнение 4:
(ae -be) +(sqr((e_y - b_y) ^2 + (e_x - b_x)^2)) -(sqr((e_y - a_y) ^2 + (e_x - a_x)^2)) =0
производная по b_y
-(e_y - b_y)/sqrt((e_y - b_y)^2)
производная по b_x
-(e_x - b_x)/sqrt((e_x - b_x)^2)
производная по a_y
(e_y - a_y)/sqrt((e_y - a_y)^2)
производная по a_x
(e_x - a_x)/sqrt((e_x - a_x)^2)
Уравнение 5:
(ae -ce) +(sqr((e_y - c_y) ^2 + (e_x - c_x)^2)) -(sqr((e_y - a_y) ^2 + (e_x - a_x)^2)) =0
производная по c_y
-(e_y - c_y)/sqrt((e_y - c_y)^2)
производная по c_x
-(e_x - c_x)/sqrt((e_x - c_x)^2)
производная по a_y
(e_y - a_y)/sqrt((e_y - a_y)^2)
производная по a_x
(e_x - a_x)/sqrt((e_x - a_x)^2)
Уравнение 6:
(be -ce) +(sqr((e_y - c_y) ^2 + (e_x - c_x)^2)) -(sqr((e_y - b_y) ^2 + (e_x - b_x)^2)) =0
производная по c_y
-(e_y - c_y)/sqrt((e_y - c_y)^2)
производная по c_x
-(e_x - c_x)/sqrt((e_x - c_x)^2)
производная по b_y
(e_y - b_y)/sqrt((e_y - b_y)^2)
производная по b_x
(e_x - b_x)/sqrt((e_x - b_x)^2)
Уравнение 7:
(af -bf) +(sqr((f_y - b_y) ^2 + (f_x - b_x)^2)) -(sqr((f_y - a_y) ^2 + (f_x - a_x)^2)) =0
производная по b_y
-(f_y - b_y)/sqrt((f_y - b_y)^2)
производная по b_x
-(f_x - b_x)/sqrt((f_x - b_x)^2)
производная по a_y
(f_y - a_y)/sqrt((f_y - a_y)^2)
производная по a_x
(f_x - a_x)/sqrt((f_x - a_x)^2)
Уравнение 8:
(af -cf) +(sqr((f_y - c_y) ^2 + (f_x - c_x)^2)) -(sqr((f_y - a_y) ^2 + (f_x - a_x)^2)) =0
производная по c_y
-(f_y - c_y)/sqrt((f_y - c_y)^2)
производная по c_x
-(f_x - c_x)/sqrt((f_x - c_x)^2)
производная по a_y
(f_y - a_y)/sqrt((f_y - a_y)^2)
производная по a_x
(f_x - a_x)/sqrt((f_x - a_x)^2)
Уравнение 9:
(bf -cf) +(sqr((f_y - c_y) ^2 + (f_x - c_x)^2)) -(sqr((f_y - b_y) ^2 + (f_x - b_x)^2)) =0
производная по c_y
-(f_y - c_y)/sqrt((f_y - c_y)^2)
производная по c_x
-(f_x - c_x)/sqrt((f_x - c_x)^2)
производная по b_y
(f_y - b_y)/sqrt((f_y - b_y)^2)
производная по b_x
(f_x - b_x)/sqrt((f_x - b_x)^2)
Примечание. Данная функция имеет подкоренное выражения. Функция корня не является непрерывной. Поэтому для данной задачи алгоритм градиентного спуска  не является оптимальным выбором и может выдавать сбой в некоторых  значениях. 
Для решения данной задачи лучше подойдет алгоритм Ньютона или алгоритм сопряженных градиентов.