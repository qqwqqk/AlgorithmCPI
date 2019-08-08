#pragma once
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF", "adoEOF")
#include <icrsint.h>
#include <stdlib.h>

class ADOLINK{
public:
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRecordset;

	void Connect(void);
	void ExitConnect(void);
	_RecordsetPtr& GetRecordset(_bstr_t SQL);
};