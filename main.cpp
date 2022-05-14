#include <iostream>
#include <omp.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

using ll = long long;
using namespace std;

void to_grayscale(const uchar *image, uchar *new_image, int n, int *histogram) {

	for (int i = 0; i < n; i++) {	
		new_image[i] = (
			0.114 * image[i * 3 + 0] +
			0.587 * image[i * 3 + 1] +
			0.299 * image[i * 3 + 2]
		);
		histogram[new_image[i]]++;
	}
}

void to_binary(uchar *image, int n, const int *histogram) {

	ll preffix[2][256];

	preffix[0][0] = 0;
	preffix[1][0] = histogram[0];

	for (int i = 1; i < 256; i++) {

		preffix[0][i] = preffix[0][i - 1] + (ll)i * histogram[i];
		preffix[1][i] = preffix[1][i - 1] + histogram[i];
	}

	int a, b;

	for (a = 0; a < 256 && histogram[a] == 0; a++);
	for (b = 255; b >= 0 && histogram[b] == 0; b--);

	double lx, rx;
	double m0 = a, m1 = (a + b) / 2.0;

	while (abs(m0 - m1) > 1e-3) {

		m0 = m1;

		lx = (double)preffix[0][(int)m0] / preffix[1][(int)m0];
		
		rx = (
			(double)(preffix[0][255] - preffix[0][(int)m0])
			/ (preffix[1][255] - preffix[1][(int)m0])
		);

		m1 = (lx + rx) / 2;

		cout << m0 << ' ' << lx << ' ' << rx << ' ' << m1 << '\n';
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
	
	// be careful with integer overflow in high resolutions (> 8k)
	to_grayscale(img.data, gry_img.data, img.rows * img.cols, histogram);

	to_binary(gry_img.data, img.rows * img.cols, histogram);

	if (argc == 2) {
		cv::imwrite(filename, gry_img);
	} else {
		cv::imwrite("images/" + string(argv[2]), gry_img);
	}
}
