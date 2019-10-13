//-----------------------------------------------------------------------------
// ZСHtmlItem.h
//
// Элемент документа в формате HTML - 
// - любой тег с возможными атрибутами, текстом и вложенными дочерними тегами.
// Каждый дочерний тег также может иметь дочернии теги.
// Тест как и атрибуты являются необязательными элементами и могут 
// отсутствовать. Обязательным и неотъемлемым элементом тега является его имя,
// например: "p", "div", "a" ...
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include <FlyHtmlWriter.h>
#include <FlyHtmlWriterSource\\ZSHtmlAtribute.h>
#include <FlyHtmlWriterSource\\ZСHtmlTeg.h>
#include <FlyHtmlWriterSource\\ZСHtmlItem.h>

//-----------------------------------------------------------------------------
ZСHtmlItem::ZСHtmlItem(const char* tegName, int textPos,
	char* text, vector<ZSHtmlAttr>* atribut) : ZСHtmlTeg(tegName) {
	txt = text ? text : "";
	level = 0;
	txtPos = textPos;
	if (atribut) a = std::move(*atribut);
}

//-----------------------------------------------------------------------------
ZСHtmlItem::~ZСHtmlItem() {
	nestedItems.clear();
}

//-----------------------------------------------------------------------------
// Конструктор копирования
ZСHtmlItem::ZСHtmlItem(const ZСHtmlItem& I) {
	Copy(I);
}

//-----------------------------------------------------------------------------
// Конструктор перемещения
ZСHtmlItem::ZСHtmlItem(ZСHtmlItem&& I) {
	Move(std::move(I));
}

//-----------------------------------------------------------------------------
ZСHtmlItem& ZСHtmlItem::operator = (const ZСHtmlItem& I) {
	return(Copy(I));
}

//-----------------------------------------------------------------------------
ZСHtmlItem& ZСHtmlItem::operator = (ZСHtmlItem&& I) {
	return(Move(std::move(I)));
}

//-----------------------------------------------------------------------------
// Дабавить дочерний тег.
// Тег добавляется в конец списка дочерних тегов методом 
// перемещения, то есть исходная структура данных, содержащая
// добавляемый тег после завершения операции остаётся пустой.
// Используется vove-семантик C++ 11.
// Данный подход используется для повышения бысмтродействия.
ZСHtmlItem& ZСHtmlItem::operator << (ZСHtmlItem& I) {
	I.level = level + 1;
	nestedItems.push_back(std::move(I));
	return *this;
}

//-----------------------------------------------------------------------------
ZСHtmlItem& ZСHtmlItem::operator << (char* s) {
	txt = s;
	return *this;
}

//-----------------------------------------------------------------------------
// Доступ к дочерним тегам по индексу
// Если индекс не задавать, то возвращается ссылка на последний 
// дочерний тег. Если индекс выходит за границу массива, то массив
// автоматически дополняется пустыми тегами.
ZСHtmlItem& ZСHtmlItem::operator() (const long i) {
	int j = i;
	int n = nestedItems.size();
	if (j < 0) j = n ? n - 1 : 0;
	if (j >= n) {
		nestedItems.resize(j + 1);
		for (int k = n; k <= j; k++) nestedItems[k].level = level + 1;
	}
	return nestedItems[j];
}

//-----------------------------------------------------------------------------
int ZСHtmlItem::CountItems() const {
	return nestedItems.size();
}

//-----------------------------------------------------------------------------
void ZСHtmlItem::Open(FILE* of) {
	ZСHtmlTeg::Open(of, level);
	for (int i = 0; i < C.CountItem(); ++i)	C(i).Open(of, level);
}

//-----------------------------------------------------------------------------
void ZСHtmlItem::Close(FILE* of) {
	for (int i = C.CountItem() - 1; i >= 0; --i) C(i).Close(of, level);
	ZСHtmlTeg::Close(of, level);
}

//-----------------------------------------------------------------------------
void ZСHtmlItem::PrintText(FILE* of) {
	if (!txt.length()) return;
	ZСHtmlTeg::Tab(of, level + 1);
	fprintf(of, "%s\n\0", txt.c_str());
}

//-----------------------------------------------------------------------------
void ZСHtmlItem::Self(FILE* of) {
	if (!of) return;
	int i, l, n;
	l = txt.length();
	n = nestedItems.size();
	for (i = 0; i < n; ++i) {
		if (i == 1 && n == 5)
			i = i;
		if (l > 0 && i == txtPos) PrintText(of);
		nestedItems[i].Open(of);
		nestedItems[i].Self(of);
		nestedItems[i].Close(of);
	}
	if (l > 0 && i <= txtPos) PrintText(of);
}

//-----------------------------------------------------------------------------
ZСHtmlItem& ZСHtmlItem::Move(ZСHtmlItem&& I) {
	level = I.level;
	txtPos = I.txtPos;
	I.level = 0;
	I.txtPos = 0;
	txt = std::move(I.txt);
	nestedItems = std::move(I.nestedItems);
	//C = std::move(I.C);
	C.t = std::move(I.C.t);
	n = std::move(I.n);
	a = std::move(I.a);
	return *this;
}

//-----------------------------------------------------------------------------
ZСHtmlItem& ZСHtmlItem::Copy(const ZСHtmlItem& I) {
	txt = I.txt;
	level = I.level;
	txtPos = I.txtPos;
	nestedItems = I.nestedItems;
	C = I.C;
	n = I.n;
	a = I.a;
	return *this;
}

//-----------------------------------------------------------------------------
