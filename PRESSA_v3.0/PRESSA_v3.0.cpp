#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <time.h>
#include <stdlib.h>
#include <string>
#define Password "ZR"
u_long x[5];
using namespace std;
ofstream MyFile[5];
int i, j, k, qqq = 0;
int total_number_of_ports = 5;
int number_of_inputs;
unsigned int counting_inputs[5] = { 0 };
unsigned int data_error[5] = { 0 };
time_t t;
struct tm sistime;
HINSTANCE hInst;
HWND hEdt_0[5][16];
HWND hEdt_1[5][16];
HWND hEdtSTAT[5][16];
HWND hSta[16];
HWND hSta13[5], hSta14[5], hSta15[5], hSta16[5], hSta17[5], hSta18[5], hSta19[5], hSta20[5], hSta21[5], hSta22[5];
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);
HWND Child;
HWND Child_2;
HWND HWNDSTART[5];
HWND HWNDSTOP[5];
HWND HWNDOTCHET;
HWND HWNDRESET[5];
HWND HWNDPORT[5];
HWND HWNDVERSIYA[5];
HWND HWNDEDIT1[5];
HWND HWNDEDIT2[5];
HWND HWNDPASSWORD;
HWND HWNDPASSWORDSTAT;
HWND HWNDPASSWORDEDIT;
HWND HWNDPASSWORDBUTTON1;
HWND HWNDPASSWORDBUTTON2;
HMENU BTN1[5] = { (HMENU)1010, (HMENU)1011, (HMENU)1012, (HMENU)1013, (HMENU)1014 };
HMENU BTN2[5] = { (HMENU)1020, (HMENU)1021, (HMENU)1022, (HMENU)1023, (HMENU)1024 };
HMENU BTN3 = (HMENU)1030;
HMENU BTN4[5] = { (HMENU)1040, (HMENU)1041, (HMENU)1042, (HMENU)1043, (HMENU)1044 };
HMENU PASSWORDBTN1 = (HMENU)105;
HMENU PASSWORDBTN2 = (HMENU)106;
string BUFstring;
char SMENA[5][32];
char BUFchar[16];
char EDIT[5][16];
char BUF_OBMENA_CHAR[16];
int BUF_OBMENA_INT[16];
unsigned int SUMMA_PRESS[5][16];
char TIME[128];
wchar_t BUFFER[3] = L"№";
HANDLE initCOM[5] = { NULL, NULL, NULL, NULL, NULL };
DCB DCB1;
LPCSTR Setting_COM;
COMMTIMEOUTS timeout{ 0,0,10000,0,0 };
static HBRUSH hBrushFon = NULL;
static HBRUSH hBrushEdit = NULL;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    setlocale(LC_ALL, "Russian");
    HWND hWnd;
    MSG msg;
    WNDCLASSEX w;
    hInst = hInstance;
    hBrushFon = CreateSolidBrush(RGB(220, 220, 150));
    hBrushEdit = CreateSolidBrush(RGB(220, 220, 150));
    w.cbSize = sizeof(WNDCLASSEX);
    w.style = CS_HREDRAW | CS_VREDRAW;
    w.lpfnWndProc = WndProc;
    w.cbClsExtra = 0;
    w.cbWndExtra = 0;
    w.hInstance = hInstance;
    w.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    w.hCursor = LoadCursor(NULL, IDC_ARROW);
    w.hbrBackground = hBrushFon;
    w.lpszMenuName = 0;
    w.lpszClassName = _T("Windows class");
    w.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&w);
    hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, _T("Windows class"), _T("Пресса v2.0 // ZR.LTD"), WS_OVERLAPPEDWINDOW | WS_VSCROLL, 100, 100, 1200, 700, NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);
    while (GetMessage(&msg, 0, 0, 0) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int Value[5]{};
    MSG msg{};

    switch (message)
    {
    case WM_COMMAND:
        if (wParam == 1010 || wParam == 1011 || wParam == 1012 || wParam == 1013 || wParam == 1014)
        {
            int ch = (int)wParam - 1010;
            ShowWindow(hSta19[ch], 1);
            ShowWindow(hSta21[ch], 0);
            UpdateWindow(hWnd);
            char versionBuf[16] = { 0 };
            SendMessageA(HWNDVERSIYA[ch], WM_GETTEXT, sizeof versionBuf, (LPARAM)versionBuf);
            if (versionBuf[0] != 'A')
            {
                ShowWindow(hSta19[ch], 0);
                MessageBox(0, _T("Укажите версию контроллера!"), L"Сообщение", 0);
                return 0;
            }
            char portBuf[16] = { 0 };
            SendMessageA(HWNDPORT[ch], WM_GETTEXT, sizeof portBuf, (LPARAM)portBuf);
            if (portBuf[0] != 'C')
            {
                ShowWindow(hSta19[ch], 0);
                MessageBox(0, _T("Укажите номер COM порта! "), L"Сообщение", 0);
                return 0;
            }
            t = time(nullptr);
            localtime_s(&sistime, &t);
            strftime(SMENA[ch], sizeof(SMENA[ch]), "%d-%b-%Y   %H-%M-%S.xls", &sistime);
            MyFile[ch].open(SMENA[ch]);
            MyFile[ch].close();
            initCOM[ch] = CreateFileA(portBuf, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
            if (initCOM[ch] == INVALID_HANDLE_VALUE)
            {
                initCOM[ch] = NULL;
                ShowWindow(hSta19[ch], 0);
                MessageBox(0, _T("COM порт не доступен"), L"Сообщение об ошибке", 0);
                return 0;
            }
            else
            {
                std::string com = std::string(portBuf) + ": baud=9600 parity=N data=8 stop=1";
                Setting_COM = (com).c_str();
                Value[ch] = BuildCommDCBA(Setting_COM, &DCB1);
                Value[ch] = SetCommState(initCOM[ch], &DCB1);
            }

            HWNDEDIT1[ch] = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER, 230, 50 + ch * 200, 220, 30, hWnd, 0, hInst, 0);
            HWNDEDIT2[ch] = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER, 500, 50 + ch * 200, 110, 30, hWnd, 0, hInst, 0);
            hSta14[ch] = CreateWindow(_T("STATIC"), _T("Пакеты данных с контроллера:"), WS_CHILD | WS_VISIBLE | SS_CENTER, 230, 30 + ch * 200, 220, 20, hWnd, 0, hInst, 0);
            hSta15[ch] = CreateWindow(_T("STATIC"), _T("Кол-во пакетов"), WS_CHILD | WS_VISIBLE | SS_CENTER, 500, 30 + ch * 200, 110, 20, hWnd, 0, hInst, 0);
            hSta16[ch] = CreateWindow(_T("STATIC"), _T("Кол-во ходов"), WS_CHILD | WS_VISIBLE | SS_CENTER, 5, 150 + ch * 200, 95, 20, hWnd, 0, hInst, 0);
            hSta17[ch] = CreateWindow(_T("STATIC"), _T("№ Пресса"), WS_CHILD | WS_VISIBLE | SS_CENTER, 5, 130 + ch * 200, 95, 20, hWnd, 0, hInst, 0);
            EnableWindow(HWNDSTOP[ch], 0);
            for (i = 0; i < 16; i++)
            {
                _itow_s(i + 1, &BUFFER[1], 3, 10);
                hSta[i] = CreateWindow(_T("STATIC"), BUFFER, WS_CHILD | WS_VISIBLE | SS_CENTER, i * 50 + 100, 130 + ch * 200, 50, 20, hWnd, 0, hInst, 0);
                hEdtSTAT[ch][i] = CreateWindow(_T("EDIT"), _T("0"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT, i * 50 + 100, 150 + ch * 200, 50, 20, hWnd, 0, hInst, 0);
                hEdt_0[ch][i] = CreateWindow(_T("EDIT"), _T("0"), WS_CHILD | WS_BORDER | ES_RIGHT, i * 50 + 100, 190 + ch * 200, 50, 20, hWnd, 0, hInst, 0);
                hEdt_1[ch][i] = CreateWindow(_T("EDIT"), _T("1"), WS_CHILD | WS_BORDER | ES_RIGHT, i * 50 + 100, 190 + ch * 200, 50, 20, hWnd, 0, hInst, 0);
            }

            EnableWindow(HWNDVERSIYA[ch], 0);
            EnableWindow(HWNDPORT[ch], 0);
            EnableWindow(HWNDSTART[ch], 0);
            EnableWindow(HWNDRESET[ch], 0);
            EnableWindow(HWNDSTOP[ch], 1);
            ShowWindow(hSta19[ch], 0);
            ShowWindow(hSta18[ch], 1);
            UpdateWindow(hWnd);
            SendMessage(hWnd, WM_USER + 100, (WPARAM)wParam, (LPARAM)lParam);
            return 0;
        }

        switch (LOWORD(wParam))
        {
        case 1020: case 1021: case 1022: case 1023: case 1024:
        {
            int ch = LOWORD(wParam) - 1020;
            Value[ch] = CloseHandle(initCOM[ch]);
            initCOM[ch] = NULL;
            EnableWindow(HWNDSTOP[ch], 0);
            EnableWindow(HWNDSTART[ch], 1);
            EnableWindow(HWNDRESET[ch], 1);
            if (Value[ch] == 0)
            {
                MessageBox(0, _T("Ошибка закрытия COM порта"), _T("Сообщение"), 0);
                return 0;
            }
            else
            {
                KillTimer(hWnd, ch + 1);
                ShowWindow(hSta21[ch], 1);
            }
        }
        break;
        case 1030:
            ShowWindow(Child, SW_SHOW);
            break;
        case 1040: case 1041: case 1042: case 1043: case 1044:
        {
            int ch = LOWORD(wParam) - 1040;
            for (i = 0; i < 16; i++)
            {
                SUMMA_PRESS[ch][i] = 0;
                SendMessageA(hEdtSTAT[ch][i], WM_SETTEXT, 0, (LPARAM)"0");
                ShowWindow(hEdt_1[ch][i], 0);
                ShowWindow(hEdt_0[ch][i], 0);
            }
            SendMessageA(HWNDEDIT1[ch], WM_SETTEXT, 0, (LPARAM)"");
            SendMessageA(HWNDEDIT2[ch], WM_SETTEXT, 0, (LPARAM)"");
            EnableWindow(HWNDSTART[ch], 1);
            EnableWindow(HWNDVERSIYA[ch], 1);
            EnableWindow(HWNDPORT[ch], 1);
            ShowWindow(hSta13[ch], 0);
            ShowWindow(hSta18[ch], 0);
            ShowWindow(hSta19[ch], 0);
            ShowWindow(hSta20[ch], 0);
            ShowWindow(hSta21[ch], 0);
            ShowWindow(hSta22[ch], 0);
            x[ch] = 0;
            counting_inputs[ch] = 0;
            data_error[ch] = 0;
        }
        break;
        }
        break;

    case WM_USER + 100:
    {
        int ch = (int)wParam - 1010;
        if (ch < 0 || ch >= total_number_of_ports) return 0;

        x[ch] = x[ch] + 1;
        char versionBuf[16] = { 0 };
        SendMessageA(HWNDVERSIYA[ch], WM_GETTEXT, sizeof versionBuf, (LPARAM)versionBuf);
        if (versionBuf[6] == '1')
        {
            ShowWindow(hSta19[ch], 0);
            number_of_inputs = 16;
        }
        if (versionBuf[6] == '3')
        {
            ShowWindow(hSta19[ch], 0);
            number_of_inputs = 12;
        }
        if (versionBuf[6] != '1' && versionBuf[6] != '3') number_of_inputs = 16;

        for (i = 0; i < 16; i++)
        {
            BUF_OBMENA_CHAR[i] = 0x30;
            BUF_OBMENA_INT[i] = 0;
        }

        DWORD bytesRead = 0;
        BOOL ok = ReadFile(initCOM[ch], &BUF_OBMENA_CHAR[0], number_of_inputs, &bytesRead, NULL);
        if (!ok || bytesRead == 0)
        {
            ShowWindow(hSta20[ch], 1);
            ShowWindow(hSta19[ch], 0);
            EnableWindow(HWNDSTOP[ch], 0);
            EnableWindow(HWNDSTART[ch], 1);
            EnableWindow(HWNDRESET[ch], 1);
            KillTimer(hWnd, ch + 1);
            return 0;
        }
        else
        {
            SendMessageA(HWNDEDIT1[ch], WM_SETTEXT, 0, (LPARAM)BUF_OBMENA_CHAR);
            wsprintfA(&EDIT[ch][0], "%lu", x[ch]);
            SendMessageA(HWNDEDIT2[ch], WM_SETTEXT, sizeof EDIT[ch], (LPARAM)EDIT[ch]);
        }
        for (i = 0; i < (int)bytesRead && i < number_of_inputs; i++)
        {
            if (BUF_OBMENA_CHAR[i] == '0')
            {
                ShowWindow(hEdt_0[ch][i], SW_SHOW);
                ShowWindow(hEdt_1[ch][i], SW_HIDE);
                BUF_OBMENA_INT[i] = 0;
                SUMMA_PRESS[ch][i] = SUMMA_PRESS[ch][i] + 0;
                wsprintfA(&EDIT[ch][i], "%lu", SUMMA_PRESS[ch][i]);
                SendMessageA(hEdtSTAT[ch][i], WM_SETTEXT, 0, (LPARAM)&EDIT[ch][i]);
            }
            else
            {
                counting_inputs[ch] = counting_inputs[ch] + 1;
                ShowWindow(hEdt_1[ch][i], SW_SHOW);
                ShowWindow(hEdt_0[ch][i], SW_HIDE);
                BUF_OBMENA_INT[i] = 1;
                SUMMA_PRESS[ch][i] = SUMMA_PRESS[ch][i] + 1;
                wsprintfA(&EDIT[ch][i], "%lu", SUMMA_PRESS[ch][i]);
                SendMessageA(hEdtSTAT[ch][i], WM_SETTEXT, 0, (LPARAM)&EDIT[ch][i]);
            }
        }
        if (counting_inputs[ch] == (unsigned)number_of_inputs)
        {
            counting_inputs[ch] = 0;
            data_error[ch] = data_error[ch] + 1;
        }
        else
        {
            data_error[ch] = 0;
            counting_inputs[ch] = 0;
        }
        if (data_error[ch] == 3)
        {
            data_error[ch] = 0;
            MessageBox(0, _T("Связь с контроллером нарушена!!!"), _T("Сообщение об ошибке"), 0);
            EnableWindow(HWNDSTOP[ch], 0);
            EnableWindow(HWNDSTART[ch], 1);
            EnableWindow(HWNDRESET[ch], 1);
            KillTimer(hWnd, ch + 1);
            return 0;
        }
        t = time(nullptr);
        localtime_s(&sistime, &t);
        strftime(TIME, sizeof(TIME), "%X", &sistime);
        SendMessageA(hSta13[ch], WM_SETTEXT, 0, (LPARAM)TIME);
        ShowWindow(hSta13[ch], 1);
        SetTimer(hWnd, ch + 1, 4950, NULL);

        MyFile[ch].open(SMENA[ch], std::ios::app);
        MyFile[ch] << TIME;
        for (i = 0; i < 16; i++) MyFile[ch] << "\t" << BUF_OBMENA_INT[i];
        MyFile[ch] << "\t" << "\n";
        MyFile[ch].close();

        UpdateWindow(hWnd);
    }
    break;

    case WM_TIMER:
        for (j = 0; j < total_number_of_ports; j++)
        {
            if (wParam == (WPARAM)(j + 1))
            {
                KillTimer(hWnd, j + 1);
                SendMessage(hWnd, WM_USER + 100, 1010 + j, (LPARAM)lParam);
            }
        }
        break;

    case WM_CREATE:
    {
        WNDCLASS w;
        memset(&w, 0, sizeof(WNDCLASS));
        w.lpfnWndProc = ChildProc;
        w.hInstance = hInst;
        w.hCursor = LoadCursor(NULL, IDC_ARROW);
        w.hbrBackground = CreateSolidBrush(RGB(155, 255, 255));
        w.lpszClassName = _T("ChildWClass");
        RegisterClass(&w);

        Child = CreateWindowEx(0, _T("ChildWClass"), _T("Авторизация"), WS_OVERLAPPEDWINDOW, 450, 200, 300, 170, hWnd, NULL, hInst, NULL);
        Child_2 = CreateWindowEx(0, _T("ChildWClass"), _T("Выбор отчета"), WS_OVERLAPPEDWINDOW, 650, 300, 500, 300, hWnd, NULL, hInst, NULL);
        HWNDPASSWORDSTAT = CreateWindow(_T("STATIC"), _T("Введите пароль"), WS_CHILD | WS_VISIBLE | SS_CENTER, 60, 10, 150, 20, Child, 0, hInst, 0);
        HWNDPASSWORDEDIT = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER, 60, 40, 150, 25, Child, 0, hInst, 0);
        HWNDPASSWORDBUTTON1 = CreateWindow(_T("BUTTON"), _T("ОК"), WS_CHILD | WS_BORDER | WS_VISIBLE | BS_NOTIFY, 60, 80, 70, 30, Child, PASSWORDBTN1, hInst, 0);
        HWNDPASSWORDBUTTON2 = CreateWindow(_T("BUTTON"), _T("Отмена"), WS_CHILD | WS_BORDER | WS_VISIBLE | BS_NOTIFY, 140, 80, 70, 30, Child, PASSWORDBTN2, hInst, 0);
        ShowWindow(Child, SW_HIDE);
        UpdateWindow(Child);

        HWNDOTCHET = CreateWindowEx(WS_EX_DLGMODALFRAME, _T("BUTTON"), _T("ОТЧЕТ"), WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE, 1000, 50, 150, 150, hWnd, BTN3, hInst, 0);
        for (j = 0; j < total_number_of_ports; j++)
        {
            HWNDSTART[j] = CreateWindowEx(WS_EX_DLGMODALFRAME, _T("BUTTON"), _T("СTAРT"), WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE | BS_PUSHLIKE, 650, 50 + j * 200, 100, 30, hWnd, BTN1[j], hInst, 0);
            HWNDRESET[j] = CreateWindowEx(WS_EX_DLGMODALFRAME, _T("BUTTON"), _T("СБРОС"), WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE, 850, 50 + j * 200, 100, 30, hWnd, BTN4[j], hInst, 0);
            HWNDSTOP[j] = CreateWindowEx(WS_EX_DLGMODALFRAME, _T("BUTTON"), _T("СТОП"), WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE | BS_PUSHLIKE, 750, 50 + j * 200, 100, 30, hWnd, BTN2[j], hInst, 0);
            hSta13[j] = CreateWindow(_T("EDIT"), _T(""), WS_CHILD | WS_BORDER | SS_CENTER, 5, 190 + j * 200, 95, 20, hWnd, 0, hInst, 0);
            hSta18[j] = CreateWindow(_T("STATIC"), _T("Соединение установлено..."), WS_CHILD, 240, 90 + j * 200, 400, 20, hWnd, 0, hInst, 0);
            hSta19[j] = CreateWindow(_T("STATIC"), _T("Ожидание соединения..."), WS_CHILD, 240, 90 + j * 200, 400, 20, hWnd, 0, hInst, 0);
            hSta20[j] = CreateWindow(_T("STATIC"), _T("Обрыв связи... , нажмите СТОП"), WS_CHILD, 240, 90 + j * 200, 400, 20, hWnd, 0, hInst, 0);
            hSta21[j] = CreateWindow(_T("STATIC"), _T("Остановлено пользователем... нажмите сброс"), WS_CHILD, 240, 90 + j * 200, 400, 20, hWnd, 0, hInst, 0);
            hSta22[j] = CreateWindow(_T("STATIC"), _T("Проверьте контроллер или порт... нажмите сброс"), WS_CHILD, 240, 90 + j * 200, 400, 20, hWnd, 0, hInst, 0);
            HWNDPORT[j] = CreateWindow(_T("COMBOBOX"), NULL, CBS_DROPDOWN | WS_CHILD | WS_VISIBLE | WS_VSCROLL, 30, 50 + j * 200, 150, 150, hWnd, 0, hInst, 0);
            for (i = 0; i < 10; i++)
            {
                char NumberCOM[5] = "COM";
                wsprintfA(&NumberCOM[3], "%lu", i);
                SendMessageA(HWNDPORT[j], CB_ADDSTRING, 0, (LPARAM)&NumberCOM[0]);
            }
            SendMessage(HWNDPORT[j], WM_SETTEXT, 0, (LPARAM)_T("COM-порт:"));
            HWNDVERSIYA[j] = CreateWindow(_T("COMBOBOX"), NULL, CBS_DROPDOWN | WS_CHILD | WS_VISIBLE | WS_VSCROLL, 30, 75 + j * 200, 150, 150, hWnd, 0, hInst, 0);
            for (i = 1; i < 4; i = i + 2)
            {
                char VERSIYA[10] = "ATmega";
                wsprintfA(&VERSIYA[6], "%lu", i * 100 + 28);
                SendMessageA(HWNDVERSIYA[j], CB_ADDSTRING, 0, (LPARAM)&VERSIYA[0]);
            }
            SendMessage(HWNDVERSIYA[j], WM_SETTEXT, 0, (LPARAM)_T("Контроллер:"));
        }
    }
    break;

    case WM_CTLCOLORSTATIC:
        SetTextColor((HDC)wParam, RGB(0, 0, 0));
        SetBkColor((HDC)wParam, RGB(220, 220, 150));
        return (LRESULT)hBrushFon;
        break;

    case WM_CTLCOLOREDIT:
        for (j = 0; j < total_number_of_ports; j++)
        {
            if (hSta13[j] == (HWND)lParam)
            {
                SetBkColor((HDC)wParam, RGB(150, 220, 250));
                return (LRESULT)hBrushEdit;
            }
            for (i = 0; i < 16; i++)
            {
                if (hEdt_0[j][i] == (HWND)lParam)
                {
                    SetBkColor((HDC)wParam, RGB(255, 0, 0));
                    return (LRESULT)hBrushEdit;
                }
                if (hEdt_1[j][i] == (HWND)lParam)
                {
                    SetBkColor((HDC)wParam, RGB(0, 255, 0));
                    return (LRESULT)hBrushEdit;
                }
            }
        }
        break;

    case WM_SIZE:
    {
        SCROLLINFO scrInfo;
        scrInfo.cbSize = sizeof(SCROLLINFO);
        scrInfo.nPage = HIWORD(lParam);
        scrInfo.nMin = 0;
        scrInfo.nMax = 3000;
        scrInfo.fMask = SIF_RANGE | SIF_PAGE;
        SetScrollInfo(hWnd, SB_VERT, &scrInfo, TRUE);
    }
    break;

    case WM_VSCROLL:
    {
        SCROLLINFO scrInfo;
        scrInfo.cbSize = sizeof(SCROLLINFO);
        scrInfo.fMask = SIF_ALL;
        GetScrollInfo(hWnd, SB_VERT, &scrInfo);
        int currentPos = scrInfo.nPos;
        switch (LOWORD(wParam))
        {
        case SB_LINEUP:
            scrInfo.nPos -= 1;
            break;
        case SB_LINEDOWN:
            scrInfo.nPos += 1;
            break;
        case SB_THUMBTRACK:
            scrInfo.nPos = scrInfo.nTrackPos;
            break;
        case WM_MOUSEWHEEL:
            break;
        default:
            return 0;
        }
        scrInfo.fMask = SIF_POS;
        SetScrollInfo(hWnd, SB_VERT, &scrInfo, TRUE);
        GetScrollInfo(hWnd, SB_VERT, &scrInfo);
        int yScroll = currentPos - scrInfo.nPos;
        ScrollWindow(hWnd, 0, yScroll, NULL, NULL);
        SendMessage(hWnd, WM_CTLCOLORSTATIC, (WPARAM)wParam, (LPARAM)lParam);
    }
    break;

    case WM_DESTROY:
        for (j = 0; j < total_number_of_ports; j++)
        {
            if (initCOM[j] != NULL && initCOM[j] != INVALID_HANDLE_VALUE)
            {
                CloseHandle(initCOM[j]);
                initCOM[j] = NULL;
            }
        }
        if (hBrushFon) { DeleteObject(hBrushFon); hBrushFon = NULL; }
        if (hBrushEdit) { DeleteObject(hBrushEdit); hBrushEdit = NULL; }
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK ChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 105:
            SendMessageA(HWNDPASSWORDEDIT, WM_GETTEXT, sizeof(BUFchar), (LPARAM)BUFchar);
            BUFstring = BUFchar;
            if (BUFstring == Password)
            {
                ShowWindow(Child, 0);
                ShowWindow(Child_2, 1);
            }
            SendMessageA(HWNDPASSWORDEDIT, WM_SETTEXT, 2, 0);
            break;
        case 106:
            ShowWindow(Child, 0);
            break;
        default:
            break;
        }
    case WM_CREATE:
        break;
    case WM_CTLCOLORSTATIC:
        SetTextColor((HDC)wParam, RGB(0, 0, 0));
        SetBkColor((HDC)wParam, RGB(155, 255, 155));
        return (LRESULT)CreateSolidBrush(RGB(155, 255, 255));
        break;
    case WM_DESTROY:
        Child = CreateWindowEx(0, _T("ChildWClass"), _T("Авторизация"), WS_OVERLAPPEDWINDOW, 750, 400, 300, 170, hWnd, NULL, hInst, NULL);
        Child_2 = CreateWindowEx(0, _T("ChildWClass"), _T("Выбор отчета"), WS_OVERLAPPEDWINDOW, 650, 300, 500, 300, hWnd, NULL, hInst, NULL);
        HWNDPASSWORDSTAT = CreateWindow(_T("STATIC"), _T("Введите пароль"), WS_CHILD | WS_VISIBLE | SS_CENTER, 60, 10, 150, 20, Child, 0, hInst, 0);
        HWNDPASSWORDEDIT = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER, 60, 40, 150, 25, Child, 0, hInst, 0);
        HWNDPASSWORDBUTTON1 = CreateWindow(_T("BUTTON"), _T("ОК"), WS_CHILD | WS_BORDER | WS_VISIBLE | BS_NOTIFY, 60, 80, 70, 30, Child, PASSWORDBTN1, hInst, 0);
        HWNDPASSWORDBUTTON2 = CreateWindow(_T("BUTTON"), _T("Отмена"), WS_CHILD | WS_BORDER | WS_VISIBLE | BS_NOTIFY, 140, 80, 70, 30, Child, PASSWORDBTN2, hInst, 0);
        UpdateWindow(Child);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
    return 0;
}