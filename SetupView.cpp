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

	m_lvFiles.InsertColumn(FILE_NAME, "File",                 150);
	m_lvFiles.InsertColumn(PROG_ICON, "Shortcut?",             75);
	m_lvFiles.InsertColumn(DESK_ICON, "Desk Icon?",            75);
	m_lvFiles.InsertColumn(ICON_NAME, "Shortcut Name",        150);
	m_lvFiles.InsertColumn(ICON_DESC, "Shortcut Description", 200);

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
	// Template shorthands.
	typedef CSetupDoc::CFileList::const_iterator CIter;

	CSetupDoc& oDoc = Doc();

	// Preseve selection.
	int nSel = m_lvFiles.Selection();

	// Clear view.
	m_lvFiles.DeleteAllItems();

	// For all files...
	for (CIter oIter = oDoc.m_aoFiles.begin(); oIter != oDoc.m_aoFiles.end(); ++oIter)
	{
		CFileProps* pFileProps = *oIter;

		int n = m_lvFiles.AppendItem(pFileProps->m_strFileName);
		m_lvFiles.ItemPtr(n, pFileProps);

		RefreshFile(pFileProps);
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

	m_lvFiles.ItemText(nItem, PROG_ICON, (pFileProps->m_bProgIcon) ? "Yes" : "");
	m_lvFiles.ItemText(nItem, DESK_ICON, (pFileProps->m_bDeskIcon) ? "Yes" : "");
	m_lvFiles.ItemText(nItem, ICON_NAME, pFileProps->m_strIconName);
	m_lvFiles.ItemText(nItem, ICON_DESC, pFileProps->m_strIconDesc);
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
