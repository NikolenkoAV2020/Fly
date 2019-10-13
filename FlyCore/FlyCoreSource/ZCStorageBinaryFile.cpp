//-----------------------------------------------------------------------------
// ZCStorageBinaryFile.cpp
// �������� ����-��������� ���
//
// �. ��������� 04.05.2019
//-----------------------------------------------------------------------------
#include <stdafx.h>
#include <FlyCoreSource\\ZCStorageBinaryFile.h>

int ZSEopItem::Size    = 4  * sizeof(float) + sizeof(char);
int ZSLocAlgMod::Size  = 14 * sizeof(float) + sizeof(char);
int ZSLocAlg2Mod::Size = 27 * sizeof(float) + sizeof(char);

//-----------------------------------------------------------------------------
ZCStorageBinaryFile::ZCStorageBinaryFile()
{
	FullName = "";
	Mjd0 = 0;
	FSize = 0;
	ISize = 0;
	IMemory = nullptr;
	CountItems = 0;
	NumFileInStorage = 0;
}

//-----------------------------------------------------------------------------
ZCStorageBinaryFile::~ZCStorageBinaryFile()
{
	//�������� ����� ���� �� ������� ��������
	Close();
}

//-----------------------------------------------------------------------------
void ZCStorageBinaryFile::Close()
{
	//�������� �����
	if (File.is_open())	File.close();
	if (IMemory) delete[] IMemory;
}

//-----------------------------------------------------------------------------
// �������� ������ ����� ��� ������� �������� - ������� ��� ����� 
// ������ � ��������� ��� ����������
const char* ZCStorageBinaryFile::GetBinaryFileName(int i, char Type)
{
	FullName = DIREOPSTORAGE;
	FullName += "\\";
	switch (Type) {
	case 0: FullName += EOP_STORAGE_FILE_NAME; break;
	case 1: FullName += EOP_PREDICT_FILE_NAME; break;
	case 2: FullName += EOP_ESTIMATE_FILE_NAME; break;
	case 3: FullName += EOP_PXY_FILE_NAME; break;
	case 4: FullName += EOP_UTC1UTC_FILE_NAME; break;
	default: assert(0);
	}
	if (Type < 3) {
		char SY[12];
		sprintf(SY, "%d\0", EOP_STORAGE_ORIGIN_Y +
				EOP_STORAGE_FILE_DURATION * i);
		FullName += SY;
	}
	FullName += ".bin";

	return FullName.c_str();
}

//-----------------------------------------------------------------------------
int ZCStorageBinaryFile::Open(
	int   NumFile, 
	char  Type, 
	float fromMJD, 
	int   ItemSize)
{
	//�������� ��������� �����, ���� ��� ����������
	Close();

	// ������ ��� ����� �� ��� ������ � ����
	const char* FileName = GetBinaryFileName(NumFile, Type);

	// �������� ������ �����.
	// ��������� ������-������ ������������ ���������������
	// �� ����� �����.
	File.open(FileName, ios::ate | ios::in | ios::out | ios::binary);
	if (File.fail()) {
		// ������� �������� ����� ������������ � ������
		// ������ � ������ ����� ���� ��������� ������-���
		// ���� �� ���������� (��� ��� ������ ��� �������� ������ 
		// ��� ������, � ������ ������ ���� ������������� ��������).
		// ������� ������� ������ ������� ������ ����, ������ ��� 
		// ������ � ������ ������...
		File.open(FileName, ios::out | ios::binary);
		// ���� � � ���� ������ ������, �� ���������� ���������
		if (File.fail()) return-2;
		// �������� ������ ��� ���������� �����
		File.close();
		// ��������� �������� � ������ � ������ � ������
		File.open(FileName, ios::ate | ios::in | ios::out |
			ios::binary);
		// �������� ������������ ��������
		if (File.fail()) return-2;
		// ���� �� �������, �� ��������� ������-������ ����� 
		// �������������� �� ����� �����
	}

	// ��������� ��������� ����
	Mjd0 = (unsigned long)fromMJD;
	// ������ ����� ������
	ISize = ItemSize;
	// ����������� ������� ��������� ��������� ���������
	FSize = static_cast<unsigned long>(File.tellp());
	// ����� ������� � �����.
	CountItems = FSize / ISize;
	// ������ ����� "������� ������"
	IMemory = new char[ISize];
	memset(IMemory, 0, ISize);
	
	return 0;
}

