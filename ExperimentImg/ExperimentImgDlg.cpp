
// ExperimentImgDlg.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "ExperimentImg.h"
#include "ExperimentImgDlg.h"
#include "afxdialogex.h"
#include<opencv2\xfeatures2d.hpp>
#include<opencv2\xfeatures2d\nonfree.hpp>
#include<opencv2\opencv.hpp>
#include "hashing.h"
using namespace cv;
using namespace xfeatures2d;
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CExperimentImgDlg �Ի���



CExperimentImgDlg::CExperimentImgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EXPERIMENTIMG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//���ضԻ����ʱ���ʼ��
	imgSrc1 = NULL;
    imgSrc2 = NULL;
	result = NULL;
}

void CExperimentImgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//	DDX_Control(pDX, IDC_EDIT_INFO, mEditInfo);
	DDX_Control(pDX, IDC_PICTURE, mPictureControl);
	DDX_Control(pDX, IDC_PICTURE2, mPictureControl2);
	DDX_Control(pDX, IDC_PICTURE3, mPictureControl3);
	DDX_Control(pDX, IDC_PICTURE4, mPictureControl4);
}

BEGIN_MESSAGE_MAP(CExperimentImgDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CExperimentImgDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_OPEN2, &CExperimentImgDlg::OnBnClickedButtonOpen2)
	ON_BN_CLICKED(IDC_BUTTON2, &CExperimentImgDlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON_PROCESS, &CExperimentImgDlg::OnBnClickedButtonProcess)
END_MESSAGE_MAP()


// CExperimentImgDlg ��Ϣ�������

