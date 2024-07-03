#include <vector>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <chrono>
#include <string>
#include <sstream>
#include <cstdlib> 
#include <windows.h>
using namespace std;
using namespace std::chrono;

struct point2d {
    long long x, y;

    // Конструктор по умолчанию
    point2d() : x(0), y(0) {}

    // Конструктор с двумя параметрами
    point2d(long long x_val, long long y_val) : x(x_val), y(y_val) {}
};

struct Result {
    vector<point2d> ch;
    long long time;
    int iterations;
};

struct vector2d {
    long long x, y;
    vector2d(point2d a, point2d b) {
        x = b.x - a.x;
        y = b.y - a.y;
    }
};

long long cross(vector2d a, vector2d b);

Result graham_scan(vector<point2d> points);

Result jarvis_march(vector<point2d> points);

bool is_int(const string& s);

void check_for_exit(const string& input);

void restart_program();

void algorithm_selection(vector<point2d>& points);


void read_points_from_file(vector<point2d>& points);

void display_menu();

void read_points_from_keyboard(vector<point2d>& points);

void enterData(vector<point2d>& points, bool& dataEntered);