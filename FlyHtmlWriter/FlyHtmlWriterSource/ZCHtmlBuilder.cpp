//-----------------------------------------------------------------------------
// ZCHtmlBuilder.cpp
//
// "Писатель" HTML- документа 
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include <FlyHtmlWriter.h>
#include <FlyHtmlWriterSource\\ZCHtmlBuilder.h>

//-----------------------------------------------------------------------------
ZCHtmlBuilder::ZCHtmlBuilder(void)
{
	fileName = "";
	shortFileName = "";
}

ZCHtmlBuilder::~ZCHtmlBuilder(void)
{
	mapedPoint.clear();
}

//-----------------------------------------------------------------------------
// Создание, удаление и получение именованных меток элементов
// html-документа.
// 
// Отметив однажды некоторый элемент именем, в дальнейшем всегда можно 
// получить его индекс.
// Используется при многократном дополнении какого-либо элемента
// в различных частях программного кода или при многократном вызове
// некоторой функции документирования, последовательно 
// формирующей данный элемент.
//-----------------------------------------------------------------------------
// Создание именнованной метки элемента, точнее индекса этого элемента
int ZCHtmlBuilder::CreateItemsMark(char* key, ZSMarkItem& mi)
{
	mapedPoint.insert(make_pair(key, mi));
	return 0;
}

// Удаление именнованной метки элемента
int ZCHtmlBuilder::DeletItemsMark(char* key)
{
	auto i = mapedPoint.find(key);
	if (i != mapedPoint.end()) mapedPoint.erase(i);
	return 0;
}

// Получение индекса элемента по его метке
ZSMarkItem ZCHtmlBuilder::GetItemsMark(char* key)
{
	ZSMarkItem mi;
	auto i = mapedPoint.find(key);
	if (i != mapedPoint.end()) mi = i->second;
	return mi;
}

//-----------------------------------------------------------------------------
// Добавить новый элемент в документ
// I должен содержать как тег, так и текст, а также массив 
// вложенных элементов (тегов).
// Если имя тега отсутствует то текст вставляется как неформатированный
// текст и не обрамляется никаким тегом.
//-----------------------------------------------------------------------------
ZCHtmlBuilder& ZCHtmlBuilder::operator << (ZСHtmlItem& I) {
	root << I; 
	return *this;
}

//-----------------------------------------------------------------------------
// Доступ к одму из элементов документа по индексу
ZСHtmlItem& ZCHtmlBuilder::operator() (long i) { 
	return root(i); 
}

//-----------------------------------------------------------------------------
// Колличество элементов в документе
int ZCHtmlBuilder::CountItems() const {
	return root.CountItems(); 
}

//-----------------------------------------------------------------------------
void ZCHtmlBuilder::UseDefaultStyles(bool useDefFile)
{
	S.cssLocation = useDefFile ? 2 : 0;
	if (useDefFile) S.Clear();
}

