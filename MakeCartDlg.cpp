//
// (C) 2004 Mike Brent aka Tursi aka HarmlessLion.com
// This software is provided AS-IS. No warranty
// express or implied is provided.
//
// This notice defines the entire license for this code.
// All rights not explicity granted here are reserved by the
// author.
//
// You may redistribute this software provided the original
// archive is UNCHANGED and a link back to my web page,
// http://harmlesslion.com, is provided as the author's site.
// It is acceptable to link directly to a subpage at harmlesslion.com
// provided that page offers a URL for that purpose
//
// Source code, if available, is provided for educational purposes
// only. You are welcome to read it, learn from it, mock
// it, and hack it up - for your own use only.
//
// Please contact me before distributing derived works or
// ports so that we may work out terms. I don't mind people
// using my code but it's been outright stolen before. In all
// cases the code must maintain credit to the original author(s).
//
// -COMMERCIAL USE- Contact me first. I didn't make
// any money off it - why should you? ;) If you just learned
// something from this, then go ahead. If you just pinched
// a routine or two, let me know, I'll probably just ask
// for credit. If you want to derive a commercial tool
// or use large portions, we need to talk. ;)
//
// If this, itself, is a derived work from someone else's code,
// then their original copyrights and licenses are left intact
// and in full force.
//
// http://harmlesslion.com - visit the web page for contact info
//

#include "stdafx.h"
#include <stdio.h>
#include "MakeCart.h"
#include "MakeCartDlg.h"

// TI Editor/Assembler GROM - (C) Texas Instruments, distributed under license
#include "tieag.c"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

unsigned char Header[]= {
//* CARTRIDGE HEADER
0xAA, 0x01,				//	DATA >AA01	* HEADER
0x01,					//	BYTE >01	* NUMBER OF PROGRAMS
0x00,					//	BYTE >00	* UNUSED
0x00, 0x00,				//	DATA >0000	* POINTER TO POWERUP LIST
0x60, 0x0C,				//	DATA >600C	* POINTER TO PROGRAM LIST
0x00, 0x00,				//	DATA >0000	* POINTER TO DSR LIST
0x00, 0x00,				//	DATA >0000	* POINTER TO SUBPROGRAM LIST

//* PROGRAM LIST
0x00, 0x00,				//	DATA >0000	* LINK TO NEXT ITEM
0x60, 0x42,				//	DATA >6042	* ADDRESS
0xFF,					//	BYTE >FF	* NAME LENGTH
0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,		// TEXT '            '
0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,		// * PADDING - MAX 21 BYTES
0x20, 0x20, 0x20, 0x20, 0x20, 

//* VANITY
0x3E, 0x3E, 0x2D, 0x2D, 0x20, 0x43, 0x41, 0x52,  	// TEXT '>>-- CART CONV BY TURSI --<<'
0x54, 0x20, 0x43, 0x4F, 0x4E, 0x56, 0x20, 0x42,
0x59, 0x20, 0x54, 0x55, 0x52, 0x53, 0x49, 0x20,
0x2D, 0x2D, 0x3C, 0x3C,

//* BOOTSTRAP LOADER (MOVE CALLS WILL VARY)
0x07, 0x20,									// ALL >20  
0x31, 0x00, 0x1C, 0xA2, 0xC2, 0x60, 0x26,	// MOVE >001C TO VDP@>02C2 FROM GROM@>6026
};

// patch offsets in the header for correct GROM index
int nHeaderPatches[] = {
	6, 14, 73, -1
};

unsigned char LoadChars[] = {				
	// This code detects the 99/4 and skips the lower case char set if detected
	// Instead it loads a copy of the uppercase set over the lowercase letters
	0xBE,0x74,0x05,							// ST >05,@>8374
	0x03,									// SCAN
	0x8e,0x74,								// CZ @>8374
	0x40,0xff,								// BR @>60xx		* to label 1
	0xbf,0x4a,0x0b,0x00,					// DST >0B00,@>834A
	0x06,0x00,0x4a,							// CALL @>004A      * 99/4A - load lowercase letters
	0x40,0xff,								// BR @>60xx        * to label 2
	0xbf,0x4a,0x0a,0x00,					// DST >0A00,@>834A	* LABEL 1 - 99/4 makes a copy for upper/lower
	0x06,0x00,0x18,							// CALL @>0018
	0x86,0x74,								// CLR @>8374		* LABEL 2 - common - load main set
	0xbf,0x4a,0x09,0x00,					// DST >0900,@>834A
	0x06,0x00,0x18,							// CALL @>0018
};
// No patches, all functions are relative

