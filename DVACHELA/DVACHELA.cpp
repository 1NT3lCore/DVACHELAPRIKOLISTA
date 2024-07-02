#include <iostream>
#include "convex_hull.h"
#include <windows.h>
using namespace std;

int main() {
    SetConsoleOutputCP(1251);
    vector<point2d> points;
    string input;
    int n;
    bool dataEntered = false;

    while (!dataEntered) {
        cout << "Выберите способ ввода данных:\n";
        cout << "1. Ввести данные с клавиатуры\n";
        cout << "2. Прочитать данные из файла\n";
        cout << "Введите ваш выбор (1 или 2), exit - закончить выполнение программы, restart - перезапуск программы: ";
        getline(cin, input);
        check_for_exit(input);

        if (input == "1") {
            do {
                std::cout << "Введите количество точек (должно быть натуральным числом и >= 3), exit - закончить выполнение программы, restart - перезапуск программы: ";
                std::getline(std::cin, input);
                check_for_exit(input); // Проверка на команду 'exit'

                if (is_int(input)) {
                    n = std::stoi(input);
                    if (n >= 3) {
                        points.resize(n); // Изменяем размер вектора, объявленного в начале функции main
                        break; // Выход из цикла, если введено корректное значение
                    }
                }

                std::cout << "Неверный ввод. Пожалуйста, введите корректное натуральное число >= 3.\n";
            } while (true);

            for (int i = 0; i < n; ++i) {
                do {
                    std::cout << "Введите точку " << i + 1 << " (x,y), exit - закончить выполнение программы, restart - перезапуск программы: ";
                    std::getline(std::cin, input);
                    check_for_exit(input);
                    std::stringstream ss(input);
                    std::string x_str, y_str;
                    std::getline(ss, x_str, ',');
                    std::getline(ss, y_str, ',');


                    if (is_int(x_str) && is_int(y_str)) {
                        points[i].x = std::stoi(x_str);
                        points[i].y = std::stoi(y_str);
                        break; // Выход из цикла, если введены корректные значения
                    }

                    std::cout << "Неверный ввод, пожалуйста, введите действительные целые числа, разделенные запятой.\n";
                } while (true);
            }
            dataEntered = true;
        }
        else if (input == "2") {
            cout << "Введите имя файла: ";
            getline(cin, input);
            check_for_exit(input);

            ifstream file(input);
            if (file.is_open()) {
                file.seekg(0, ios::end);
                if (file.tellg() == 0) {
                    cout << "Файл пуст.\n";
                }
                else {
                    file.seekg(0, ios::beg);
                    string line;
                    while (getline(file, line)) { // Считываем строку из файла
                        stringstream ss(line);
                        string x_str, y_str;
                        getline(ss, x_str, ',');
                        getline(ss, y_str, ',');

                        if (is_int(x_str) && is_int(y_str)) {
                            long long x = stoll(x_str);
                            long long y = stoll(y_str);
                            points.emplace_back(x, y); // Добавляем точку в вектор
                        }
                        else {
                            cout << "Некорректные данные в строке: " << line << '\n';
                        }
                    }
                    if (!points.empty()) {
                        dataEntered = true;
                    }
                    else {
                        cout << "Все строки файла содержат некорректные данные.\n";
                    }
                }
                file.close();
            }
            else {
                cout << "Не удалось открыть файл. Пожалуйста, проверьте имя файла и путь к нему.\n";
            }
        }
    }
    Result graham_result, jarvis_result;
    algorithm_selection(points);

    string decision;
    do {
        cout << "Хотите ли вы завершить программу (exit), начать заново (restart) или выбрать другой алгоритм (change)? Введите ваш выбор: ";
        getline(cin, decision);

        if (decision == "exit") {
            check_for_exit(decision); // Выход из программы
        }
        else if (decision == "restart") {
            check_for_exit(decision);
            points.clear();
            // Здесь должен быть код для повторного ввода точек
        }
        else if (decision == "change") {
            algorithm_selection(points); // Вызов функции выбора алгоритма
        }
        else {
            cout << "Неверный ввод. Пожалуйста, введите 'exit', 'restart' или 'change'.\n";
        }
    } while (decision != "exit");

    return 0;
}