//-----------------------------------------------------------------------------
// ������ countItems ������� �������� � ��� � ����� ����� File
void ZCStorageBinaryFile::WriteEmptyEopItems(int addCountItems)
{
	if (addCountItems <= 0) return;
	// ������� ������ ���
	// ��������� ZSEopItem ���������������� �� ���������
	// �������� ����������, ������������� ������� � 
	// ��������� ��� ������ � ������� ����� Key ����� �������� ���
	// ������ ���� � �������� ���������� ���������� �� IERS �� ����.

	// ������������� ����������� ������� � �����
	CountItems += addCountItems;
	// ������������� ������ �����
	FSize += addCountItems * ISize;
	// ������ ���������� ����������� ������� �������
	while (addCountItems) {
		File.write((const char*)IMemory, ISize);
		--addCountItems;
	}
}

//-----------------------------------------------------------------------------
bool ZCStorageBinaryFile::operator >> (void* const I) 
{
	// ��������� ���� ��� ������ I
	unsigned long Mjd = (unsigned long)GetItemMjd(I);
	// �������� ��������� ���� �� ������������
	if (Mjd < Mjd0) return false;
	// ����� ������ � �������� �������� ���������
	// ������� ����� �� ���� �������
	unsigned long NumDay = Mjd - Mjd0;
	// �������� ������� � ��������� ����� ������
	if (NumDay >= CountItems) return false;
	// ������������� ��������� ������ � �����
	// �� ������ �������.
	File.seekp(NumDay*ISize);
	// ��������� ������ ������� ��� ���� � ���������
	File.read((char*)I, ISize);
	return true;
}

//-----------------------------------------------------------------------------
ZCStorageBinaryFile& ZCStorageBinaryFile::operator << (const void* I)
{
	// ��������� ���� ��� ������ I
	unsigned long Mjd = (unsigned long)GetItemMjd((void*)I);
	// �������� ��������� ���� �� ������������
	if (Mjd < Mjd0) return *this;
	// ����� ������ � �������� �������� ���������
	// ������� ����� � �� ��������� - ���������� �����
	// ��� � ���� ������������� �� 0. 1-� ������ ��� 0.
	// ������� ���� � ��������� ������������ ���������� �� ���������
	// ����� ���� �� � ���������� ������� ������� �� ������� 365...
	unsigned long NumDay = Mjd - Mjd0;

	// ������������� ��������� ������ � �����
	// �� ����� ������ CountItems, �� ����
	// �� ������ ����� ������, ��������� �������������
	// ������� ������ ���������� � ���� ������ ��������
	// ������ ������� ������, ���� ������� ����� ����� ���� ����.
	// ���� �� NumDay < CountItems, �� ��������� ������ ���������������
	// �� ��� ������������ ������ � ������� NumDay. ������ ������
	// ����� ����������������.
	File.seekp(Min(CountItems, NumDay)*ISize);
	// ��� ������������� ����������� ���������� ����� 
	// �������� ��������
	WriteEmptyEopItems(NumDay - CountItems);

	// ��������������� ������ � �������� ����� ����� ������
	// ����������� �� ���������� ����� � ������� IERS
	File.write((const char*)I, ISize);

	// ���� ������ ���� ��������� � ����� �����
	// ��������� ���������, ��...
	if (NumDay == CountItems) {
		// ��������� �������� ������� �����,
		FSize = static_cast<unsigned long>(File.tellp());
		// ���������� ����������� ������� ���������� � �������
		// �������� ���� ��������� ��� 
		++CountItems;
	}

	return *this;
}

//-----------------------------------------------------------------------------
float ZCStorageBinaryFile::GetItemMjd(void* I) const
{
	float* pMjd = (float*)I;
	return (*pMjd);
}

//-----------------------------------------------------------------------------
unsigned long ZCStorageBinaryFile::GetISize() const
{
	return ISize;
}

//-----------------------------------------------------------------------------
unsigned long ZCStorageBinaryFile::GetFSize() const
{
	return FSize;
}

//-----------------------------------------------------------------------------
unsigned long ZCStorageBinaryFile::GetCountItems() const
{
	return CountItems;
}

//-----------------------------------------------------------------------------
const char* ZCStorageBinaryFile::GetFileName() const
{
	return FullName.c_str();
}

//-----------------------------------------------------------------------------
