// MVCamDlg.h : ͷ�ļ�
//

#pragma once
#include "HalconCpp.h"
using namespace HalconCpp;

#include <string>
#include <vector>
using namespace std;

// CMVCamDlg �Ի���
class CMVCamDlg : public CDialogEx
{
// ����
public:
	CMVCamDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MVCAM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButton1();
    afx_msg LRESULT OnFileNameMsg(WPARAM wParam, LPARAM lParam);

protected:
    wstring CharToWStr(const char *Data);
    void UpdateFileName(const string& fileName);
    void EnumImageFile();
    void HImage2CBitmap(const HObject& pImage, CBitmap *wImage);
    HObject mirrImg;//����ͷ�ɼ�ͼ��Ϊ�˿��Ա��棬��Ϊ��Ա����
    HANDLE m_hThread; //�ɼ��߳�
    //static UINT CameraAction(LPVOID pParam);
    DWORD m_threadIdSync;//���ڲ���ResetThread����
    DWORD m_threadIdSobel;//���ڲ���ResetThread����
    //void ResetThread(HANDLE hThread, DWORD ThreadId);

protected:
    HTuple hWindowHandle;//��ʾԭͼ������ͷ�ɼ�ͼ
    HTuple hWndImg; //��ʾ������

    typedef struct _tagThreadPar
    {
        CMVCamDlg *pParent;
        HANDLE hResetThreadEvent;
    }ThreadPar;

    vector<string> m_vecAllFile;
    bool m_bPause;
    HANDLE m_FileMutex;//���ļ�����

    HANDLE m_DispMutex;//��ʾ��ͬһ���ڻ���

    void DealImage(const HObject& ImageSrc);
    void SetWindowParam(const HTuple& hWnd, const HTuple& Width, const HTuple& Height);

    volatile bool m_bRun;
    HANDLE m_hSyncThread;
    HANDLE m_hSobelThread;
    static UINT ImageAction(LPVOID pParam);

    typedef struct _tagMonitorThreadPar
    {
        CMVCamDlg *pParent;
        HANDLE hEventSync;
        HANDLE hEventSobel;
    }MonitorThreadPar;
    bool m_bMonitor;
    HANDLE m_hMonitorthrad;
    static UINT MonitorThread(LPVOID pParam);
public:
    afx_msg void OnDestroy();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedPause();
    afx_msg void OnBnClickedButton3();
};
