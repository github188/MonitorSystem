// PrecitionEdit.cpp : 实现文件
//

#include "stdafx.h"
//#include "sp_draw.h"
#include "../../resource.h"
#include "canvas/PrecitionEdit.h"
#include "dialog/mccurvedialog.h"


// CPrecitionEdit

IMPLEMENT_DYNAMIC(CPrecitionEdit, CEdit)
CPrecitionEdit::CPrecitionEdit()
{
}

CPrecitionEdit::~CPrecitionEdit()
{
}


BEGIN_MESSAGE_MAP(CPrecitionEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()



// CPrecitionEdit 消息处理程序


void CPrecitionEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar<=122&&nChar>=97)
	{
		return;
	}

	CMcCurvePage1* pPage=(CMcCurvePage1*)GetParent();
	int nprecition;
	char szText[20];
	char *token;
	char seps[]=".";
	if (GetDlgCtrlID()==IDC_EDIT15||GetDlgCtrlID()==IDC_EDIT16)
	{
		CComboBox* pbox=(CComboBox*)pPage->GetDlgItem(IDC_COMBO2);
		nprecition=pbox->GetCurSel();
		switch (nprecition)
		{
		case 0:
			if (nChar==46)
			{
				return;
			}
			break;
			
		case 1:
			GetWindowText(szText,20);
			token=strtok(szText,seps);
			if (token!=NULL)
			{
				token=strtok(NULL,seps);
				if (token!=NULL)
				{
					if (strlen(token)>=1&&(nChar<=57&&nChar>=48))
					{
						return;
					}
				}
			}
			break;
		case 2:
			GetWindowText(szText,20);
			token=strtok(szText,seps);
			if (token!=NULL)
			{
				token=strtok(NULL,seps);
				if (token!=NULL)
				{
					if (strlen(token)>=2&&(nChar<=57&&nChar>=48))
					{
						return;
					}
				}
				
			}
			break;
		}
	}
	else if (GetDlgCtrlID()==IDC_EDIT17||GetDlgCtrlID()==IDC_EDIT18)
	{
		CComboBox* pbox=(CComboBox*)pPage->GetDlgItem(IDC_COMBO1);
		nprecition=pbox->GetCurSel();
		switch (nprecition)
		{
		case 0:
			if (nChar==46)
			{
				return;
			}
			break;
		case 1:
			GetWindowText(szText,20);
			token=strtok(szText,seps);
			if (token!=NULL)
			{
				token=strtok(NULL,seps);
				if (token!=NULL)
				{
					if (strlen(token)>=1&&(nChar<=57&&nChar>=48))
					{
						return;
					}
				}
				
			}
			break;
		case 2:
			GetWindowText(szText,20);
			token=strtok(szText,seps);
			if (token!=NULL)
			{
				token=strtok(NULL,seps);
				if (token!=NULL)
				{
					if (strlen(token)>=2&&(nChar<=57&&nChar>=48))
					{
						return;
					}
				}
			}
			break;
		}
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
