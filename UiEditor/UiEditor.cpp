// UiEditor.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "UiEditor.h"
#include "..\GueeLib\GDummyDesktop.h"
#pragma comment( lib, "..\\Win32\\Debug\\GueeLib.lib")

#include "..\GueeLib\GDialog.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	GDialog		widget;
	widget.show();
	GDummyDesktop::destkop().exec();
}