BOOL CExperimentImgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
//	mEditInfo.SetWindowTextW(CString("File Path"));
	CComboBox * cmb_function = ((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION));
	cmb_function->InsertString(0,_T("SIFT"));
	cmb_function->InsertString(1,_T("SURF"));
	cmb_function->InsertString(2,_T("ORB"));
	//cmb_function->InsertString(3,_T("FERNS"));
	cmb_function->SetCurSel(0);

	CComboBox * cmb_function2 = ((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION2));
	cmb_function2->InsertString(0, _T("RANSAC"));
	cmb_function2->InsertString(1, _T("k-d tree"));
	cmb_function2->InsertString(2, _T("cross"));
	//hashingƥ���㷨���������޹�
	cmb_function2->InsertString(3, _T("hashing"));
	//cmb_function2->InsertString(3, _T("hashing"));
	cmb_function2->SetCurSel(0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CExperimentImgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CExperimentImgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		if (imgSrc1 != NULL)
		{
			int height;
			int width;
			CRect rect;
			CRect rect1;
			CRect rect2;
			height = imgSrc1->GetHeight();
			width = imgSrc1->GetWidth();

			mPictureControl.GetClientRect(&rect);
			CDC *pDC = mPictureControl.GetDC();
			SetStretchBltMode(pDC->m_hDC, STRETCH_HALFTONE);

			if (width <= rect.Width() && height <= rect.Width())
			{
				rect1 = CRect(rect.TopLeft(), CSize(width, height));
				imgSrc1->StretchBlt(pDC->m_hDC, rect1, SRCCOPY);
			}
			else
			{
				float xScale = (float)rect.Width() / (float)width;
				float yScale = (float)rect.Height() / (float)height;
				float ScaleIndex = (xScale <= yScale ? xScale : yScale);
				rect1 = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex, (int)height*ScaleIndex));
				//��picture control������ͼ�����ź�Ĵ�С
//				CWnd *pWnd;
//				pWnd = GetDlgItem(IDC_PICTURE);
//				pWnd->MoveWindow(CRect((int)rect.top, (int)rect.left, (int)width*ScaleIndex, (int)height*ScaleIndex));
				imgSrc1->StretchBlt(pDC->m_hDC, rect1, SRCCOPY);
			}
			ReleaseDC(pDC);
		}
		if (imgSrc2 != NULL)
		{
			int height;
			int width;
			CRect rect;
			CRect rect1;
			CRect rect2;
			height = imgSrc2->GetHeight();
			width = imgSrc2->GetWidth();

			mPictureControl2.GetClientRect(&rect);
			CDC *pDC = mPictureControl2.GetDC();
			SetStretchBltMode(pDC->m_hDC, STRETCH_HALFTONE);


			if (width <= rect.Width() && height <= rect.Width())
			{
				rect1 = CRect(rect.TopLeft(), CSize(width, height));
				imgSrc2->StretchBlt(pDC->m_hDC, rect1, SRCCOPY);
			}
			else
			{
				float xScale = (float)rect.Width() / (float)width;
				float yScale = (float)rect.Height() / (float)height;
				float ScaleIndex = (xScale <= yScale ? xScale : yScale);
				rect1 = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex, (int)height*ScaleIndex));
				//��picture control������ͼ�����ź�Ĵ�С
				//				CWnd *pWnd;
				//				pWnd = GetDlgItem(IDC_PICTURE);
				//				pWnd->MoveWindow(CRect((int)rect.top, (int)rect.left, (int)width*ScaleIndex, (int)height*ScaleIndex));
				imgSrc2->StretchBlt(pDC->m_hDC, rect1, SRCCOPY);
			}
			ReleaseDC(pDC);
		}
		if (result != NULL)
		{
			int height;
			int width;
			CRect rect;
			CRect rect1;
			CRect rect2;
			height = result->GetHeight();
			width = result->GetWidth();

			mPictureControl3.GetClientRect(&rect);
			CDC *pDC = mPictureControl3.GetDC();
			SetStretchBltMode(pDC->m_hDC, STRETCH_HALFTONE);


			if (width <= rect.Width() && height <= rect.Width())
			{
				rect1 = CRect(rect.TopLeft(), CSize(width, height));
				result->StretchBlt(pDC->m_hDC, rect1, SRCCOPY);
			}
			else
			{
				float xScale = (float)rect.Width() / (float)width;
				float yScale = (float)rect.Height() / (float)height;
				float ScaleIndex = (xScale <= yScale ? xScale : yScale);
				rect1 = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex, (int)height*ScaleIndex));
				//��picture control������ͼ�����ź�Ĵ�С
				//				CWnd *pWnd;
				//				pWnd = GetDlgItem(IDC_PICTURE);
				//				pWnd->MoveWindow(CRect((int)rect.top, (int)rect.left, (int)width*ScaleIndex, (int)height*ScaleIndex));
				result->StretchBlt(pDC->m_hDC, rect1, SRCCOPY);
			}
			ReleaseDC(pDC);
		}
		if (original != NULL)
		{
			int height;
			int width;
			CRect rect;
			CRect rect1;
			CRect rect2;
			height = original->GetHeight();
			width = original->GetWidth();

			mPictureControl4.GetClientRect(&rect);
			CDC *pDC = mPictureControl4.GetDC();
			SetStretchBltMode(pDC->m_hDC, STRETCH_HALFTONE);


			if (width <= rect.Width() && height <= rect.Width())
			{
				rect1 = CRect(rect.TopLeft(), CSize(width, height));
				original->StretchBlt(pDC->m_hDC, rect1, SRCCOPY);
			}
			else
			{
				float xScale = (float)rect.Width() / (float)width;
				float yScale = (float)rect.Height() / (float)height;
				float ScaleIndex = (xScale <= yScale ? xScale : yScale);
				rect1 = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex, (int)height*ScaleIndex));
				//��picture control������ͼ�����ź�Ĵ�С
				//				CWnd *pWnd;
				//				pWnd = GetDlgItem(IDC_PICTURE);
				//				pWnd->MoveWindow(CRect((int)rect.top, (int)rect.left, (int)width*ScaleIndex, (int)height*ScaleIndex));
				original->StretchBlt(pDC->m_hDC, rect1, SRCCOPY);
			}
			ReleaseDC(pDC);
		}
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CExperimentImgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT CExperimentImgDlg::Update(void* p)
{
	
	return 0;
}

