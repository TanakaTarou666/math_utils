#include "sparse_matrix.h"

// コンストラクタ
SparseMatrix::SparseMatrix(int rows, int cols) : rows_(rows), cols_(cols) {
    // 行ポインタ、列インデックス、データを初期化
    row_pointers_ = new int[rows + 1]();
    col_indices_ = nullptr;  // サイズは後で設定する
    values_ = nullptr;       // サイズは後で設定する
    nnz_ = 0;
}

// コンストラクタ
SparseMatrix::SparseMatrix(int rows, int cols, int nnz) : rows_(rows), cols_(cols), nnz_(nnz) {
    // 行ポインタ、列インデックス、データを初期化
    row_pointers_ = new int[rows + 1]();
    col_indices_ = new int[nnz];
    values_ = new double[nnz];
}

// デフォルトコンストラクタ
SparseMatrix::SparseMatrix() : rows_(0), cols_(0), nnz_(0) {
    // 行ポインタ、列インデックス、データを初期化
    row_pointers_ = nullptr;
    col_indices_ = nullptr;
    values_ = nullptr;
}

// コピーコンストラクタ
SparseMatrix::SparseMatrix(const SparseMatrix& arg) : rows_(arg.rows_), cols_(arg.cols_), nnz_(arg.nnz_) {
    // 行ポインタ、列インデックス、データをコピー
    row_pointers_ = new int[rows_ + 1];
    col_indices_ = new int[nnz_];
    values_ = new double[nnz_];

    for (int i = 0; i <= rows_; i++) {
        row_pointers_[i] = arg.row_pointers_[i];
    }

    for (int i = 0; i < nnz_; i++) {
        col_indices_[i] = arg.col_indices_[i];
        values_[i] = arg.values_[i];
    }
}

// 特殊なコンストラクタ（対角行列を生成）
SparseMatrix::SparseMatrix(int size, const char* s) : rows_(size), cols_(size) {
    if (strcmp(s, "diag") != 0) return;
    // 対角行列の場合、非ゼロ要素の数は size となる
    nnz_ = size;

    // 行ポインタ、列インデックス、データを初期化
    row_pointers_ = new int[size + 1];
    col_indices_ = new int[size];
    values_ = new double[size];

    // 各要素の初期化
    for (int i = 0; i < size; i++) {
        row_pointers_[i] = i;
        col_indices_[i] = i;
    }

    // 行ポインタの最後を設定
    row_pointers_[size] = size;
}

// デストラクタ
SparseMatrix::~SparseMatrix() {
    // データを解放
    delete[] row_pointers_;
    delete[] col_indices_;
    delete[] values_;
}

// 要素アクセス演算子（非const版）
double& SparseMatrix::operator()(int row, int index) { return values_[row_pointers_[row] + index]; }

// 要素アクセス演算子（const版）
double SparseMatrix::operator()(int row, int index) const { return values_[row_pointers_[row] + index]; }

// 非ゼロ要素の値を返す
double& SparseMatrix::value(int row, int index) { return values_[row_pointers_[row] + index]; }

// 非ゼロ要素のインデックスを返す
int& SparseMatrix::dense_index(int row, int index) { return col_indices_[row_pointers_[row] + index]; }

// インデックスアクセス演算子（非const版）
int& SparseMatrix::operator()(int row, int index, const char* s) {
    if (strcmp(s, "index") != 0) {
        std::cerr << "Invalid string parameter!" << std::endl;
        exit(1);
    }
    return col_indices_[row_pointers_[row] + index];
}

// 行の要素数を返す演算子
int SparseMatrix::operator()(int row, const char* s) const {
    if (strcmp(s, "row") != 0) {
        std::cerr << "Invalid string parameter!!" << std::endl;
        exit(1);
    }
    int result = row_pointers_[row + 1] - row_pointers_[row];
    return result;
}

// 行数を返す
int SparseMatrix::rows() const { return rows_; }

// 列数を返す
int SparseMatrix::cols() const { return cols_; }

// 非ゼロ要素数を返す
int SparseMatrix::nnz() const { return nnz_; }

// 特定の行の非ゼロ要素数を返す
int SparseMatrix::nnz(int row) {
    int result = row_pointers_[row + 1] - row_pointers_[row];
    return result;
}

