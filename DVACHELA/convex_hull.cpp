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
            iterations++; // ����������� ������� ��������
        }

        cur = next;
    } while (cur != 0); // ���� �� �������� � ��������� �����
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
    // �������� ���� � ������������ ����� ���������
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);

    // ������� ��������� STARTUPINFO
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // ��������� ����� ��������� ���������
    if (!CreateProcessA(
        exePath,   // ���� � ������������ �����
        NULL,      // ��������� ������
        NULL,      // �������� ������ ��������
        NULL,      // �������� ������ ������
        FALSE,     // ������������ ������������
        0,         // ����� ��������
        NULL,      // ����� ���� ���������
        NULL,      // ������� �������
        &si,       // ��������� STARTUPINFO
        &pi        // ��������� PROCESS_INFORMATION
    )) {
        std::cerr << "\n�� ������� ������������� ���������.\n" << std::endl;
    }
    else {
        // ��������� ����������� �������� � ������
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    // ��������� ������� ��������� ���������
    exit(0);
}

void check_for_exit(const string& input) {
    if (input == "exit") {
        std::cout << "\n��������� �������� �������������.\n" << std::endl;
        exit(0); // ���������� ���������
    }
    else if (input == "restart") {
        std::cout << "\n���������� ���������...\n" << std::endl;
        restart_program(); // ����� ������� �����������
    }
}

void algorithm_selection(vector<point2d>& points) {
    std::string input;
    Result graham_result, jarvis_result;
    int choice;
    do {
        std::cout << "\n�������� �������� ��� ���������� �������� ��������:\n";
        std::cout << "1. ������������ �������\n";
        std::cout << "2. ���� ��������\n";
        std::cout << "3. �������� ��� ���������\n";
        std::cout << "������� ��� ����� (1, 2 ��� 3): ";
        std::getline(std::cin, input);
        check_for_exit(input);

        if (is_int(input)) {
            choice = std::stoi(input);
            if (choice >= 1 && choice <= 3) break; // ����� �� �����, ���� ������� ���������� ��������
        }

        std::cout << "\n�������� �����. ����������, ������� 1, 2 ��� 3.\n";
    } while (true);

    // ���������� ���������� ��������� ��� ���������
    switch (choice) {
    case 1:
        graham_result = graham_scan(points);
        // ����� ����� �������� �������� ��� �������
        cout << "\n����� �������� �������� �������:\n";
        for (const auto& pt : graham_result.ch) {
            cout << pt.x << ' ' << pt.y << '\n';
        }
        break;
    case 2:
        jarvis_result = jarvis_march(points);
        // ����� ����� �������� �������� ��� ��������
        cout << "\n����� �������� �������� ��������:\n";
        for (const auto& pt : jarvis_result.ch) {
            cout << pt.x << ' ' << pt.y << '\n';
        }
        break;
    case 3:
        graham_result = graham_scan(points);
        jarvis_result = jarvis_march(points);
        // ����� ����������� ���������
        cout << "\n��������� ����������:\n";
        cout << "����� ���������� �������: " << graham_result.time << " �����������\n";
        cout << "����� ���������� ��������: " << jarvis_result.time << " �����������\n";
        cout << "���������� �������� �������: " << graham_result.iterations << "\n";
        cout << "���������� �������� ��������: " << jarvis_result.iterations << "\n\n";
        // ����������� ����� �������� ���������
        if (graham_result.time < jarvis_result.time) {
            cout << "�������� ������� �������.\n";
        }
        else {
            cout << "�������� �������� �������.\n";
        }
        if (graham_result.iterations < jarvis_result.iterations) {
            cout << "�������� ������� ����������� �� ������� ���������� ��������.\n";
        }
        else {
            cout << "�������� �������� ����������� �� ������� ���������� ��������\n";
        }
        // ����� ����� �������� �������� ��� ���������
        cout << "\n����� �������� �������� �������:\n";
        for (const auto& pt : graham_result.ch) {
            cout << pt.x << ' ' << pt.y << '\n';
        }
        cout << "\n����� �������� �������� ��������:\n";
        for (const auto& pt : jarvis_result.ch) {
            cout << pt.x << ' ' << pt.y << '\n';
        }
        break;
    }
}

void enterData(vector<point2d>& points, bool& dataEntered) {
    string input;
    int n;
    while (!dataEntered) {
        cout << "�������� ������ ����� ������:\n";
        cout << "1. ������ ������ � ����������\n";
        cout << "2. ��������� ������ �� �����\n";
        cout << "������� ��� ����� (1 ��� 2), exit - ��������� ���������� ���������, restart - ���������� ���������: ";
        getline(cin, input);
        check_for_exit(input);

        if (input == "1") {
            do {
                std::cout << "\n������� ���������� ����� (������ ���� ����������� ������ � >= 3), exit - ��������� ���������� ���������, restart - ���������� ���������: ";
                std::getline(std::cin, input);
                check_for_exit(input); // �������� �� ������� 'exit'

                if (is_int(input)) {
                    n = std::stoi(input);
                    if (n >= 3) {
                        points.resize(n); // �������� ������ �������, ������������ � ������ ������� main
                        break; // ����� �� �����, ���� ������� ���������� ��������
                    }
                }

                std::cout << "\n�������� ����. ����������, ������� ���������� ����������� ����� >= 3.\n";
            } while (true);

            for (int i = 0; i < n; ++i) {
                do {
                    std::cout << "\n������� ����� " << i + 1 << " (x,y), exit - ��������� ���������� ���������, restart - ���������� ���������: ";
                    std::getline(std::cin, input);
                    check_for_exit(input);
                    std::stringstream ss(input);
                    std::string x_str, y_str;
                    std::getline(ss, x_str, ',');
                    std::getline(ss, y_str, ',');


                    if (is_int(x_str) && is_int(y_str)) {
                        points[i].x = std::stoi(x_str);
                        points[i].y = std::stoi(y_str);
                        break; // ����� �� �����, ���� ������� ���������� ��������
                    }

                    std::cout << "\n\n�������� ����, ����������, ������� �������������� ����� �����, ����������� �������.\n";
                } while (true);
            }
            dataEntered = true;
        }
        else if (input == "2") {
            cout << "\n������� ��� �����: ";
            getline(cin, input);
            check_for_exit(input);

            ifstream file(input);
            if (file.is_open()) {
                file.seekg(0, ios::end);
                if (file.tellg() == 0) {
                    cout << "\n\n���� ����.\n";
                }
                else {
                    file.seekg(0, ios::beg);
                    string line;
                    while (getline(file, line)) { // ��������� ������ �� �����
                        stringstream ss(line);
                        string x_str, y_str;
                        getline(ss, x_str, ',');
                        getline(ss, y_str, ',');

                        if (is_int(x_str) && is_int(y_str)) {
                            long long x = stoll(x_str);
                            long long y = stoll(y_str);
                            points.emplace_back(x, y); // ��������� ����� � ������
                        }
                        else {
                            cout << "\n\n������������ ������ � ������: " << line << '\n';
                        }
                    }
                    if (!points.empty()) {
                        dataEntered = true;
                    }
                    else {
                        cout << "\n\n��� ������ ����� �������� ������������ ������.\n";
                    }
                }
                file.close();
            }
            else {
                cout << "\n\n�� ������� ������� ����. ����������, ��������� ��� ����� � ���� � ����.\n";
            }
        }
        else {
            cout << "\n\n�������� �����. \n";
        }
    }
}