//-----------------------------------------------------------------------------
// Создание таблицы стилей по умолчанию.
// Если данный метод не вызывать то таблица стилей в 
// создаваемом документе будет отсутствовать. Тогда либо совсем без
// стлей, либо задавать их самостоятельно, 
// либо воспользоваться файлом ".css"
//-----------------------------------------------------------------------------
void ZCHtmlBuilder::BuildStyles()
{
	S.Add("html") << "font-size \n 125%";
	S.Add("body") 
		<< "font-size \n 1em"
		<< "color \n #ff8c40"
		<< "background \n #000000";

	// Заголовки
	S.Add("h1, h2, h3, h4, h5, h6")
		<< "font-family \n \'Times New Roman\', Times, serf"
		<< "font-weight \n bold"
		<< "text-indent \n 36pt"
		<< "text-align  \n left"
		<< "text-shadow \n 0 0 3px #FFF, 0 0 6px #FFF, 0 0 10px #FFF, 0 0 14px #49ff18, 0 0 20px #49FF18, 0 0 28px #49FF18, 0 0 35px #49FF18, 0 0 45px #49ff18, -1px -5px 7px rgba(28, 31, 170, 0.39)"
		<< "color       \n #aa4900";
	S.Add("h1", "font-size \n 2.5em");
	S.Add("h2", "font-size \n 2.1em");
	S.Add("h3", "font-size \n 1.7em");
	S.Add("h4", "font-size \n 1.3em");
	S.Add("h5", "font-size \n 1.1em");
	S.Add("h6", "font-size \n 1.0em");

	// Параграф 
	S.Add("p, div")
		<< "text-indent \n 36pt"
		<< "text-align  \n justify"
		<< "line-height \n 100%";
	S.Add("p")
		<< "font-family \n Verdana, Arial, Helvetica, sans-serf"
		<< "font-size   \n 1.0em";
	S.Add("p.simpletext")
		<< "padding \n 3pt 42pt 1pt 0pt";
	S.Add("p.rose")
		<< "color \n #ff00ff"
		<< "font  \n 150% Arial";

	// Блок по умолчанию
	S.Add("div")
		<< "font-family \n \'Courier New\'"
		<< "font-size   \n 100%";

	// Стиль для эпиграфа и постскриптума
	S.Add("div.epistyle, div.poststyle")
		<< "color  \n #fff "
		<< "text-indent \n 0pt"
		<< "text-align  \n right"
		<< "font-family \n \'Times New Roman\'"
		<< "font-weight \n 400"
		<< "font-style  \n oblique"
		<< "font-size   \n 90%"
		<< "margin-right\n 20pt";
	S.Add("div.epistyle")
		<< "text-title  \n \"Вместо эпиграфа.\"";

	// Стиль для левой колонки повитковой таблицы с параметрами движения
	S.Add("div.predVitLeftCol")
		<< "color  \n #fff "
		<< "text-indent \n 0pt"
		<< "text-align  \n left"
		<< "font-family \n \'Times New Roman\'"
		<< "font-size   \n 100%";
	S.Add("div.predVitHider")
		<< "color  \n #fff "
		<< "text-indent \n 0pt"
		<< "text-align  \n right"
		<< "font-family \n \'Times New Roman\'"
		<< "font-size   \n 100%";
	S.Add("div.predSubHeader")
		<< "color  \n #fff "
		<< "text-indent \n 0pt"
		<< "text-align  \n center"
		<< "font-family \n \'Times New Roman\'"
		<< "font-size   \n 100%";

	S.Add("a")
		<< "color \n #35940f"
		<< "font-size \n 80%"
		<< "text-shadow \n #ffffff 1px 1px 8px";

	S.Add("a:hover")
		<< "color \n #2cf704"
		<< "font-weight \n 400"
		<< "font-size \n 100%";

	S.Add("a:active")
		<< "color \n #e6ef09"
		<< "font-weight \n 700"
		<< "font-size \n 100%"
		<< "text-shadow \n #001ed0 -2px -2px 8px";

	S.Add("a:visited")
		<< "color \n #c7c6c6"
		<< "text-shadow \n #ffffff 1px 1px 11px";

	//-------------------------------------------------------------------------
	S.Add("table")
		<< "font-family \n \'Times New Roman\'"
		<< "text-align \n right"
		<< "margin-left \n 36pt"
		<< "overflow \n visible"
		<< "white-space \n nowrap";

	S.Add("table sub")
		<< "vertical-align \n -30%"
		<< "font-size \n 90%"
		<< "line-height \n 50%";

	S.Add("table sup")
		<< "vertical-align \n +30%"
		<< "font-size \n 90%"
		<< "line-height \n 50%";

	//-------------------------------------------------------------------------
	// Простая таблица-невидимка
	S.Add("table.simple")
		<< "text-align \n left";

	//-------------------------------------------------------------------------
	// Таблица с результатами повиткового прогноза
	S.Add("table.predByVit")
		<< "border \n 4px solid #555555"
		<< "color \n #000000"
		<< "background \n #ffc096"
		<< "line-height \n 125%";

	S.Add("table.predByVit th")
		<< "background \n #38cf42"
		<< "text-align \n left"
		<< "padding \n 3pt 15pt 3pt 15pt";

	S.Add("table.predByVit tr th ~th")
		<< "text-align \n right";

	S.Add("table.predByVit td")
		<< "padding \n 3pt 15pt 3pt 15pt";

	//-------------------------------------------------------------------------
	// Таблица с НУ и ЛШС
	S.Add("table.NU")
		<< "color \n #000000"
		<< "background \n #ffc096"
		<< "line-height \n 125%"
		<< "border \n 1px solid #555555"
		<< "border-radius \n 17px"
		<< "-webkit-box-shadow \n 16px 7px 19px -2px rgba(235, 248, 255, 0.89)"
		<< "box-shadow \n 16px 7px 19px -2px rgba(235, 248, 255, 0.89)";

	S.Add("table.NU th")
		<< "background \n #38cf42"
		<< "text-align \n left"
		<< "padding \n 3pt 15pt 3pt 15pt";

	S.Add("table.NU td")
		<< "padding \n 3pt 15pt 3pt 15pt"
		<< "font-weight \n 800"
		<< "font-family \n \"Courier New\", Courier, monospace";

	//-------------------------------------------------------------------------
	// Таблица для документирования исходных данных
	S.Add("table.FlyID, table.FlyID2")
		<< "line-height \n 125%"
		<< "border \n 1px solid #555555"
		<< "border-radius \n 17px"
		<< "-webkit-box-shadow \n 16px 7px 19px -2px rgba(235, 248, 255, 0.89)"
		<< "box-shadow \n 16px 7px 19px -2px rgba(235, 248, 255, 0.89)";

	S.Add("table.FlyID, table.FlyID2 th")
		<< "text-align \n left"
		<< "padding \n 3pt 15pt 3pt 15pt";

	S.Add("table.FlyID, table.FlyID2 td")
		<< "font-weight \n 600"
		<< "padding \n 3pt 15pt 3pt 15pt";

	//-------------------------------------------------------------------------
	// Таблица для документирования исходных данных 2
	S.Add("table.FlyID2 th")
		<< "text-align \n center";

	S.Add("table.FlyID2 td")
		<< "font-family \n monospace";

	//-------------------------------------------------------------------------
	// Таблица для документирования простых результатов
	S.Add("table.FlyRezult")
		<< "line-height \n 125%";

	S.Add("table.FlyRezult th")
		<< "text-align \n center"
		<< "padding \n 3pt 15pt 3pt 15pt";

	S.Add("table.FlyRezult td")
		<< "text-align \n right"
		<< "font-weight \n 600"
		<< "font-family \n monospace"
		<< "padding \n 3pt 15pt 3pt 15pt";
}

