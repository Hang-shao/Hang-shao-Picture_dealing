#pragma once
#include "afxwin.h"

#define WM_MyMessage (WM_USER + n)
// newform2 �Ի���

class newform2 : public CDialogEx
{
	DECLARE_DYNAMIC(newform2)

public:
	newform2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~newform2();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int canshu;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};