void CExperimentImgDlg::ThreadDraw(DrawPara *p)
{
	CRect rect;
	GetClientRect(&rect);
	CDC     memDC;             // ���ڻ����ͼ���ڴ滭��  
	CBitmap memBitmap;         // ���ڻ����ͼ���ڴ滭��
	memDC.CreateCompatibleDC(NULL);  // ������ԭ���ʼ��ݵĻ���
	memBitmap.CreateCompatibleBitmap(p->pDC, p->width, p->height);  // ������ԭλͼ���ݵ��ڴ滭��
	memDC.SelectObject(&memBitmap);      // ���������뻭���Ĺ���
	memDC.FillSolidRect(rect, p->pDC->GetBkColor());
	p->pDC->SetStretchBltMode(HALFTONE);
	// ��pImgSrc���������Ż����ڴ滭����
	p->pImgSrc->StretchBlt(memDC.m_hDC, 0, 0, p->width, p->height);

	// ���ѻ��õĻ������Ƶ������Ļ�������
	p->pDC->BitBlt(p->oriX, p->oriY, p->width, p->height, &memDC, 0, 0, SRCCOPY);
	memBitmap.DeleteObject();
	memDC.DeleteDC();
}

void CExperimentImgDlg::ImageCopy(CImage* pImgSrc, CImage* pImgDrt)
{
	int MaxColors = pImgSrc->GetMaxColorTableEntries();
	RGBQUAD* ColorTab;
	ColorTab = new RGBQUAD[MaxColors];

	CDC *pDCsrc, *pDCdrc;
	if (!pImgDrt->IsNull())
	{
		pImgDrt->Destroy();
	}
	pImgDrt->Create(pImgSrc->GetWidth(), pImgSrc->GetHeight(), pImgSrc->GetBPP(), 0);

	if (pImgSrc->IsIndexed())
	{
		pImgSrc->GetColorTable(0, MaxColors, ColorTab);
		pImgDrt->SetColorTable(0, MaxColors, ColorTab);
	}

	pDCsrc = CDC::FromHandle(pImgSrc->GetDC());
	pDCdrc = CDC::FromHandle(pImgDrt->GetDC());
	pDCdrc->BitBlt(0, 0, pImgSrc->GetWidth(), pImgSrc->GetHeight(), pDCsrc, 0, 0, SRCCOPY);
	pImgSrc->ReleaseDC();
	pImgDrt->ReleaseDC();
	delete ColorTab;

}

void CExperimentImgDlg::OnBnClickedButtonProcess()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CComboBox* cmb_function = ((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION));
	CComboBox* cmb_function2 = ((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION2));
	int type = cmb_function->GetCurSel();
	int type2 = cmb_function2->GetCurSel();
	startTime = GetTickCount();
	CTime endTime=doMatch(type,type2);
	CString timeStr;

	this->Invalidate();

	timeStr.Format(_T("��ʱ:%dms"), endTime - startTime);
	AfxMessageBox(timeStr);
	
}

