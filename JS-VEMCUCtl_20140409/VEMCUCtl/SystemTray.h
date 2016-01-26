#ifndef _SYSTEM_TRAY_H_
#define _SYSTEM_TRAY_H_

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CSystemTray window

class CSystemTray : public CWnd
{
	// Construction/destruction
public:
	CSystemTray();
	CSystemTray(CWnd* pWnd, UINT uCallbackMessage, LPCTSTR szTip, HICON icon, UINT uID);
	virtual ~CSystemTray();

	DECLARE_DYNAMIC(CSystemTray)

	// Operations
public:
	BOOL Enabled() { return m_bEnabled; }
	BOOL Visible() { return !m_bHidden; }

	// Create the tray icon
	BOOL Create(CWnd* pParent, UINT uCallbackMessage, LPCTSTR szTip, HICON icon, UINT uID);

	// Change or retrieve the Tooltip text
	BOOL    SetTooltipText(LPCTSTR pszTooltipText);
	BOOL    SetTooltipText(UINT nID);
	CString GetTooltipText() const;

	// Change or retrieve the icon displayed
	BOOL  SetIcon(HICON hIcon);
	BOOL  SetIcon(LPCTSTR lpszIconName);
	BOOL  SetIcon(UINT nIDResource);
	BOOL  SetStandardIcon(LPCTSTR lpIconName);
	BOOL  SetStandardIcon(UINT nIDResource);
	HICON GetIcon() const;
	void  HideIcon();
	void  ShowIcon();
	void  RemoveIcon();
	void  MoveToRight();

	// For icon animation
	BOOL  SetIconList(UINT uFirstIconID, UINT uLastIconID); 
	BOOL  SetIconList(HICON* pHIconList, UINT nNumIcons); 
	BOOL  Animate(UINT nDelayMilliSeconds, int nNumSeconds = -1);
	BOOL  StepAnimation();
	BOOL  StopAnimation();

	// Change menu default item
	void GetMenuDefaultItem(UINT& uItem, BOOL& bByPos);
	BOOL SetMenuDefaultItem(UINT uItem, BOOL bByPos);

	// Change or retrieve the window to send notification messages to
	BOOL  SetNotificationWnd(CWnd* pNotifyWnd);
	CWnd* GetNotificationWnd() const;

	// Default handler for tray notification message
	virtual LRESULT OnTrayNotification(WPARAM uID, LPARAM lEvent);

public:
	static UINT  m_nIDEvent;

	CMenu  *m_pMenu;
	CMenu  *pSubMenu;
	BOOL      m_bEnabled;   // does O/S support tray icon?
	BOOL      m_bHidden;    // Has the icon been hidden?
	NOTIFYICONDATA  m_tnd;

	CArray<HICON, HICON> m_IconList;

	UINT      m_uIDTimer;
	int          m_nCurrentIcon;
	COleDateTime m_StartTime;
	int             m_nAnimationPeriod;
	HICON      m_hSavedIcon;
	UINT         m_DefaultMenuItemID;
	BOOL        m_DefaultMenuItemByPos;

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void Initialise();

protected:
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};


#endif

/////////////////////////////////////////////////////////////////////////////
