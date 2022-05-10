#include <iostream>
#include <omp.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp> // serve so pra mostrar na tela

using ll = long long;
using namespace std;

void to_grayscale(const uchar *image, uchar *new_image, int n, int *histogram) {

    uchar b, g, r;
    
    for (int i = 0; i < n; i++) {

        b = image[i * 3 + 0];
        g = image[i * 3 + 1];
        r = image[i * 3 + 2];

        new_image[i] = 0.114 * b + 0.587 * g + 0.299 * r; // imprecisão de ponto flutuante
        // gry_img.data[i] = b / 3 + g / 3 + r / 3;
        histogram[new_image[i]]++;
    }

    // for (int i = 0; i < 256; i++) {
    //     cout << i << ' ' << histogram[i] << '\n';
    // } cout << '\n';
}

void to_binary(uchar *image, int n, const int *histogram) {
    
    ll preffix_num[257], preffix_den[257];
    
    preffix_num[0] = 0;
    preffix_den[0] = 0;

    for (int i = 1; i <= 256; i++) {

        preffix_num[i] = preffix_num[i - 1] + (i - 1) * histogram[i - 1];
        preffix_den[i] = preffix_den[i - 1] + histogram[i - 1];
    }

    int a, b;

    for (a = 0; a < 256 && histogram[a] == 0; a++);
    for (b = 255; b >= 0 && histogram[b] == 0; b--);

    double lx, rx;
    double m0 = a, m1 = (a + b) / 2.0;

    cout << a << ' ' << b << '\n';

    while (abs(m0 - m1) > 1e-3) {

        m0 = m1;

        lx = (double)preffix_num[(int)m0 + 1] / preffix_den[(int)m0 + 1];
        
        rx = (
            (double)(preffix_num[b + 1] - preffix_num[(int)m0 + 1])
            / (preffix_den[b + 1] - preffix_den[(int)m0 + 1])
        );

        m1 = (lx + rx) / 2;

        // cout << m0 << ' ' << lx << ' ' << rx << ' ' << m1 << '\n';
    }

    for (int i = 0; i < n; i++) {

        if (image[i] <= m0) {
            image[i] = 0;
        } else {
            image[i] = 255;
        }
    }
}

int main(int argc, char *argv[]) {

    if (argc < 2) {

        cout << "A image must be indicated inside the 'images' folder!\n";
        return 1;
    }

    string filename;

    try {
        filename = cv::samples::findFile("images/" + string(argv[1]));
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
    
    cv::Mat img = cv::imread(filename, cv::IMREAD_COLOR);

    if (img.empty()) {

        cout << "Could not read the image: " << filename << '\n';
        return 1;
    }

    cv::Mat gry_img = cv::Mat(img.rows, img.cols, CV_8UC1);
    int histogram[256] = {0};
    
    // cuidado com int overflow em resoluções muito altas
    to_grayscale(img.data, gry_img.data, img.rows * img.cols, histogram);

    to_binary(gry_img.data, img.rows * img.cols, histogram);

    if (argc == 2) {
        cv::imwrite(filename, gry_img);
    } else {
        cv::imwrite("images/" + string(argv[2]), gry_img);
    }
}
