//-----------------------------------------------------------------------------
// ZСHtmlCssStyles.h
//
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include <FlyHtmlWriter.h>
#include <FlyHtmlWriterSource\\ZСHtmlCssStyles.h>

string ZСHtmlCssStyles::defautFlyProjectCssFile = "FlyProjectHtmlStyles";

//-----------------------------------------------------------------------------
// Элемент таблицы стилей
//-----------------------------------------------------------------------------
ZСHtmlCssItem::ZСHtmlCssItem(const char* sn) :
ZСHtmlTeg(sn) {

}

// Документирование тега с атрибутами тега
void ZСHtmlCssItem::Open(FILE* of, int level) {
	// Отступ от левого края документа - реализация лесенки
	// для читаемости кода документа
	fprintf(of, "\t\0");
	// Имя тега
	fprintf(of, "%s {\n\0", n.c_str());
	// Атрибуты тега
	for each (ZSHtmlAttr ai in a){
		fprintf(of, "\t\t%s: %s;\n\0", ai.n.c_str(), ai.v.c_str());
	}
}

void ZСHtmlCssItem::Close(FILE* of, int level) {
	fprintf(of, "\t}\n\0");
}

//-----------------------------------------------------------------------------
// Таблица стилей
//-----------------------------------------------------------------------------
ZСHtmlCssStyles::ZСHtmlCssStyles() {
}

ZСHtmlCssStyles::~ZСHtmlCssStyles() {
	Clear();
}

//-----------------------------------------------------------------------------
void ZСHtmlCssStyles::Clear()    {
	t.clear(); 
	cssLocation = 0;
}

bool ZСHtmlCssStyles::isEmpty() const {
	return t.size() ? false : true;
}

int  ZСHtmlCssStyles::CetCount() const {
	return t.size();
}

//-----------------------------------------------------------------------------
// Добавить новый тег
ZСHtmlCssStyles& ZСHtmlCssStyles::Add(const char* sn, const char* sa) {
	int n = (int)t.size();
	t.resize(n + 1);
	t[n] = sn;
	t[n] << sa;
	return *this;
}

//-----------------------------------------------------------------------------
ZСHtmlCssStyles& ZСHtmlCssStyles::operator += (const char* sn) {
	return Add(sn, nullptr);
}

//-----------------------------------------------------------------------------
// Наполнить последний добавленный тег атрибутами 
ZСHtmlCssStyles& ZСHtmlCssStyles::operator << (const char* ss) {
	int n = (int)t.size();
	if (n) {
		t[n - 1] << ss;
		if (t[n - 1].isEmpty()) t.resize(n - 1);
	}
	return *this;
}

//-----------------------------------------------------------------------------
// Доступ к заданному тегу
ZСHtmlCssItem& ZСHtmlCssStyles::operator() (const long i) {
	int j = i;
	int n = t.size();
	if (j < 0) j = n ? n - 1 : 0;
	if (j >= n) t.resize(j + 1);
	return t[j];
}

//-----------------------------------------------------------------------------
void ZСHtmlCssStyles::Self(FILE* of) {
	if (!t.size()) return;
	if (!cssLocation) fprintf(of, "<style type=\"text/css\">\n\0");
	for each (ZСHtmlCssItem si in t){
		si.Open(of);
		si.Close(of);
	}
	if (!cssLocation) fprintf(of, "</style>\n\0");
}

//-----------------------------------------------------------------------------
