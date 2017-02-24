
// MVCamDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MVCam.h"
#include "MVCamDlg.h"
#include "afxdialogex.h"
#include "atlimage.h"
#include <io.h>

#define  WM_FILENAME_MESSAGE  (WM_USER + 1)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    // 对话框数据
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    // 实现
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMVCamDlg 对话框



CMVCamDlg::CMVCamDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CMVCamDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_hThread = INVALID_HANDLE_VALUE;

    m_bRun = false;
    m_bPause = false;
}

void CMVCamDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMVCamDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &CMVCamDlg::OnBnClickedButton1)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BUTTON2, &CMVCamDlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_PAUSE, &CMVCamDlg::OnBnClickedPause)
    ON_BN_CLICKED(IDC_BUTTON3, &CMVCamDlg::OnBnClickedButton3)
    ON_MESSAGE(WM_FILENAME_MESSAGE, &CMVCamDlg::OnFileNameMsg)
END_MESSAGE_MAP()


// CMVCamDlg 消息处理程序

BOOL CMVCamDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 将“关于...”菜单项添加到系统菜单中。

    // IDM_ABOUTBOX 必须在系统命令范围内。
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

    // 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);			// 设置大图标
    SetIcon(m_hIcon, FALSE);		// 设置小图标

    GetDlgItem(IDC_FILE_NAME)->SetWindowText(L"");


    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// void CMVCamDlg::ResetThread(HANDLE hThread, DWORD ThreadId)
// {
//     //线程以TerminateThread方式结束后建立新线程就会有泄漏
//     if (ThreadId == m_threadIdSync)
//     {
//         m_threadIdSync = 0;
//         m_hSyncThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ImageAction, this, 0, &m_threadIdSync);
//     }
//     else if (ThreadId == m_threadIdSobel)
//     {
//         m_threadIdSobel = 0;
//         m_hSobelThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ImageAction, this, 0, &m_threadIdSobel);
//     }
//     else
//         TRACE(L"ThreadId error\r\n");
//     TerminateThread(hThread, 9);//如果放上面则后继的CreateThread得不到执行
// }

void CMVCamDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMVCamDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // 用于绘制的设备上下文

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMVCamDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CMVCamDlg::EnumImageFile()
{
    m_vecAllFile.clear();
    string strPath = "D:\\Work_PRJ\\Apintec\\2017-02-14\\20170224\\";
    string strFile = strPath + "*.bmp";
    _finddata_t data;
    intptr_t lf = _findfirst(strFile.c_str(), &data);
    if (lf != -1)
    {
        do
        {
            string strName = data.name;
            m_vecAllFile.push_back(strPath + strName);
        } while (_findnext(lf, &data) == 0);
    }
    _findclose(lf);

}

void CMVCamDlg::OnBnClickedButton1()
{
    if (m_bRun)
        return;
    SetSystem("do_low_error", "false");
    EnumImageFile();
    //////////////////////////////////////////////////////////////////////////
    CRect tmpRect;
    GetDlgItem(IDC_CAM)->GetClientRect(tmpRect);
    OpenWindow(0, 0, tmpRect.Width(), tmpRect.Height(), (Hlong)(GetDlgItem(IDC_CAM)->GetSafeHwnd()), "visible", "", &hWindowHandle);
    //SetWindowParam(hWindowHandle, 640, 480);//camera
    SetWindowParam(hWindowHandle, 8192, 8192);
    //////////////////////////////////////////////////////////////////////////
    CRect RectImg;
    GetDlgItem(IDC_IMG)->GetClientRect(RectImg);
    OpenWindow(0, 0, RectImg.Width(), RectImg.Height(), (Hlong)(GetDlgItem(IDC_IMG)->GetSafeHwnd()), "visible", "", &hWndImg);
    SetWindowParam(hWndImg, 8192, 8192);

    m_bRun = true;
    m_bPause = false;
    //DWORD thid;
   //m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&CameraAction, this, 0, &thid);

    m_FileMutex = CreateMutexW(NULL, FALSE, NULL);
    m_DispMutex = CreateMutexW(NULL, FALSE, NULL);

    HANDLE hEvent1 = CreateEventW(NULL, TRUE, FALSE, NULL);
    HANDLE hEvent2 = CreateEventW(NULL, TRUE, FALSE, NULL);
    m_threadIdSync = 0;
    ThreadPar *ActionPar = new ThreadPar();
    ActionPar->pParent = this;
    ActionPar->hResetThreadEvent = hEvent1;
    m_hSyncThread = CreateThread(NULL, 64*1024, (LPTHREAD_START_ROUTINE)&ImageAction, ActionPar, 0, &m_threadIdSync);
    m_threadIdSobel = 0;
    ThreadPar *ActionSobelPar = new ThreadPar();
    ActionSobelPar->pParent = this;
    ActionSobelPar->hResetThreadEvent = hEvent2;
    m_hSobelThread = CreateThread(NULL, 64 * 1024, (LPTHREAD_START_ROUTINE)&ImageAction, ActionSobelPar, 0, &m_threadIdSobel);

    m_bMonitor = true;
    DWORD dwID;
    MonitorThreadPar *MonitorPar = new MonitorThreadPar();
    MonitorPar->pParent = this;
    MonitorPar->hEventSync = hEvent1;
    MonitorPar->hEventSobel = hEvent2;

    m_hMonitorthrad = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&MonitorThread, MonitorPar, 0, &dwID);
}

