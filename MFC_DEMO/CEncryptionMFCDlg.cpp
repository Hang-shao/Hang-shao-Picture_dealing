#include "stdafx.h"
//#include "Encryption_MFC.h"
#include "CEncryptionMFCDlg.h"
#include "afxdialogex.h"
#include <stdio.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <opencv2\imgproc\types_c.h>
#include "stdafx.h"
#include "MFC_DEMO.h"
#include "MFC_DEMODlg.h"
#include <opencv2/opencv.hpp>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef PI
#define PI 3.141592658
#endif

using namespace cv;
using namespace std;

IMPLEMENT_DYNAMIC(CEncryptionMFCDlg, CDialogEx)


// CEncryptionMFCDlg �Ի���
CEncryptionMFCDlg::CEncryptionMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ENCRYPTION_MFC_DIALOG, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CEncryptionMFCDlg::~CEncryptionMFCDlg()
{
}
void CEncryptionMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EN_PROGRESS, EnProgress);
	DDX_Control(pDX, IDC_DE_PROGRESS, DeProgress);
}

void CEncryptionMFCDlg::WndCreate(LPVOID pParent)

{
	if (adlg == NULL) {
		Create(IDD_ENCRYPTION_MFC_DIALOG);                         //�����Ի���

		ShowWindow(SW_SHOW);                         //��ʾ�Ի���

		adlg = pParent;                         //��������ָ�봫�ݽ���
	}
}

BEGIN_MESSAGE_MAP(CEncryptionMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHOOSE_IMG, &CEncryptionMFCDlg::OnBnClickedChooseImg)
	ON_BN_CLICKED(IDC_ENCRYPTING, &CEncryptionMFCDlg::OnBnClickedEncrypting)
	ON_BN_CLICKED(IDC_DECRYPTING, &CEncryptionMFCDlg::OnBnClickedDecrypting)
	ON_BN_CLICKED(IDC_SAVE_ENCRYPTING, &CEncryptionMFCDlg::OnBnClickedSaveEncrypting)
	ON_BN_CLICKED(IDC_SAVE_DECRYPTING, &CEncryptionMFCDlg::OnBnClickedSaveDecrypting)
	ON_BN_CLICKED(IDC_LOADENCRYPTION2EN_WINDOW, &CEncryptionMFCDlg::OnBnClickedLoadencryption2enWindow)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_UNCHANGED_CHOOSE_IMG, &CEncryptionMFCDlg::OnBnClickedUnchangedChooseImg)
END_MESSAGE_MAP()


// CEncryptionMFCDlg ��Ϣ�������

BOOL CEncryptionMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//���ý�����En��ɫ
	::SendMessageA(EnProgress.GetSafeHwnd(), PBM_SETBARCOLOR, 0, RGB(255, 250, 250));
	EnProgress.SetBarColor(RGB(67, 205, 128));
	//���ý�����De��ɫ
	::SendMessageA(DeProgress.GetSafeHwnd(), PBM_SETBARCOLOR, 0, RGB(255, 250, 250));
	DeProgress.SetBarColor(RGB(67, 205, 128));

	//��ȡ���д���λ�ô�С
	CRect rect;
	GetWindowRect(&rect);
	m_listRect.AddTail(rect);//�Ի��������
	CWnd* pWnd = GetWindow(GW_CHILD);//��ȡ�Ӵ���
	while (pWnd)
	{
		pWnd->GetWindowRect(rect);//�Ӵ��������
		m_listRect.AddTail(rect);           //CList<CRect,CRect> m_listRect��Ա����
		pWnd = pWnd->GetNextWindow();//ȡ��һ���Ӵ���
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CEncryptionMFCDlg::OnPaint()
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
		CDialog::UpdateWindow();
	}
}

HCURSOR CEncryptionMFCDlg::OnQueryDragIcon()
{
	return HCURSOR();
}

