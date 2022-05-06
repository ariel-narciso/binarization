#include <bits/stdc++.h>
#include <omp.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp> // serve so pra mostrar na tela

#define CHN 3

using namespace std;

int main() {

    string filename = cv::samples::findFile("bloodborne.jpg");
    cv::Mat img = cv::imread(filename, cv::IMREAD_COLOR);

    if (img.empty()) {

        cout << "Deu ruin!!\n";
        return 1;
    }

    cv::Mat gry_img = cv::Mat(img.rows, img.cols, CV_8UC1);

    uchar b, g, r;
    int histogram[256] = {0};

    // cuidado com int overflow
    for (int i = 0; i < img.rows * img.cols; i++) {

        b = img.data[i * CHN + 0];
        g = img.data[i * CHN + 1];
        r = img.data[i * CHN + 2];

        gry_img.data[i] = 0.114 * b + 0.587 * g + 0.299 * r; // imprecisão de ponto flutuante
        // gry_img.data[i] = b / 3 + g / 3 + r / 3;

        histogram[gry_img.data[i]]++;
    }

    for (int i = 0; i < 256; i++) {
        cout << i << ' ' << histogram[i] << '\n';
    }

    // cv::imshow("Oie", gry_img);
    // cv::imwrite("bloodborne_weight.png", gry_img);
    // cv::waitKey(0);
}