void CMVCamDlg::SetWindowParam(const HTuple& hWnd,  const HTuple& Width, const HTuple& Height)
{
    SetColored(hWnd, 12);
    SetDraw(hWnd, "margin");
    SetLineWidth(hWnd, 1);
    SetShape(hWnd, "original");
    //set_lut(hWnd, "default");
    //set_paint(hWnd, "default");
    //SetWindowExtents(hWnd, 0, 0, Height, Width);
    //SetPart(hWnd, 0, 0, 480, 640);//很重要，否则显示图片不完整，480为Height, 640为Width
    SetPart(hWnd, 0, 0, Height, Width);

}

UINT CMVCamDlg::ImageAction(LPVOID pParam)
{
    ThreadPar *pPar = (ThreadPar*)pParam;
    bool bNeedReset = false;
    CMVCamDlg *pdlg = pPar->pParent;
    HObject SobelAmpImg;
    int i = 0;
    MSG msg; 
    while (pdlg->m_bRun)
    {
        if (pdlg->m_bPause)
        {
            Sleep(50);
            continue;
        }
        if (bNeedReset)
        {
            SetEvent(pPar->hResetThreadEvent);
            //pdlg->ResetThread(GetCurrentThread(),GetCurrentThreadId());
            break;
        }
        if (WaitForSingleObject(pdlg->m_FileMutex, 5000) == WAIT_OBJECT_0)
        {
            string strFileName = pdlg->m_vecAllFile[i];
            //DWORD dwStart = GetTickCount();
            ReadImage(&SobelAmpImg, strFileName.c_str());
            //DWORD dwTime = GetTickCount() - dwStart;
            //TRACE(L"------------------------线程 %d ReadImage 耗时 %dms\r\n", GetCurrentThreadId(), dwTime);
            pdlg->UpdateFileName(strFileName);
            ReleaseMutex(pdlg->m_FileMutex);
        }
        else
        {
            continue;//未获取到互斥量，下次还是读该文件
        }
        i++;
        if (i >= pdlg->m_vecAllFile.size())
        {
            bNeedReset = true;
            i = 0;
        }            
        pdlg->DealImage(SobelAmpImg);

        //必须处理消息，否则主线程失去响应
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        Sleep(20);
    }
    delete pPar;
    return 5;
}

void CMVCamDlg::UpdateFileName(const string& fileName)
{
    int nLen = fileName.length();
    CHAR* msgStr = new CHAR[nLen+1];
    strncpy_s(msgStr, nLen+1, fileName.c_str(), nLen);
    msgStr[nLen] = 0;
    if (m_bRun)
        PostMessage(WM_FILENAME_MESSAGE, 0, (LPARAM)msgStr);
    else
        delete[] msgStr;
}

LRESULT CMVCamDlg::OnFileNameMsg(WPARAM wParam, LPARAM lParam)
{
    CHAR*  lpFileName = (CHAR*)lParam;
    string strFileName = lpFileName;
    delete lpFileName;
    string strText = strFileName.substr(strFileName.find_last_of('\\') + 1);
    wstring strWText = CharToWStr(strText.c_str());
    GetDlgItem(IDC_FILE_NAME)->SetWindowText(strWText.c_str());
    return 0;
}

wstring CMVCamDlg::CharToWStr(const char *Data)
{
    wstring strRet = L"";
    int nlen = strlen(Data);
    strRet.resize(nlen, L' ');
    MultiByteToWideChar(CP_ACP, 0, (LPCSTR)Data, nlen, (LPWSTR)strRet.c_str(), nlen);
    return strRet;
}

