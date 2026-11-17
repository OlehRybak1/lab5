#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

constexpr double MAX_SIDE = 1000000.0;

class Triangle {
protected:
    double a;
    double b;
    double angle;

    Triangle(double sideA, double sideB, double angleDegrees)
        : a(sideA), b(sideB), angle(angleDegrees) {
        if (!isfinite(a) || !isfinite(b) ||
            !isfinite(angle) || a <= 0 || b <= 0 ||
            a >= MAX_SIDE || b >= MAX_SIDE ||
            angle <= 0 || angle >= 180) {
            throw invalid_argument("Некоректні параметри трикутника.");
        }
    }

public:
    virtual ~Triangle() = default;
    virtual double area() const = 0;
    virtual const char* name() const = 0;

    double getSideA() const { return a; }
    double getSideB() const { return b; }
    double getAngle() const { return angle; }
};

class RightTriangle : public Triangle {
public:
    RightTriangle(double legA, double legB)
        : Triangle(legA, legB, 90.0) {
    }

    double area() const override {
        return a * b / 2.0;
    }

    const char* name() const override {
        return "Прямокутний";
    }
};

class IsoscelesTriangle : public Triangle {
public:
    IsoscelesTriangle(double equalSide, double apexAngle)
        : Triangle(equalSide, equalSide, apexAngle) {
    }

    double area() const override {
        const double pi = acos(-1.0);
        const double radians = angle * pi / 180.0;
        return a * b * sin(radians) / 2.0;
    }

    const char* name() const override {
        return "Рівнобедрений";
    }
};

class EquilateralTriangle : public Triangle {
public:
    explicit EquilateralTriangle(double side)
        : Triangle(side, side, 60.0) {
    }

    double area() const override {
        return sqrt(3.0) * a * a / 4.0;
    }

    const char* name() const override {
        return "Рівносторонній";
    }
};

double readNumber(const string& prompt, double upperBound) {
    string line;

    while (true) {
        cout << prompt;
        if (!getline(cin, line)) {
            throw runtime_error("Введення перервано.");
        }

        istringstream input(line);
        double value = 0.0;
        char extra = '\0';

        if ((input >> value) && !(input >> extra) &&
            isfinite(value) && value > 0 && value < upperBound) {
            return value;
        }
        cout << "Помилка! Введіть додатне число (менше за " << upperBound << ").\n";
    }
}

int readChoice(const string& prompt) {
    string line;

    while (true) {
        cout << prompt;
        if (!getline(cin, line)) {
            throw runtime_error("Введення перервано.");
        }

        istringstream input(line);
        int choice = 0;
        char extra = '\0';

        if ((input >> choice) && !(input >> extra) && (choice >= 0 && choice <= 3)) {
            return choice;
        }
        cout << "Помилка! Оберіть 1, 2, 3 або 0 для завершення.\n";
    }
}

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    vector<Triangle*> shapes;

    try {
        cout << "=== СТВОРЕННЯ ТРИКУТНИКІВ (ДОВІЛЬНА КІЛЬКІСТЬ) ===\n";
        cout << "1 - Прямокутний трикутник\n";
        cout << "2 - Рівнобедрений трикутник\n";
        cout << "3 - Рівносторонній трикутник\n";
        cout << "0 - Завершити введення та порахувати площі\n\n";

        while (true) {
            cout << "--- Фігура №" << (shapes.size() + 1) << " ---\n";
            int type = readChoice("Оберіть дію (1-3 додати, 0 вивести): ");

            if (type == 0) {
                break;
            }

            if (type == 1) {
                double legA = readNumber("  Введіть перший катет: ", MAX_SIDE);
                double legB = readNumber("  Введіть другий катет: ", MAX_SIDE);
                shapes.push_back(new RightTriangle(legA, legB));
            }
            else if (type == 2) {
                double isoSide = readNumber("  Введіть бічну сторону: ", MAX_SIDE);
                double apexAngle = readNumber("  Введіть кут при вершині (1..179): ", 180.0);
                shapes.push_back(new IsoscelesTriangle(isoSide, apexAngle));
            }
            else if (type == 3) {
                double eqSide = readNumber("  Введіть сторону: ", MAX_SIDE);
                shapes.push_back(new EquilateralTriangle(eqSide));
            }
            cout << "\n";
        }

        cout << fixed << setprecision(2);
        cout << "\n=== РЕЗУЛЬТАТИ ОБЧИСЛЕНЬ ===\n\n";

        if (shapes.empty()) {
            cout << "Жодного трикутника не створено.\n";
        }
        else {
            for (size_t i = 0; i < shapes.size(); ++i) {
                cout << (i + 1) << ". Тип: " << shapes[i]->name()
                    << " | Сторона A = " << shapes[i]->getSideA()
                    << ", Сторона B = " << shapes[i]->getSideB()
                    << ", Кут = " << shapes[i]->getAngle() << "°"
                    << " | Площа = " << shapes[i]->area() << "\n";
            }
        }
    }
    catch (const exception& e) {
        cerr << "\nПомилка: " << e.what() << "\n";
    }

    // Очищення всієї динамічної пам'яті
    for (size_t i = 0; i < shapes.size(); ++i) {
        delete shapes[i];
    }

    return 0;
}