// ゼロ要素を削除した疎行列を返す
SparseMatrix SparseMatrix::remove_zeros() {
    // 非ゼロ要素の数を数える
    int non_zero_count = 0;
    for (int i = 0; i < nnz_; i++) {
        if (values_[i] != 0.0) {
            non_zero_count++;
        }
    }

    // 新しいスパースマトリクスを作成
    SparseMatrix non_zero_matrix(rows_, cols_, non_zero_count);

    // 新しいスパースマトリクスの行ポインタ、列インデックス、データ
    int* new_row_pointers = new int[rows_ + 1];
    int* new_col_indices = new int[non_zero_count];
    double* new_values = new double[non_zero_count];

    int current_non_zero_index = 0;
    new_row_pointers[0] = 0;

    for (int i = 0; i < rows_; i++) {
        for (int j = row_pointers_[i]; j < row_pointers_[i + 1]; j++) {
            if (values_[j] != 0.0) {
                new_col_indices[current_non_zero_index] = col_indices_[j];
                new_values[current_non_zero_index] = values_[j];
                current_non_zero_index++;
            }
        }
        new_row_pointers[i + 1] = current_non_zero_index;
    }

    // 新しいスパースマトリクスに行ポインタ、列インデックス、データをセット
    non_zero_matrix.set_row_pointers(new_row_pointers);
    non_zero_matrix.set_col_indices(new_col_indices);
    non_zero_matrix.set_values(new_values);
    non_zero_matrix.set_nnz(non_zero_count);

    return non_zero_matrix;
}

// コピー代入演算子
SparseMatrix& SparseMatrix::operator=(const SparseMatrix& arg) {
    if (this == &arg) {
        return *this; // 自己代入の場合、何もしない
    }

    // 新しいリソースを一時的に確保
    int* new_row_pointers = new int[arg.rows_ + 1];
    int* new_col_indices = new int[arg.nnz_];
    double* new_values = new double[arg.nnz_];

    // メンバー変数をコピー
    for (int i = 0; i <= arg.rows_; i++) {
        new_row_pointers[i] = arg.row_pointers_[i];
    }

    for (int i = 0; i < arg.nnz_; i++) {
        new_col_indices[i] = arg.col_indices_[i];
        new_values[i] = arg.values_[i];
    }

    // 既存のリソースを解放
    delete[] row_pointers_;
    delete[] col_indices_;
    delete[] values_;

    // メンバー変数を新しいリソースに設定
    row_pointers_ = new_row_pointers;
    col_indices_ = new_col_indices;
    values_ = new_values;
    rows_ = arg.rows_;
    cols_ = arg.cols_;
    nnz_ = arg.nnz_;

    return *this;
}

// ムーブ代入演算子
SparseMatrix& SparseMatrix::operator=(SparseMatrix&& arg) {
    if (this == &arg) {
        return *this; // 自己代入の場合、何もしない
    }

    // 既存のリソースを解放
    delete[] row_pointers_;
    delete[] col_indices_;
    delete[] values_;

    // メンバー変数をムーブ
    rows_ = arg.rows_;
    cols_ = arg.cols_;
    nnz_ = arg.nnz_;
    row_pointers_ = arg.row_pointers_;
    col_indices_ = arg.col_indices_;
    values_ = arg.values_;

    // 右辺値のリソースを無効化
    arg.rows_ = 0;
    arg.cols_ = 0;
    arg.nnz_ = 0;
    arg.row_pointers_ = nullptr;
    arg.col_indices_ = nullptr;
    arg.values_ = nullptr;

    return *this;
}

// 行列の乗算演算子
Matrix SparseMatrix::operator*(Matrix& arg) {
    int numRowsResult = rows_;
    int numColsResult = arg.cols();
    Matrix result(numRowsResult, numColsResult, 0.0);
    double* dataA = (*this).get_values();
    int* dataArowpointers = (*this).get_row_pointers();
    int* dataAcolindices = (*this).get_col_indices();
    double* dataB = arg.get_values();
    double* dataResult = result.get_values();

    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < numColsResult; j++) {
            *(dataResult + j) = 0.0;
        }
        int tmp_nnz = (*this).nnz(i);
        for (int k = 0; k < tmp_nnz; k++) {
            double* tmp_dataB = dataB + *(dataAcolindices + k) * numColsResult;
            for (int j = 0; j < numColsResult; j++) {
                *(dataResult + j) += *(dataA + k) * *(tmp_dataB + j);
            }
        }
        dataA += tmp_nnz;
        dataResult += numColsResult;
        dataAcolindices += tmp_nnz;
    }
    return result;
}

// 行列の値を表示する
void SparseMatrix::print_values() {
    int rows = (*this).rows();
    int cols = (*this).cols();

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < *((*this).get_row_pointers() + i + 1) - *((*this).get_row_pointers() + i); ++j) {
            std::cout << "(" << i << ", " << *((*this).get_col_indices() + *((*this).get_row_pointers() + i) + j) << ") : " << (*this)(i, j) << "\t";
        }
        std::cout << std::endl;
    }
}

// 値のポインタを取得する
double* SparseMatrix::get_values() { return values_; }

