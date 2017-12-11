#include <utility>
#include <vector>
#include <iostream>

int main(int argc, char* argv[]) {
    std::vector<std::pair<double, double>> data;

    for(double in1, in2; std::cin >> in1 && std::cin >> in2;){
        data.emplace_back(in1, in2);
    }

    auto n = std::size(data);

    double sumx = 0;
    double sumx2 = 0;
    double sumxy = 0;
    double sumy = 0;
    double sumy2 = 0;
    for(unsigned c = 0; c < n; ++c){
        sumx += data[c].first;
        sumx2 += data[c].first * data[c].first;
        sumxy += data[c].first * data[c].second;
        sumy += data[c].second;
        sumy2 += data[c].second * data[c].second;
    }

    double det = n * sumx2 - sumx * sumx;

    double a = (n * sumxy - sumx * sumy) / det;
    double b = (sumy * sumx2 - sumx * sumxy) / det;

    std::cout << b << std::endl << a << std::endl;


}