//-----------------------------------------------------------------------------
// Начало создания нового документа
void ZCHtmlBuilder::Init(
	const char* docTitle,   // Заголовок окна в браузере
	const char* docHeader,  // Заголовок документа
	const int   headerLevel,// Уровень заголовка (h1-0...h6-5)
	const char* docText,	// Текст документа, заключается в тег <p>
	const char* file)		// Имя файла без расширения
{
	fileName = file ? file : "" ;

	docHat = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
	docHat += "<html lang = \"ru\" xml:lang = \"ru\" xmlns=\"http://www.w3.org/1999/xhtml\">\n";
	docHat += "<head>";
	docHat += "\t<title>"; docHat += docTitle; 	docHat += "</title>\n";
//	docHat+= "\t<meta charset = \"utf-8\">\n";
	docHat += "\t<meta name = \"author\"      Content = \"Вальтер Артёменко\">\n";
	docHat += "\t<meta name = \"copyright\"   Content = \"\"ИC-35\"\">\n";
	docHat += "\t<meta name = \"Generator\"	Content = \"Fly project\">\n";
	docHat += "\t<meta name = \"description\" Content = \"Решение прикладных задач динамики полёта КА\">\n";
	docHat += "\t<meta name = \"keywords\"    Content = \"ДЗЗ ГИС КА орбита \"прогноз движения КА\"\">\n";
	docHat += "\t<meta name = \"Publisher-Email\"  Content = \"mail@сервер.домен\">\n";
	docHat += "\t<meta name = \"Publisher-URL\"    Content = \"http://www.Ваш_сайт\">\n";
	docHat += "\t<meta http-equiv = \"expires\"    Content = \"0\">\n";
	docHat += "\t<meta http-equiv = \"Page-Enter\" Content = \"RevealTrans(Duration = 1.0, Transition = 19)\">\n";
	docHat += "\t<meta http-equiv = \"Page-Exit\"  Content = \"RevealTrans(Duration = 1.0, Transition = 18)\">\n";
	
	docEpigraf = "\t<div class=\"epistyle\" title=\"Вместо эпиграфа.\">\n";
	docEpigraf+= "\t\tDon't repeat himself neve...\n";
	docEpigraf+= "\t</div>";

	docFinal   = "\t<div class=\"poststyle\">\n";
	docFinal  += "\t\tFly project document\n";
	docFinal  += "\t</div>";

	// Корневой тег - body
	root = "body";
	root.txt = docText ? docText : "";
	root.txtPos = 0;
	// Добавляем заголовок если это необходимо
	if (docHeader) {
		int l = headerLevel;
		if (l > 5) l = 5;
		if (l < 0) l = 0;
		root.txtPos = 1;
		char si[12]="h"; 
		itoa(l + 1, &si[1], 10);
		root << ZСHtmlItem((const char*)&si[0], 0, (char*)docHeader);
	}
}

