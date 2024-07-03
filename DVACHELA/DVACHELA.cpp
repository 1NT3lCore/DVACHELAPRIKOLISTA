#include <iostream>
#include "convex_hull.h"
#include <windows.h>
using namespace std;

int main() {
    SetConsoleOutputCP(1251);
    vector<point2d> points;
    bool dataEntered = false;

    enterData(points, dataEntered);

    Result graham_result, jarvis_result;
    algorithm_selection(points);

    string decision;
    do {
        cout << "\nХотите ли вы завершить программу (exit), начать заново (restart) или выбрать другой алгоритм (change)? Введите ваш выбор: ";
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
            cout << "\n\nНеверный ввод. Пожалуйста, введите 'exit', 'restart' или 'change'.\n";
        }
    } while (decision != "exit");

    return 0;
}