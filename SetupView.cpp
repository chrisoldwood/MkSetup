/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SETUPVIEW.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CSetupView class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CSetupView::CSetupView(CSetupDoc& rDoc)
	: CView(rDoc)
{
	DEFINE_CTRLMSG_TABLE
		NFY_CTRLMSG(IDC_FILES, LVN_ITEMCHANGED, OnListSelChange  )
		NFY_CTRLMSG(IDC_FILES, NM_DBLCLK,       OnListDoubleClick)
	END_CTRLMSG_TABLE
}

/******************************************************************************
** Method:		Destructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CSetupView::~CSetupView()
{
}

/******************************************************************************
** Method:		OnCreate()
**
** Description:	Create the files listview.
**
** Parameters:	rcClient	The client rectangle.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSetupView::OnCreate(const CRect& /*rcClient*/)
{
	DWORD dwExStyle = WS_EX_CLIENTEDGE;
	DWORD dwStyle   = WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_BORDER;

	dwStyle |= LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_SORTASCENDING | LVS_NOSORTHEADER;

	// Create the listview.
	m_lvFiles.Create(*this, IDC_FILES, ClientRect(), dwExStyle, dwStyle);

	m_lvFiles.InsertColumn(0, "File",       150);
	m_lvFiles.InsertColumn(1, "Shortcut?",   75);
	m_lvFiles.InsertColumn(2, "Desk Icon?",  75);

	m_lvFiles.FullRowSelect();
}

/******************************************************************************
** Method:		OnResize()
**
** Description:	Resize the child window.
**
** Parameters:	iFlag		The resize method.
**				rNewSize	The new window dimensions.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSetupView::OnResize(int /*iFlag*/, const CSize& /*rNewSize*/)
{
	m_lvFiles.Move(ClientRect());
}

/******************************************************************************
** Method:		RefreshFileList()
**
** Description:	Refresh the listview of files.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSetupView::RefreshFileList()
{
	CSetupDoc& oDoc = Doc();

	// Preseve selection.
	int nSel = m_lvFiles.Selection();

	// Clear view.
	m_lvFiles.DeleteAllItems();

	// For all files...
	for (int i = 0; i < oDoc.m_aoFiles.Size(); ++i)
	{
		CFileProps* pFileProps = oDoc.m_aoFiles[i];

		int n = m_lvFiles.AppendItem(pFileProps->m_strFileName);

		m_lvFiles.ItemText(n, 1, (pFileProps->m_bProgIcon) ? "Yes" : "");
		m_lvFiles.ItemText(n, 2, (pFileProps->m_bDeskIcon) ? "Yes" : "");

		m_lvFiles.ItemPtr(n, pFileProps);
	}

	// Restore selection.
	if (nSel != LB_ERR)
	{
		if (nSel >= m_lvFiles.ItemCount())
			--nSel;

		m_lvFiles.Select(nSel);
	}
}

/******************************************************************************
** Method:		RefreshFileList()
**
** Description:	Refresh the listview of files.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSetupView::RefreshFile(const CFileProps* pFileProps)
{
	int nItem = m_lvFiles.FindItem(pFileProps);

	ASSERT(nItem != LB_ERR);

	m_lvFiles.ItemText(nItem, 1, (pFileProps->m_bProgIcon) ? "Yes" : "");
	m_lvFiles.ItemText(nItem, 2, (pFileProps->m_bDeskIcon) ? "Yes" : "");
}

/******************************************************************************
** Method:		OnListSelChange()
**
** Description:	List item (de)selected, update the main UI.
**
** Parameters:	See WM_NOTIFY.
**
** Returns:		See WM_NOTIFY.
**
*******************************************************************************
*/

LRESULT CSetupView::OnListSelChange(NMHDR& oHdr)
{
	NMLISTVIEW& oMsgHdr = reinterpret_cast<NMLISTVIEW&>(oHdr);

	if (oMsgHdr.uChanged & LVIF_STATE)
		App.m_AppCmds.UpdateUI();

	return 0;
}

/******************************************************************************
** Method:		OnListDoubleClick()
**
** Description:	List item double-clicked. Invoke "Edit | File Props" command.
**
** Parameters:	See WM_NOTIFY.
**
** Returns:		See WM_NOTIFY.
**
*******************************************************************************
*/

LRESULT CSetupView::OnListDoubleClick(NMHDR& /*oHdr*/)
{
	if (m_lvFiles.IsSelection())
		App.m_AppWnd.PostCommand(ID_EDIT_PROPS);

	return 0;
}
