//-----------------------------------------------------------------------------
// ZСHtmlTeg.h
//
// Описание тега с атрибутами. 
// Тег состоит из имени и массива атрибутов
// Каждый атрибут состоит из двух строк: имени атрибута и его значения.
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include <FlyHtmlWriter.h>
#include <FlyHtmlWriterSource\\ZСHtmlTeg.h>

//-----------------------------------------------------------------------------
ZСHtmlTeg::ZСHtmlTeg(const char* sn, const char* sa) {
	*this = sn;
	*this << sa;
}

//-----------------------------------------------------------------------------
ZСHtmlTeg::~ZСHtmlTeg() { 
	a.clear(); 
}

//-----------------------------------------------------------------------------
bool ZСHtmlTeg::isEmpty() const {
	return a.size() ? false : true; 
}

//-----------------------------------------------------------------------------
int ZСHtmlTeg::CetCountAttr() const { 
	return a.size(); 
}

//-------------------------------------------------------------------------
// Присвоить тегу значение "d", "div", "font", "style" ...
ZСHtmlTeg& ZСHtmlTeg::operator = (const char* sn) {
	if (!sn) { n = ""; a.clear();  return *this; }
	n = sn;
	return *this;
}

//-------------------------------------------------------------------------
// Добавить в тег атрибуты со значениями через список инициализации
ZСHtmlTeg& ZСHtmlTeg::operator+= (const std::initializer_list<const char*> ss) {
	return (*this << ss);
}

//-----------------------------------------------------------------------------
ZСHtmlTeg& ZСHtmlTeg::operator << (vector<ZSHtmlAttr>& va) {
	a = std::move(va);
	return *this;
}

//-----------------------------------------------------------------------------
ZСHtmlTeg& ZСHtmlTeg::operator << (ZSHtmlAttr& _a) {
	a.push_back(std::move(_a));
	return *this;
}

//-----------------------------------------------------------------------------
ZСHtmlTeg& ZСHtmlTeg::operator << (const char* ss) {
	if (!ss) { a.clear();  return *this; }
	string s1, s2;
	char   *se, *s = (char*)ss;
	int    j = 0;
	while (1) {
		se = FindSprChar(s); if (!se) break;
		(j ? s2 : s1).assign(s, se - s);
		if (j) {
			a.emplace_back(ZSHtmlAttr(s1.c_str(), s2.c_str()));
		}
		s = ScipSprChar(se); if (!s) break;
		j = !j;
	}
	if (!j) a.emplace_back(ZSHtmlAttr(s1.c_str()));
	return *this;
}

//-----------------------------------------------------------------------------
ZСHtmlTeg& ZСHtmlTeg::operator << (char* ss[]) {
	a.emplace_back(ZSHtmlAttr(ss[0], ss[1]));
	return *this;
}

//-----------------------------------------------------------------------------
ZСHtmlTeg& ZСHtmlTeg::operator << (const std::initializer_list<const char*> ss) {
	int i = 1;
	const char *s1 = nullptr, *s2;
	for (const char* s : ss) {
		i = !i;
		(i ? s2 : s1) = s;
		if (i) {
			a.emplace_back(ZSHtmlAttr(s1, s2));
			s1 = nullptr;
	}	}
	if (s1) a.emplace_back(ZSHtmlAttr(s1));
	return *this;
}

//-------------------------------------------------------------------------
// Доступ к атрибуту по индексу
ZSHtmlAttr& ZСHtmlTeg::operator[] (const long i) {
	int j = i;
	int n = a.size();
	if (j <  0) j = n ? n - 1 : 0;
	if (j >= n) a.resize(j + 1);
	return a[j];
}

//-----------------------------------------------------------------------------
void ZСHtmlTeg::Tab(FILE* of, int level) {
	for (int i = 0; i<level; i++) fprintf(of, "\t\0");
}

//-----------------------------------------------------------------------------
// Документирование тега с атрибутами тега
void ZСHtmlTeg::Open(FILE* of, int level) {
	// Отступ от левого края документа - реализация лесенки
	// для читаемости кода документа
	Tab(of, level);
	// Имя тега
	if (!n.length()) return;
	fprintf(of, "<%s\0", n.c_str());
	// Атрибуты тега
	for each (ZSHtmlAttr ai in a){
		fprintf(of, " %s=\"%s\"\0", ai.n.c_str(), ai.v.c_str());
	}
	fprintf(of, ">\n\0");
}

//-----------------------------------------------------------------------------
void ZСHtmlTeg::Close(FILE* of, int level) {
	if (!n.length()) return;
	if (n == "hr" || n == "br") return;
	// Отступ от левого края документа - реализация лесенки
	// для читаемости кода документа
	Tab(of, level);
	// Тег сущьности закрывается
	fprintf(of, "</%s>\n\0", n.c_str());
}

//-----------------------------------------------------------------------------
