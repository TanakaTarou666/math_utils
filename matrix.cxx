#include "matrix.h"

// コンストラクタ（行数と列数を指定）
Matrix::Matrix(int rows, int cols) : rows_(rows), cols_(cols), values_(new double[rows * cols]) {}

// コンストラクタ（行数、列数、および初期値を指定）
Matrix::Matrix(int rows, int cols, double arg) : rows_(rows), cols_(cols), values_(new double[rows * cols]) {
    for (int i = 0; i < rows * cols; i++) {
        values_[i] = arg;
    }
}

// デフォルトコンストラクタ
Matrix::Matrix(void) : rows_(0), cols_(0), values_(NULL) {}

// コピーコンストラクタ
Matrix::Matrix(const Matrix& other) : rows_(other.rows_), cols_(other.cols_), values_(new double[other.rows_ * other.cols_]) {
    for (int i = 0; i < rows_ * cols_; i++) {
        values_[i] = other.values_[i];
    }
}

// デストラクタ
Matrix::~Matrix() { delete[] values_; }

// 代入演算子
Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            delete[] values_;
            rows_ = other.rows_;
            cols_ = other.cols_;
            values_ = new double[rows_ * cols_];
        }
        for (int i = 0; i < rows_ * cols_; i++) {
            values_[i] = other.values_[i];
        }
    }
    return *this;
}

// 行数を取得するメソッド
int Matrix::rows() const { return rows_; }

// 列数を取得するメソッド
int Matrix::cols() const { return cols_; }

// 要素にアクセスする演算子（非const版）
double& Matrix::operator()(int row, int col) { return values_[row * cols_ + col]; }

// 要素にアクセスする演算子（const版）
double Matrix::operator()(int row, int col) const { return values_[row * cols_ + col]; }

// 行にアクセスする演算子
Vector Matrix::operator[](int row) {
    if (row >= 0 && row < rows_) {
        return Vector(values_ + row * cols_, cols_);
    } else {
        throw std::out_of_range("Row index out of range");
    }
}

// 単項プラス演算子
Matrix Matrix::operator+() const { return *this; }

// 単項マイナス演算子
Matrix Matrix::operator-() const {
    Matrix result(*this);
    int total_size = rows_ * cols_;
    for (int i = 0; i < total_size; i++) {
        result.values_[i] *= -1.0;
    }
    return result;
}

// 加算代入演算子
Matrix& Matrix::operator+=(const Matrix& rhs) {
    if (rows_ != rhs.rows_ || cols_ != rhs.cols_) {
        std::cerr << "Matrix::operator+=: Size Unmatched" << std::endl;
        exit(1);
    }
    int total_size = rows_ * cols_;
    for (int i = 0; i < total_size; i++) {
        values_[i] += rhs.values_[i];
    }
    return *this;
}

// 減算代入演算子
Matrix& Matrix::operator-=(const Matrix& rhs) {
    if (rows_ != rhs.rows_ || cols_ != rhs.cols_) {
        std::cerr << "Matrix::operator-=: Size Unmatched" << std::endl;
        exit(1);
    }
    int total_size = rows_ * cols_;
    for (int i = 0; i < total_size; i++) {
        values_[i] -= rhs.values_[i];
    }
    return *this;
}

// 行列を出力するメソッド
std::ostream& Matrix::print(std::ostream& lhs) const {
    lhs << "(";
    int rows = rows_;
    int cols = cols_;
    for (int row = 0; row < rows - 1; row++) {
        lhs << "(";
        for (int col = 0; col < cols; col++) {
            lhs << values_[row * cols + col] << ", ";
        }
        lhs << ")" << std::endl;
    }
    lhs << "(";
    for (int col = 0; col < cols; col++) {
        lhs << values_[(rows - 1) * cols + col] << ", ";
    }
    lhs << "))";
    return lhs;
}

// データへのポインタを取得するメソッド
double* Matrix::get_values() { return values_; }

// 行列の出力演算子
std::ostream& operator<<(std::ostream& lhs, const Matrix& rhs) { return rhs.print(lhs); }

// 行列の加算演算子
Matrix operator+(const Matrix& lhs, const Matrix& rhs) {
    Matrix result(lhs);
    return (result += rhs);
}

// 行列の減算演算子
Matrix operator-(const Matrix& lhs, const Matrix& rhs) {
    Matrix result(lhs);
    return (result -= rhs);
}

// スカラー倍の行列演算子
Matrix operator*(double factor, const Matrix& rhs) {
    if (rhs.rows() == 0 || rhs.cols() == 0) {
        std::cerr << "operator*(double , const Matrix &): Size unmatched" << std::endl;
        exit(1);
    }
    Matrix result(rhs);
    int rows = result.rows();
    int cols = result.cols();
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            result(row, col) *= factor;
        }
    }
    return result;
}

// 行列とベクトルの乗算演算子
Vector operator*(const Matrix& lhs, const Vector& rhs) {
    if (lhs.cols() != rhs.size() || lhs.rows() == 0) {
        std::cerr << "operator*(const Matrix &, const Vector &): Size unmatched" << std::endl;
        exit(1);
    }
    int rows = lhs.rows();
    int cols = lhs.cols();
    Vector result(rows);
    for (int row = 0; row < rows; row++) {
        result[row] = 0.0;
        for (int col = 0; col < cols; col++) {
            result[row] += lhs(row, col) * rhs[col];
        }
    }
    return result;
}

// 行列同士の乗算演算子
Matrix operator*(Matrix& lhs, Matrix& rhs) {
    if (lhs.cols() != rhs.rows() || lhs.rows() == 0 || rhs.cols() == 0) {
        std::cerr << "operator*(const Matrix &, const Matrix &): Size unmatched" << std::endl;
        exit(1);
    }

    int rows = lhs.rows();
    int cols = rhs.cols();
    int inner_dim = lhs.cols();
    Matrix result(rows, cols);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result(i, j) = 0.0;
            for (int k = 0; k < inner_dim; ++k) {
                result(i, j) += lhs(i, k) * rhs(k, j);
            }
        }
    }

    return result;
}

// 行列の等価比較演算子
bool operator==(const Matrix& lhs, const Matrix& rhs) {
    if (lhs.rows() != rhs.rows() || lhs.cols() != rhs.cols()) {
        return false;
    }
    int rows = lhs.rows();
    int cols = lhs.cols();
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (lhs(row, col) != rhs(row, col)) {
                return false;
            }
        }
    }
    return true;
}

// 行列の不等価比較演算子
bool operator!=(const Matrix& lhs, const Matrix& rhs) {
    return !(lhs == rhs);
}

// 行列の平方和を計算する関数
double squared_sum(const Matrix& arg) {
    double result = 0.0;
    int rows = arg.rows();
    int cols = arg.cols();
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            result += arg(row, col) * arg(row, col);
        }
    }
    return result;
}

// フロベニウスノルムを計算する関数
double frobenius_norm(const Matrix& arg) {
    return sqrt(squared_sum(arg));
}

// 行列の転置を計算する関数
Matrix transpose(Matrix& arg) {
    if (arg.rows() == 0 || arg.cols() == 0) {
        std::cerr << "transpose(const Matrix): zero-sized matrix" << std::endl;
        exit(1);
    }
    int rows = arg.rows();
    int cols = arg.cols();
    Matrix result(cols, rows);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result(j, i) = arg(i, j);
        }
    }
    return result;
}
