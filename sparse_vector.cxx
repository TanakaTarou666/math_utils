#include "sparse_vector.h"

#include <string.h>
#include <cmath>

// コンストラクタ
SparseVector::SparseVector(int size, int nnz) try
    : size_(size), nnz_(nnz), indices_(new int[nnz]), values_(new double[nnz]) {
} catch (std::bad_alloc) {
    std::cerr << "SparseVector::SparseVector(int nnz_): Out of Memory!" << std::endl;
    throw;
}

// コピーコンストラクタ
SparseVector::SparseVector(const SparseVector &arg) try
    : size_(arg.size_),
      nnz_(arg.nnz_),
      indices_(new int[nnz_]),
      values_(new double[nnz_]) {
    for (int i = 0; i < nnz_; i++) {
        indices_[i] = arg.indices_[i];
        values_[i] = arg.values_[i];
    }
} catch (std::bad_alloc) {
    std::cerr << "SparseVector::SparseVector(const SparseVector &): Out of Memory!" << std::endl;
    throw;
}

// ムーブコンストラクタ
SparseVector::SparseVector(SparseVector &&arg)
    : size_(arg.size_),
      nnz_(arg.nnz_),
      indices_(arg.indices_),
      values_(arg.values_) {
    arg.size_ = 0;
    arg.nnz_ = 0;
    arg.indices_ = nullptr;
    arg.values_ = nullptr;
}

// デストラクタ
SparseVector::~SparseVector(void) {
    delete[] indices_;
    delete[] values_;
}

// コピー代入
SparseVector &SparseVector::operator=(const SparseVector &arg) {
    if (this == &arg) return *this;
    if (this->nnz_ != arg.nnz_) {
        nnz_ = arg.nnz_;
        delete[] indices_;
        delete[] values_;
        try {
            indices_ = new int[nnz_];
            values_ = new double[nnz_];
        } catch (std::bad_alloc) {
            std::cerr << "Out of Memory" << std::endl;
            throw;
        }
    }
    size_ = arg.size_;
    for (int i = 0; i < nnz_; i++) {
        indices_[i] = arg.indices_[i];
        values_[i] = arg.values_[i];
    }
    return *this;
}

// ムーブ代入
SparseVector &SparseVector::operator=(SparseVector &&arg) {
    if (this == &arg) return *this;
    size_ = arg.size_;
    nnz_ = arg.nnz_;
    delete[] indices_;
    delete[] values_;
    values_ = arg.values_;
    indices_ = arg.indices_;
    arg.nnz_ = 0;
    arg.indices_ = nullptr;
    arg.values_ = nullptr;
    return *this;
}

// 非ゼロ要素の値を返す
double& SparseVector::value(int index) { return values_[index]; }

// 非ゼロ要素のインデックスを返す
int& SparseVector::dense_index(int index) { return indices_[index]; }

// サイズを返す
int SparseVector::size(void) const { return size_; }

// 非ゼロ要素数を返す
int SparseVector::nnz(void) const { return nnz_; }

// 要素アクセス演算子（非const版）
double &SparseVector::operator()(int index) { return values_[index]; }

// 要素アクセス演算子（const版）
double SparseVector::operator()(int index) const { return values_[index]; }

// インデックスアクセス演算子（非const版）
int &SparseVector::operator()(int index, const char *s) {
    if (strcmp(s, "index") != 0) {
        std::cerr << "Invalid string parameter" << std::endl;
        exit(1);
    }
    return indices_[index];
}

// インデックスアクセス演算子（const版）
int SparseVector::operator()(int index, const char *s) const {
    if (strcmp(s, "index") != 0) {
        std::cerr << "Invalid string parameter" << std::endl;
        exit(1);
    }
    return indices_[index];
}

// 単項プラス演算子
SparseVector SparseVector::operator+(void) const { return *this; }

// 単項マイナス演算子
SparseVector SparseVector::operator-(void) const {
    SparseVector result = *this;
    for (int i = 0; i < result.nnz_; i++) result(i) *= -1.0;
    return result;
}

// 等価比較演算子
bool SparseVector::operator==(const SparseVector &rhs) const {
    if (size_ != rhs.size_ || nnz_ != rhs.nnz()) return false;
    for (int i = 0; i < nnz_; i++) {
        if (values_[i] != rhs(i) || indices_[i] != rhs(i, "index"))
            return false;
    }
    return true;
}

// 不等価比較演算子
bool SparseVector::operator!=(const SparseVector &rhs) const {
    return !(*this == rhs);
}

// 値を変更するメソッド
void SparseVector::modifyvalues_(int n, int index, double value) {
    this->values_[n] = value;
    this->indices_[n] = index;
}

// 出力演算子
std::ostream &operator<<(std::ostream &os, const SparseVector &rhs) {
    os << "(";
    if (rhs.nnz() > 0) {
        for (int i = 0;; i++) {
            os << rhs(i,"index") << ":" << rhs(i);
            if (i >= rhs.nnz() - 1) break;
            os << ", ";
        }
    }
    os << ')';
    return os;
}

// 最大ノルムを計算する関数
double max_norm(const SparseVector &arg) {
    if (arg.nnz() < 1) {
        std::cout << "Can't calculate norm for 0-sized vector" << std::endl;
        exit(1);
    }
    double result = fabs(arg(0));
    for (int i = 1; i < arg.nnz(); i++) {
        double tmp = fabs(arg(i));
        if (result < tmp) result = tmp;
    }
    return result;
}

// 2ノルムを計算する関数
double squared_norm(const SparseVector &arg) { return sqrt(norm_square(arg)); }

// 2ノルムの二乗を計算する関数
double norm_square(const SparseVector &arg) {
    double result = 0.0;
    for (int i = 0; i < arg.nnz(); i++) {
        result += arg(i) * arg(i);
    }
    return result;
}

// L1ノルムの二乗を計算する関数
double L1norm_square(const SparseVector &arg) {
    double result = 0.0;
    for (int i = 0; i < arg.nnz(); i++) {
        result += fabs(arg(i));
    }
    return result;
}

// SparseVectorとVectorの内積を計算する演算子
double operator*(const SparseVector &lhs, const Vector &rhs) {
    double result = 0.0;
    for (int ell = 0; ell < lhs.nnz(); ell++) {
        result += lhs(ell) * rhs[lhs(ell, "index")];
    }
    return result;
}

// VectorとSparseVectorの内積を計算する演算子
double operator*(const Vector &lhs, const SparseVector &rhs) {
    double result = 0.0;
    for (int ell = 0; ell < rhs.nnz(); ell++) {
        result += rhs(ell) * lhs[rhs(ell, "index")];
    }
    return result;
}