#pragma once

class CIniFile
{
public:
	CIniFile(CString m_FName);
	virtual ~CIniFile(void);
	
	CString m_Name;
public:
	
	CString ReadString(CString m_Sec, CString m_Ident, CString m_Def);
	BOOL WriteString(CString m_Sec, CString m_Ident, CString m_Val);
	BOOL ReadSections(CStringArray& m_Secs);
	BOOL ReadSection(CString m_Sec, CStringArray& m_Secs);

};