// UINT CMVCamDlg::CameraAction(LPVOID pParam)
// {
//     CMVCamDlg *pdlg = (CMVCamDlg*)pParam;
// 
//     // Local iconic variables 
//     HObject  Image;
// 
//     // Local control variables 
//     HTuple  AcqHandle;
// 
//     //Code generated by Image Acquisition 01
//     //Attention: The initialization may fail in case parameters need to
//     //be set in a specific order (e.g., image resolution vs. offset).
//     OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "rgb", -1, "false",
//         "default", "[0] Lenovo EasyCamera", 0, -1, &AcqHandle);
// 
//     //set_framegrabber_param(AcqHandle, "white_balance", "auto");
// 
//     MSG msg;
//     GrabImageStart(AcqHandle, -1);
//     Hlong Pointer, Width, Height;
//     char     typ[128];
//     while (pdlg->m_bRun)
//     {
//         GrabImageAsync(&Image, AcqHandle, -1);//异步
//         if (pdlg->m_bPause)
//         {
//             Sleep(100);
//             continue;
//         }
//         MirrorImage(Image, &pdlg->mirrImg, "column");//镜面图像
//         //write_image(mirrImg, "bmp", 16711680, "D:\\12.bmp"); //存文件
//         //DispImage(mirrImg, hWindowHandle);//黑白图像
//         DispColor(pdlg->mirrImg, pdlg->hWindowHandle);
//         //pdlg->DealImage(pdlg->mirrImg);
// 
//         //必须处理消息，否则主线程失去响应
//         if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//         {
//             TranslateMessage(&msg);
//             DispatchMessage(&msg);
//         }
//         Sleep(20);
//     }
//     CloseFramegrabber(AcqHandle);
//     return 0;
// }


void CMVCamDlg::HImage2CBitmap(const HObject& pImage, CBitmap *wImage)
{
    HTuple csType;
    HTuple channels, lPointer;
    HTuple width, height;
    HTuple lPointerR, lPointerG, lPointerB;
    CountChannels(pImage, &channels);
    //获取Halcon数据的Long指针
    if (channels == 3)
    {
        GetImagePointer3(pImage, &lPointerR, &lPointerG, &lPointerB, &csType, &width, &height);
    }
    else
    {
        GetImagePointer1(pImage, &lPointer, &csType, &width, &height);
    }
    //创建文件头
    char tmp[sizeof(BITMAPINFO) + 1024];
    BITMAPINFO *bmi = (BITMAPINFO*)tmp;
    HBITMAP hBmp;
    memset(bmi, 0, sizeof(BITMAPINFO));
    bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi->bmiHeader.biWidth = width.L();
    //因HImage是从上到下，bmp是从下到上，故为负值使图像上下翻转
    bmi->bmiHeader.biHeight = -height.L();
    bmi->bmiHeader.biPlanes = 1;
    bmi->bmiHeader.biBitCount = 8 * channels.L();
    bmi->bmiHeader.biCompression = BI_RGB;
    bmi->bmiHeader.biSizeImage = 0;        //if biCompression is BI_RGB,this can be 0
    bmi->bmiHeader.biClrImportant = 0;
    //初始化数据
    switch (8 * channels.L())
    {
    case 8:
        for (int i = 0; i < 256; i++)
        {
            bmi->bmiColors[i].rgbBlue = i;
            bmi->bmiColors[i].rgbGreen = i;
            bmi->bmiColors[i].rgbRed = i;

        }
        break;
    case 32:
    case 24:
        ((DWORD*)bmi->bmiColors)[0] = 0x00FF0000;
        ((DWORD*)bmi->bmiColors)[1] = 0x0000FF00;
        ((DWORD*)bmi->bmiColors)[2] = 0x000000FF;
        break;
    }

    hBmp = ::CreateDIBSection(NULL, bmi, DIB_RGB_COLORS, NULL, 0, 0);
    //Halcon转换成BYTE数据
    BYTE *pData = new BYTE[width.L()*height.L()*channels.L()];
    long byteStep = 0;
    if (channels == 3)
    {
        byteStep = 3 * width.L();
        BYTE *pHimageR = (BYTE*)lPointerR.LArr();
        BYTE *pHimageG = (BYTE*)lPointerG.LArr();
        BYTE *pHimageB = (BYTE*)lPointerB.LArr();
        for (int i = 0; i < height.L(); ++i)
        {
            for (int j = 0; j < width.L(); ++j)
            {
                *(pData + i*byteStep + 3 * j + 0) = *pHimageB;
                *(pData + i*byteStep + 3 * j + 1) = *pHimageG;
                *(pData + i*byteStep + 3 * j + 2) = *pHimageR;
                pHimageR++;
                pHimageG++;
                pHimageB++;
            }
        }
    }
    else
    {
        byteStep = width.L();
        BYTE *pHimage = (BYTE*)lPointer.LArr();
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                *(pData + i*byteStep + j) = *pHimage;
                pHimage++;
            }
        }
    }

    //BYTE数据拷贝
    SetDIBits(NULL, hBmp, 0, height.I(), pData, bmi, DIB_RGB_COLORS);

    //CBitmap关联HBITMAP
    wImage->Attach(hBmp);

    delete[] pData;

    return;
}

