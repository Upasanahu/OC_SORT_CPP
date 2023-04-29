#include "OCsort.h"
#include "fstream"
#include "iostream"
#include "numeric"
#include "vector"
#include <Eigen/Core>
using namespace std;
using namespace Eigen;
using namespace ocsort;
using ocsort::associate;
/**
 * 读取CSV文件，并且返回一个Matrix
 * @param filename
 * @return
 */
Eigen::Matrix<double, Eigen::Dynamic, 6> read_csv_to_eigen(const std::string &filename) {
    // 读取CSV文件
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        exit(1);
    }
    // 解析CSV格式
    std::string line;
    std::vector<std::vector<float>> data;
    while (std::getline(file, line)) {
        std::vector<float> row;
        std::istringstream iss(line);
        std::string field;
        while (std::getline(iss, field, ',')) {
            row.push_back(std::stof(field));
        }
        data.push_back(row);
    }
    // 转换为Eigen::Matrix
    Eigen::Matrix<double, Eigen::Dynamic, 6> matrix(data.size(), data[0].size());
    for (int i = 0; i < data.size(); ++i) {
        for (int j = 0; j < data[0].size(); ++j) {
            matrix(i, j) = data[i][j];
        }
    }
    return matrix;
}

template<typename AnyCls>
ostream &operator<<(ostream &os, const vector<AnyCls> &v) {
    os << "{";
    for (auto it = v.begin(); it != v.end(); ++it) {
        os << "(" << *it << ")";
        if (it != v.end() - 1) os << ", ";
    }
    os << "}";
    return os;
}
int main(int argc, char *argv[]) {
    // 初始化一个OCsort对象
    OCSort A = OCSort(0, 50, 1, 0.22136877277096445, 1, "giou", 0.3941737016672115, false);
    // 使用OCsort追踪，现在不停的给他传入我们的观测值(检测框)
    std::ostringstream filename;
    // todo :WARNING: 第9帧的时候出错了，
    for (int i = 1; i < 150; ++i) {
        // 读取输入数据
        std::cout << "============== " << i << " =============" << std::endl;
        filename << "../BINARY_DATA/" << i << ".csv";
        Eigen::Matrix<double, Eigen::Dynamic, 6> dets = read_csv_to_eigen(filename.str());
        filename.str("");
        std::cout << "input:\n"
                  << dets << std::endl;
        // 推理
        auto res = A.update(dets);
        // 打印输出
        std::cout << "predict:\n"
                  << res << std::endl;
    }
}
