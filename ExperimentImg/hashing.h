#pragma once
#include<string>
#include<opencv2\opencv.hpp>
using namespace std;
using namespace cv;
int HanmingDistance(string &str1, string &str2)
{
	if ((str1.size() != 64) || (str2.size() != 64))
		return -1;
	int distance = 0;
	for (int i = 0; i < 64; i++)
	{
		if (str1[i] != str2[i])
			distance++;
	}
	return distance;
}
//Phash
std::string getHashValue(Mat &src)
{
	Mat img, dst;
	string hashValue(64, '\0');
	double grayScale[64];
	double mean = 0.0;
	
	if (src.channels() == 3)
	{
		cvtColor(src, src, CV_BGR2GRAY);
		img = Mat_<double>(src);
	}
	else
	{
		img = Mat_<double>(src);
	}

	/* ��һ�������ųߴ�*/
	resize(img, img, Size(8, 8));

	/* �ڶ�������ɢ���ұ任��DCTϵ����ȡ*/
	dct(img, dst);

	/* ����������ȡDCTϵ����ֵ�����Ͻ�8*8�����DCTϵ����*/
	int k = 0;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j)
		{
			grayScale[k] = dst.at<double>(i, j);
			mean += dst.at<double>(i, j) / 64;
			++k;
		}
	}

	/* ���Ĳ��������ϣֵ��*/
	for (int i = 0; i < 64; ++i)
	{
		if (grayScale[i] >= mean)
		{
			hashValue[i] = '1';
		}
		else
		{
			hashValue[i] = '0';
		}
	}
	return hashValue;
}

CString isSimiler(Mat src1,Mat src2) {
	string pHashSrc1 = getHashValue(src1);
	string pHashSrc2 = getHashValue(src2);
	int distance = HanmingDistance(pHashSrc1, pHashSrc2);
	if (distance <= 5) {
		return _T("ʮ������");
	}
	else if (distance <= 10) {
		return _T("һ���̶�������");
	}
	else {
		return _T("�ǳ���ͬ");
	}
}

