
// ExperimentImgDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\core\core.hpp>
#include<vector>
using namespace std;

using namespace cv;

struct DrawPara
{
	CImage* pImgSrc;
	CDC* pDC;
	int oriX;
	int oriY;
	int width;
	int height;
};

// CExperimentImgDlg �Ի���
class CExperimentImgDlg : public CDialogEx
{
// ����
public:
	CExperimentImgDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXPERIMENTIMG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	CImage* getImage() { return imgSrc1; }
	void ThreadDraw(DrawPara *p);
	static UINT Update(void* p);
	void ImageCopy(CImage* pImgSrc, CImage* pImgDrt);
	void CImageToMat(CImage *cimage, Mat& mat);
	void MatToCImage(CImage *cimage, Mat& mat);
	CTime doMatch(int type,int type2);
	void RANSAC(vector<DMatch> matches);
	void kdtree(Mat descriptors1, Mat descriptors2);
	void cross(Mat descriptors1, Mat descriptors2);

// ʵ��
protected:
	HICON m_hIcon;
	CImage * imgSrc1;
	CImage * imgSrc2;
	vector< KeyPoint> keypoints1;
	vector< KeyPoint> keypoints2;
	Mat srcImage1;
	Mat srcImage2;
	CImage * original;
	CImage * result;
	Mat resultMat;
	CTime startTime;
//	ThreadParam * m_pThreadParam;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
	CStatic mPictureControl;
	CStatic mPictureControl2;
	CStatic mPictureControl3;
	CStatic mPictureControl4;

    afx_msg void OnBnClickedButtonProcess();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonOpen2();
};
