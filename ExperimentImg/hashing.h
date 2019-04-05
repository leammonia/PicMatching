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
//平均值hash
std::string getHashValue(Mat &src)
{
	string hashValue(64, '\0');
	Mat img;
	if (src.channels() == 3)
		cvtColor(src, img, CV_BGR2GRAY);
	else
		img = src.clone();
	resize(img, img, Size(8, 8));
	uchar *pData;
	for (int i = 0; i < img.rows; i++)
	{
		pData = img.ptr<uchar>(i);
		for (int j = 0; j < img.cols; j++)
		{
			pData[j] = pData[j] / 4;
		}
	}

	int average = mean(img).val[0];

	Mat mask = (img >= (uchar)average);
	int index = 0;
	for (int i = 0; i < mask.rows; i++)
	{
		pData = mask.ptr<uchar>(i);
		for (int j = 0; j < mask.cols; j++)
		{
			if (pData[j] == 0)
				hashValue[index++] = '0';
			else
				hashValue[index++] = '1';
		}
	}
	return hashValue;
}
std::wstring StringToWString(const std::string &str)
  {
     std::wstring wstr(str.length(), L' ');
     std::copy(str.begin(), str.end(), wstr.begin());
     return wstr;
 }
CString isSimiler(Mat src1,Mat src2) {
	string pHashSrc1 = getHashValue(src1);
	string pHashSrc2 = getHashValue(src2);
	int distance = HanmingDistance(pHashSrc1, pHashSrc2);
	wstringstream ss;
	if (distance <= 15) {
		ss << L"十分相似 ";
	}
	else if (distance <= 20) {
		ss << L"一定程度上相似 ";
	}
	else {
		ss << L"非常不同 ";
	}
	
	//ss << distance<<L" "<< StringToWString(pHashSrc1)<<L" "<< StringToWString(pHashSrc2);
	return ss.str().c_str();
}