//-----------------------------------------------------------------------------
// Открытие файла для записи с заданным именем и с заданным расширением
FILE* ZCHtmlBuilder::OpenFile(const char* fName, const char* ext)
{
	if (!fName || !strlen(fName)) return nullptr;
	if (!ext || !strlen(ext)) return nullptr;
	// Имя создаваемого документа 
	string Name = fName; Name += "."; Name += ext;
	// Открытие файла для записи
	return(fopen(Name.c_str(), "w"));
}

//-----------------------------------------------------------------------------
// Закрытие файла
void ZCHtmlBuilder::CloseFile(FILE* of)
{
	if (of) fclose(of);
}

//-----------------------------------------------------------------------------
// Создание CSS-файла
int ZCHtmlBuilder::FlashStyles(FILE* f)
{
	if (!f) return 1;
	// Файл для записи таблицы стилей...
	// ... либо файл документа html,
	FILE *of = f;
	// ... либо отдельный css-файл.
	if (S.cssLocation) {
		of = OpenFile(fileName.c_str(), "css");
		string sfn = S.cssLocation == 1 ?
					 shortFileName : 
					 ZСHtmlCssStyles::defautFlyProjectCssFile;
		sfn += "."; sfn += "css";
		fprintf(f, "\t<link rel=\"stylesheet\" href=\"%s\" type=\"text/css\">\n",
		sfn.c_str());
	}
	if (!of) return 2;

	S.Self(of);

	CloseFile(of);
	return 0;
}

//-----------------------------------------------------------------------------
// Создание Html-документа по сформированному описанию
// Документ создаётся полностью до конца
// Если при инициализации Init задано имя файла fileName, 
// то при вызове метода Flash имя файла задавать не обязательно
int ZCHtmlBuilder::Flash(const char* fName)
{
	// Имя файла без расширения сохраняется в 
	// соответствующем поле класса
	if (fName) fileName = fName;
	if (!fileName.length()) return-1;

	// Открытие файла для записи
	FILE *of = OpenFile(fileName.c_str()); if (!of) return 1;
	
	// Формируется короткое имя файла (без пути)
	int i1 = fileName.find_last_of("/");
	int i2 = fileName.find_last_of("\\");
	int i  = i1 > i2 ? i1 : i2; i = i < 0 ? 0 : i;
	if (i > 0) {
		int l = fileName.length(); l -= i;
		shortFileName = fileName.substr(i + 1, l - 1);
	}
	else shortFileName = fileName;

	// Документирование заголовка ...
	// одновременно открываются теги <html> и <head>
	fprintf(of, "%s", docHat.c_str());
	// Документирование таблицы стилей. Если таблица не задана,
	// то в документ ничего не выдаётся
	FlashStyles(of);
	// Закрытие тега заголовка HTML-файла
	fprintf(of, "</head>\n");

	// Открываем корневой элемент документа
	root.Open(of);
	// Документирование эпиграфа
	fprintf(of, "%s\n", docEpigraf.c_str());
	// Документирование всего содержания корневого тега вместе со 
	// всеми вложенными элементами 
	root.Self(of);
	// Документирование PS
	fprintf(of, "%s\n", docFinal.c_str());
	// Закрытие корневого элемента
	root.Close(of);

	// Закрытие сформированного файла
	fprintf(of, "</html>\n");
	CloseFile(of);
	return 0;
}

//-----------------------------------------------------------------------------
// Начало поэтапного документирования.
void* ZCHtmlBuilder::BeginFlash(const char* fileName)
{
	return nullptr;
}

//-----------------------------------------------------------------------------
// Промежуточное документирование всех сформированных элементов.
// Осуществляется дозапись уже созданного
// документа html на который указывает file.
int ZCHtmlBuilder::MediateFlash(const void* file)
{
	if (!file) return-1;
	FILE* of = (FILE*)file;

	return 0;
}

//-----------------------------------------------------------------------------
// Завершение последовательного промежуточного (поэтапного) 
// документирования. Осуществляется дозапись уже созданного
// документа html на который указывает file. Записывается
// постскриптум и закрываются теги <body> и <html>
int ZCHtmlBuilder::EndFlash(const void* file)
{
	if (!file) return-1;
	FILE* of = (FILE*)file;

	return 0;
}

//-----------------------------------------------------------------------------