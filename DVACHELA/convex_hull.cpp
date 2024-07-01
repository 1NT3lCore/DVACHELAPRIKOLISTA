#include "convex_hull.h"
using namespace std;
using namespace std::chrono;
long long cross(vector2d a, vector2d b) {
    return a.x * b.y - b.x * a.y;
}

Result graham_scan(vector<point2d> points) {
    auto start = high_resolution_clock::now();
    int iterations = 0;
    /* F */
    for (int i = 1; i < points.size(); ++i) {
        if (points[i].x < points[0].x || (points[i].x == points[0].x && points[i].y < points[0].y)) {
            swap(points[0], points[i]);
        }
    }

    sort(
        points.begin() + 1,
        points.end(),
        [&points, &iterations](point2d a, point2d b) {
            iterations++;
            return cross(
                vector2d(points[0], a),
                vector2d(points[0], b)
            ) > 0;
        }
    );

    /* S */
    vector<point2d> ch = { points[0], points[1] };

    for (int i = 2; i < points.size(); ++i) {
        while (ch.size() >= 2 && cross(
            vector2d(ch[ch.size() - 2], ch[ch.size() - 1]),
            vector2d(ch[ch.size() - 1], points[i])
        ) < 0)
        {
            ch.pop_back();
            iterations++;
        }
        ch.push_back(points[i]);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    return { ch, duration.count(), iterations };
}

Result jarvis_march(vector<point2d> points) {
    auto start = high_resolution_clock::now();
    int iterations = 0;
    for (int i = 1; i < points.size(); ++i) {
        if (points[i].x < points[0].x || (points[i].x == points[0].x && points[i].y < points[0].y)) {
            swap(points[0], points[i]);
        }
    }

    /* Jarvis Scan algorithm */
    vector<point2d> ch;
    int cur = 0;

    do {
        ch.push_back(points[cur]);
        int next = (cur + 1) % points.size();

        for (int i = 0; i < points.size(); ++i) {
            if (cross(vector2d(points[cur], points[i]), vector2d(points[cur], points[next])) > 0) {
                next = i;
            }
            iterations++; // Увеличиваем счетчик итераций
        }

        cur = next;
    } while (cur != 0); // Пока не вернемся к начальной точке
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    return { ch, duration.count(), iterations };
}

bool is_int(const string& s) {
    if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
        return false;
    char* p;
    strtol(s.c_str(), &p, 10);
    return (*p == 0);
}

void restart_program() {
    // Получаем путь к исполняемому файлу программы
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);

    // Создаем структуру STARTUPINFO
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Запускаем новый экземпляр программы
    if (!CreateProcessA(
        exePath,   // Путь к исполняемому файлу
        NULL,      // Командная строка
        NULL,      // Атрибуты защиты процесса
        NULL,      // Атрибуты защиты потока
        FALSE,     // Наследование дескрипторов
        0,         // Флаги создания
        NULL,      // Новый блок окружения
        NULL,      // Текущий каталог
        &si,       // Структура STARTUPINFO
        &pi        // Структура PROCESS_INFORMATION
    )) {
        std::cerr << "Не удалось перезапустить программу." << std::endl;
    }
    else {
        // Закрываем дескрипторы процесса и потока
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    // Завершаем текущий экземпляр программы
    exit(0);
}

void check_for_exit(const string& input) {
    if (input == "exit") {
        std::cout << "Программа прервана пользователем." << std::endl;
        exit(0); // Завершение программы
    }
    else if (input == "restart") {
        std::cout << "Перезапуск программы..." << std::endl;
        restart_program(); // Вызов функции перезапуска
    }
}

void algorithm_selection(vector<point2d>& points) {
    std::string input;
    Result graham_result, jarvis_result;
    int choice;
    do {
        std::cout << "Выберите алгоритм для вычисления выпуклой оболочки:\n";
        std::cout << "1. Сканирование Грэхема\n";
        std::cout << "2. Марш Джарвиса\n";
        std::cout << "3. Сравнить оба алгоритма\n";
        std::cout << "Введите ваш выбор (1, 2 или 3): ";
        std::getline(std::cin, input);
        check_for_exit(input);

        if (is_int(input)) {
            choice = std::stoi(input);
            if (choice >= 1 && choice <= 3) break; // Выход из цикла, если введено корректное значение
        }

        std::cout << "Неверный выбор. Пожалуйста, введите 1, 2 или 3.\n";
    } while (true);

    // Выполнение выбранного алгоритма или сравнение
    switch (choice) {
    case 1:
        graham_result = graham_scan(points);
        // Вывод точек выпуклой оболочки для Грэхема
        cout << "Точки выпуклой оболочки Грэхема:\n";
        for (const auto& pt : graham_result.ch) {
            cout << pt.x << ' ' << pt.y << '\n';
        }
        break;
    case 2:
        jarvis_result = jarvis_march(points);
        // Вывод точек выпуклой оболочки для Джарвиса
        cout << "Точки выпуклой оболочки Джарвиса:\n";
        for (const auto& pt : jarvis_result.ch) {
            cout << pt.x << ' ' << pt.y << '\n';
        }
        break;
    case 3:
        graham_result = graham_scan(points);
        jarvis_result = jarvis_march(points);
        // Вывод результатов сравнения
        cout << "Сравнение алгоритмов:\n";
        cout << "Время выполнения Грэхема: " << graham_result.time << " микросекунд\n";
        cout << "Время выполнения Джарвиса: " << jarvis_result.time << " микросекунд\n";
        cout << "Количество итераций Грэхема: " << graham_result.iterations << "\n";
        cout << "Количество итераций Джарвиса: " << jarvis_result.iterations << "\n";
        // Определение более быстрого алгоритма
        if (graham_result.time < jarvis_result.time) {
            cout << "Алгоритм Грэхема быстрее.\n";
        }
        else {
            cout << "Алгоритм Джарвиса быстрее.\n";
        }
        if (graham_result.iterations < jarvis_result.iterations) {
            cout << "Алгоритм Грэхема выполняется за меньшее количество итераций.\n";
        }
        else {
            cout << "Алгоритм Джарвиса выполняется за меньшее количество итераций\n";
        }
        // Вывод точек выпуклой оболочки для сравнения
        cout << "Точки выпуклой оболочки Грэхема:\n";
        for (const auto& pt : graham_result.ch) {
            cout << pt.x << ' ' << pt.y << '\n';
        }
        cout << "Точки выпуклой оболочки Джарвиса:\n";
        for (const auto& pt : jarvis_result.ch) {
            cout << pt.x << ' ' << pt.y << '\n';
        }
        break;
    }
}