// MVCamDlg.h : 头文件
//

#pragma once
#include "HalconCpp.h"
using namespace HalconCpp;

#include <string>
#include <vector>
using namespace std;

// CMVCamDlg 对话框
class CMVCamDlg : public CDialogEx
{
// 构造
public:
	CMVCamDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MVCAM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
    HObject mirrImg;//摄像头采集图，为了可以保存，设为成员变量
    HANDLE m_hThread; //采集线程
    //static UINT CameraAction(LPVOID pParam);
    DWORD m_threadIdSync;//用于测试ResetThread函数
    DWORD m_threadIdSobel;//用于测试ResetThread函数
    //void ResetThread(HANDLE hThread, DWORD ThreadId);

protected:
    HTuple hWindowHandle;//显示原图或摄像头采集图
    HTuple hWndImg; //显示处理结果

    typedef struct _tagThreadPar
    {
        CMVCamDlg *pParent;
        HANDLE hResetThreadEvent;
    }ThreadPar;

    vector<string> m_vecAllFile;
    bool m_bPause;
    HANDLE m_FileMutex;//读文件互斥

    HANDLE m_DispMutex;//显示在同一窗口互斥

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