CTime CExperimentImgDlg::doMatch(int type,int type2) {
	
	if (imgSrc1 == NULL || imgSrc2 == NULL) return GetTickCount();

	CImageToMat(imgSrc1, srcImage1);
	CImageToMat(imgSrc2, srcImage2);


	//������ļ�⣬������keypoint���͵�vector��
	//����������������
	Mat descriptors1;
	Mat descriptors2;

	if (type2==3) {
		CString result = _T("����ͼƬ")+isSimiler(srcImage1, srcImage2);

		CTime endTime = GetTickCount();
		UINT i;
		i = MessageBox(result, TEXT("ƥ����"), MB_OK | MB_ICONASTERISK);
		if (i == IDOK)
		{
			return endTime;
		}
	}
	Ptr<Feature2D> features2d;
	switch (type)
	{
	case 0:  //SIFT
		features2d = SIFT::create();
		break;
	case 1: //SURF
		features2d = SURF::create();
		break;
	case 2://ORB(FAST+BRIEF)
		features2d = ORB::create();
		break;
	case 3://FERNS

		break;
	default:
		break;
	}
	features2d->detectAndCompute(srcImage1, Mat(), keypoints1, descriptors1);
	features2d->detectAndCompute(srcImage2, Mat(), keypoints2, descriptors2);

	drawKeypoints(srcImage1, keypoints1, srcImage1);
	drawKeypoints(srcImage2, keypoints2, srcImage2);

	
	//������ƥ��
	//����ͼƬ��������������ȡ���������ǾͿ���ʹ��BruteForceMatcher���������ͼƬ��descriptor����ƥ�䣬�õ�ƥ��Ľ����matches��
	vector<DMatch> matches;
	BFMatcher matcher;
	matcher.match(descriptors1, descriptors2, matches);

	Mat originalMat;
	drawMatches(srcImage1, keypoints1, srcImage2, keypoints2, matches, originalMat);

	switch (type2)
	{
	case 0:  //RANSAC
		RANSAC(matches);
		break;
	case 1: //kd-tree 
		if (type == 2) return GetTickCount();//��֧��ORB
		kdtree(descriptors1, descriptors2);
		break;
	case 2://cross ��hashingΨһ�Ĺ�ͬ����ֻ�� ����hamming����
		if (type == 0 || type == 1) return GetTickCount();//��֧��SIFT��SURF
		cross(descriptors1, descriptors2);
		break;
	default:
		break;
	}

	
	MatToCImage(imgSrc1, srcImage1);
	MatToCImage(imgSrc2, srcImage2);

	if (original == NULL)
		original = new CImage();
	MatToCImage(original, originalMat);
	if (result == NULL)
		result = new CImage();
	MatToCImage(result, resultMat);
	return GetTickCount();

}