void CMVCamDlg::OnDestroy()
{
    OnBnClickedButton3();
    CDialogEx::OnDestroy();
}

void CMVCamDlg::DealImage(const HObject& ImageSrc)
{
    HObject Amp;
    //DWORD dwStart = GetTickCount();
    SobelAmp(ImageSrc, &Amp, "sum_sqrt", 7);
    if (m_bPause)
        return;
    //DWORD dwTime = GetTickCount() - dwStart;
    //TRACE(L"线程 %d SobelAmp 耗时 %dms\r\n", GetCurrentThreadId(), dwTime);
    //bin_threshold(Amp, &Edg);
    //disp_region(Edg, WindowsHandle);
    //disp_image(Amp, c);
    //edges_sub_pix(Image, &Edg, "lanser2", 1.0, 10, 70);//边缘
    //measure_pos(Image, MeasureHandle, Sigma, Threshold, Transition, Select, \
     //   RowEdge, ColumnEdge, Amplitude, Distance)
    if (WaitForSingleObject(m_DispMutex, 1000) == WAIT_OBJECT_0)
    {
        DispImage(ImageSrc, hWindowHandle);
        DispObj(Amp, hWndImg);
        ReleaseMutex(m_DispMutex);
    }
}

UINT CMVCamDlg::MonitorThread(LPVOID pParam)
{
    MonitorThreadPar *pMonitorPar = (MonitorThreadPar*)pParam;
    CMVCamDlg *pDlg = pMonitorPar->pParent;
    HANDLE MonitorEvent[2] = { pMonitorPar ->hEventSync,pMonitorPar->hEventSobel };
    DWORD dwStartSync = GetTickCount();
    DWORD dwStartSobel = GetTickCount();
    while (pDlg->m_bMonitor)
    {
        DWORD dwRet = WaitForMultipleObjects(2, MonitorEvent, FALSE, 1000);
        switch (dwRet)
        {
        case WAIT_OBJECT_0 + 0:
        {
            DWORD dwTim = GetTickCount() - dwStartSync;
            TRACE(L"本线程%d退出，总耗时 %dms\r\n", pDlg->m_threadIdSync, dwTim);
            dwStartSync = GetTickCount();

            pDlg->m_threadIdSync = 0;
            ThreadPar *ActionPar = new ThreadPar();
            ActionPar->pParent = pDlg;
            ActionPar->hResetThreadEvent = pMonitorPar->hEventSync;
            pDlg->m_hSyncThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ImageAction, ActionPar, 0, &(pDlg->m_threadIdSync));
            ResetEvent(pMonitorPar->hEventSync);
        }
        break;
        case WAIT_OBJECT_0 + 1:
        {
            DWORD dwTim = GetTickCount() - dwStartSobel;
            TRACE(L"本线程%d退出，总耗时 %dms\r\n", pDlg->m_threadIdSobel, dwTim);
            dwStartSobel = GetTickCount();

            pDlg->m_threadIdSobel = 0;
            ThreadPar *ActionSobelPar = new ThreadPar();
            ActionSobelPar->pParent = pDlg;
            ActionSobelPar->hResetThreadEvent = pMonitorPar->hEventSobel;
            pDlg->m_hSobelThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ImageAction, ActionSobelPar, 0, &(pDlg->m_threadIdSobel));
            ResetEvent(pMonitorPar->hEventSobel);
        }
        break;
        default:
            break;
        }
        Sleep(10);
    }
    CloseHandle(pMonitorPar->hEventSync);
    CloseHandle(pMonitorPar->hEventSobel);
    delete pMonitorPar;
    return 0;
}

void CMVCamDlg::OnBnClickedButton2()
{
    bool bPause = m_bPause;
    m_bPause = true;
    CBitmap bmp;
    HImage2CBitmap(mirrImg, &bmp);

    CImage arch;
    arch.Attach(bmp);
    arch.Save(_T("D:\\save.bmp"), Gdiplus::ImageFormatBMP);
    m_bPause = bPause;
}


void CMVCamDlg::OnBnClickedPause()
{
    m_bPause = !m_bPause;

    ((CButton*)GetDlgItem(IDC_PAUSE))->SetCheck(m_bPause);
}


void CMVCamDlg::OnBnClickedButton3()
{
    if (!m_bRun)
        return;
    m_bRun = false;
    m_bPause = true;
    HANDLE hThreadWait[2] = { m_hSyncThread, m_hSobelThread };
    DWORD dwWait = WaitForMultipleObjects(2, hThreadWait, TRUE, INFINITE);

    m_bMonitor = false;
    WaitForSingleObject(m_hMonitorthrad, INFINITE);

    CloseHandle(m_FileMutex);
    CloseHandle(m_DispMutex);
    TRACE(L"All Custom Thread Quit\r\n");
}
