#include "vector.h"

// デフォルトコンストラクタ
Vector::Vector(void) : size_(0), values_(nullptr), part_of_matrix_(false) {}

// 値とサイズを指定するコンストラクタ
Vector::Vector(double* values, int size) : values_(values), size_(size) { part_of_matrix_ = true; }

// サイズを指定するコンストラクタ
Vector::Vector(int n) try : size_(n), values_(new double[n]), part_of_matrix_(false) {
} catch (std::bad_alloc) {
    std::cerr << "Vector::Vector(int n) : Out of Memory" << std::endl;
    std::cerr << "n:" << n << std::endl;
    throw;
}

// サイズ、値、およびフラグを指定するコンストラクタ
Vector::Vector(int n, double value, const char* flag) try : size_(n), values_(new double[n]), part_of_matrix_(false) {
    if (strcmp(flag, "all") != 0) {
        std::cerr << "Unknown option: \"" << flag << "\"" << std::endl;
        throw;
    }
    for (int i = 0; i < size_; i++) {
        values_[i] = value;
    }
} catch (std::bad_alloc) {
    std::cerr << "Vector::Vector(int n, double value, const char* flag) : Out of Memory" << std::endl;
    std::cerr << "n:" << n << std::endl;
    throw;
}

// コピーコンストラクタ
Vector::Vector(const Vector& arg) try : size_(arg.size()), values_(new double[arg.size()]), part_of_matrix_(false) {
    int arg_size = arg.size();
    for (int i = 0; i < arg_size; i++) {
        values_[i] = arg.values_[i];
    }
} catch (std::bad_alloc) {
    std::cerr << "Vector::Vector(const Vector& arg) : Out of Memory" << std::endl;
    throw;
}

// 代入演算子
Vector& Vector::operator=(const Vector& rhs) {
    if (this != &rhs) {
        int rhs_size = rhs.size();
        if (size_ != rhs_size) {
            size_ = rhs_size;
            delete[] values_;
            try {
                values_ = new double[size_];
            } catch (std::bad_alloc) {
                std::cerr << "Vector::operator=: Out of Memory" << std::endl;
                throw;
            }
        }
        for (int i = 0; i < size_; i++) {
            values_[i] = rhs.values_[i];
        }
    }
    return *this;
}

// デストラクタ
Vector::~Vector(void) {
    if (part_of_matrix_ == false) delete[] values_;
}

// サイズを取得するメソッド
int Vector::size(void) const { return size_; }

// インデックスで要素にアクセスするためのconstメソッド
double Vector::operator[](int index) const { return values_[index]; }

// インデックスで要素にアクセスするための非constメソッド
double& Vector::operator[](int index) { return values_[index]; }

// 加算代入演算子
Vector& Vector::operator+=(const Vector& rhs) {
    if (size_ != rhs.size()) {
        std::cerr << "Vector::operator+=: Size Unmatched" << std::endl;
        exit(1);
    }
    int size = size_;
    for (int i = 0; i < size; i++) {
        values_[i] += rhs[i];
    }
    return *this;
}

// 減算代入演算子
Vector& Vector::operator-=(const Vector& rhs) {
    if (size_ != rhs.size()) {
        std::cerr << "Vector::operator-=: Size Unmatched!" << std::endl;
        exit(1);
    }
    int size = size_;
    for (int i = 0; i < size; i++) {
        values_[i] -= rhs[i];
    }
    return *this;
}

// ベクトルを出力するメソッド
std::ostream& Vector::print(std::ostream& lhs) const {
    lhs << "(";
    int size = size_;
    for (int i = 0; i < size; i++) {
        lhs << values_[i] << ", ";
    }
    lhs << ")";
    return lhs;
}

// 単項プラス演算子
Vector Vector::operator+(void) const { return *this; }

// 単項マイナス演算子
Vector Vector::operator-(void) const {
    Vector result(*this);
    int size = size_;
    for (int i = 0; i < size; i++) {
        result[i] = -result[i];
    }
    return result;
}

// データへのポインタを取得するメソッド
double* Vector::get_values() { return values_; }

// ベクトルの出力演算子
std::ostream& operator<<(std::ostream& lhs, const Vector& rhs) { return rhs.print(lhs); }

// ベクトルの加算演算子
Vector operator+(const Vector& lhs, const Vector& rhs) {
    Vector result(lhs);
    return result += rhs;
}

// ベクトルの減算演算子
Vector operator-(const Vector& lhs, const Vector& rhs) {
    Vector result(lhs);
    return result -= rhs;
}

// ベクトルの内積演算子
double operator*(const Vector& lhs, const Vector& rhs) {
    if (lhs.size() != rhs.size()) {
        std::cerr << "operator*(const Vector &, const Vector &): Size Unmatched" << std::endl;
        exit(1);
    }
    double result = 0.0;
    int size = lhs.size();
    for (int i = 0; i < size; i++) {
        result += lhs[i] * rhs[i];
    }
    return result;
}

// ベクトルとスカラーの乗算演算子
Vector operator*(double lhs, const Vector& rhs) {
    Vector result(rhs);
    int size = result.size();
    for (int i = 0; i < size; i++) {
        result[i] = lhs * rhs[i];
    }
    return result;
}

// ベクトルとスカラーの除算演算子
Vector operator/(const Vector& lhs, double rhs) {
    Vector result(lhs);
    int size = result.size();
    for (int i = 0; i < size; i++) {
        result[i] = lhs[i] / rhs;
    }
    return result;
}

// ベクトルの等価比較演算子
bool operator==(const Vector& lhs, const Vector& rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }
    int size = lhs.size();
    for (int i = 0; i < size; i++) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}

// ベクトルの不等価比較演算子
bool operator!=(const Vector& lhs, const Vector& rhs) {
    if (lhs.size() != rhs.size()) {
        return true;
    }
    int size = lhs.size();
    for (int i = 0; i < size; i++) {
        if (lhs[i] != rhs[i]) {
            return true;
        }
    }
    return false;
}

// ベクトルの平方和を計算する関数
double squared_sum(const Vector& arg) {
    double result = 0.0;
    int size = arg.size();
    for (int i = 0; i < size; i++) {
        result += arg[i] * arg[i];
    }
    return result;
}

// ベクトルのpノルムを計算する関数
double norm(const Vector& arg, int p) {
    double result = 0.0;
    int size = arg.size();
    for (int i = 0; i < size; i++) {
        result += pow(fabs(arg[i]), p);
    }
    result = pow(result, 1.0 / (double)p);
    return result;
}

// ベクトルの最大ノルムを計算する関数
double max_norm(const Vector& arg) {
    double result = fabs(arg[0]);
    int size = arg.size();
    for (int i = 1; i < size; i++) {
        double tmp = fabs(arg[i]);
        if (result < tmp) {
            result = tmp;
        }
    }
    return result;
}

// ベクトルの平方ノルムを計算する関数
double squared_norm(const Vector& arg) {
    return sqrt(squared_sum(arg));
}