void CExperimentImgDlg::RANSAC(vector<DMatch> matches) {
	//����ƥ�����о����������Сֵ       
	//������ָ���������������ŷʽ���룬�������������Ĳ��죬ֵԽС��������������Խ�ӽ�       
	double max_dist = matches[0].distance;
	double min_dist = matches[0].distance;
	for (int i = 1; i<matches.size(); i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	//ɸѡ���Ϻõ�ƥ���       
	vector<DMatch> goodMatches;
	for (int i = 0; i<matches.size(); i++)
	{
		if (matches[i].distance < 0.6 * max_dist)
		{
			goodMatches.push_back(matches[i]);
		}
	}
	//RANSACƥ�����
	vector<DMatch> m_Matches;
	m_Matches = goodMatches;
	int ptCount = goodMatches.size();
	if (ptCount < 100)//ƥ������ ʧ��
	{
		return;
	}

	//����ת��Ϊfloat����
	vector <KeyPoint> RAN_KP1, RAN_KP2;
	//size_t�Ǳ�׼C���ж���ģ�ӦΪunsigned int����64λϵͳ��Ϊlong unsigned int,��C++��Ϊ����Ӧ��ͬ��ƽ̨�����ӿ���ֲ�ԡ�
	for (size_t i = 0; i < m_Matches.size(); i++)
	{
		RAN_KP1.push_back(keypoints1[goodMatches[i].queryIdx]);
		RAN_KP2.push_back(keypoints2[goodMatches[i].trainIdx]);
		//RAN_KP1��Ҫ�洢img01������img02ƥ��ĵ�
		//goodMatches�洢����Щƥ���Ե�img01��img02������ֵ
	}
	//����任
	vector <Point2f> p01, p02;
	for (size_t i = 0; i < m_Matches.size(); i++)
	{
		p01.push_back(RAN_KP1[i].pt);
		p02.push_back(RAN_KP2[i].pt);
	}
	
	vector<uchar> RansacStatus;
	Mat Fundamental = findFundamentalMat(p01, p02, RansacStatus, FM_RANSAC);
	//���¶���ؼ���RR_KP��RR_matches���洢�µĹؼ���ͻ�������ͨ��RansacStatus��ɾ����ƥ���
	vector <KeyPoint> RR_KP1, RR_KP2;
	vector <DMatch> RR_matches;
	int index = 0;
	for (size_t i = 0; i < m_Matches.size(); i++)
	{
		if (RansacStatus[i] != 0)
		{
			RR_KP1.push_back(RAN_KP1[i]);
			RR_KP2.push_back(RAN_KP2[i]);
			m_Matches[i].queryIdx = index;
			m_Matches[i].trainIdx = index;
			RR_matches.push_back(m_Matches[i]);
			index++;
		}
	}


	drawMatches(srcImage1, RR_KP1, srcImage2, RR_KP2, RR_matches, resultMat);

}
void CExperimentImgDlg::kdtree(Mat descriptors1, Mat descriptors2) {

	const float minRatio = 1.f / 1.5f;
	const int k = 2;
	FlannBasedMatcher matcher(new flann::KDTreeIndexParams(5), new flann::SearchParams(50));
	vector<vector<DMatch>> knnmatches;
	matcher.knnMatch(descriptors1, descriptors2, knnmatches, k);

	vector<DMatch> KDMatch;
	for (size_t i = 0; i < knnmatches.size(); i++) {
		const DMatch& bestMatch = knnmatches[i][0];
		const DMatch& betterMatch = knnmatches[i][1];

		float  distanceRatio = bestMatch.distance / betterMatch.distance;
		if (distanceRatio < minRatio)
			KDMatch.push_back(bestMatch);
	}

	drawMatches(srcImage1, keypoints1, srcImage2, keypoints2, KDMatch, resultMat);
}
void CExperimentImgDlg::cross(Mat descriptors1, Mat descriptors2) {

	vector<DMatch> hashmatches;
	BFMatcher matcher(NORM_HAMMING, true);
	matcher.match(descriptors1, descriptors2, hashmatches);

	drawMatches(srcImage1, keypoints1, srcImage2, keypoints2,hashmatches, resultMat);
}

void CExperimentImgDlg::OnBnClickedButton2()//���
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (imgSrc1 != NULL)
	{
		imgSrc1->Destroy();
		delete imgSrc1;
		imgSrc1 = new CImage();
	}
	if (imgSrc2 != NULL)
	{
		imgSrc2->Destroy();
		delete imgSrc2;
		imgSrc2 = new CImage();
	}
	if (result != NULL)
	{
		result->Destroy();
		delete result;
		result = new CImage();
	}
	if (original != NULL)
	{
		original->Destroy();
		delete original;
		original = new CImage();
	}
	
	this->Invalidate();
}

void CExperimentImgDlg::OnBnClickedButtonOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//TerminateThread((AFX_THREADPROC)&CExperimentImgDlg::Update, 1);
	TCHAR szFilter[] = _T("JPEG(*jpg)|*.jpg|*.bmp|*.png|TIFF(*.tif)|*.tif|All Files ��*.*��|*.*||");
	CString filePath("");

	CFileDialog fileOpenDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (fileOpenDialog.DoModal() == IDOK)
	{
		VERIFY(filePath = fileOpenDialog.GetPathName());
		CString strFilePath(filePath);
		//		mEditInfo.SetWindowTextW(strFilePath);	//���ı�������ʾͼ��·��

		if (imgSrc1 != NULL)
		{
			imgSrc1->Destroy();
			delete imgSrc1;
		}
		imgSrc1 = new CImage();
		imgSrc1->Load(strFilePath);
		this->Invalidate();

	}
	//AfxBeginThread((AFX_THREADPROC)&CExperimentImgDlg::Update, this);
}

