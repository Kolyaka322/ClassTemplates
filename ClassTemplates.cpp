#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <type_traits>

template <typename T>
class Matrix {
private:
    size_t rows;
    size_t cols;
    T** data;

    void allocate() {
        if (rows == 0 || cols == 0) {
            rows = 0;
            cols = 0;
            data = nullptr;
            return;
        }
        data = new T * [rows];
        for (size_t i = 0; i < rows; ++i) {
            data[i] = new T[cols]{};
        }
    }

    void deallocate() {
        if (data != nullptr) {
            for (size_t i = 0; i < rows; ++i) {
                delete[] data[i];
            }
            delete[] data;
            data = nullptr;
        }
        rows = 0;
        cols = 0;
    }

    void checkSameDimensions(const Matrix<T>& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Розміри матриць не збігаються");
        }
    }

public:
    Matrix(size_t r = 0, size_t c = 0) : rows(r), cols(c), data(nullptr) {
        allocate();
    }

    Matrix(const Matrix<T>& other) : rows(other.rows), cols(other.cols), data(nullptr) {
        allocate();
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                data[i][j] = other.data[i][j];
            }
        }
    }

    Matrix<T>& operator=(const Matrix<T>& other) {
        if (this != &other) {
            deallocate();
            rows = other.rows;
            cols = other.cols;
            allocate();
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    data[i][j] = other.data[i][j];
                }
            }
        }
        return *this;
    }

    ~Matrix() {
        deallocate();
    }

    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }

    void FillFromKeyboard() {
        std::cout << "Введіть елементи матриці (" << rows << "x" << cols << "):\n";
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                std::cout << "[" << i << "][" << j << "] = ";
                std::cin >> data[i][j];
            }
        }
    }

    void FillRandom(int minVal = 1, int maxVal = 10) {
        if (minVal > maxVal) {
            int tmp = minVal;
            minVal = maxVal;
            maxVal = tmp;
        }
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                int r = minVal + std::rand() % (maxVal - minVal + 1);
                data[i][j] = static_cast<T>(r);
            }
        }
    }

    void Print() const {
        if (data == nullptr || rows == 0 || cols == 0) {
            std::cout << "[Порожня матриця]\n";
            return;
        }
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                std::cout << std::setw(8) << data[i][j];
            }
            std::cout << "\n";
        }
    }

    T Max() const {
        if (data == nullptr || rows == 0 || cols == 0) {
            throw std::runtime_error("Матриця порожня");
        }
        T maxVal = data[0][0];
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                if (data[i][j] > maxVal) {
                    maxVal = data[i][j];
                }
            }
        }
        return maxVal;
    }

    T Min() const {
        if (data == nullptr || rows == 0 || cols == 0) {
            throw std::runtime_error("Матриця порожня");
        }
        T minVal = data[0][0];
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                if (data[i][j] < minVal) {
                    minVal = data[i][j];
                }
            }
        }
        return minVal;
    }

    Matrix<T> operator+(const Matrix<T>& other) const {
        checkSameDimensions(other);
        Matrix<T> result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result.data[i][j] = data[i][j] + other.data[i][j];
            }
        }
        return result;
    }

    Matrix<T> operator-(const Matrix<T>& other) const {
        checkSameDimensions(other);
        Matrix<T> result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result.data[i][j] = data[i][j] - other.data[i][j];
            }
        }
        return result;
    }

    Matrix<T> operator*(const Matrix<T>& other) const {
        if (cols != other.rows) {
            throw std::invalid_argument("Кількість стовпців першої матриці має дорівнювати кількості рядків другої");
        }
        Matrix<T> result(rows, other.cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < other.cols; ++j) {
                T sum{};
                for (size_t k = 0; k < cols; ++k) {
                    sum += data[i][k] * other.data[k][j];
                }
                result.data[i][j] = sum;
            }
        }
        return result;
    }

    Matrix<T> operator/(const Matrix<T>& other) const {
        checkSameDimensions(other);
        Matrix<T> result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                if (other.data[i][j] == T{}) {
                    throw std::domain_error("Ділення на нуль");
                }
                result.data[i][j] = data[i][j] / other.data[i][j];
            }
        }
        return result;
    }
};

int main() {
    system("chcp 1251>null");
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    try {
        Matrix<double> A(2, 2);
        Matrix<double> B(2, 2);

        A.FillRandom(1, 10);
        B.FillRandom(1, 10);

        std::cout << "Матриця A:\n";
        A.Print();

        std::cout << "Матриця B:\n";
        B.Print();

        std::cout << "Max A: " << A.Max() << ", Min A: " << A.Min() << "\n";
        std::cout << "Max B: " << B.Max() << ", Min B: " << B.Min() << "\n";

        Matrix<double> sum = A + B;
        std::cout << "A + B:\n";
        sum.Print();

        Matrix<double> diff = A - B;
        std::cout << "A - B:\n";
        diff.Print();

        Matrix<double> prod = A * B;
        std::cout << "A * B:\n";
        prod.Print();

        Matrix<double> quot = A / B;
        std::cout << "A / B:\n";
        quot.Print();

        Matrix<int> C(2, 2);
        C.FillFromKeyboard();
        std::cout << "Матриця C:\n";
        C.Print();
    }
    catch (const std::exception& ex) {
        std::cerr << "Помилка: " << ex.what() << "\n";
    }

    return 0;
}
