#pragma once

#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

// CEncryptionMFCDlg �Ի���
class CEncryptionMFCDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEncryptionMFCDlg)
	
	// ����
public:
	CEncryptionMFCDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CEncryptionMFCDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENCRYPTION_MFC_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	LPVOID adlg;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	//afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedChooseImg();
public:

	void WndCreate(LPVOID pParent);

	//��ӳ�Ա�������б��
	int count_n;
	int count_m;
	//ԭͼ�����ܡ�����ͼ������
	cv::Mat lena;
	cv::Mat encryption_lena;
	cv::Mat decryption_lena;
	CProgressCtrl EnProgress;
	CProgressCtrl DeProgress;
	//���Ŵ�������
	CList<CRect, CRect&> m_listRect;

	afx_msg void DrawMat(cv::Mat& img, UINT nID);
	afx_msg void OnBnClickedEncrypting();
	afx_msg void OnBnClickedDecrypting();
	afx_msg void EncrytingMat(cv::Mat& Enimg, const double Enx_chaos[], const double Eny_chaos[]);
	afx_msg void DecrytingMat(cv::Mat& Deimg, const double Enx_chaos[], const double Eny_chaos[]);
	afx_msg void OnBnClickedSaveEncrypting();
	afx_msg void OnBnClickedSaveDecrypting();
	afx_msg void OnBnClickedLoadencryption2enWindow();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedUnchangedChooseImg();
};
//����ѭ�����ƺ�������
template <typename T>
void array_rightshift(std::vector<T>& a, int& N);
//����ѭ�����ƺ�������
template <typename T>
void array_rightshift(std::vector<T>& a, int& N)
{
	//���ÿռ任ʱ�䣬�����ȳ���ʱ����
	//��ԭ�����num_shift1λ����λλ����Ԫ�ط�����ʱ����ǰ�棬��ԭ����ǰ���Ԫ�ط�����ʱ������棬����ʱ���鸳ֵ��ԭ����
	int bitnum = static_cast<int>(a.size());
	int num_shift1 = N % bitnum;
	std::vector<T> temp(bitnum);
	for (int i = 0; i < num_shift1; i++)
	{
		temp[i] = a[bitnum - num_shift1 + i];
	}
	for (int j = 0; j < bitnum - num_shift1; j++)
	{
		temp[j + num_shift1] = a[j];
	}
	for (int k = 0; k < bitnum; k++)
	{
		a[k] = temp[k];
	}
}
