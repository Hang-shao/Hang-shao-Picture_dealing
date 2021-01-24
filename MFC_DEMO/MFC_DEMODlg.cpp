// MFC_DEMODlg.cpp : ʵ���ļ�
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include <opencv2\tracking.hpp>
#include <stdio.h>
#include <string>
#include "stdafx.h"
#include "MFC_DEMO.h"
#include "MFC_DEMODlg.h"
#include "afxdialogex.h"
#include "NewForm.h"
#include "newform2.h"
#include "newform6.h"


//#include "CEncryptionMFCDlg.h"
using namespace cv;
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

// CMFC_DEMODlg �Ի���
CMFC_DEMODlg::CMFC_DEMODlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFC_DEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC_DEMODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFC_DEMODlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_32774, &CMFC_DEMODlg::On32774)
	ON_COMMAND(ID_32789, &CMFC_DEMODlg::On32789)
	ON_COMMAND(ID_32790, &CMFC_DEMODlg::On32790)
	ON_COMMAND(ID_32794, &CMFC_DEMODlg::On32794)
	ON_COMMAND(ID_32795, &CMFC_DEMODlg::On32795)
	ON_COMMAND(ID_32796, &CMFC_DEMODlg::On32796)
	ON_COMMAND(ID_32798, &CMFC_DEMODlg::On32798)
	ON_STN_CLICKED(IDCANCEL, &CMFC_DEMODlg::OnStnClickedCancel)
	ON_COMMAND(ID_32799, &CMFC_DEMODlg::On32799)
	ON_COMMAND(ID_32793, &CMFC_DEMODlg::On32793)
	ON_COMMAND(ID_32807, &CMFC_DEMODlg::On32807)   //ֱ��ͼ
	ON_COMMAND(ID_32815, &CMFC_DEMODlg::On32815)   //����
	ON_COMMAND(ID_32826, &CMFC_DEMODlg::On32826)   //ʵʱͼ���Ե���
	ON_COMMAND(ID_32811, &CMFC_DEMODlg::On32811)
	ON_COMMAND(ID_32816, &CMFC_DEMODlg::On32816) // �˳�
	ON_COMMAND(ID_32812, &CMFC_DEMODlg::On32812)
	ON_COMMAND(ID_32813, &CMFC_DEMODlg::On32813)
	ON_COMMAND(ID_32814, &CMFC_DEMODlg::On32814)
	ON_COMMAND(ID_32819, &CMFC_DEMODlg::VIDEO_deal_fun)  //������ͷ
	ON_COMMAND(ID_32820, &CMFC_DEMODlg::On32820)  //�������ָ���
	ON_COMMAND(ID_32821, &CMFC_DEMODlg::On32821)  //�ҶȻ�
	ON_COMMAND(ID_32822, &CMFC_DEMODlg::On32822)  //ģ����

	//ON_COMMAND(ID_32825, &CMFC_DEMODlg::On32825)  //����Ƶ
END_MESSAGE_MAP()


// CMFC_DEMODlg ��Ϣ�������

BOOL CMFC_DEMODlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CMenu menu;

	menu.LoadMenuA(IDR_MENU2);
	SetMenu(&menu);

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
	namedWindow("view", WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle("view");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_STATIC)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);

	namedWindow("view2", WINDOW_AUTOSIZE);
	HWND hWnd2 = (HWND)cvGetWindowHandle("view2");
	HWND hParent2 = ::GetParent(hWnd2);
	::SetParent(hWnd2, GetDlgItem(IDD_DIALOG1)->m_hWnd);
	::ShowWindow(hParent2, SW_HIDE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMFC_DEMODlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFC_DEMODlg::OnPaint()
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
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFC_DEMODlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//��ʾ����һ��Pic�ؼ�
void CMFC_DEMODlg::ShowPic_1(Mat input_1)
{
	Mat imagedst; // Mat���ھ����д洢ͼƬ�����ݽṹ
	CRect rect;  //��������࣬��¼һ��������Ϣ���ĸ������ȣ�
	/* GetDlgItem(IDC_STATIC)����ȡ�ؼ�IDC_STATIC���
	   GetClientRect(&rect)����ȡ���ָ��ؼ�����Ĵ�С
	*/
	GetDlgItem(IDC_STATIC)->GetClientRect(&rect);
	//��ȡͼ���С���������ߵȣ�
	Rect dst(rect.left, rect.top, rect.right, rect.bottom); 
	//����ͼ���С�����и�ʽת��
	resize(input_1, imagedst, cv::Size(rect.Width(), rect.Height()));
	imshow("view", imagedst);
}
//��ʾ���ڶ���Pic�ؼ�
void CMFC_DEMODlg::ShowPic_2(Mat input_2)
{
	Mat imagedst;
	CRect rect2;
	GetDlgItem(IDD_DIALOG1)->GetClientRect(&rect2);
	Rect dst2(rect2.left, rect2.top, rect2.right, rect2.bottom);
	resize(input_2, imagedst, cv::Size(rect2.Width(), rect2.Height()));
	imshow("view2", imagedst);
}

//��
void CMFC_DEMODlg::On32774()
{
    open2();
}
//�˳�
void CMFC_DEMODlg::On32816()
{
	OnCancel();
}

//���Ϊ
void CMFC_DEMODlg::On32811()
{
	// TODO: �ڴ���������������
	save2();
}
void CMFC_DEMODlg::OnCancel()
{
	DestroyWindow();
	//exit(0);
	CDialogEx::OnCancel();
}
// ʵʱ��Ƶ����
static void refineSegments(const Mat& img, Mat& mask, Mat& dst);
void CMFC_DEMODlg::VIDEO_deal_fun()
{
	bool update_bg_model = true;
	Mat tmp_frame, bgmask, out_frame, foreground, bw, gray;;
	cap.open(0);
	if (!cap.isOpened())
	{
		AfxMessageBox("������ͷʧ�ܣ�");
		exit(-1);
	}
	cap >> tmp_frame;
	if (tmp_frame.empty())
	{
		AfxMessageBox("��ȡ֡ʧ�ܣ�");
		exit(-1);
	}
	Ptr<BackgroundSubtractorMOG2> bgsubtractor = createBackgroundSubtractorMOG2();
	bgsubtractor->setVarThreshold(10);
	for (;;)
	{
		cap >> tmp_frame;
		if (tmp_frame.empty())
			break;
		bgsubtractor->apply(tmp_frame, bgmask, update_bg_model ? -1 : 0);
		ShowPic_1(tmp_frame);
		if (num_32820 == 1)
		{
			refineSegments(tmp_frame, bgmask, out_frame); // �����ָ�
			ShowPic_2(out_frame);
		}
		else if (num_32821 == 1)
		{
			cvtColor(tmp_frame, out_frame, CV_BGR2GRAY); //�ҶȻ�
			ShowPic_2(out_frame);
		}
		else if (num_32822 == 1)
		{
			blur(tmp_frame, out_frame, Size(7, 7));//ģ����
			ShowPic_2(out_frame);
		}
		else if (num_32826 == 1)
		{
			GaussianBlur(tmp_frame, out_frame, cvSize(3,3), 2, 2, BORDER_DEFAULT);//��˹ƽ��
			Canny(tmp_frame, out_frame,20,100,3);//��Ե���
			ShowPic_2(out_frame);
		}
		else 
		{
			printf("OK!");
		}
		waitKey(30);
	}
	cap.release();//�ͷ���Դ
}

/*ģ����*/
void CMFC_DEMODlg::On32822()
{
	num_32822 = 1;
}

/*�ҶȻ�*/
void CMFC_DEMODlg::On32821()
{
	num_32821 = 1;
}

/*�������ָ���*/
void CMFC_DEMODlg::On32820()
{
	num_32820 = 1;
}
static void refineSegments(const Mat& img, Mat& mask, Mat& dst)
{
	int niters = 3;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Mat temp;
	dilate(mask, temp, Mat(), Point(-1, -1), niters);  // ���ͣ���������������ֵ�����������
	erode(temp, temp, Mat(), Point(-1, -1), niters * 2); // ��ʴ�����������Сֵ��������͹��
	dilate(temp, temp, Mat(), Point(-1, -1), niters);  // ���ͣ���������������ֵ�����������
	/*findContours( InputOutputArray image, OutputArrayOfArrays contours,
					OutputArray hierarchy, int mode,  
                    int method, Point offset=Point());
	*/
	findContours(temp, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);  // ������ȡ
	dst = Mat::zeros(img.size(), CV_8UC3);
	if (contours.size() == 0)
		return;
	int idx = 0, largestComp = 0;
	double maxArea = 0;
	for (; idx >= 0; idx = hierarchy[idx][0])
	{
		const vector<Point>& c = contours[idx];
		double area = fabs(contourArea(Mat(c)));  // �����ֵ
		if (area > maxArea)
		{
			maxArea = area;
			largestComp = idx;
		}
	}
	Scalar color(0, 0, 255); //��ͼ�����óɵ�һ�ҶȺ���ɫ
	drawContours(dst, contours, largestComp, color, FILLED, LINE_8, hierarchy); // �������
}

//�ҶȻ�
void CMFC_DEMODlg::On32789()
{
	// TODO: �ڴ���������������
	out=ImageGray1();
	ShowPic_2(out);
}

//��ֵ��
void CMFC_DEMODlg::On32790()
{
	ImageEZH1();		
}

//��ֵ�˲�
void CMFC_DEMODlg::On32794()
{
	ImageJZ1();
}

//��˹�˲�
void CMFC_DEMODlg::On32795()
{
	// TODO: �ڴ���������������
	 ImageGS1();
}

//������˹�˲�
void CMFC_DEMODlg::On32796()
{
	// TODO: �ڴ���������������
	out = ImageLPLS1();
	ShowPic_2(out);

}

//canny����
void CMFC_DEMODlg::On32798()
{
	 ImageCANNY1();
}

void CMFC_DEMODlg::OnStnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

//sobel����
void CMFC_DEMODlg::On32799()
{
	// TODO: �ڴ���������������
	 ImageSOBEL();
}

//ֱ��ͼ
void CMFC_DEMODlg::On32807()
{
	// TODO: �ڴ���������������
	SonWnd_2.WndCreate(this);
}
//�������
void CMFC_DEMODlg::On32815()
{
	// TODO: �ڴ���������������
	SonWnd_3.WndCreate(this);
}
//Ƶ���˲�
void CMFC_DEMODlg::On32793()
{
	// TODO: �ڴ���������������
	SonWnd.WndCreate2(this);
}

//ʵʱͼ���Ե���
void CMFC_DEMODlg::On32826()
{
	num_32826 = 1;
}

//��ɫ����
void CMFC_DEMODlg::On32812()
{
	// TODO: �ڴ���������������
	ImageHUIDU();
}

//�Ҷȷֶ�����
void CMFC_DEMODlg::On32813()
{
	// TODO: �ڴ���������������
	ImageHUIDU_2();
}

//�Ҷȷ�����
void CMFC_DEMODlg::On32814()
{
	// TODO: �ڴ���������������
	ImageHUIDU_3();
}