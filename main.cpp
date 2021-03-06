#include <iostream>
#include <numeric>
#include <omp.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#define EPS 1e-3

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

int threshold(uchar *image, int n, int *histogram) {

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

	if (a == b) {
		return a;
	}

	if (a > b) {
		return -1;
	}

	double lx, rx;
	double m0 = a, m1 = (a + b) / 2.0;

	while (abs(m0 - m1) > EPS) {

		m0 = m1;

		lx = (double)preffix[0][(int)m0] / preffix[1][(int)m0];
		
		rx = (
			(double)(preffix[0][255] - preffix[0][(int)m0])
			/ (preffix[1][255] - preffix[1][(int)m0])
		);

		m1 = (lx + rx) / 2;
	}

	return m0;
}

void to_binary(uchar *image, int n, int threshold) {

	for (int i = 0; i < n; i++) {
		if (image[i] <= threshold) {
			image[i] = 0;
		} else {
			image[i] = 255;
		}
	}
}

int main(int argc, char *argv[]) {

	if (argc < 3) {

		cout << "A number and a image must be indicated inside the 'images' folder!\n";
		return 1;
	}

	string filename;
	int n = atoi(argv[1]);

	double begin, end, t;

	try {
		filename = cv::samples::findFile("images/" + string(argv[2]));
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

	double read[n], gray[n], thrshld[n], binary[n], write[n], total[n];

	for (int i = 0; i < n; i++) {

		begin = omp_get_wtime();
		img = cv::imread(filename, cv::IMREAD_COLOR);
		end = omp_get_wtime();

		read[i] = end - begin;

		cv::Mat gry_img = cv::Mat(img.rows, img.cols, CV_8UC1);
		int histogram[256] = {0};
		
		// be careful with integer overflow in high resolutions (> 8k)
		begin = omp_get_wtime();
		to_grayscale(img.data, gry_img.data, img.rows * img.cols, histogram);
		end = omp_get_wtime();
		
		gray[i] = end - begin;

		begin = omp_get_wtime();
		t = threshold(gry_img.data, img.rows * img.cols, histogram);
		end = omp_get_wtime();
		
		thrshld[i] = end - begin;

		if (t == -1) {

			cout << "Could not convert to binary image\n";
			return 1;
		}

		begin = omp_get_wtime();
		to_binary(gry_img.data, img.rows * img.cols, t);
		end = omp_get_wtime();
		
		binary[i] = end - begin;
		
		begin = omp_get_wtime();
		if (argc == 3) {
			cv::imwrite(filename, gry_img);
		} else {
			cv::imwrite("images/" + string(argv[3]), gry_img);
		}
		end = omp_get_wtime();

		write[i] = end - begin;
		total[i] = read[i] + gray[i] + thrshld[i] + binary[i] + write[i];
	}

	double read_median = accumulate(read, read + n, 0.0) / n;
	double gray_median = accumulate(gray, gray + n, 0.0) / n;
	double thrshld_median = accumulate(thrshld, thrshld + n, 0.0) / n;
	double binary_median = accumulate(binary, binary + n, 0.0) / n;
	double write_median = accumulate(write, write + n, 0.0) / n;
	double total_median = accumulate(total, total + n, 0.0) / n;

	double read_sd = 0, gray_sd = 0, thrshld_sd = 0, binary_sd = 0, write_sd = 0, total_sd = 0;

	for (double x : read) {
		read_sd += (x - read_median) * (x - read_median);
	}

	read_sd = sqrt(read_sd / n);

	// ---------------------------------------------------------

	for (double x : gray) {
		gray_sd += (x - gray_median) * (x - gray_median);
	}

	read_sd = sqrt(gray_sd / n);

	// ----------------------------------------------------------

	for (double x : thrshld) {
		thrshld_sd += (x - thrshld_median) * (x - thrshld_median);
	}

	thrshld_sd = sqrt(thrshld_sd / n);

	// ------------------------------------------------------------

	for (double x : binary) {
		binary_sd += (x - binary_median) * (x - binary_median);
	}

	binary_sd = sqrt(binary_sd / n);

	// ------------------------------------------------------------

	for (double x : write) {
		write_sd += (x - write_median) * (x - write_median);
	}

	write_sd = sqrt(write_sd / n);

	// ------------------------------------------------------------

	for (double x : total) {
		total_sd += (x - total_median) * (x - total_median);
	}

	total_sd = sqrt(total_sd / n);

	cout << "read\n\tmedian --> " << read_median << "\n\tsd --> " << read_sd << '\n';
	cout << "gray\n\tmedian --> " << gray_median << "\n\tsd --> " << gray_sd << '\n';
	cout << "threshold\n\tmedian --> " << thrshld_median << "\n\tsd --> " << thrshld_sd << '\n';
	cout << "binary\n\tmedian --> " << binary_median << "\n\tsd --> " << binary_sd << '\n';
	cout << "write\n\tmedian --> " << write_median << "\n\tsd --> " << write_sd << '\n';
	cout << "total\n\tmedian --> " << total_median << "\n\tsd --> " << total_sd << '\n';
}



/*
read
	median: 
	sd:
gray
*/