//ѡ��ͼƬ�ؼ���Ӧ����
void CEncryptionMFCDlg::OnBnClickedChooseImg()
{
	CString picPath;   //����ͼƬ·������  
	//CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY |
		//OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, NULL, this);   //ѡ���ļ��Ի���  
	CFileDialog dlg(true, _T("*.tiff;*.bmp; *.tif;*.jpg"), NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image files (*.tiff;*.bmp ;*.jpg;)|*.tiff;*.bmp; *.jpg |ALL Files (*.*) |*.*||"), NULL);


	//���ļ��Ի���ı�����
	dlg.m_ofn.lpstrTitle = _T("ѡ��ͼƬ");
	if (dlg.DoModal() == IDOK)
	{
		picPath = dlg.GetPathName();  //��ȡͼƬ·��  
	}
	//CString to string  ʹ����������ǵ��ַ���ѡ�á�ʹ�ö��ֽ��ַ�������Ȼ�ᱨ��  
	std::string picpath = picPath.GetBuffer(0);
	lena = cv::imread(picpath, cv::IMREAD_COLOR);
	//lena = cv::imread("D:\\code\\C\\vs\\Hyperchaotic\\lena512color.tiff", cv::IMREAD_COLOR);
	if (lena.empty())
	{
		MessageBox(_T("��ȡ�ļ�Ϊ�գ���ѡ��һ��ͼ���ļ�"));
		return;
	}
	else if (lena.rows * lena.cols < 1024 * 1024)
	{
		;
	}
	else if (lena.rows * lena.cols < 2048 * 2048)
	{
		lena = cv::imread(picpath, cv::IMREAD_REDUCED_COLOR_2);
	}
	else if (lena.rows * lena.cols < 4096 * 4096)
	{
		lena = cv::imread(picpath, cv::IMREAD_REDUCED_COLOR_4);
	}
	else
	{
		lena = cv::imread(picpath, cv::IMREAD_REDUCED_COLOR_8);
	}

	SetDlgItemText(IDC_ImagePath, picPath);
	DrawMat(lena, IDC_ORIGINAL1);
	AfxMessageBox("ͼ���ȡ���");

	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
void CEncryptionMFCDlg::OnBnClickedUnchangedChooseImg()
{
	CString picPath;   //����ͼƬ·������  
//CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY |
	//OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, NULL, this);   //ѡ���ļ��Ի���  
	CFileDialog dlg(true, _T("*.tiff;*.bmp; *.tif;*.jpg"), NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image files (*.tiff;*.bmp ;*.jpg;)|*.tiff;*.bmp; *.jpg |ALL Files (*.*) |*.*||"), NULL);


	//���ļ��Ի���ı�����
	dlg.m_ofn.lpstrTitle = _T("ѡ��ͼƬ");
	if (dlg.DoModal() == IDOK)
	{
		picPath = dlg.GetPathName();  //��ȡͼƬ·��  
	}
	//CString to string  ʹ����������ǵ��ַ���ѡ�á�ʹ�ö��ֽ��ַ�������Ȼ�ᱨ��  
	std::string picpath = picPath.GetBuffer(0);
	lena = cv::imread(picpath, cv::IMREAD_COLOR);
	if (lena.empty())
	{
		MessageBox(_T("��ȡ�ļ�Ϊ�գ���ѡ��һ��ͼ���ļ�"));
		return;
	}
	SetDlgItemText(IDC_ImagePath, picPath);
	DrawMat(lena, IDC_ORIGINAL1);
	AfxMessageBox("ͼ���ȡ���");
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}



//���ܿؼ���Ӧ����
void CEncryptionMFCDlg::OnBnClickedEncrypting()
{
	CString str_encryp_password;
	//����������ؼ���ȡ���룬����str_encryp_passward
	GetDlgItem(IDC_ENCRYPTION_PASSWORD)->GetWindowText(str_encryp_password);
	if (lena.empty())
	{
		AfxMessageBox("δ����ͼ����������һ��ԭʼͼ��");
		return;
	}
	else if (str_encryp_password.IsEmpty())
	{
		AfxMessageBox("����Ϊ�գ�����������8λ���룡");
		return;
	}
	else if (str_encryp_password.GetLength() != 8)
	{
		AfxMessageBox("���볤�ȴ�������������8λ���룡");
		return;
	}
	for (int i = 0; i < str_encryp_password.GetLength(); ++i)
	{
		if (str_encryp_password[i] < '0' || str_encryp_password[i]>'9')
		{
			AfxMessageBox("�����������ʹ��������뷶Χ��0-9֮������֣�");
			return;
		}
	}
	//��̬������������ڴ�
	double* x_chaos = new double[lena.rows * lena.cols * lena.channels()];
	double* y_chaos = new double[lena.rows * lena.cols * lena.channels()];
	//���ͼ��������Ϣ��ʼ����ֵ
	double tempx_last = 0.1 * (str_encryp_password[0] - '0') + 0.01 * (str_encryp_password[1] - '0');
	double tempy_last = 0.1 * (str_encryp_password[2] - '0') + 0.01 * (str_encryp_password[3] - '0');
	double tempz_last = 0.1 * (str_encryp_password[4] - '0') + 0.01 * (str_encryp_password[5] - '0');
	double tempw_last = 0.1 * (str_encryp_password[6] - '0') + 0.01 * (str_encryp_password[7] - '0');
	double tempx_new, tempy_new, tempz_new, tempw_new;
	//��ɢ����ʱ��
	double tt0 = 0.001;
	//����ȥ��Ԫ�ظ���
	int num_ignore = 100000;
	//������������
	for (int i = 0; i < lena.rows * lena.cols * lena.channels() + num_ignore; ++i)
	{
		tempx_new = tempx_last + tt0 * (24 * (tempy_last - tempx_last) + 4 * (1 + 0.02 * tempw_last * tempw_last) * tempy_last);
		tempy_new = tempy_last + tt0 * (19 * tempx_last - tempx_last * tempz_last);
		tempz_new = tempz_last + tt0 * (tempx_last * tempx_last - 9 * tempz_last);
		tempw_new = tempw_last + tt0 * tempy_last;
		//��ȥǰ10000�ε������������˲̬��Ϊ
		if (i >= num_ignore)
		{
			*(x_chaos + i - num_ignore) = tempx_new;
			*(y_chaos + i - num_ignore) = tempy_new;
		}
		tempx_last = tempx_new;
		tempy_last = tempy_new;
		tempz_last = tempz_new;
		tempw_last = tempw_new;
	}
	//�Ի������н��д���
	double x_max = *std::max_element(x_chaos, x_chaos + lena.rows * lena.cols * lena.channels() - 1);
	double x_min = *std::min_element(x_chaos, x_chaos + lena.rows * lena.cols * lena.channels() - 1);
	double y_max = *std::max_element(y_chaos, y_chaos + lena.rows * lena.cols * lena.channels() - 1);
	double y_min = *std::min_element(y_chaos, y_chaos + lena.rows * lena.cols * lena.channels() - 1);
	for (int i = 0; i < lena.rows * lena.cols * lena.channels(); ++i)
	{
		//x��һ��,���Ǳ任��ȡģ
		*(x_chaos + i) = (*(x_chaos + i) - x_min) / (x_max - x_min);
		*(x_chaos + i) = 1.0 / PI * asin(sqrt(*(x_chaos + i)));
		*(x_chaos + i) = int(floor(pow(10, 8) * (*(x_chaos + i)))) % 256;
		*(y_chaos + i) = (*(y_chaos + i) - y_min) / (y_max - y_min);
		*(y_chaos + i) = 1.0 / PI * asin(sqrt(*(y_chaos + i)));
		*(y_chaos + i) = int(floor(pow(10, 8) * (*(y_chaos + i)))) % 256;
	}

	//��ʼ�����ܱ��
	count_n = 0;
	count_m = 0;
	//���ý�������Χ������
	EnProgress.SetRange32(0, lena.rows * lena.cols * lena.channels() - 1);
	int nFirstStep = EnProgress.SetStep(1);
	//���ü��ܺ�����ͼ����м���
	EncrytingMat(lena, x_chaos, y_chaos);
	//����ͼ��
	DrawMat(encryption_lena, IDC_ENCRYPTION1);
	AfxMessageBox("ͼ��������");

	//�ͷſռ��ֹ�ڴ�й©
	//�ÿ�ָ��
	delete[]x_chaos;
	delete[]y_chaos;
	x_chaos = nullptr;
	y_chaos = nullptr;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
//���ܿؼ���Ӧ����
void CEncryptionMFCDlg::OnBnClickedDecrypting()
{

	CString str_decryp_password;
	//����������ؼ���ȡ���룬����str_encryp_passward
	GetDlgItem(IDC_DECRYPTION_PASSWORD)->GetWindowText(str_decryp_password);
	if (encryption_lena.empty())
	{
		AfxMessageBox("δ�������ͼ�����ȶ�ԭʼͼ����м��ܣ�");
		return;
	}
	else if (str_decryp_password.IsEmpty())
	{
		AfxMessageBox("����Ϊ�գ�����������8λ���룡");
		return;
	}
	else if (str_decryp_password.GetLength() != 8)
	{
		AfxMessageBox("���볤�ȴ�������������8λ���룡");
		return;
	}
	for (int i = 0; i < str_decryp_password.GetLength(); ++i)
	{
		if (str_decryp_password[i] < '0' || str_decryp_password[i]>'9')
		{
			AfxMessageBox("�����������ʹ��������뷶Χ��0-9֮������֣�");
			return;
		}
	}
	//��̬������������ڴ�
	double* x_chaos = new double[encryption_lena.rows * encryption_lena.cols * encryption_lena.channels()];
	double* y_chaos = new double[encryption_lena.rows * encryption_lena.cols * encryption_lena.channels()];
	//���ͼ��������Ϣ��ʼ����ֵ
	double tempx_last = 0.1 * (str_decryp_password[0] - '0') + 0.01 * (str_decryp_password[1] - '0');
	double tempy_last = 0.1 * (str_decryp_password[2] - '0') + 0.01 * (str_decryp_password[3] - '0');
	double tempz_last = 0.1 * (str_decryp_password[4] - '0') + 0.01 * (str_decryp_password[5] - '0');
	double tempw_last = 0.1 * (str_decryp_password[6] - '0') + 0.01 * (str_decryp_password[7] - '0');
	double tempx_new, tempy_new, tempz_new, tempw_new;
	//��ɢ����ʱ��
	double tt0 = 0.001;
	//����ȥ��Ԫ�ظ���
	int num_ignore = 100000;
	//������������
	for (int i = 0; i < encryption_lena.rows * encryption_lena.cols * encryption_lena.channels() + num_ignore; ++i)
	{
		tempx_new = tempx_last + tt0 * (24 * (tempy_last - tempx_last) + 4 * (1 + 0.02 * tempw_last * tempw_last) * tempy_last);
		tempy_new = tempy_last + tt0 * (19 * tempx_last - tempx_last * tempz_last);
		tempz_new = tempz_last + tt0 * (tempx_last * tempx_last - 9 * tempz_last);
		tempw_new = tempw_last + tt0 * tempy_last;
		//��ȥǰ10000�ε������������˲̬��Ϊ
		if (i >= num_ignore)
		{
			*(x_chaos + i - num_ignore) = tempx_new;
			*(y_chaos + i - num_ignore) = tempy_new;
		}
		tempx_last = tempx_new;
		tempy_last = tempy_new;
		tempz_last = tempz_new;
		tempw_last = tempw_new;
	}
	//�Ի������н��д���
	double x_max = *std::max_element(x_chaos, x_chaos + encryption_lena.rows * encryption_lena.cols * encryption_lena.channels() - 1);
	double x_min = *std::min_element(x_chaos, x_chaos + encryption_lena.rows * encryption_lena.cols * encryption_lena.channels() - 1);
	double y_max = *std::max_element(y_chaos, y_chaos + encryption_lena.rows * encryption_lena.cols * encryption_lena.channels() - 1);
	double y_min = *std::min_element(y_chaos, y_chaos + encryption_lena.rows * encryption_lena.cols * encryption_lena.channels() - 1);
	for (int i = 0; i < encryption_lena.rows * encryption_lena.cols * encryption_lena.channels(); ++i)
	{
		//x��һ��,���Ǳ任��ȡģ
		*(x_chaos + i) = (*(x_chaos + i) - x_min) / (x_max - x_min);
		*(x_chaos + i) = 1.0 / PI * asin(sqrt(*(x_chaos + i)));
		*(x_chaos + i) = int(floor(pow(10, 8) * (*(x_chaos + i)))) % 256;
		*(y_chaos + i) = (*(y_chaos + i) - y_min) / (y_max - y_min);
		*(y_chaos + i) = 1.0 / PI * asin(sqrt(*(y_chaos + i)));
		*(y_chaos + i) = int(floor(pow(10, 8) * (*(y_chaos + i)))) % 256;
	}

	//���ý�������Χ������
	DeProgress.SetRange32(0, encryption_lena.rows * encryption_lena.cols * encryption_lena.channels() - 1);
	int nFirstStep = DeProgress.SetStep(1);
	//���ý��ܺ�����ͼ����н���
	DecrytingMat(encryption_lena, x_chaos, y_chaos);
	//����ͼ��
	DrawMat(decryption_lena, IDC_DECRYPTION1);
	AfxMessageBox("ͼ��������");

	//�ͷſռ��ֹ�ڴ�й©
	//�ÿ�ָ��
	delete[]x_chaos;
	delete[]y_chaos;
	x_chaos = nullptr;
	y_chaos = nullptr;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

//������ܿؼ���Ӧ����
void CEncryptionMFCDlg::OnBnClickedSaveEncrypting()
{
	if (encryption_lena.empty())
	{
		AfxMessageBox("δ��⵽����ͼ�����Ƚ��м���");
		return;
	}
	CString picPath;   //����ͼƬ·������  
	CString FileName;   //����ͼƬ·������ 
	//CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY |
	//OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, NULL, this);   //ѡ���ļ��Ի���  
	CFileDialog dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("image files (*.tiff;*.bmp ;*.jpg;)|*.tiff;*.bmp; *.jpg |ALL Files (*.*) |*.*||"), AfxGetMainWnd());


	//���ļ��Ի���ı�����
	dlg.m_ofn.lpstrTitle = _T("�������ͼƬ");
	if (dlg.DoModal() == IDOK)
	{
		picPath = dlg.GetPathName();  //��ȡͼƬ·��
		FileName = dlg.GetFileName(); //��ȡ�ļ���
		picPath += _T(".bmp");
		USES_CONVERSION;
		if (!picPath.IsEmpty())
		{
			//CString to string  ʹ����������ǵ��ַ���ѡ�á�ʹ�ö��ֽ��ַ�������Ȼ�ᱨ��  
			std::string picpath = picPath.GetBuffer(0);
			cv::imwrite(picpath, encryption_lena);
			AfxMessageBox("����ͼ�񱣴�ɹ�");
			//�������
		}
	}

	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

//������ܿؼ���Ӧ����
void CEncryptionMFCDlg::OnBnClickedSaveDecrypting()
{
	if (decryption_lena.empty())
	{
		AfxMessageBox("δ��⵽����ͼ�����Ƚ��м���");
		return;
	}
	CString picPath;   //����ͼƬ·������  
	CString FileName;   //����ͼƬ·������ 
	//CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY |
	//OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, NULL, this);   //ѡ���ļ��Ի���  
	CFileDialog dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("image files (*.tiff;*.bmp ;*.jpg;)|*.tiff;*.bmp; *.jpg |ALL Files (*.*) |*.*||"), AfxGetMainWnd());

	//���ļ��Ի���ı�����
	dlg.m_ofn.lpstrTitle = _T("�������ͼƬ");
	if (dlg.DoModal() == IDOK)
	{
		picPath = dlg.GetPathName();  //��ȡͼƬ·��
		FileName = dlg.GetFileName(); //��ȡ�ļ���
		picPath += _T(".bmp");
		USES_CONVERSION;
		if (!picPath.IsEmpty())
		{
			//CString to string  ʹ����������ǵ��ַ���ѡ�á�ʹ�ö��ֽ��ַ�������Ȼ�ᱨ��  
			std::string picpath = picPath.GetBuffer(0);
			cv::imwrite(picpath, decryption_lena);
			AfxMessageBox("����ͼ�񱣴�ɹ�");
			//�������
		}
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

//������ͼ�����ܴ��ڿؼ���Ӧ����
void CEncryptionMFCDlg::OnBnClickedLoadencryption2enWindow()
{
	CString picPath;   //����ͼƬ·������  
//CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY |
	//OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, NULL, this);   //ѡ���ļ��Ի���  
	CFileDialog dlg(true, _T("*.tiff;*.bmp; *.tif;*.jpg"), NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image files (*.tiff;*.bmp ;*.jpg;)|*.tiff;*.bmp; *.jpg |ALL Files (*.*) |*.*||"), NULL);


	//���ļ��Ի���ı�����
	dlg.m_ofn.lpstrTitle = _T("ѡ��ͼƬ");
	if (dlg.DoModal() == IDOK)
	{
		picPath = dlg.GetPathName();  //��ȡͼƬ·��  
	}
	//CString to string  ʹ����������ǵ��ַ���ѡ�á�ʹ�ö��ֽ��ַ�������Ȼ�ᱨ��  
	std::string picpath = picPath.GetBuffer(0);
	encryption_lena = cv::imread(picpath, cv::IMREAD_COLOR);
	if (encryption_lena.empty())
	{
		MessageBox(_T("��ȡ�ļ�Ϊ�գ���ѡ��һ��ͼ���ļ�"));
		return;
	}
	SetDlgItemText(IDC_ImagePath1, picPath);
	DrawMat(encryption_lena, IDC_ENCRYPTION1);
	if (!encryption_lena.empty())
	{
		AfxMessageBox("����ͼ���ȡ�ɹ�");
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
//��ͼ����
void CEncryptionMFCDlg::DrawMat(cv::Mat& img, UINT nID)
{
	cv::Mat imgTmp;
	CRect rect;
	GetDlgItem(nID)->GetClientRect(&rect);  // ��ȡ�ؼ���С
	cv::resize(img, imgTmp, cv::Size(rect.Width(), rect.Height()));// ��С��Ŵ�Mat������

	// תһ�¸�ʽ ,��ο��Է�����,
	switch (imgTmp.channels())
	{
	case 1:
		cv::cvtColor(imgTmp, imgTmp, CV_GRAY2BGRA); // GRAY��ͨ��
		break;
	case 3:
		cv::cvtColor(imgTmp, imgTmp, CV_BGR2BGRA);  // BGR��ͨ��
		break;
	default:
		break;
	}

	int pixelBytes = imgTmp.channels() * (imgTmp.depth() + 1); // ����һ�����ض��ٸ��ֽ� 
	// ����bitmapinfo(����ͷ)
	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = 8 * pixelBytes;
	bitInfo.bmiHeader.biWidth = imgTmp.cols;
	bitInfo.bmiHeader.biHeight = -imgTmp.rows;
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant = 0;
	bitInfo.bmiHeader.biClrUsed = 0;
	bitInfo.bmiHeader.biSizeImage = 0;
	bitInfo.bmiHeader.biXPelsPerMeter = 0;
	bitInfo.bmiHeader.biYPelsPerMeter = 0;
	// Mat.data + bitmap����ͷ -> MFC
	CDC* pDC = GetDlgItem(nID)->GetDC();
	::StretchDIBits(
		pDC->GetSafeHdc(),
		0, 0, rect.Width(), rect.Height(),
		0, 0, rect.Width(), rect.Height(),
		imgTmp.data,
		&bitInfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);
	ReleaseDC(pDC);
}
//����ͼ����ܺ���
void CEncryptionMFCDlg::EncrytingMat(cv::Mat& Leimg, const double Enx_chaos[], const double Eny_chaos[])
{
	std::vector<int> lenarow(Leimg.cols);
	encryption_lena = Leimg.clone();
	//�����任λ������
	int num_shf;

	int tempCount_n = 0;
	int tempCount_m = 0;
	//��������λ����ɢ
	//ע������б任ʹ�õ���x��������
	for (int k = 0; k < Leimg.channels(); ++k)
	{
		for (int i = 0; i < Leimg.rows; ++i)
		{
			//��ȡͼ�����������ݵ�lenarow����
			for (int j = 0; j < Leimg.cols; ++j)
			{
				lenarow[j] = Leimg.at<cv::Vec3b>(i, j)[k];
			}
			//ǿ������ת��������x����������ȷ������λ��λ��
			num_shf = static_cast<int>(*(Enx_chaos + tempCount_n));
			//�����Զ��������ѭ�����ƺ�������ͼ�������н���ѭ����λ
			array_rightshift(lenarow, num_shf);
			//�ж���Ϊ��һ�У���ֱ����x�������н�����򣻲�Ϊ��һ�У�������ͼ�����ǰһ���������x�����������
			if (i == 0)
			{
				for (int j = 0; j < Leimg.cols; ++j)
				{
					//���������ؾ���ֵ
					int temp = lenarow[j] ^ static_cast<int>(*(Enx_chaos + tempCount_n));
					encryption_lena.at<cv::Vec3b>(i, j)[k] = temp;
					++tempCount_n;
				}
			}
			else
			{
				for (int j = 0; j < Leimg.cols; j++)
				{
					//���������ؾ���ֵ
					int temp = lenarow[j] ^ static_cast<int>(encryption_lena.at<cv::Vec3b>(i - 1, j)[k]);
					temp = temp ^ static_cast<int>(*(Enx_chaos + tempCount_n));
					encryption_lena.at<cv::Vec3b>(i, j)[k] = temp;
					++tempCount_n;
				}
			}
		}
	}

	std::vector<int> lenacols(Leimg.rows);
	//��������λ����ɢ
	//ע������б任ʹ�õ���y��������
	for (int k = 0; k < Leimg.channels(); ++k)
	{
		for (int j = 0; j < Leimg.cols; ++j)
		{
			//��ȡͼ�����������ݵ�lenacols����
			for (int i = 0; i < Leimg.rows; ++i)
			{
				lenacols[i] = encryption_lena.at<cv::Vec3b>(i, j)[k];
			}
			//ǿ������ת��������y����������ȷ������λ��λ��
			num_shf = static_cast<int>(*(Eny_chaos + tempCount_m));
			//�����Զ��������ѭ�����ƺ�������ͼ�������н���ѭ����λ
			array_rightshift(lenacols, num_shf);
			//�ж���Ϊ��һ�У���ֱ����y�������н�����򣻲�Ϊ��һ�У�������ͼ�����ǰһ���������y�����������
			if (j == 0)
			{
				for (int i = 0; i < Leimg.rows; ++i)
				{
					//���������ؾ���ֵ
					int temp = lenacols[i] ^ static_cast<int>(*(Eny_chaos + tempCount_m));
					encryption_lena.at<cv::Vec3b>(i, j)[k] = temp;
					++tempCount_m;
				}
			}
			else
			{
				for (int i = 0; i < Leimg.rows; ++i)
				{
					//���������ؾ���ֵ
					int temp = lenacols[i] ^ encryption_lena.at<cv::Vec3b>(i, j - 1)[k];
					temp = temp ^ static_cast<int>(*(Eny_chaos + tempCount_m));
					encryption_lena.at<cv::Vec3b>(i, j)[k] = temp;
					++tempCount_m;
				}
			}
			EnProgress.SetPos((tempCount_m - 1) * 1);//����һ��λ��
		}
	}
	count_n = tempCount_n;
	count_m = tempCount_m;
}

//����ͼ����ܺ���
void CEncryptionMFCDlg::DecrytingMat(cv::Mat& Enimg, const double Enx_chaos[], const double Eny_chaos[])
{
	//int tempCount_n = count_n - 1;
	//int tempCount_m = count_m - 1;
	int tempCount_n = Enimg.rows * Enimg.cols * Enimg.channels() - 1;
	int tempCount_m = Enimg.rows * Enimg.cols * Enimg.channels() - 1;
	decryption_lena = Enimg.clone();
	std::vector<int> lenacols(Enimg.rows);
	//�������б�������ͼ�����
	for (int k = Enimg.channels() - 1; k >= 0; k--)
	{
		for (int j = Enimg.cols - 1; j >= 0; j--)
		{
			//�ж��Ƿ�Ϊ��һ�У�ִ��������෴�Ĳ���
			if (j == 0)
			{
				for (int i = Enimg.rows - 1; i >= 0; i--)
				{
					lenacols[i] = static_cast<int>(Enimg.at<cv::Vec3b>(i, j)[k]) ^ static_cast<int>(*(Eny_chaos + tempCount_m));
					tempCount_m--;
				}
			}
			else
			{
				for (int i = Enimg.rows - 1; i >= 0; i--)
				{
					lenacols[i] = static_cast<int>(Enimg.at<cv::Vec3b>(i, j)[k]) ^ static_cast<int>(*(Eny_chaos + tempCount_m));
					lenacols[i] = lenacols[i] ^ static_cast<int>(Enimg.at<cv::Vec3b>(i, j - 1)[k]);
					tempCount_m--;
				}
			}
			int num_shf = Enimg.rows - static_cast<int>(*(Eny_chaos + tempCount_m + 1));
			array_rightshift(lenacols, num_shf);
			for (int i = 0; i < Enimg.rows; i++)
			{
				decryption_lena.at<cv::Vec3b>(i, j)[k] = lenacols[i];
			}
		}
	}

	std::vector<int> lenarow(Enimg.cols);
	//�����н���
	//�������б�������ͼ�����
	for (int k = Enimg.channels() - 1; k >= 0; k--)
	{
		for (int i = Enimg.rows - 1; i >= 0; i--)
		{
			//�ж��Ƿ�Ϊ��һ�У�ִ��������෴�Ĳ���
			if (i == 0)
			{
				for (int j = Enimg.cols - 1; j >= 0; j--)
				{
					lenarow[j] = static_cast<int>(decryption_lena.at<cv::Vec3b>(i, j)[k]) ^ static_cast<int>(*(Enx_chaos + tempCount_n));
					tempCount_n--;
				}
			}
			else
			{
				for (int j = Enimg.cols - 1; j >= 0; j--)
				{
					lenarow[j] = static_cast<int>(decryption_lena.at<cv::Vec3b>(i, j)[k]) ^ static_cast<int>(*(Enx_chaos + tempCount_n));
					lenarow[j] = lenarow[j] ^ static_cast<int>(decryption_lena.at<cv::Vec3b>(i - 1, j)[k]);
					tempCount_n--;
				}
			}
			int num_shf = Enimg.cols - static_cast<int>(*(Enx_chaos + tempCount_n + 1));
			array_rightshift(lenarow, num_shf);
			for (int j = 0; j < Enimg.cols; j++)
			{
				decryption_lena.at<cv::Vec3b>(i, j)[k] = lenarow[j];
			}
			DeProgress.SetPos(Enimg.rows * Enimg.cols * Enimg.channels() - tempCount_n);//����һ��λ��
		}
	}
}



void CEncryptionMFCDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (m_listRect.GetCount() > 0)
	{
		CRect dlgNow;
		GetWindowRect(&dlgNow);
		POSITION pos = m_listRect.GetHeadPosition();//��һ��������ǶԻ����Rect  

		CRect dlgSaved;
		dlgSaved = m_listRect.GetNext(pos);
		ScreenToClient(dlgNow);

		float x = dlgNow.Width() * 1.0 / dlgSaved.Width();//���ݵ�ǰ��֮ǰ����ĶԻ���Ŀ�������  
		float y = dlgNow.Height() * 1.0 / dlgSaved.Height();
		ClientToScreen(dlgNow);

		CRect childSaved;

		CWnd* pWnd = GetWindow(GW_CHILD);
		while (pWnd)
		{
			childSaved = m_listRect.GetNext(pos);//���λ�ȡ�Ӵ����Rect  
			childSaved.left = dlgNow.left + (childSaved.left - dlgSaved.left) * x;//���ݱ��������ؼ��������Ҿ���Ի���ľ���  
			childSaved.right = dlgNow.right + (childSaved.right - dlgSaved.right) * x;
			childSaved.top = dlgNow.top + (childSaved.top - dlgSaved.top) * y;
			childSaved.bottom = dlgNow.bottom + (childSaved.bottom - dlgSaved.bottom) * y;
			ScreenToClient(childSaved);
			pWnd->MoveWindow(childSaved);
			pWnd = pWnd->GetNextWindow();
		}
	}
	// TODO: �ڴ˴������Ϣ����������
}