void CExperimentImgDlg::OnBnClickedButtonOpen2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//TerminateThread((AFX_THREADPROC)&CExperimentImgDlg::Update, 1);
	TCHAR szFilter[] = _T("JPEG(*jpg)|*.jpg|*.bmp|*.png|TIFF(*.tif)|*.tif|All Files ��*.*��|*.*||");
	CString filePath("");

	CFileDialog fileOpenDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (fileOpenDialog.DoModal() == IDOK)
	{
		VERIFY(filePath = fileOpenDialog.GetPathName());
		CString strFilePath(filePath);
		//		mEditInfo.SetWindowTextW(strFilePath);	//���ı�������ʾͼ��·��

		if (imgSrc2 != NULL)
		{
			imgSrc2->Destroy();
			delete imgSrc2;
		}
		imgSrc2 = new CImage();
		imgSrc2->Load(strFilePath);
		this->Invalidate();

	}
	//AfxBeginThread((AFX_THREADPROC)&CExperimentImgDlg::Update, this);
}

void CExperimentImgDlg::CImageToMat(CImage *cimage, Mat& mat)
{
	if (cimage==NULL)
	{
		return;
	}


	int nChannels = cimage->GetBPP() / 8;
	if ((1 != nChannels) && (3 != nChannels))
	{
		return;
	}
	int nWidth = cimage->GetWidth();
	int nHeight = cimage->GetHeight();


	//�ؽ�mat
	if (1 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC1);
	}
	else if (3 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC3);
	}


	//��������


	uchar* pucRow;									//ָ������������ָ��
	uchar* pucImage = (uchar*)cimage->GetBits();		//ָ����������ָ��
	int nStep = cimage->GetPitch();					//ÿ�е��ֽ���,ע���������ֵ�����и�


	for (int nRow = 0; nRow < nHeight; nRow++)
	{
		pucRow = (mat.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++)
		{
			if (1 == nChannels)
			{
				pucRow[nCol] = *(pucImage + nRow * nStep + nCol);
			}
			else if (3 == nChannels)
			{
				for (int nCha = 0; nCha < 3; nCha++)
				{
					pucRow[nCol * 3 + nCha] = *(pucImage + nRow * nStep + nCol * 3 + nCha);
				}
			}
		}
	}
}

void CExperimentImgDlg::MatToCImage(CImage *cimage, Mat& mat)
{
	if (0 == mat.total())
	{
		return;
	}


	int nChannels = mat.channels();
	if ((1 != nChannels) && (3 != nChannels))
	{
		return;
	}
	int nWidth = mat.cols;
	int nHeight = mat.rows;


	//�ؽ�cimage
	cimage->Destroy();
	cimage->Create(nWidth, nHeight, 8 * nChannels);


	//��������


	uchar* pucRow;									//ָ������������ָ��
	uchar* pucImage = (uchar*)cimage->GetBits();		//ָ����������ָ��
	int nStep = cimage->GetPitch();					//ÿ�е��ֽ���,ע���������ֵ�����и�


	if (1 == nChannels)								//���ڵ�ͨ����ͼ����Ҫ��ʼ����ɫ��
	{
		RGBQUAD* rgbquadColorTable;
		int nMaxColors = 256;
		rgbquadColorTable = new RGBQUAD[nMaxColors];
		cimage->GetColorTable(0, nMaxColors, rgbquadColorTable);
		for (int nColor = 0; nColor < nMaxColors; nColor++)
		{
			rgbquadColorTable[nColor].rgbBlue = (uchar)nColor;
			rgbquadColorTable[nColor].rgbGreen = (uchar)nColor;
			rgbquadColorTable[nColor].rgbRed = (uchar)nColor;
		}
		cimage->SetColorTable(0, nMaxColors, rgbquadColorTable);
		delete[]rgbquadColorTable;
	}


	for (int nRow = 0; nRow < nHeight; nRow++)
	{
		pucRow = (mat.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++)
		{
			if (1 == nChannels)
			{
				*(pucImage + nRow * nStep + nCol) = pucRow[nCol];
			}
			else if (3 == nChannels)
			{
				for (int nCha = 0; nCha < 3; nCha++)
				{
					*(pucImage + nRow * nStep + nCol * 3 + nCha) = pucRow[nCol * 3 + nCha];
				}
			}
		}
	}



}