// 行ポインタのポインタを取得する
int* SparseMatrix::get_row_pointers() { return row_pointers_; }

// 列インデックスのポインタを取得する
int* SparseMatrix::get_col_indices() { return col_indices_; }

// 新しい行ポインタを設定する
void SparseMatrix::set_row_pointers(int* new_row_pointers) {
    // 以前のメモリを解放
    if (row_pointers_ != nullptr) delete[] row_pointers_;

    // 新しいポインタを設定
    row_pointers_ = new_row_pointers;
}

// 新しい列インデックスを設定するメンバ関数
void SparseMatrix::set_col_indices(int* new_col_indices) {
    // 以前のメモリを解放
    if (col_indices_ != nullptr) delete[] col_indices_;

    // 新しいポインタを設定
    col_indices_ = new_col_indices;
}

// 新しい値を設定するメンバ関数
void SparseMatrix::set_values(double* new_values) {
    // 以前のメモリを解放
    if (values_ != nullptr) delete[] values_;

    // 新しいポインタを設定
    values_ = new_values;
}

// 非ゼロ要素数を設定する
void SparseMatrix::set_nnz(int nnz) { nnz_ = nnz; }

// 転置行列を返す
SparseMatrix SparseMatrix::transpose() {
    // 転置行列の行数と列数は元の行列の列数と行数になります
    SparseMatrix transposed(cols_, rows_);

    // 転置行列の行ポインタ
    int* transposed_row_pointers = new int[cols_ + 1];

    // 各列に含まれる非ゼロ要素の数を数える
    int* count_non_zeros = new int[cols_];
    for (int i = 0; i < cols_; i++) {
        count_non_zeros[i] = 0;
    }
    for (int i = 0; i < nnz_; i++) {
        count_non_zeros[col_indices_[i]]++;
    }

    // 転置行列の行ポインタを設定
    transposed_row_pointers[0] = 0;
    for (int i = 1; i <= cols_; i++) {
        transposed_row_pointers[i] = transposed_row_pointers[i - 1] + count_non_zeros[i - 1];
    }

    // 転置行列の列インデックスとデータ
    int* transposed_col_indices = new int[nnz_];
    double* transposed_values = new double[nnz_];

    // 一時的な配列を用意して、各列に挿入される行の位置を管理します
    int* current_row = new int[cols_];
    for (int i = 0; i < cols_; i++) {
        current_row[i] = transposed_row_pointers[i];
    }

    // 転置行列を生成
    for (int i = 0; i < rows_; i++) {
        for (int j = row_pointers_[i]; j < row_pointers_[i + 1]; j++) {
            int col = col_indices_[j];
            int dest = current_row[col];

            transposed_col_indices[dest] = i;
            transposed_values[dest] = values_[j];

            current_row[col]++;
        }
    }

    // 転置行列に行ポインタ、列インデックス、データをセット
    transposed.set_row_pointers(transposed_row_pointers);
    transposed.set_col_indices(transposed_col_indices);
    transposed.set_values(transposed_values);
    transposed.set_nnz(nnz_);

    // 元の行列から作成したポインタを解放
    delete[] count_non_zeros;
    delete[] current_row;

    return transposed;
}

// 行列の積を計算する
void SparseMatrix::product(Matrix& lhs, Matrix& transpose_rhs) {
    int numrows_A = lhs.rows();
    int numcols_A = lhs.cols();
    int numrows_B = transpose_rhs.rows();
    int numcols_B = transpose_rhs.cols();

    for (int i = 0; i < rows_; i++) {
        int tmp_nnz = (*this).nnz(i);
        for (int j = 0; j < tmp_nnz; j++) {
            double tmp_result = 0.0;
            for (int k = 0; k < numcols_A; k++) {
                tmp_result += lhs(i, k) * transpose_rhs((*this).dense_index(i, j), k);
            }
            (*this)(i, j) = tmp_result;
        }
    }
}

// ワンホットエンコードを行う
SparseMatrix SparseMatrix::one_hot_encode() {
    // 一時的な行列を作成する（要素が全てゼロの疎行列）
    SparseMatrix encoded(nnz_, rows_ + cols_, nnz_ * 2);

    double* encoded_values = encoded.get_values();
    int* encoded_col_indices = encoded.get_col_indices();
    int* encoded_row_pointers = encoded.get_row_pointers();
    for (int i = 0; i <= nnz_; i++) {
        encoded_row_pointers[i] = 2 * i;
    }
    int k = 0;
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < (*this).nnz(i); j++) {
            encoded_values[k] = i; // 1
            encoded_col_indices[k] = i;
            k++;
            encoded_values[k] = i; // 1
            encoded_col_indices[k] = rows_ + (*this).dense_index(i, j);
            k++;
        }
    }
    return encoded;
}