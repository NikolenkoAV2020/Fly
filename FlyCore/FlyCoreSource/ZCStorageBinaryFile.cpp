//-----------------------------------------------------------------------------
// ZCStorageBinaryFile.cpp
// Бинарный файл-хранилище ПВЗ
//
// А. Николенко 04.05.2019
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
	//Закрытие файла если он остался открытым
	Close();
}

//-----------------------------------------------------------------------------
void ZCStorageBinaryFile::Close()
{
	//Закрытие файла
	if (File.is_open())	File.close();
	if (IMemory) delete[] IMemory;
}

//-----------------------------------------------------------------------------
// Двоичный формат файла для быстрой загрузки - длинное имя файла 
// вместе с каталогом его размещения
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
	//Закритие бинарного файла, если это необходимо
	Close();

	// Создаём имя файла по его номеру и типу
	const char* FileName = GetBinaryFileName(NumFile, Type);

	// Открытие нового файла.
	// Указатель чтения-записи одновременно устанавливается
	// на конец файла.
	File.open(FileName, ios::ate | ios::in | ios::out | ios::binary);
	if (File.fail()) {
		// Попытка открытия файла одновременно в режиме
		// чтения и записи может быть неудачной потому-что
		// файл не существует (так это бывает при открытии только 
		// для чтения, в режиме записи файл автоматически создаётся).
		// Поэтому пробуем сперва создать данный файл, открыв его 
		// только в режиме записи...
		File.open(FileName, ios::out | ios::binary);
		// Если и в этом случае ошибка, то завершение процедуры
		if (File.fail()) return-2;
		// Закрытие только что созданного файла
		File.close();
		// Повторное открытие в режиме и чтения и записи
		File.open(FileName, ios::ate | ios::in | ios::out |
			ios::binary);
		// Проверка корректности открытия
		if (File.fail()) return-2;
		// Если всё успешно, то указаталь чтения-записи файла 
		// позиционирован на конец файла
	}

	// Начальная юлианская дата
	Mjd0 = (unsigned long)fromMJD;
	// Размер одной записи
	ISize = ItemSize;
	// Определение размера открытого бинарного хранилища
	FSize = static_cast<unsigned long>(File.tellp());
	// Число записей в файле.
	CountItems = FSize / ISize;
	// Память одной "нулевой записи"
	IMemory = new char[ISize];
	memset(IMemory, 0, ISize);
	
	return 0;
}

//-----------------------------------------------------------------------------
// Запись countItems нулевых структур с ПВЗ в конец файла File
void ZCStorageBinaryFile::WriteEmptyEopItems(int addCountItems)
{
	if (addCountItems <= 0) return;
	// Нулевая запись ПВЗ
	// Структура ZSEopItem инициализируется по умолчанию
	// нулевыми значениями, следовательно наличие в 
	// хранилище ПВЗ записи с нулевым полем Key будет означать что
	// записи этой в исходной информации полученной из IERS не было.

	// Увеличивается колличество записей в файле
	CountItems += addCountItems;
	// Увеличивается размер файла
	FSize += addCountItems * ISize;
	// Запись требуемого колличества нулевых записей
	while (addCountItems) {
		File.write((const char*)IMemory, ISize);
		--addCountItems;
	}
}

//-----------------------------------------------------------------------------
bool ZCStorageBinaryFile::operator >> (void* const I) 
{
	// Юлианская дата изи записи I
	unsigned long Mjd = (unsigned long)GetItemMjd(I);
	// Проверка юлианской даты на допустимость
	if (Mjd < Mjd0) return false;
	// Номер записи в открытом бинарном хранилище
	// которую нужно из него извлечь
	unsigned long NumDay = Mjd - Mjd0;
	// Проверка наличия в хранилище такой записи
	if (NumDay >= CountItems) return false;
	// Устанавливаем указатель записи в файле
	// на нужную позицию.
	File.seekp(NumDay*ISize);
	// Считываем данные которые уже были в хранилище
	File.read((char*)I, ISize);
	return true;
}

//-----------------------------------------------------------------------------
ZCStorageBinaryFile& ZCStorageBinaryFile::operator << (const void* I)
{
	// Юлианская дата изи записи I
	unsigned long Mjd = (unsigned long)GetItemMjd((void*)I);
	// Проверка юлианской даты на допустимость
	if (Mjd < Mjd0) return *this;
	// Номер записи в открытом бинарном хранилище
	// которую нужно в нём сохранить - фактически номер
	// дня в году отсчитываемый от 0. 1-е января это 0.
	// Конечно если в хранилище записывается информация на интервале
	// более года то и индексация записей выходит за пределы 365...
	unsigned long NumDay = Mjd - Mjd0;

	// Устанавливаем указатель записи в файле
	// на конец записи CountItems, то есть
	// на начало новой записи, игнорируя недостоверный
	// участок данных записанных в файл длинна которого
	// меньше размера записи, если конечно такое имеет мето быть.
	// Если же NumDay < CountItems, то указатель записи устанавливается
	// на уже существующую запись с номером NumDay. Данная запись
	// далее перезаписывается.
	File.seekp(Min(CountItems, NumDay)*ISize);
	// При необходимости выполняется дополнение файла 
	// нулевыми записями
	WriteEmptyEopItems(NumDay - CountItems);

	// Непосредственно запись в бинарный архив новой записи
	// загруженной из текстового файла в формате IERS
	File.write((const char*)I, ISize);

	// Если запись была добавлена в конец файла
	// бинарного хранилища, то...
	if (NumDay == CountItems) {
		// обновляем значение размера файла,
		FSize = static_cast<unsigned long>(File.tellp());
		// увеличение колличества записей записанных в текущий
		// бинарный файл хранилища ПВЗ 
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
