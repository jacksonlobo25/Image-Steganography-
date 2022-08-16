#include <iostream>
#include <fstream>

#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

bool isBitSet(char ch, int pos) {
	ch = ch >> pos;
	if (ch & 1)
		return true;
	return false;
}

void encode(string img, string txt, string out) {
	Mat image = imread(img);
	if (image.empty()) {
		cout << "\nImage Error";
		exit(-1);
	}

	ifstream file(txt);
	if (!file.is_open()) {
		cout << "\nText File Error";
		exit(-1);
	}

	char ch;
	file.get(ch);
	int bit_count = 0;
	bool last_null_char = false;
	bool encoded = false;

	for (int row = 0; row < image.rows; row++) {
		for (int col = 0; col < image.cols; col++) {
			for (int color = 0; color < 3; color++) {

				Vec3b pixel = image.at<Vec3b>(Point(row, col));

				if (isBitSet(ch, 7 - bit_count))
					pixel.val[color] |= 1;
				else
					pixel.val[color] &= ~1;

				image.at<Vec3b>(Point(row, col)) = pixel;

				bit_count++;

				if (last_null_char && bit_count == 8) {
					encoded = true;
					goto OUT;
				}

				if (bit_count == 8) {
					bit_count = 0;
					file.get(ch);

				if (file.eof()) {
						last_null_char = true;
						ch = '\0';
					}
				}

			}
		}
	}
OUT:;

	if (!encoded) {
		cout << "\nMessage too big. Try with larger image.\n";
		exit(-1);
	}

	imwrite(out, image);
	cout << "\nHurray!Message is encoded";
}

void decode(string out_img) {
	Mat image = imread(out_img);
	if (image.empty()) {
		cout << "\nImage Error";
		exit(-1);
	}

	char ch = 0;
	int bit_count = 0;

	cout<<"\nThe decoded message is : \n";

	for (int row = 0; row < image.rows; row++) {
		for (int col = 0; col < image.cols; col++) {
			for (int color = 0; color < 3; color++) {

				Vec3b pixel = image.at<Vec3b>(Point(row, col));

				if (isBitSet(pixel.val[color], 0))
					ch |= 1;

				bit_count++;

				if (bit_count == 8) {

					if (ch == '\0')
						goto OUT;

					bit_count = 0;
					cout << ch;
					ch = 0;
				}
				else {
					ch = ch << 1;
				}

			}
		}
	}
OUT:;
}

int main(int argc, char** argv) {
	
	string img;
	string txt;
	string out;
	string out_img;
	int ch;
	while (1) {
		cout << "\n\nTo Encode press 1\n";
		cout << "To Decode press 2\n";
		cout << "Enter your choice:";
		cin >> ch;
		switch (ch) {
		case 1:cout << "\n---ENCODING---\n";
			cout << "Enter the image path(bmp):";
			cin >> img;
			cout << "Enter the text file path:";
			cin >> txt;
			cout << "Enter the output image path:";
			cin >> out;
			encode(img, txt, out);
			break;
		case 2:cout << "\n---DECODING---\n";
			cout << "Enter the image path(bmp):";
			cin >> out_img;
			decode(out_img);
			break;
		default:cout << "Wrong Choice";
		}
	}
	return 0;
}