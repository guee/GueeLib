#include "stdafx.h"
#include "GDialog.h"
#include "GDummyDesktop.h"


GDialog::GDialog()
{
	m_widgetAtts[eWA_PopupDialog]	= 1;
	m_widgetAtts[eWA_WindowFrame]	= 1;
	m_widgetAtts[eWA_SystemWindowFrame]	= 1;
	m_widgetAtts[eWA_ResizeBorder]	= 1;

	moveToCenter();
}


GDialog::~GDialog()
{
}