unsigned char Moves[] = {
/* Reserve space for the maximum number of moves */
0x31, 0x18, 0x00, 0x8F, 0x9D, 0x00, 0x60, 0x00,	//	MOVE >1800 TO @>2000 FROM GROM@>6000
0x31, 0x18, 0x00, 0x8F, 0xB5, 0x00, 0x80, 0x00,	//	MOVE >0800 TO @>3800 FROM GROM@>8000
0x31, 0x10, 0x00, 0x8F, 0x1D, 0x00, 0x88, 0x00,	//	MOVE >1000 TO @>A000 FROM GROM@>8800
0x31, 0x18, 0x00, 0x8F, 0x2D, 0x00, 0xA0, 0x00,	//	MOVE >1800 TO @>B000 FROM GROM@>A000
0x31, 0x18, 0x00, 0x8F, 0x45, 0x00, 0xC0, 0x00,	//	MOVE >1800 TO @>C800 FROM GROM@>C000
0x31, 0x18, 0x00, 0x8F, 0x5D, 0x00, 0xE0, 0x00,	//	MOVE >1800 TO @>E000 FROM GROM@>E000
0x31, 0x10, 0x00, 0x8F, 0x1D, 0x00, 0x88, 0x00,	//	MOVE >1000 TO @>A000 FROM GROM@>8800
0x31, 0x18, 0x00, 0x8F, 0x2D, 0x00, 0xA0, 0x00,	//	MOVE >1800 TO @>B000 FROM GROM@>A000
0x31, 0x18, 0x00, 0x8F, 0x45, 0x00, 0xC0, 0x00,	//	MOVE >1800 TO @>C800 FROM GROM@>C000
0x31, 0x18, 0x00, 0x8F, 0x5D, 0x00, 0xE0, 0x00,	//	MOVE >1800 TO @>E000 FROM GROM@>E000
};
#define MAXMOVES 10
// No patches, above code is dynamically generated

unsigned char Finish[] = {
0x07, 0x20,					// ALL >20              * Clear screen again
0xBF, 0x00, 0xA0, 0x00,		// DST >A000, @>8300	* Load address for XML
0x0F, 0xF0,					// XML >F0				* EXecute cart
0x0B						// EXIT					* if it returns
};

struct GROM {
	int nSize;
	unsigned char Data[0x2000];
} Rom[8];	// 0x0000, 0x2000, 0x4000, 0x6000, 0x8000, 0xA000, 0xC000, 0xE000 - approx 48k at >1800 bytes each
			// Note that 0x0000, 0x2000, and 0x4000 are used for the OS/TI BASIC and not normally replaced

struct {
	int nStart, nEnd;
	char RAM[0x6000];
} LowRam, HighRam;

struct _defs {
	char szName[7];
	int nAddress;
	bool bNeedEA;
};

struct _defs PreDefs[] = {
	// NAME		ADDRESS		// NEEDS EA?
	"UTLTAB",	0x2022,		true,		// probably true anyway
	"PAD",		0x8300,		false,
	"GPLWS",	0x83e0,		false,
	"SOUND",	0x8400,		false, 
	"VDPRD",	0x8800,		false,
	"VDPSTA",	0x8802,		false,
	"VDPWD",	0x8c00,		false,
	"SPCHRD",	0x9000,		false,
	"SPCHWT",	0x9400,		false,
	"GRMRD",	0x9800,		false,
	"GRMRA",	0x9802,		false,
	"GRMWD",	0x9c00,		false,
	"GRMWA",	0x9c02,		false,
	"SCAN",		0x000e,		false,
	"VSBW",		0x210c,		true,
	"VMBW",		0x2110,		true,
	"VSBR",		0x2114,		true,
	"VWTR",		0x211c,		true,
	"KSCAN",	0x2108,		true,
	"GPLLNK",	0x2100,		true,
	"XMLLNK",	0x2104,		true,
	"DSRLNK",	0x2120,		true,
	"LOADER",	0x2124,		true,

	"*",0,false
};

// globals in an object program is not good... this program went downhill fast :)
struct _defs Defines[1024];
int nNumDefs=0;
struct _defs Fixups[1024];
int nNumFixups=0;
int nFirstAddr=0;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMakeCartDlg dialog

CMakeCartDlg::CMakeCartDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMakeCartDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMakeCartDlg)
	m_Filename = _T("");
	m_ProgName = _T("");
	m_Use2k = FALSE;
	m_UseEA = FALSE;
	m_LoadChars = FALSE;
	m_GROMIdx = 3;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMakeCartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMakeCartDlg)
	DDX_Control(pDX, IDC_GROMIDX, m_ctrlGROMIdx);
	DDX_Control(pDX, IDC_CONSOLE, m_ctrlConsole);
	DDX_Control(pDX, IDC_LOADCHARS, m_ctrlChars);
	DDX_Text(pDX, IDC_FILENAME, m_Filename);
	DDX_Text(pDX, IDC_PROGRAMNAME, m_ProgName);
	DDV_MaxChars(pDX, m_ProgName, 21);
	DDX_Check(pDX, IDC_USE2K, m_Use2k);
	DDX_Check(pDX, IDC_USEEA, m_UseEA);
	DDX_Check(pDX, IDC_LOADCHARS, m_LoadChars);
	DDX_CBIndex(pDX, IDC_GROMIDX, m_GROMIdx);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMakeCartDlg, CDialog)
	//{{AFX_MSG_MAP(CMakeCartDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CLOSE, OnBClose)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BEGIN, OnBegin)
	ON_BN_CLICKED(IDC_USEEA, OnUseea)
	ON_BN_CLICKED(IDC_CONSOLE, OnConsole)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMakeCartDlg message handlers

