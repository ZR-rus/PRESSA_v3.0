/////////////////////////////// ПРОГРАММА АСК "ПРЕССА" ////////////////////////////
// Разработчик: ZR 02/05/2024.
///////////////////////////////////////////////////////////////////////////////////
//#define UNICODE
#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <time.h>
#include <stdlib.h>
///////////////////////////////////////////////////////////////////////////////////
#define fon CreateSolidBrush(RGB(220, 220, 150))
#define Password "ZR"
///////////////////////////////////////////////////////////////////////////////////
u_long x[5];
using namespace std;
ofstream MyFile[5];
int i, j, k, qqq = 0;
unsigned int total_number_of_ports = 5;
unsigned int number_of_inputs;//колличество входов увыбранного контроллера
unsigned int counting_inputs = 0;//подсчет сработавших прессов за одну отправку(для выявления ошибки!!!)
unsigned int data_error = 0;// ошибочные данные с контроллера
time_t t;
struct tm sistime;
HINSTANCE hInst;
HWND hEdt_0[5][16];
HWND hEdt_1[5][16];
HWND hEdtSTAT[5][16];
HWND hSta[16];//заголоаки прессов
HWND hSta13[5], hSta14[5], hSta15[5], hSta16[5], hSta17[5], hSta18[5], hSta19[5], hSta20[5], hSta21[5], hSta22[5];
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);// объявляем функцию-обработчик основного окна
LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);// объявляем функцию-обработчик дочернего окна
//COLORREF SetTextColor(COLORREF crColor);
// кнопки и окна
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
// идентификаторы сообщений
HMENU BTN1[5] = { (HMENU)1010, (HMENU)1011,(HMENU)1012 };//старт
HMENU BTN2[5] = { (HMENU)1020, (HMENU)1021 };//стоп
HMENU BTN3 = (HMENU)1030;//отчет
HMENU BTN4[5] = { (HMENU)1040, (HMENU)1041 };//сброс
HMENU PASSWORDBTN1 = (HMENU)105;//подтверждение пароля
HMENU PASSWORDBTN2 = (HMENU)106;//отмена пароля
//массивы данных
string  BUFstring;
//char ZR[128]={0,0,0,0,0};
char SMENA[5][32];
char BUFchar[16];
char EDIT[5][16];
//char EDIT2 [16];
char BUF_OBMENA_CHAR[16];
int BUF_OBMENA_INT[16];
unsigned int SUMMA_PRESS[5][16];
char TIME[128];
wchar_t BUFFER[3] = L"№";

// объявления для COM порта
HANDLE initCOM[5];
//DWORD LenBUF_OBMENA;
DCB DCB1;
LPCSTR Setting_COM;
COMMTIMEOUTS timeout{ 0,0,10000,0,0 };

