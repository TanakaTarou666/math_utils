#include "dss_tensor.h"

// コンストラクタ
DSSTensor::DSSTensor(SparseMatrix& arg, int depth) : depth_(depth) {
    rows_ = arg.rows();
    cols_ = arg.cols();
    nnz_ = arg.nnz();

    row_pointers_ = new int[rows_ + 1]();
    col_indices_ = new int[nnz_];
    elements_ = new SparseVector[nnz_];

    int* arg_row_pointers = arg.get_row_pointers();
    int* arg_col_indices = arg.get_col_indices();

    for (int i = 0; i < rows_ + 1; i++) {
        row_pointers_[i] = arg_row_pointers[i];
    }

    for (int i = 0; i < nnz_; i++) {
        col_indices_[i] = arg_col_indices[i];
    }
}

// コンストラクタ（要素指定）
DSSTensor::DSSTensor(SparseMatrix& arg, int depth, SparseVector* elements)
    : depth_(depth) {
    rows_ = arg.rows();
    cols_ = arg.cols();
    nnz_ = arg.nnz();

    row_pointers_ = new int[rows_ + 1]();
    col_indices_ = new int[nnz_];
    elements_ = new SparseVector[nnz_];

    int* arg_row_pointers = arg.get_row_pointers();
    int* arg_col_indices = arg.get_col_indices();

    for (int i = 0; i < rows_ + 1; i++) {
        row_pointers_[i] = arg_row_pointers[i];
    }

    for (int i = 0; i < nnz_; i++) {
        col_indices_[i] = arg_col_indices[i];
        elements_[i] = elements[i];
    }
}

// デフォルトコンストラクタ
DSSTensor::DSSTensor() : rows_(0), cols_(0), depth_(0), nnz_(0) {
    row_pointers_ = nullptr;
    col_indices_ = nullptr;
    elements_ = nullptr;
}

// デストラクタ
DSSTensor::~DSSTensor() {
    delete[] row_pointers_;
    delete[] col_indices_;
    delete[] elements_;
}

// 要素アクセス演算子（非const版）
SparseVector& DSSTensor::operator()(int row, int index) {
    return elements_[row_pointers_[row] + index];
}

// 要素アクセス演算子（const版）
SparseVector DSSTensor::operator()(int row, int index) const {
    return elements_[row_pointers_[row] + index];
}

// インデックスアクセス演算子（非const版）
int& DSSTensor::operator()(int row, int index, const char* s) {
    if (strcmp(s, "index") != 0) {
        std::cerr << "Invalid string parameter" << std::endl;
        exit(1);
    }
    return col_indices_[row_pointers_[row] + index];
}

// インデックスアクセス演算子（const版）
int DSSTensor::operator()(int row, int index, const char* s) const {
    if (strcmp(s, "index") != 0) {
        std::cerr << "Invalid string parameter" << std::endl;
        exit(1);
    }
    return col_indices_[row_pointers_[row] + index];
}

// 行の要素数を返す演算子
int DSSTensor::operator()(int row, const char* s) const {
    if (strcmp(s, "row") != 0) {
        std::cerr << "Invalid string parameter" << std::endl;
        exit(1);
    }
    int result = row_pointers_[row + 1] - row_pointers_[row];
    return result;
}

// 非ゼロ要素のインデックスを返す
int& DSSTensor::dense_index(int row, int index) { return col_indices_[row_pointers_[row] + index]; }

// コピー代入演算子
DSSTensor& DSSTensor::operator=(const DSSTensor& arg) {
    if (this == &arg) {
        return *this;  // 自己代入の場合、何もしない
    }

    // メンバー変数をコピー
    rows_ = arg.rows_;
    cols_ = arg.cols_;
    depth_ = arg.depth_;
    nnz_ = arg.nnz_;

    // 既存のリソースを解放
    delete[] row_pointers_;
    delete[] col_indices_;
    delete[] elements_;

    // 新しいリソースを確保
    row_pointers_ = new int[rows_ + 1]();
    col_indices_ = new int[nnz_];
    elements_ = new SparseVector[nnz_];

    // メンバー変数をコピー
    for (int i = 0; i <= rows_; i++) {
        row_pointers_[i] = arg.row_pointers_[i];
    }

    for (int i = 0; i < nnz_; i++) {
        col_indices_[i] = arg.col_indices_[i];
        elements_[i] = arg.elements_[i];
    }

    return *this;
}

// ムーブ代入演算子
DSSTensor& DSSTensor::operator=(DSSTensor&& arg) {
    if (this == &arg) {
        return *this;  // 自己代入の場合、何もしない
    }

    // 既存のリソースを解放
    delete[] row_pointers_;
    delete[] col_indices_;
    delete[] elements_;

    // メンバー変数をムーブ
    rows_ = arg.rows_;
    cols_ = arg.cols_;
    depth_ = arg.depth_;
    nnz_ = arg.nnz_;
    row_pointers_ = arg.row_pointers_;
    col_indices_ = arg.col_indices_;
    elements_ = arg.elements_;

    // 右辺値のリソースを無効化
    arg.rows_ = 0;
    arg.cols_ = 0;
    arg.depth_ = 0;
    arg.nnz_ = 0;
    arg.row_pointers_ = nullptr;
    arg.col_indices_ = nullptr;
    arg.elements_ = nullptr;

    return *this;
}

// 行数を返す
int DSSTensor::rows(void) const { return rows_; }

// 列数を返す
int DSSTensor::cols(void) const { return cols_; }

// 深さを返す
int DSSTensor::depth(void) const { return depth_; }

// 非ゼロ要素数を返す
int DSSTensor::nnz(void) const { return nnz_; }

// 特定の行の非ゼロ要素数を返す
int DSSTensor::nnz(int row) const {
    int result = row_pointers_[row + 1] - row_pointers_[row];
    return result;
}

// 非ゼロ要素の配列を返す
SparseVector* DSSTensor::get_elements() { return elements_; }

// 行ポインタ配列を返す
int* DSSTensor::get_row_pointers() { return row_pointers_; }

// 列インデックス配列を返す
int* DSSTensor::get_col_indices() { return col_indices_; }