BOOL CMakeCartDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_ctrlChars.SetCheck(BST_CHECKED);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMakeCartDlg::OnConsole() 
{
	// Create a console window if SHIFT is held down
	AllocConsole();
	freopen("CONOUT$", "wb", stdout);
	m_ctrlConsole.EnableWindow(false);

	// To clean up gracefully
	//fclose(stdout);
	//FreeConsole();
	
}

void CMakeCartDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMakeCartDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMakeCartDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMakeCartDlg::OnOK() 
{
	// Ignore
}

void CMakeCartDlg::OnCancel() 
{
	// Ignore
}

void CMakeCartDlg::OnBClose() 
{
	EndDialog(IDOK);	
}

void CMakeCartDlg::OnBrowse() 
{
	UpdateData(true);

	CFileDialog dlg(true, NULL, m_Filename);

	if (IDOK == dlg.DoModal()) {
		m_Filename=dlg.GetPathName();
		UpdateData(false);
	}
}

void CMakeCartDlg::OnClose() 
{
	CDialog::OnClose();
	EndDialog(IDOK);
}

void CMakeCartDlg::OnBegin() 
{
	FILE *fp;
	bool bIsEA3=false;

	printf("\n\n\n\n\n--- New Run ---\n");

	UpdateData(true);

	if (m_GROMIdx == 0) {
		if (IDYES != AfxMessageBox("Warning: GROM 0 is the operating system. Replacing\nit will almost never work. Are you sure?", MB_YESNO)) {
			return;
		}
	}

	if (m_ProgName.GetLength() < 1) {
		m_ProgName="Program File";
	}

	LowRam.nStart=0x3fff;
	LowRam.nEnd=0x2000;
	memset(LowRam.RAM, 0, 0x2000);
	HighRam.nStart=0xffff;
	HighRam.nEnd=0xA000;
	memset(HighRam.RAM, 0, 0x4000);

	int nMore=1;

	// set up the defs table for EA#3
	while (PreDefs[nNumDefs].szName[0] != '*') {
		Defines[nNumDefs].nAddress=PreDefs[nNumDefs].nAddress;
		strcpy(Defines[nNumDefs].szName, PreDefs[nNumDefs].szName);
		Defines[nNumDefs].bNeedEA = PreDefs[nNumDefs].bNeedEA;
		nNumDefs++;
	}
	memset(Fixups, 0, sizeof(Fixups));

#if 0
	// first check if it's an E/A#3 file - this could be the
	// entire size of memory, but at 5 bytes per real byte! (plus overhead)
	// To load multiple files, name them with incrementing names like E/A#5
	// This is a lot more complex than loading EA#5 as we must do the linking too
	bool bIsEA3=false;
	do {
		unsigned char buf[256];
		int nFirstHi, nLastHi;
		int nFirstLo, nLastLo;
		int nRelocationBase=0;

		// We don't need to be quite as picky as the real thing since we
		// don't need the real ref/def table in low memory
		nFirstHi=0xa000;
		nLastHi=0xffd7;
		nFirstLo=0x2676;
		nLastLo=0x3f37;

		fp=fopen(m_Filename, "rb");
		if (NULL == fp) {
			sprintf((char*)buf, "Failed to open file %s", (const char*)m_Filename);
			AfxMessageBox((char*)buf);
			return;
		}

		nSize=fread(buf, 1, 256, fp);
		fclose(fp);

		// Now check this file - the first bytes is 0, followed by a 4 byte hex length
		if ((buf[0] != '0') || (!isxdigit(buf[1])) || (!isxdigit(buf[2])) || (!isxdigit(buf[3])) || (!isxdigit(buf[4])) ) {
			if (nSize > 128) {
				// no? skip 128 bytes and try again
				memmove(buf, &buf[128], nSize-128);
				nSize-=128;
			}
		}
		// So, yeah, this check is done twice in the success case.
		if ((buf[0] != '0') || (!isxdigit(buf[1])) || (!isxdigit(buf[2])) || (!isxdigit(buf[3])) || (!isxdigit(buf[4])) ) {
			printf("Does not look like EA#3 (first tag not 0)\n");
			break;
		}

		bIsEA3=true;

		// Now re-open the file and read the tags
		fp=fopen(m_Filename, "rb");
		if (NULL == fp) {
			sprintf((char*)buf, "Failed to open file %s?", (const char*)m_Filename);
			AfxMessageBox((char*)buf);
			return;
		}

		while (!feof(fp)) {
			nSize=fread(buf, 1, 81, fp);
			buf[80]='\0';

			nMore=0;		// todo: implement multiple file loading

			int p=0;
			int nTmp;

			while (buf[p]) {
				// read the tags one by one
				switch (buf[0]) {
					case '0':	// module ID - 1 word address, 6 bytes name
						if (nSize == 80) {
							// uncompressed
							if (1 != sscanf(&buf[p], "%4X", &nTmp)) {
								sprintf((char*)buf, "Parse error for tag 0 in line %s", buf);
								AfxMessageBox((char*)buf);
								return;
							}
							p+=13;
						} else {
							// compressed?
							nTmp=buf[p+1]*256+buf[p+2];
							p+=9;
						}

						if (nLastHi-nFirstHi >= nTmp) {
							nRelocationBase=nFirstHi;
							nFirstHi+=nTmp;
						} else if (nLastLo-nFirstLo >= nTmp) {
							nRelocationBase=nFirstLo;
							nFirstLo+=nTmp;
						} else {
							sprintf(buf, "%s is too large for remaining memory (>%04x bytes) - can not load.", (const char*)m_Filename, nTmp);
							AfxMessageBox((char*)buf);
							return;
						}
							
						printf("- Loading >%04X bytes to >%04X from %s\n", nTmp, nRelocationBase, (const char*)m_Filename);
						break;

					case '1':	// absolute entry point
					case '2':	// relocatable entry point
						if (1 != sscanf(&buf[p], "%4X", &nTmp)) {
							sprintf((char*)buf, "Parse error for tag 1/2 in line %s", buf);
							AfxMessageBox((char*)buf);
							return;
						}
						if (buf[p]=='2') nTmp+=nRelocationBase;
						nFirstAddr=nTmp;
						p+=5;
						printf("- Setting entry point to >%04X\n", nTmp);
						break;

					case '3':	// relocatable reference
					case '4':	// absolute reference

// EA#3 support to resume later

		if (nSize > 0x4000) {
			sprintf((char*)buf, "%s has unusually large size of >%04x - can not load.", (const char*)m_Filename, nSize);
			AfxMessageBox((char*)buf);
			return;
		}

		if ((nLoad < 0x2000)||(nLoad > 0xffff)) {
			sprintf((char*)buf, "%s does not load into the 32k RAM space (>%04x) - can not load.", (const char*)m_Filename, nLoad);
			AfxMessageBox((char*)buf);
			return;
		}

		if ((nLoad > 0x3fff) && (nLoad < 0xA000)) {
			sprintf((char*)buf, "%s does not load into the 32k RAM space (>%04x) - can not load.", (const char*)m_Filename, nLoad);
			AfxMessageBox((char*)buf);
			return;
		}

		printf("- Loading >%04X bytes to >%04X from %s\n", nSize, nLoad, (const char*)m_Filename);
		if (nSize > 0x2000) {
			printf("- This file is unusually large... (loading anyway)\n");
		}

		// Looks to be okay
		if (nLoad < 0xa000) {
			// make sure we don't overrun
			if (nLoad+nSize > 0x4000) {
				printf("- File overruns low memory by %d bytes - truncating.\n", (nLoad+nSize)-0x4000);
				nSize=0x4000-nLoad;
			}
			memcpy(&LowRam.RAM[nLoad-0x2000], &buf[6], nSize);
			if (nLoad < LowRam.nStart) {
				LowRam.nStart = nLoad;
			}
			if (nLoad+nSize >= LowRam.nEnd) {
				LowRam.nEnd = nLoad+nSize;
			}
		} else {
			// make sure we don't overrun
			if (nLoad+nSize > 0x10000) {
				printf("- File overruns high memory by %d bytes - truncating.\n", (nLoad+nSize)-0x10000);
				nSize=0x10000-nLoad;
			}
			memcpy(&HighRam.RAM[nLoad-0xa000], &buf[6], nSize);
			if (nLoad < HighRam.nStart) {
				HighRam.nStart = nLoad;
			}
			if (nLoad+nSize >= HighRam.nEnd) {
				HighRam.nEnd = nLoad+nSize;
			}
		}

		if (nMore) {
			int l=m_Filename.GetLength()-1;
			char c=m_Filename[l];
			c++;
			m_Filename=m_Filename.Left(l);
			m_Filename+=c;
		}
	} while (nMore);

	if (bIsEA3) {
		// solve any fixups
		for (int i=0; Fixups[i].nAddress!=0; i++) {
			nTmp=GetLabel(Fixups[i].szName);
			if (0 == nTmp) {
				sprintf((char*)buf, "Unresolved reference %s", Fixups[i].szName);
				AfxMessageBox((char*)buf);
				return;
			}

// EA#3 to be resumed in future
#endif

	if (!bIsEA3) {
		// Load up the E/A#5 files
		do {
			unsigned char buf[0x4000+6+128];
			int nSize, nLoad;

			fp=fopen(m_Filename, "rb");
			if (NULL == fp) {
				sprintf((char*)buf, "Failed to open file %s", (const char*)m_Filename);
				AfxMessageBox((char*)buf);
				return;
			}

			nSize=fread(buf, 1, 0x4000+128+6, fp);
			fclose(fp);

			// Now check this file - the first byte must be >00 or >FF
			if ((buf[0]!=0x00)&&(buf[0]!=0xff)&&(nSize>128)) {
				// no? skip 128 bytes and try again
				memmove(buf, &buf[128], nSize-128);
				nSize-=128;
			}
			// So, yeah, this check is done twice in the success case.
			if ((buf[0]!=0x00)&&(buf[0]!=0xff)) {
				sprintf((char*)buf, "%s does not appear to be a TI EA#3 or #5 file.. unable to load.", (const char*)m_Filename);
				AfxMessageBox((char*)buf);
				return;
			}
			// Now read the little header.
			nMore=(buf[0]<<8)|buf[1];
			nSize=(buf[2]<<8)|buf[3];
			nLoad=(buf[4]<<8)|buf[5];
			if (nFirstAddr == 0) {
				nFirstAddr=nLoad;
			}

			if (nSize > 0x4000) {
				sprintf((char*)buf, "%s has unusually large size of >%04x - can not load.", (const char*)m_Filename, nSize);
				AfxMessageBox((char*)buf);
				return;
			}

			if ((nLoad < 0x2000)||(nLoad > 0xffff)) {
				sprintf((char*)buf, "%s does not load into the 32k RAM space (>%04x) - can not load.", (const char*)m_Filename, nLoad);
				AfxMessageBox((char*)buf);
				return;
			}

			if ((nLoad > 0x3fff) && (nLoad < 0xA000)) {
				sprintf((char*)buf, "%s does not load into the 32k RAM space (>%04x) - can not load.", (const char*)m_Filename, nLoad);
				AfxMessageBox((char*)buf);
				return;
			}

			printf("- Loading >%04X bytes to >%04X from %s\n", nSize, nLoad, (const char*)m_Filename);
			if (nSize > 0x2000) {
				printf("- This file is unusually large... (loading anyway)\n");
			}

			// Looks to be okay
			if (nLoad < 0xa000) {
				// make sure we don't overrun
				if (nLoad+nSize > 0x4000) {
					printf("- File overruns low memory by %d bytes - truncating.\n", (nLoad+nSize)-0x4000);
					nSize=0x4000-nLoad;
				}
				memcpy(&LowRam.RAM[nLoad-0x2000], &buf[6], nSize);
				if (nLoad < LowRam.nStart) {
					LowRam.nStart = nLoad;
				}
				if (nLoad+nSize >= LowRam.nEnd) {
					LowRam.nEnd = nLoad+nSize;
				}
			} else {
				// make sure we don't overrun
				if (nLoad+nSize > 0x10000) {
					printf("- File overruns high memory by %d bytes - truncating.\n", (nLoad+nSize)-0x10000);
					nSize=0x10000-nLoad;
				}
				memcpy(&HighRam.RAM[nLoad-0xa000], &buf[6], nSize);
				if (nLoad < HighRam.nStart) {
					HighRam.nStart = nLoad;
				}
				if (nLoad+nSize >= HighRam.nEnd) {
					HighRam.nEnd = nLoad+nSize;
				}
			}

			if (nMore) {
				int l=m_Filename.GetLength()-1;
				char c=m_Filename[l];
				c++;
				m_Filename=m_Filename.Left(l);
				m_Filename+=c;
			}
		} while (nMore);
	}

	DumpAsGROM();
}

void CMakeCartDlg::DumpAsGROM() {
	// Now we have the memory layout of all the ROMs
	DWORD nROMAddr;
	int nROM;
	int GPLAddr;
	int i;
	int nMovesLeft=MAXMOVES;

	nROM=m_GROMIdx;

	Rom[nROM].nSize=0;
	memset(Rom[nROM].Data, 0, 0x2000);

	// If we need the E/A routines, then first copy the ROM into the first bank
	if (m_UseEA) {
		memcpy(Rom[m_GROMIdx].Data, TIEAG, 0x2000);
		// We shuffle things around just a tad to make it fit transparently
		// and still have access to the EA routines
		// First move the init code to a higher and safer address, branches must be 
		// less than >6100 due to the 1 byte jumps we need to fix up
		memmove(&Rom[m_GROMIdx].Data[0x00A0], &Rom[m_GROMIdx].Data[0x0025], 0x4a);
		// This one is full either way! (but more fixups later)
		Rom[m_GROMIdx].nSize=0x2000;
	}

	// Start by copying the header into the GROM space
	// This will overwrite parts of the E/A cart
	memcpy(Rom[m_GROMIdx].Data, Header, sizeof(Header));

	// Fill in the cartridge title
	m_ProgName.MakeUpper();
	memcpy(&Rom[m_GROMIdx].Data[17], (const char*)m_ProgName, m_ProgName.GetLength());
	Rom[m_GROMIdx].Data[16]=(unsigned char)m_ProgName.GetLength();
	Rom[m_GROMIdx].nSize=sizeof(Header)+sizeof(Moves)+sizeof(Finish);
	GPLAddr=sizeof(Header);

	// Now, again, if we loaded the E/A cartridge, we toss the first bank altogether
	if (m_UseEA) {
		// Final fixups - copy our startup code to the end of the EA startup
		// I guess I could have just provided a patched ROM, but this is better
		// documenting??
		memmove(&Rom[m_GROMIdx].Data[0xea], &Rom[m_GROMIdx].Data[0x42], 9);
		// Add a CALL to load the utilities
		Rom[m_GROMIdx].Data[0xf3]=0x06;		// CALL @>6B8E
		Rom[m_GROMIdx].Data[0xf4]=0x6B;
		Rom[m_GROMIdx].Data[0xf5]=0x8E;
		// Add a RTN
		Rom[m_GROMIdx].Data[0xf6]=0x00;
		// Patch the loader to call here
		Rom[m_GROMIdx].Data[0x42]=0x06;		// CALL @>60A0
		Rom[m_GROMIdx].Data[0x43]=0x60;
		Rom[m_GROMIdx].Data[0x44]=0xA0;
		// Patch the 3 BRanches in that code
		Rom[m_GROMIdx].Data[0xae]=0xB8;
		Rom[m_GROMIdx].Data[0xb7]=0xd1;
		Rom[m_GROMIdx].Data[0xd0]=0xc3;
		// And patch the start address for the MOVEs
		GPLAddr=0x45;
		// Now activate the next ROM
		nROM++;
		Rom[nROM].nSize=0;
		memset(Rom[nROM].Data, 0, 0x2000);
		printf("Loading Editor/Assembler cartridge into first GROM\n");
	} else {
		if (m_LoadChars) {
			printf("Enabling code to load character set.\n");
			// If we have to load the character set, then we insert a little extra code here
			memcpy(&Rom[m_GROMIdx].Data[GPLAddr], LoadChars, sizeof(LoadChars));
			// There are two branches to fix up. The dests must be less than >6100
			Rom[m_GROMIdx].Data[GPLAddr+7]=(GPLAddr+17)&0xff;
			Rom[m_GROMIdx].Data[GPLAddr+16]=(GPLAddr+24)&0xff;
			GPLAddr+=sizeof(LoadChars);
			Rom[m_GROMIdx].nSize+=sizeof(LoadChars);
		}
	}

	// Start with the low memory bank
	if (LowRam.nStart < LowRam.nEnd) {
		nROMAddr=LowRam.nStart;
		while (nROMAddr < (DWORD)LowRam.nEnd) {
			// copy the data into the GROM from RAM
			int nMax=(m_Use2k?0x2000:0x1800)-Rom[nROM].nSize;
			if ((DWORD)nMax > LowRam.nEnd-nROMAddr) {
				nMax=LowRam.nEnd-nROMAddr;
			}
			memcpy(&Rom[nROM].Data[Rom[nROM].nSize], &LowRam.RAM[nROMAddr-0x2000], nMax);

			if (nMovesLeft == 0) {
				AfxMessageBox("Too many data segments - can not process!");
				return;
			}
			nMovesLeft--;
			// Write out the MOVE instruction to load this block
			Rom[m_GROMIdx].Data[GPLAddr++]=0x31;
			Rom[m_GROMIdx].Data[GPLAddr++]=nMax>>8;
			Rom[m_GROMIdx].Data[GPLAddr++]=nMax&0xff;
			Rom[m_GROMIdx].Data[GPLAddr++]=0x8F;
			Rom[m_GROMIdx].Data[GPLAddr++]=(unsigned char)(((nROMAddr-0x8300)&0xff00)>>8);
			Rom[m_GROMIdx].Data[GPLAddr++]=(unsigned char)(nROMAddr&0xff);
			Rom[m_GROMIdx].Data[GPLAddr++]=(Rom[nROM].nSize+(0x2000*nROM))>>8;
			Rom[m_GROMIdx].Data[GPLAddr++]=Rom[nROM].nSize&0xff;

			for (i=8; i>0; i--) {
				printf("%02x ", Rom[m_GROMIdx].Data[GPLAddr-i]);
			}
			printf("  MOVE >%04X TO @>%04X FROM GROM@>%04X\n", nMax, nROMAddr, (Rom[nROM].nSize+(0x2000*nROM)));

			Rom[nROM].nSize+=nMax;
			nROMAddr+=nMax;
			if (Rom[nROM].nSize >= (m_Use2k?0x2000:0x1800)) {
				nROM++;
				if (nROM >= 5) {
					AfxMessageBox("Rom is too large! Unable to process.");
					return;
				}
				Rom[nROM].nSize=0;
				memset(Rom[nROM].Data, 0, 0x2000);
			}
		}
	}

	// Now do the high memory bank
	if (HighRam.nStart < HighRam.nEnd) {
		nROMAddr=HighRam.nStart;
		while (nROMAddr < (DWORD)HighRam.nEnd) {
			// copy the data into the GROM from RAM
			int nMax=(m_Use2k?0x2000:0x1800)-Rom[nROM].nSize;
			if ((DWORD)nMax > HighRam.nEnd-nROMAddr) {
				nMax=HighRam.nEnd-nROMAddr;
			}
			memcpy(&Rom[nROM].Data[Rom[nROM].nSize], &HighRam.RAM[nROMAddr-0xA000], nMax);

			if (nMovesLeft == 0) {
				AfxMessageBox("Too many data segments - can not process!");
				return;
			}
			nMovesLeft--;
			// Write out the MOVE instruction to load this block
			Rom[m_GROMIdx].Data[GPLAddr++]=0x31;
			Rom[m_GROMIdx].Data[GPLAddr++]=nMax>>8;
			Rom[m_GROMIdx].Data[GPLAddr++]=nMax&0xff;
			Rom[m_GROMIdx].Data[GPLAddr++]=0x8F;
			Rom[m_GROMIdx].Data[GPLAddr++]=(unsigned char)(((nROMAddr-0x8300)&0xff00)>>8);
			Rom[m_GROMIdx].Data[GPLAddr++]=(unsigned char)(nROMAddr&0xff);
			Rom[m_GROMIdx].Data[GPLAddr++]=(Rom[nROM].nSize+(0x2000*nROM))>>8;
			Rom[m_GROMIdx].Data[GPLAddr++]=Rom[nROM].nSize&0xff;

			for (i=8; i>0; i--) {
				printf("%02x ", Rom[m_GROMIdx].Data[GPLAddr-i]);
			}
			printf("  MOVE >%04X TO @>%04X FROM GROM@>%04X\n", nMax, nROMAddr, (Rom[nROM].nSize+(0x2000*nROM)));

			Rom[nROM].nSize+=nMax;
			nROMAddr+=nMax;
			if (Rom[nROM].nSize >= (m_Use2k?0x2000:0x1800)) {
				nROM++;
				if (nROM >= 8) {
					AfxMessageBox("Rom is too large! Unable to process.");
					return;
				}
				Rom[nROM].nSize=0;
				memset(Rom[nROM].Data, 0, 0x2000);
			}
		}
	}

	// Now copy the finish over
	memcpy(&Rom[m_GROMIdx].Data[GPLAddr], Finish, sizeof(Finish));

	// Set the start address (assumes a position in Finish)
	GPLAddr+=4;
	Rom[m_GROMIdx].Data[GPLAddr++]=nFirstAddr>>8;
	Rom[m_GROMIdx].Data[GPLAddr++]=nFirstAddr&0xff;

	// patch up the boot header for the correct GROM
	i=0;
	while (nHeaderPatches[i] != -1) {
		Rom[m_GROMIdx].Data[nHeaderPatches[i]]=m_GROMIdx*0x20;
		i++;
	}

	// And now, save it!
	CString OutName=m_ProgName.Left(7);
	OutName.Replace(" ","");
	CFileDialog dlg(false, NULL, OutName);

	if (IDOK == dlg.DoModal()) {
		CString cFn=dlg.GetPathName();

		if (cFn.Right(1) != "G") {
			cFn+='G';
		}

		FILE *fp=fopen(cFn, "wb");
		if (NULL != fp) {
			for (int i=m_GROMIdx; i<nROM; i++) {
				fwrite(Rom[i].Data, 0x2000, 1, fp);
			}
			fwrite(Rom[nROM].Data, Rom[nROM].nSize+(Rom[nROM].nSize%2), 1, fp);
		}
		fclose(fp);

		printf("Saved GROM program!\n");
		printf(    " Total size   : >%04X\n", (nROM-m_GROMIdx)*0x2000+Rom[nROM].nSize);
		if (LowRam.nStart < LowRam.nEnd) {
			printf(" Low  RAM Bank: >%04X - >%04X\n", LowRam.nStart, LowRam.nEnd);
		}
		if (HighRam.nStart < HighRam.nEnd) {
			printf(" High RAM Bank: >%04X - >%04X\n", HighRam.nStart, HighRam.nEnd);
		}
		printf(" GPL MOVEs    : %d\n", MAXMOVES-nMovesLeft);
		printf(" Start Address: >%04X\n", nFirstAddr);

		printf("\n");

		char out[256];
		sprintf(out, "Conversion completed - GROM cart at >%04x, length >%04x", m_GROMIdx*0x2000, (nROM-m_GROMIdx)*0x2000+Rom[nROM].nSize);
		AfxMessageBox(out);
	}
}

void CMakeCartDlg::OnUseea() 
{
	UpdateData(true);
	
	if (m_UseEA) {
		if (GetAsyncKeyState(VK_SHIFT)) {
			AfxMessageBox("Warning: This will include portions of the TI Editor/Assembler cartridge in your work.\n"
						  "That file is (C) Texas Instruments and distributed under license - you may not legally\n"
						  "be able to distribute the derived ROM file. If possible, consider updating the original\n"
						  "program not to rely on the cartridge."
						  );
			m_ctrlChars.EnableWindow(false);
		} else {
			AfxMessageBox("Warning: This will include portions of the TI Editor/Assembler cartridge in your work.\n"
						  "That file is (C) Texas Instruments and distributed under license - you may not legally\n"
						  "be able to distribute the derived ROM file. If possible, consider updating the original\n"
						  "program not to rely on the cartridge. E/A based GROMS will start with GROM #3, hold\n"
						  "SHIFT while selecting this item to override and allow changing the base GROM."
						  );
			m_ctrlChars.EnableWindow(false);
			m_GROMIdx=3;
			m_ctrlGROMIdx.SetCurSel(3);
			m_ctrlGROMIdx.EnableWindow(false);
		}
	} else {
		m_ctrlChars.EnableWindow(true);
		m_ctrlGROMIdx.EnableWindow(true);
	}
}

/*

GPL code to load it:
	MOVE >1800 TO @>A000 FROM GROM@>6000
	MOVE >1800 TO @>B800 FROM GROM@>8000
	MOVE >10C0 TO @>D000 FROM GROM@>A000

MOVE:
  MOVE                         (0)
                               (0)
                               (1)
  Destination NOT GROM         (1)

  Destination NOT VDP register (0)
  Source NOT VDP/CPU           (0)
  Source NOT GROM over CPU     (0)
  Number is direct operand     (1)
                              -----
                               >31
Destination address:
  10VI1111, Address, Address
  V - 0=CPU, 1-VDP
  I - 0=Direct, 1=Indirect
  ( >8F - CPU, Direct )
  Address has offset of >8300
  ( >A000 - >8300 = >1D00 )

* GROM HEADER FOR LOADING A Rom PROGRAM AS A GROM CART
* SYSTEM WILL STILL REQUIRE 32K RAM TO FUNCTION!
* THE MOVES NEED TO BE ADJUSTED FOR THE ACTUAL SIZE OF
* THE CARTRIDGE.
* (C) 2004 BY M.BRENT AKA TURSI

* CARTRIDGE HEADER
6000: AA 01				DATA >AA01	* HEADER
6002: 01				BYTE >01	* NUMBER OF PROGRAMS
6003: 00				BYTE >00	* UNUSED
6004: 00 00				DATA >0000	* POINTER TO POWERUP LIST
6006: 60 0C				DATA >600C	* POINTER TO PROGRAM LIST
6008: 00 00				DATA >0000	* POINTER TO DSR LIST
600A: 00 00				DATA >0000	* POINTER TO SUBPROGRAM LIST

* PROGRAM LIST
600C: 00 00				DATA >0000	* LINK TO NEXT ITEM
600E: 60 38				DATA >6038	* ADDRESS
6010: 0B				BYTE >0B	* NAME LENGTH
6011: 4C 4F 42 53 54 45 52 20		TEXT 'LOBSTER BAY'
6019: 42 41 59 00 00 00 00 00		                * PADDING - MAX 21 BYTES
6021: 00 00 00 00 00

* VANITY
6026: 3E 3E 2D 2D 20 43 41 52  		TEXT '>>-- CART CONV BY TURSI --<<'
602E: 54 20 43 4F 4E 56 20 42 
6036: 59 20 54 55 52 53 49 20
603E: 2D 2D 3C 3C

* BOOTSTRAP LOADER (MOVE CALLS WILL VARY)
6042: 07 20				ALL >20  
6044: 31 00 1C A2 C2 60 26		MOVE >001C TO VDP@>02C2 FROM GROM@>6026
604B: 31 17 96 8F 1D 00 60 6A		MOVE >1796 TO @>A000 FROM GROM@>606A
6053: 31 18 00 8F 34 96 80 00		MOVE >1800 TO @>B796 FROM GROM@>8000
605B: 31 11 2A 8F 4C 96 A0 00		MOVE >112A TO @>CF96 FROM GROM@>A000
6063: BF 00 A0 00			DST >A000, @>8300	* Load address for XML
6067: 0F F0				XML >F0			* EXecute cart
6069: 0B				EXIT			* if it returns

606A: START PROGRAM HERE!
-------------------

EXAMPLES:
MOVE >0001 TO @>8359 FROM GROM@>6000
31 00 01 59 60 00

MOVE S1 TO D from S2
31 -- MOVE: >20 - >3f - type 6?
      001G RVCN
      G - Destination NOT GROM (0=GROM)
      R - Destination is VDP register (1=true)
      V - Source is VDP or CPU (1=true)
      C - Source is GROM over CPU (1=true)
      N - Number is direct operand (1=true)
      31 = 0011 0001
           Destination NOT GROM
           Destination NOT VDP register
           Source NOT VDP/CPU
           Source NOT GROM over CPU
           Number is direct operand
00 01 -- count (1)
59    -- Dest: Scratchpad access: >8359
60 00 -- Source: GROM >6000


MOVE >0008 TO REG>01 FROM GROM@>0451
39 00 08 00 04 51 
39 -- MOVE
     39 = 0011 1001
           Destination NOT GROM
           Destination IS VDP register
           Source NOT VDP/CPU
           Source NOT GROM over CPU
           Number is direct operand
00 08 -- count (8)
00 -- Dest: VDP Reg - 1 (1)
0451 -- Source: GROM >0451

MOVE >0071 TO @>8301 FROM @>8300
35 00 71 01 00
35 -- MOVE
    35 = 0011 0101
           Destination NOT GROM
           Destination NOT VDP register
           Source IS VDP/CPU
           Source NOT GROM over CPU
           Number is direct operand
00 71 - count (>71)
01 - Dest:Scratchpad >8301
00 - Source: Scratchpad >8300

*/