//=====================================ОСНОВНАЯ ПРОГРАММА (MAIN)========================================//

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    setlocale(LC_ALL, "Russian");
    // прячем консоль
    //HWND Hide;
    //AllocConsole();
    //Hide = FindWindowA("ConsoleWindowClass", NULL);
    //ShowWindow(Hide,0);
    // переменные
    HWND                hWnd;
    MSG                 msg;
    WNDCLASSEX            w;
    hInst = hInstance;
    // ЗАПОЛНЯЕМ СТРУКТУРУ КЛАССА ОКНА
    w.cbSize = sizeof(WNDCLASSEX);//размер структуры в байтах
    w.style = CS_HREDRAW | CS_VREDRAW;//стиль класса окна
    w.lpfnWndProc = WndProc;//указатель на функцию окна (оконную процедуру)
    w.cbClsExtra = 0;
    w.cbWndExtra = 0;
    w.hInstance = hInstance;// дискриптор экземпляра приложения в которм находится оконное приложение
    w.hIcon = LoadIcon(NULL, IDI_APPLICATION);//дискриптор пиктограммы
    w.hCursor = LoadCursor(NULL, IDC_ARROW);//дискриптор курсора
    w.hbrBackground = fon;//дискриптор кисти для закраски фона окна
    w.lpszMenuName = 0;//указатель на меню
    w.lpszClassName = _T("Windows class");//указатель на имя класса
    w.hIconSm = LoadIcon(NULL, IDI_APPLICATION);//дискриптор малой пиктограммы «?»
    // РЕГИСТРИРУЕМ КЛАСС ОКНА
    RegisterClassEx(&w);
    // СОЗДАЁМ ОСНОВНОЕ ОКНО ПРИЛОЖЕНИЯ
    hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, _T("Windows class"), _T("Пресса v2.0 // ZR.LTD"), WS_OVERLAPPEDWINDOW | WS_VSCROLL, 400, 200, 1200, 700, NULL, NULL, hInstance, NULL);// создание окна
    ShowWindow(hWnd, iCmdShow);// показать окно
    UpdateWindow(hWnd);// обновить окно
    // ВЫПОЛНЯЕМ ЦИКЛ ОБРАБОТКИ СООБЩЕНИЙ ДО ЗАКРЫТИЯ ПРИЛОЖЕНИЯ
    while (GetMessage(&msg, 0, 0, 0) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

//========================================== ОБРАБОТЧИК СООБЩЕНИЙ ОСНОВНОГО ОКНА ==========================================//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)//это оконная процедура для главного окна
{

    int Value[5];
    MSG    msg;
    printf(".");

    switch (message)
    {
    case WM_COMMAND:
        cout << " COMMAND ";

        if (wParam == 1010 || wParam == 1011 || wParam == 1012)// кнопки "СТАРТ"
        {
            ShowWindow(hSta19[wParam - 1010], 1);// "оджидание соединения"
            ShowWindow(hSta21[wParam - 1010], 0);// "остановленно пользователем"
            UpdateWindow(hWnd);
            SendMessageA(HWNDVERSIYA[wParam - 1010], WM_GETTEXT, sizeof BUF_OBMENA_CHAR, (LPARAM)BUF_OBMENA_CHAR);// считываем введеную версию контроллера
            if (BUF_OBMENA_CHAR[0] != 'A')
            {
                ShowWindow(hSta19[wParam - 1010], 0);// "оджидание соединения"
                MessageBox(0, _T("Укажите версию контроллера!"), L"Сообщение", 0);
                return 0;
            }
            SendMessageA(HWNDPORT[wParam - 1010], WM_GETTEXT, sizeof BUF_OBMENA_CHAR, (LPARAM)BUF_OBMENA_CHAR);// считываем введеный номер порта
            if (BUF_OBMENA_CHAR[0] != 'C')
            {
                ShowWindow(hSta19[wParam - 1010], 0);// "оджидание соединения"
                MessageBox(0, _T("Укажите номер COM порта! "), L"Сообщение", 0);
                return 0;
            }
            t = time(nullptr);//текущее время
            localtime_s(&sistime, &t);// преобразуем в структуру tm
            strftime(SMENA[wParam - 1010], sizeof(SMENA[wParam - 1010]), "%d-%b-%Y   %H-%M-%S.xls", &sistime);// преобразуем в строку нужные нам части структуры tm
            MyFile[wParam - 1010].open(SMENA[wParam - 1010]); // открытие текстового файла
            MyFile[wParam - 1010].close(); // закрытие файла
            ///////////////открытие порта///////////////////////////////////////////////////////////////////
            initCOM[wParam - 1010] = CreateFileA(BUF_OBMENA_CHAR, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
            if (initCOM[wParam - 1010] == INVALID_HANDLE_VALUE)//проверка доступности порта
            {
                ShowWindow(hSta19[wParam - 1010], 0);// "оджидание соединения"
                MessageBox(0, _T("COM порт не доступен"), L"Сообщение об ошибке", 0);
                return 0;//выход
            }
            else
            {
                string setting = ": baud=9600 parity=N data=8 stop=1";
                string com = BUF_OBMENA_CHAR + setting;
                Setting_COM = (com).c_str(); // преобразование string в LPCSTR
                //MessageBoxA(0,Setting_COM,"check the settings:",0);
                Value[wParam - 1010] = BuildCommDCBA(Setting_COM, &DCB1);//присвоить настройки порту
                Value[wParam - 1010] = SetCommState(initCOM[wParam - 1010], &DCB1);//инициализация порта
            }
            Value[wParam - 1010] = CloseHandle(initCOM[wParam - 1010]);
            initCOM[wParam - 1010] = CreateFileA(BUF_OBMENA_CHAR, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
            if (initCOM[wParam - 1010] == INVALID_HANDLE_VALUE)//проверка доступности порта
            {
                ShowWindow(hSta19[wParam - 1010], 0);// "оджидание соединения"
                MessageBox(0, _T("COM порт не доступен"), L"Сообщение об ошибке", 0);
                return 0;//выход
            }
            else
            {
                string setting = ": baud=9600 parity=N data=8 stop=1";
                string com = BUF_OBMENA_CHAR + setting;
                Setting_COM = (com).c_str(); // преобразование string в LPCSTR
                //MessageBoxA(0,Setting_COM,"check the settings:",0);
                Value[wParam - 1010] = BuildCommDCBA(Setting_COM, &DCB1);//присвоить настройки порту
                Value[wParam - 1010] = SetCommState(initCOM[wParam - 1010], &DCB1);//инициализация порта
            }
            ////////////////проверка наличия данных в порте////////////////////////////////////////////////
               /* for (i=0;i<16;i++)
                    {
                        BUF_OBMENA_CHAR[i]=0x20;// очищаем буфер памяти com порта
                        BUF_OBMENA_INT[i]=0;// очищаем буфер памяти com порта
                    }
                SetCommTimeouts(initCOM[j],&timeout);
                          Value[j]= ReadFile(initCOM[j],&BUF_OBMENA_CHAR[0], 16,0, NULL);// считываем порт в буфер
                if(BUF_OBMENA_CHAR[0]==' ')
                {
                    Value[j]=CloseHandle(initCOM[j]);
                    EnableWindow(HWNDSTART, 0);//делаем кнопку старт неактивной
                    ShowWindow(hSta19[0], 0);
                    ShowWindow(hSta22[0], 1);// Проверьте контроллер или порт... нажмите сброс
                    return 0;
                }*/


                ////////////////изменения в окне//////////////////////////////////////////////////////
            HWNDEDIT1[wParam - 1010] = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER, 230, 50 + (wParam - 1010) * 200, 220, 30, hWnd, 0, hInst, 0);// Данные
            HWNDEDIT2[wParam - 1010] = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER, 500, 50 + (wParam - 1010) * 200, 110, 30, hWnd, 0, hInst, 0);// Пакеты
            hSta14[wParam - 1010] = CreateWindow(_T("STATIC"), _T("Пакеты данных с контроллера:"), WS_CHILD | WS_VISIBLE | SS_CENTER, 230, 30 + (wParam - 1010) * 200, 220, 20, hWnd, 0, hInst, 0);
            hSta15[wParam - 1010] = CreateWindow(_T("STATIC"), _T("Кол-во пакетов"), WS_CHILD | WS_VISIBLE | SS_CENTER, 500, 30 + (wParam - 1010) * 200, 110, 20, hWnd, 0, hInst, 0);//надпись
            hSta16[wParam - 1010] = CreateWindow(_T("STATIC"), _T("Кол-во ходов"), WS_CHILD | WS_VISIBLE | SS_CENTER, 5, 150 + (wParam - 1010) * 200, 95, 20, hWnd, 0, hInst, 0); // надпись
            hSta17[wParam - 1010] = CreateWindow(_T("STATIC"), _T("№ Пресса"), WS_CHILD | WS_VISIBLE | SS_CENTER, 5, 130 + (wParam - 1010) * 200, 95, 20, hWnd, 0, hInst, 0);//надпись
            EnableWindow(HWNDSTOP[wParam - 1010], 0);//делаем кнопку стоп неактивной
            for (i = 0; i < 16; i++)// рисуем заголовки прессов
            {
                _itow_s(i + 1, &BUFFER[1], 3, 10);
                hSta[i] = CreateWindow(_T("STATIC"), BUFFER, WS_CHILD | WS_VISIBLE | SS_CENTER, i * 50 + 100, 130 + (wParam - 1010) * 200, 50, 20, hWnd, 0, hInst, 0);
                hEdtSTAT[wParam - 1010][i] = CreateWindow(_T("EDIT"), _T("0"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT, i * 50 + 100, 150 + (wParam - 1010) * 200, 50, 20, hWnd, 0, hInst, 0);
                hEdt_0[wParam - 1010][i] = CreateWindow(_T("EDIT"), _T("0"), WS_CHILD | WS_BORDER | ES_RIGHT, i * 50 + 100, 190 + (wParam - 1010) * 200, 50, 20, hWnd, 0, hInst, 0);// рисуем новые ячейки для данных
                hEdt_1[wParam - 1010][i] = CreateWindow(_T("EDIT"), _T("1"), WS_CHILD | WS_BORDER | ES_RIGHT, i * 50 + 100, 190 + (wParam - 1010) * 200, 50, 20, hWnd, 0, hInst, 0);// рисуем новые ячейки для данных
            }

            EnableWindow(HWNDVERSIYA[wParam - 1010], 0);//делаем окно выбора контроллера неактивным
            EnableWindow(HWNDPORT[wParam - 1010], 0);//делаем окно выбора ком порта неактивным
            EnableWindow(HWNDSTART[wParam - 1010], 0);//делаем кнопку старт неактивной
            EnableWindow(HWNDRESET[wParam - 1010], 0);//делаем кнопку сброс неактивной
            EnableWindow(HWNDSTOP[wParam - 1010], 1);//делаем кнопку стоп активной
            ShowWindow(hSta19[wParam - 1010], 0);// "ожидание соединения"
            ShowWindow(hSta18[wParam - 1010], 1);// "соединение установлено"
            UpdateWindow(hWnd);
            SendMessage(hWnd, WM_USER + 100, (WPARAM)wParam, (LPARAM)lParam);// отправить сообщение в очередь


        }
        //----------------------------------------------------------------------------------------------------------
        switch (LOWORD(wParam))
        {
            ////////////////кнопка "стоп"///////////////////////////////////////////////////////////////////
        case 1020:
            //for (j=0;j<total_number_of_ports;j++)
            //{
            Value[0] = CloseHandle(initCOM[0]);//закрытие порта
            EnableWindow(HWNDSTOP[0], 0);//делаем кнопку стоп неактивной
            EnableWindow(HWNDSTART[0], 1);//делаем кнопку старт неактивной
            EnableWindow(HWNDRESET[0], 1);//делаем кнопку сброс активной
            if (Value[0] == 0) //ошибка закрытия
            {
                MessageBox(0, _T("Ошибка закрытия COM порта"), _T("Сообщение"), 0);
                return 0;
            }
            else
            {
                KillTimer(hWnd, 1);//отключить первый таймер
                ShowWindow(hSta21[0], 1);// "остановленно пользователем"
            }
            // }
            break;
            /*        ///////////////кнопка "ОТЧЕТ"////////////////////////////////////////////////////////////////////
                    case 1030:
                    ShowWindow(Child, SW_SHOW);// показать окно
                    break;
            */        ////////////// кнопка "СБРОС" //////////////////////////////////////////////////////////////////
        case 1040:
            for (i = 0; i < 16; i++)
            {
                SUMMA_PRESS[0][0] = 0;
                SendMessageA(hEdtSTAT[0][i], WM_SETTEXT, 0, (LPARAM)"0");// количество ходов
                ShowWindow(hEdt_1[0][i], 0);// скрыть окно
                ShowWindow(hEdt_0[0][i], 0);// скрыть окно
            }
            SendMessageA(HWNDEDIT1[0], WM_SETTEXT, 0, (LPARAM)"");
            SendMessageA(HWNDEDIT2[0], WM_SETTEXT, 0, (LPARAM)"");
            EnableWindow(HWNDSTART[0], 1);//делаем кнопку старт активной
            EnableWindow(HWNDVERSIYA[0], 1);//делаем окно выбора контроллера активным
            EnableWindow(HWNDPORT[0], 1);//делаем окно выбора ком порта активным
            ShowWindow(hSta13[0], 0);// скрыть окно
            ShowWindow(hSta18[0], 0);// скрыть окно
            ShowWindow(hSta19[0], 0);// скрыть окно
            ShowWindow(hSta20[0], 0);// скрыть окно
            ShowWindow(hSta21[0], 0);// скрыть окно
            ShowWindow(hSta22[0], 0);// скрыть окно
            x[0] = 0;// очищаем кол-во пакетов
            break;
        }
        break;
        /////////////////////////рабочий цикл №1///////////////////////////////////////////////////////////////////
    case WM_USER + 100:
        printf("WM_USER + 100");
        {

            x[wParam - 1010] = x[wParam - 1010] + 1;// считаем количество пакетов
            for (i = 0; i < 16; i++)
            {
                BUF_OBMENA_CHAR[i] = 0x30;// очищаем буфер памяти com порта
                BUF_OBMENA_INT[i] = 0;// очищаем буфер памяти com порта
            }
            SendMessageA(HWNDVERSIYA[wParam - 1010], WM_GETTEXT, sizeof BUF_OBMENA_CHAR, (LPARAM)BUF_OBMENA_CHAR);// считываем введеную версию контроллера
            if (BUF_OBMENA_CHAR[6] == '1')
            {
                ShowWindow(hSta19[wParam - 1010], 0);// "оджидание соединения"
                number_of_inputs = 16;// колличество входов у контроллера на ATmega128
                //MessageBox(0,_T("Укажите версию контроллера!"),L"Сообщение",0);

            }
            if (BUF_OBMENA_CHAR[6] == '3')
            {
                ShowWindow(hSta19[wParam - 1010], 0);// "оджидание соединения"
                number_of_inputs = 12;// колличество входов у контроллера на ATmega328
                //MessageBox(0,_T("Укажите версию контроллера!"),L"Сообщение",0);

            }

            //SetCommTimeouts(initCOM[0],&timeout);
            Value[wParam - 1010] = ReadFile(initCOM[wParam - 1010], &BUF_OBMENA_CHAR[0], number_of_inputs, 0, NULL);// считываем порт в буфер
            if (Value[wParam - 1010] == 0) //ошибка чтения порта
            {
                //SendMessage( hWnd, WM_USER + 100, (WPARAM)wParam, (LPARAM)lParam );// отправить сообщение в очередь
                //MessageBox(0,_T("НЕТ СВЯЗИ!!!"),_T("Сообщение об ошибке"),0);
                ShowWindow(hSta20[wParam - 1010], 1);// "обрыв связи"
                return 0;
            }
            else
            {
                SendMessageA(HWNDEDIT1[wParam - 1010], WM_SETTEXT, 0, (LPARAM)BUF_OBMENA_CHAR);//выводим на экран содержимое пакета
                wsprintfA(&EDIT[wParam - 1010][0], "%lu", x[wParam - 1010]);//преобразуем число циклов в символ
                SendMessageA(HWNDEDIT2[wParam - 1010], WM_SETTEXT, sizeof EDIT[wParam - 1010], (LPARAM)EDIT[wParam - 1010]);// выводим на экран количество пакетов

            }
            for (i = 0; i < number_of_inputs; i++)// преобразование символов в числа и подсчет ходов
            {
                if (BUF_OBMENA_CHAR[i] == '0')
                {
                    ShowWindow(hEdt_0[wParam - 1010][i], SW_SHOW);// показать окно
                    ShowWindow(hEdt_1[wParam - 1010][i], SW_HIDE);// скрыть окно
                    BUF_OBMENA_INT[i] = 0;
                    SUMMA_PRESS[wParam - 1010][i] = SUMMA_PRESS[wParam - 1010][i] + 0;
                    wsprintfA(&EDIT[wParam - 1010][i], "%lu", SUMMA_PRESS[wParam - 1010][i]);//преобразуем число в символ
                    SendMessageA(hEdtSTAT[wParam - 1010][i], WM_SETTEXT, 0, (LPARAM)&EDIT[wParam - 1010][i]);// выводим в окно количество ходов
                }
                else
                {
                    counting_inputs = counting_inputs + 1; // подсчет сработавших прессов за одну отправку
                    ShowWindow(hEdt_1[wParam - 1010][i], SW_SHOW);// показать окно
                    ShowWindow(hEdt_0[wParam - 1010][i], SW_HIDE);// скрыть окно
                    BUF_OBMENA_INT[i] = 1;
                    SUMMA_PRESS[wParam - 1010][i] = SUMMA_PRESS[wParam - 1010][i] + 1;
                    wsprintfA(&EDIT[wParam - 1010][i], "%lu", SUMMA_PRESS[wParam - 1010][i]);//преобразуем число в символ
                    SendMessageA(hEdtSTAT[wParam - 1010][i], WM_SETTEXT, 0, (LPARAM)&EDIT[wParam - 1010][i]);// выводим в окно количество ходов
                }
            }
            if (counting_inputs == number_of_inputs)//подсчет возможных ошибок данных с контроллера
            {
                counting_inputs = 0;
                data_error = data_error + 1;
            }
            else// сброс счетчиков ошибок
            {
                data_error = 0;
                counting_inputs = 0;
            }
            if (data_error == 3)//при накоплении 3 ошибок сообщение и выход из программы
            {
                data_error = 0;
                MessageBox(0, _T("Связь с контроллером нарушена!!!"), _T("Сообщение об ошибке"), 0);
                return 0;
            }
            t = time(nullptr);//текущее время
            localtime_s(&sistime, &t);// преобразуем в структуру tm
            strftime(TIME, sizeof(TIME), "%X", &sistime);// преобразуем в строку нужные нам части структуры tm
            SendMessageA(hSta13[wParam - 1010], WM_SETTEXT, 0, (LPARAM)TIME);// выводим на экран время
            ShowWindow(hSta13[wParam - 1010], 1);// показать  окно времени
            SetTimer(hWnd, wParam - 1010 + 1, 4950, NULL);//включить первый таймер на 4950 миллисек   Условная задержка

            MyFile[wParam - 1010].open(SMENA[wParam - 1010], std::ios::app);//открытие файла для добавления информации
            MyFile[wParam - 1010] << TIME << "\t" << BUF_OBMENA_INT[0] << "\t" << BUF_OBMENA_INT[1] << "\t" << BUF_OBMENA_INT[2] << "\t" << BUF_OBMENA_INT[3] << "\t" << BUF_OBMENA_INT[4] << "\t" << BUF_OBMENA_INT[5] << "\t"
                << BUF_OBMENA_INT[6] << "\t" << BUF_OBMENA_INT[7] << "\t" << BUF_OBMENA_INT[8] << "\t" << BUF_OBMENA_INT[9] << "\t" << BUF_OBMENA_INT[10] << "\t" << BUF_OBMENA_INT[11] << "\t" << BUF_OBMENA_INT[12] << "\t"
                << BUF_OBMENA_INT[13] << "\t" << BUF_OBMENA_INT[14] << "\t" << BUF_OBMENA_INT[15] << "\t" << "\n";// запись в файл
            MyFile[wParam - 1010].close();// закрытие файла

            UpdateWindow(hWnd);// обновляем окно
        }
        break;
        ////////////////////////////////////// второй рабочий цикл ////////////////////////////////////////
         /*       case WM_USER + 101:
                     printf("WM_USER + 101");
                     {

                        x[1] = x[1] + 1;// считаем количество пакетов
                          for (i=0;i<16;i++)
                            {
                               BUF_OBMENA_CHAR[i]=0x30;// очищаем буфер памяти com порта
                             BUF_OBMENA_INT[i]=0;// очищаем буфер памяти com порта
                            }
                        SendMessageA(HWNDVERSIYA[1],WM_GETTEXT,sizeof BUF_OBMENA_CHAR,(LPARAM)BUF_OBMENA_CHAR);// считываем введеную версию контроллера
                        if(BUF_OBMENA_CHAR[6] =='1')
                          {
                             ShowWindow(hSta19[1], 0);// "оджидание соединения"
                             number_of_inputs=16;// колличество входов у контроллера на ATmega128
                             //MessageBox(0,_T("Укажите версию контроллера!"),L"Сообщение",0);

                          }
                        if(BUF_OBMENA_CHAR[6] =='3')
                          {
                             ShowWindow(hSta19[1], 0);// "оджидание соединения"
                             number_of_inputs=12;// колличество входов у контроллера на ATmega328
                             //MessageBox(0,_T("Укажите версию контроллера!"),L"Сообщение",0);

                          }

                     //SetCommTimeouts(initCOM[0],&timeout);
                               Value[1]= ReadFile(initCOM[1],&BUF_OBMENA_CHAR[0], number_of_inputs,0, NULL);// считываем порт в буфер
                               if (Value[1]==0) //ошибка чтения порта
                                   {
                          //SendMessage( hWnd, WM_USER + 100, (WPARAM)wParam, (LPARAM)lParam );// отправить сообщение в очередь
                          //MessageBox(0,_T("НЕТ СВЯЗИ!!!"),_T("Сообщение об ошибке"),0);
                          ShowWindow(hSta20[1], 1);// "обрыв связи"
                          return 0;
                                 }
                               else
                                   {
                                        SendMessageA(HWNDEDIT1[1],WM_SETTEXT,0,(LPARAM) BUF_OBMENA_CHAR);//выводим на экран содержимое пакета
                          wsprintfA(&EDIT[1][0],"%lu",x[1]);//преобразуем число циклов в символ
                          SendMessageA(HWNDEDIT2[1],WM_SETTEXT,sizeof EDIT[1],(LPARAM) EDIT[1]);// выводим на экран количество пакетов

                                   }
                      for(i=0;i<number_of_inputs;i++)// преобразование символов в числа и подсчет ходов
                        {
                          if (BUF_OBMENA_CHAR[i] == '0')
                            {
                               ShowWindow(hEdt_0[1][i], SW_SHOW);// показать окно
                               ShowWindow(hEdt_1[1][i], SW_HIDE);// скрыть окно
                               BUF_OBMENA_INT [i] = 0;
                               SUMMA_PRESS[1][i]=SUMMA_PRESS[1][i]+0;
                               wsprintfA(&EDIT[1][i],"%lu",SUMMA_PRESS[1][i]);//преобразуем число в символ
                               SendMessageA(hEdtSTAT[1][i],WM_SETTEXT,0,(LPARAM) &EDIT[1][i]);// выводим в окно количество ходов
                            }
                          else
                            {
                               counting_inputs= counting_inputs+1; // подсчет сработавших прессов за одну отправку
                               ShowWindow(hEdt_1[1][i], SW_SHOW);// показать окно
                               ShowWindow(hEdt_0[1][i], SW_HIDE);// скрыть окно
                               BUF_OBMENA_INT [i] = 1;
                               SUMMA_PRESS[1][i]=SUMMA_PRESS[1][i]+1;
                               wsprintfA(&EDIT[1][i],"%lu",SUMMA_PRESS[1][i]);//преобразуем число в символ
                               SendMessageA(hEdtSTAT[1][i],WM_SETTEXT,0,(LPARAM) &EDIT[1][i]);// выводим в окно количество ходов
                            }
                        }
                      if(counting_inputs==number_of_inputs)//подсчет возможных ошибок данных с контроллера
                        {
                           counting_inputs=0;
                           data_error=data_error+1;
                        }
                        else// сброс счетчиков ошибок
                        {
                          data_error=0;
                          counting_inputs=0;
                        }
                      if(data_error==3)//при накоплении 3 ошибок сообщение и выход из программы
                        {
                           data_error=0;
                           MessageBox(0,_T("Связь с контроллером нарушена!!!"),_T("Сообщение об ошибке"),0);
                           return 0;
                        }
                      t=time(nullptr);//текущее время
                      sistime = localtime(&t);// преобразуем в структуру tm
                      strftime(TIME, sizeof(TIME), "%X", sistime);// преобразуем в строку нужные нам части структуры tm
                      SendMessageA(hSta13[1],WM_SETTEXT,0,(LPARAM)TIME);// выводим на экран время
                      ShowWindow(hSta13[1], 1);// показать  окно времени
                      SetTimer(hWnd,2,4950,NULL);//включить второй таймер на 4950 миллисек   Условная задержка

        MyFile[1].open(SMENA[1],std::ios::app);//открытие файла для добавления информации
        MyFile[1] << TIME <<"\t"<<BUF_OBMENA_INT[0]<<"\t"<<BUF_OBMENA_INT[1]<<"\t"<<BUF_OBMENA_INT[2]<<"\t"<<BUF_OBMENA_INT[3]<<"\t"<<BUF_OBMENA_INT[4]<<"\t"<<BUF_OBMENA_INT[5]<<"\t"
        <<BUF_OBMENA_INT[6]<<"\t"<<BUF_OBMENA_INT[7]<<"\t"<<BUF_OBMENA_INT[8]<<"\t"<<BUF_OBMENA_INT[9]<<"\t"<<BUF_OBMENA_INT[10]<<"\t"<<BUF_OBMENA_INT[11]<<"\t"<<BUF_OBMENA_INT[12]<<"\t"
        <<BUF_OBMENA_INT[13]<<"\t"<<BUF_OBMENA_INT[14]<<"\t"<<BUF_OBMENA_INT[15]<<"\t"<<"\n";// запись в файл
        MyFile[1].close();// закрытие файла

                     UpdateWindow(hWnd);// обновляем окно
                     }
                break;
        */
    case WM_TIMER:// по окочанию таймера выполнить...
        printf("TIMER");
        for (j = 0; j < total_number_of_ports; j++)
        {
            if (wParam == j + 1)
            {
                KillTimer(hWnd, j + 1);//отключить таймер
                SendMessage(hWnd, WM_USER + 100, 1010 + j, (LPARAM)lParam); // отправить сообщение в очередь о повторном запуске цикла
            }
        }
        break;
        ////////////////////////////////////////////////////////////////////////////////////////////////
    case WM_CREATE:// Вызывается, когда впервые создается основное окно
        printf("CREATE");
        /*   /////////////////// создаем дочернее окно //////////////////////////////////////////////////////
           WNDCLASS w;
           // ЗАПОЛНЯЕМ СТРУКТУРУ КЛАССА ОКНА
           memset(&w,0,sizeof(WNDCLASS));
           w.lpfnWndProc    = ChildProc;//указатель на функцию окна (оконную процедуру)
           w.hInstance      = hInst;// дискриптор экземпляра приложения в которм находится оконное приложение
           w.hCursor        = LoadCursor(NULL, IDC_ARROW);//дискриптор курсора
           w.hbrBackground  = CreateSolidBrush(RGB(155, 255, 255));//дискриптор кисти для закраски фона окна
           w.lpszClassName  = _T("ChildWClass");//указатель на имя класса
           // РЕГИСТРИРУЕМ КЛАСС ОКНА
           RegisterClass(&w);

           // СОЗДАЁМ ОСНОВНОЕ ОКНО ПРИЛОЖЕНИЯ
           Child = CreateWindowEx(0,_T("ChildWClass"),_T("Авторизация"), WS_OVERLAPPEDWINDOW,750,400,300,170, hWnd, NULL, hInst, NULL);// создание дочернего окна
           Child_2 = CreateWindowEx(0,_T("ChildWClass"),_T("Выбор отчета"), WS_OVERLAPPEDWINDOW,650,300,500,300, hWnd, NULL, hInst, NULL);// создание дочернего окна
           HWNDPASSWORDSTAT= CreateWindow(_T("STATIC"),_T("Введите пароль"),WS_CHILD|WS_VISIBLE|SS_CENTER,60,10,150,20,Child,0,hInst,0);
           HWNDPASSWORDEDIT= CreateWindowEx(WS_EX_CLIENTEDGE,_T("EDIT"),_T(""),WS_CHILD|WS_VISIBLE|WS_BORDER|SS_CENTER,60,40,150,25,Child,0,hInst,0);
           HWNDPASSWORDBUTTON1= CreateWindow(_T("BUTTON"),_T("ОК"),WS_CHILD|WS_BORDER|WS_VISIBLE|BS_NOTIFY,60,80,70,30,Child,PASSWORDBTN1,hInst,0);
           HWNDPASSWORDBUTTON2= CreateWindow(_T("BUTTON"),_T("Отмена"),WS_CHILD|WS_BORDER|WS_VISIBLE|BS_NOTIFY,140,80,70,30,Child,PASSWORDBTN2,hInst,0);
           ShowWindow(Child, SW_HIDE);// скрыть окно
           UpdateWindow(Child);// обновить окно
        */    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //  HWNDOTCHET = CreateWindowEx(WS_EX_DLGMODALFRAME, _T("BUTTON"), _T("ОТЧЕТ"),WS_CHILD |BS_DEFPUSHBUTTON| WS_VISIBLE ,1000, 50, 150, 150, hWnd, BTN3, hInst, 0);// кнопка
        for (j = 0; j < total_number_of_ports; j++)
        {
            HWNDSTART[j] = CreateWindowEx(WS_EX_DLGMODALFRAME, _T("BUTTON"), _T("СTAРT"), WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE | BS_PUSHLIKE, 650, 50 + j * 200, 100, 30, hWnd, BTN1[j], hInst, 0);// кнопка
            HWNDRESET[j] = CreateWindowEx(WS_EX_DLGMODALFRAME, _T("BUTTON"), _T("СБРОС"), WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE, 850, 50 + j * 200, 100, 30, hWnd, BTN4[j], hInst, 0);// кнопка
            HWNDSTOP[j] = CreateWindowEx(WS_EX_DLGMODALFRAME, _T("BUTTON"), _T("СТОП"), WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE | BS_PUSHLIKE, 750, 50 + j * 200, 100, 30, hWnd, BTN2[j], hInst, 0);// кнопка
            hSta13[j] = CreateWindow(_T("EDIT"), _T(""), WS_CHILD | WS_BORDER | SS_CENTER, 5, 190 + j * 200, 95, 20, hWnd, 0, hInst, 0);//рисуем ячейку для времени
            hSta18[j] = CreateWindow(_T("STATIC"), _T("Соединение установлено..."), WS_CHILD, 240, 90 + j * 200, 400, 20, hWnd, 0, hInst, 0);//надпись
            hSta19[j] = CreateWindow(_T("STATIC"), _T("Ожидание соединения..."), WS_CHILD, 240, 90 + j * 200, 400, 20, hWnd, 0, hInst, 0);//надпись
            hSta20[j] = CreateWindow(_T("STATIC"), _T("Обрыв связи... , нажмите СТОП"), WS_CHILD, 240, 90 + j * 200, 400, 20, hWnd, 0, hInst, 0);//надпись
            hSta21[j] = CreateWindow(_T("STATIC"), _T("Остановлено пользователем... нажмите сброс"), WS_CHILD, 240, 90 + j * 200, 400, 20, hWnd, 0, hInst, 0);//надпись
            hSta22[j] = CreateWindow(_T("STATIC"), _T("Проверьте контроллер или порт... нажмите сброс"), WS_CHILD, 240, 90 + j * 200, 400, 20, hWnd, 0, hInst, 0);//надпись
            HWNDPORT[j] = CreateWindow(_T("COMBOBOX"), NULL, CBS_DROPDOWN | WS_CHILD | WS_VISIBLE | WS_VSCROLL, 30, 50 + j * 200, 150, 150, hWnd, 0, hInst, 0);// окно для выбора com портa
            for (i = 0; i < 10; i++)//заполняем список для выбора com порта
            {
                char NumberCOM[5] = "COM";
                wsprintfA(&NumberCOM[3], "%lu", i);//преобразуем число в символ
                SendMessageA(HWNDPORT[j], CB_ADDSTRING, 0, (LPARAM)&NumberCOM[0]);
            }
            SendMessage(HWNDPORT[j], WM_SETTEXT, 0, (LPARAM)_T("COM-порт:"));
            HWNDVERSIYA[j] = CreateWindow(_T("COMBOBOX"), NULL, CBS_DROPDOWN | WS_CHILD | WS_VISIBLE | WS_VSCROLL, 30, 75 + j * 200, 150, 150, hWnd, 0, hInst, 0);// окно для выбора версии контроллера
            for (i = 1; i < 4; i = i + 2)//заполняем список для выбора версии
            {
                char VERSIYA[10] = "ATmega";
                wsprintfA(&VERSIYA[6], "%lu", i * 100 + 28);//преобразуем число в символ
                SendMessageA(HWNDVERSIYA[j], CB_ADDSTRING, 0, (LPARAM)&VERSIYA[0]);
            }
            SendMessage(HWNDVERSIYA[j], WM_SETTEXT, 0, (LPARAM)_T("Контроллер:"));
        }


        break;
    case WM_CTLCOLORSTATIC:// изменение цветов в статических окнах
        printf(" COLORSTATIC ");
        SetTextColor((HDC)wParam, RGB(0, 0, 0));
        SetBkColor((HDC)wParam, RGB(220, 220, 150));
        return (LRESULT)fon;
        break;
    case WM_CTLCOLOREDIT:// изменение цветов в динамических окнах
        printf(" COLOREDIT ");
        for (j = 0; j < total_number_of_ports; j++)
        {
            if (hSta13[j] == (HWND)lParam)
            {
                //SetTextColor((HDC)wParam, RGB(0,0,0));
                SetBkColor((HDC)wParam, RGB(150, 220, 250));
                return (LRESULT)fon;
            }
            for (i = 0; i < 16; i++)
            {
                if (hEdt_0[j][i] == (HWND)lParam)
                {
                    SetBkColor((HDC)wParam, RGB(255, 0, 0));
                    return (LRESULT)fon;
                }
                if (hEdt_1[j][i] == (HWND)lParam)
                {

                    SetBkColor((HDC)wParam, RGB(0, 255, 0));
                    return (LRESULT)fon;
                }
            }
        }
        break;
        //case WM_CTLCOLORBTN:// изменение цветов в кнопках
        //break;
        //case WM_PAINT:
        //    printf(" PAINT ");
        //{
        //    PAINTSTRUCT ps;
        //    HDC hdc = BeginPaint(hWnd, &ps);
            //SetTextColor(hdc,RGB(0, 0 , 255));// цвет текста
            //SetBkMode(hdc, TRANSPARENT);
            //SetBkColor(hdc, RGB(0, 255 , 255)); свой цвет фона
            //RECT  rect;
            //GetClientRect(hWnd, &rect);
            //rect.left = 10;
            //rect.top = 10;
            //DrawText(hdc,_T("Результат:"),-1,&rect,DT_LEFT);
            //DrawText(hdc,_T("Данные от контроллера"),-1,&rect,DT_CENTER);
            //TextOut(hdc,180,30,_T("Пакеты данных с контроллера:"),28);
            //TextOut(hdc,450,30,_T("Кол-во пакетов"),14);
            //TextOut(hdc,5,150,_T("Кол-во ходов"),12);
            //TextOut(hdc,5,130,_T("№ Пресса"),8);
            //int a;
            //wchar_t BUFFER [3] = L"№";
            //for (a=1;a<13;a++)
            //{
            // _itow(a, &BUFFER[1], 10);
            // TextOut(hdc,a*50+60,130, BUFFER,3);
            //}
        //    EndPaint(hWnd, &ps);
        //}
        //break;
    case WM_SIZE://установка вертикального размера окна c прокруткой
    {
        SCROLLINFO scrInfo;
        scrInfo.cbSize = sizeof(SCROLLINFO);
        scrInfo.nPage = HIWORD(lParam); //размер страницы устанавливаем равным высоте окна
        scrInfo.nMin = 0; //диапазон прокрутки устанавливаем по размеру содержимого
        scrInfo.nMax = 3000; //(вместо CONTENT_HEIGHT подставь нужное значение)
        scrInfo.fMask = SIF_RANGE | SIF_PAGE; //применяем новые параметры
        SetScrollInfo(hWnd, SB_VERT, &scrInfo, TRUE);
    }
    break;
    case WM_VSCROLL:
    {
        SCROLLINFO scrInfo;
        scrInfo.cbSize = sizeof(SCROLLINFO);
        scrInfo.fMask = SIF_ALL; //получаем текущие параметры scrollbar-а
        GetScrollInfo(hWnd, SB_VERT, &scrInfo);
        int currentPos = scrInfo.nPos; //запоминаем текущее положение содержимого
        switch (LOWORD(wParam))
        { //действия прокрутки
        case SB_LINEUP: //клик на стрелку вверх
            scrInfo.nPos -= 1;
            break;
        case SB_LINEDOWN: //клик на стрелку вниз
            scrInfo.nPos += 1;
            break;
        case SB_THUMBTRACK: //перетаскивание ползунка
            scrInfo.nPos = scrInfo.nTrackPos;
            break;
        case WM_MOUSEWHEEL://колесико мышки
            //scrInfo.nPos  += 1;
            break;
        default:
            return 0; //все прочие действия (например нажатие PageUp/PageDown) игнорируем
        }
        scrInfo.fMask = SIF_POS; //пробуем применить новое положение
        SetScrollInfo(hWnd, SB_VERT, &scrInfo, TRUE);
        GetScrollInfo(hWnd, SB_VERT, &scrInfo); //(см. примечание ниже)
        int yScroll = currentPos - scrInfo.nPos; // вычисляем величину прокрутки
        ScrollWindow(hWnd, 0, yScroll, NULL, NULL); //выполняем прокрутку
        SendMessage(hWnd, WM_CTLCOLORSTATIC, (WPARAM)wParam, (LPARAM)lParam); // отправить сообщение в очередь о изменении цвета
    }
    break;
    case WM_DESTROY:// Вызывается, когда закрываешь окно
        //закрытие портa
        for (j = 0; j < total_number_of_ports; j++)
        {
            Value[j] = CloseHandle(initCOM[j]);
            while (Value[j] != 0) {}
        }

        // {
         //    return 0;
        // }
        // else
        // {
        //   Value[j] = CloseHandle(initCOM[j]);
        // }
         //}
         //if (Value[0]==0) //error close port
         //   {
         //	   MessageBox(hWnd,_T("Передумал?"),L"Сообщение",MB_ICONEXCLAMATION|MB_YESNO|MB_APPLMODAL);
         //   }
         //else
         //   {
         //	   MessageBox(0,_T("СОМ порт закрыт"),L"Сообщение",MB_ICONEXCLAMATION);
         //   }
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//=====================================ОБРАБОТЧИК СООБЩЕНИЙ ДОЧЕРНЕГО ОКНА==============================================//

LRESULT CALLBACK ChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)//это оконная процедура для дочернего окна
{
    printf("*");
    switch (message)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
            /////////////// кнопка "Подтверждение пароля"//////////////////////////////////////////////////////////
        case 105:
            printf("OK Child");
            SendMessageA(HWNDPASSWORDEDIT, WM_GETTEXT, sizeof(BUFchar), (LPARAM)BUFchar);// считываем
            BUFstring = BUFchar;
            if (BUFstring == Password)
            {
                ShowWindow(Child, 0);// скрыть окно
                ShowWindow(Child_2, 1);// показать окно выбора отчета
                //system("start kontrol_1_03.09.2024_7.00-19.00.xls");
            }
            else
            {
                //MessageBox(Child,_T("Пароль не верный"),_T("Сообщение"),MB_ICONSTOP);
                //ShowWindow(Child, 0);// скрыть окно
            }
            SendMessageA(HWNDPASSWORDEDIT, WM_SETTEXT, 2, 0);
            break;
            ////////////// кнопка "Отмена пароля" ////////////////////////////////////////////////////////////////
        case 106:
            ShowWindow(Child, 0);// скрыть окно
            break;
        default:
            break;
        }
    case WM_CREATE:

        //MessageBox(0,_T("Данная функция пока не доступна. /ZR"),_T("Сообщение"),0);
        //system("start DATA.xls");
        break;
    case WM_CTLCOLORSTATIC:// изменение цветов в статических окнах
        printf(" COLORSTATIC ");
        SetTextColor((HDC)wParam, RGB(0, 0, 0));
        SetBkColor((HDC)wParam, RGB(155, 255, 155));
        return (LRESULT)CreateSolidBrush(RGB(155, 255, 255));
        break;
    case WM_DESTROY:// Вызывается, когда закрываешь окно
        printf("close child");
        /////////////////// создаем дочернее окно //////////////////////////////////////////////////////
        WNDCLASS w;
        // ЗАПОЛНЯЕМ СТРУКТУРУ КЛАССА ОКНА
        memset(&w, 0, sizeof(WNDCLASS));
        w.lpfnWndProc = ChildProc;//указатель на функцию окна (оконную процедуру)
        w.hInstance = hInst;// дискриптор экземпляра приложения в которм находится оконное приложение
        w.hCursor = LoadCursor(NULL, IDC_ARROW);//дискриптор курсора
        w.hbrBackground = CreateSolidBrush(RGB(155, 255, 255));//дискриптор кисти для закраски фона окна
        w.lpszClassName = _T("ChildWClass");//указатель на имя класса
        // РЕГИСТРИРУЕМ КЛАСС ОКНА
        RegisterClass(&w);
        // СОЗДАЁМ ОСНОВНОЕ ОКНО ПРИЛОЖЕНИЯ
        Child = CreateWindowEx(0, _T("ChildWClass"), _T("Авторизация"), WS_OVERLAPPEDWINDOW, 750, 400, 300, 170, hWnd, NULL, hInst, NULL);// создание дочернего окна
        Child_2 = CreateWindowEx(0, _T("ChildWClass"), _T("Выбор отчета"), WS_OVERLAPPEDWINDOW, 650, 300, 500, 300, hWnd, NULL, hInst, NULL);// создание дочернего окна
        HWNDPASSWORDSTAT = CreateWindow(_T("STATIC"), _T("Введите пароль"), WS_CHILD | WS_VISIBLE | SS_CENTER, 60, 10, 150, 20, Child, 0, hInst, 0);
        HWNDPASSWORDEDIT = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER, 60, 40, 150, 25, Child, 0, hInst, 0);
        HWNDPASSWORDBUTTON1 = CreateWindow(_T("BUTTON"), _T("ОК"), WS_CHILD | WS_BORDER | WS_VISIBLE | BS_NOTIFY, 60, 80, 70, 30, Child, PASSWORDBTN1, hInst, 0);
        HWNDPASSWORDBUTTON2 = CreateWindow(_T("BUTTON"), _T("Отмена"), WS_CHILD | WS_BORDER | WS_VISIBLE | BS_NOTIFY, 140, 80, 70, 30, Child, PASSWORDBTN2, hInst, 0);
        //HWNDPORT[j]= CreateWindow(_T("COMBOBOX"),NULL,CBS_DROPDOWN|WS_CHILD|WS_VISIBLE|WS_VSCROLL,30,50+j*200,150,150,hWnd,0,hInst,0);// окно для выбора com портa
        //ShowWindow(Child, SW_HIDE);// скрыть окно
        UpdateWindow(Child);// обновить окно
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
    return 0;
}


