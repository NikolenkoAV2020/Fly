//-----------------------------------------------------------------------------
// Z�HtmlTeg.h
//
// �������� ���� � ����������. 
// ��� ������� �� ����� � ������� ���������
// ������ ������� ������� �� ���� �����: ����� �������� � ��� ��������.
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include <FlyHtmlWriter.h>
#include <FlyHtmlWriterSource\\Z�HtmlTeg.h>

//-----------------------------------------------------------------------------
Z�HtmlTeg::Z�HtmlTeg(const char* sn, const char* sa) {
	*this = sn;
	*this << sa;
}

//-----------------------------------------------------------------------------
Z�HtmlTeg::~Z�HtmlTeg() { 
	a.clear(); 
}

//-----------------------------------------------------------------------------
bool Z�HtmlTeg::isEmpty() const {
	return a.size() ? false : true; 
}

//-----------------------------------------------------------------------------
int Z�HtmlTeg::CetCountAttr() const { 
	return a.size(); 
}

//-------------------------------------------------------------------------
// ��������� ���� �������� "d", "div", "font", "style" ...
Z�HtmlTeg& Z�HtmlTeg::operator = (const char* sn) {
	if (!sn) { n = ""; a.clear();  return *this; }
	n = sn;
	return *this;
}

//-------------------------------------------------------------------------
// �������� � ��� �������� �� ���������� ����� ������ �������������
Z�HtmlTeg& Z�HtmlTeg::operator+= (const std::initializer_list<const char*> ss) {
	return (*this << ss);
}

//-----------------------------------------------------------------------------
Z�HtmlTeg& Z�HtmlTeg::operator << (vector<ZSHtmlAttr>& va) {
	a = std::move(va);
	return *this;
}

//-----------------------------------------------------------------------------
Z�HtmlTeg& Z�HtmlTeg::operator << (ZSHtmlAttr& _a) {
	a.push_back(std::move(_a));
	return *this;
}

//-----------------------------------------------------------------------------
Z�HtmlTeg& Z�HtmlTeg::operator << (const char* ss) {
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
Z�HtmlTeg& Z�HtmlTeg::operator << (char* ss[]) {
	a.emplace_back(ZSHtmlAttr(ss[0], ss[1]));
	return *this;
}

//-----------------------------------------------------------------------------
Z�HtmlTeg& Z�HtmlTeg::operator << (const std::initializer_list<const char*> ss) {
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
// ������ � �������� �� �������
ZSHtmlAttr& Z�HtmlTeg::operator[] (const long i) {
	int j = i;
	int n = a.size();
	if (j <  0) j = n ? n - 1 : 0;
	if (j >= n) a.resize(j + 1);
	return a[j];
}

//-----------------------------------------------------------------------------
void Z�HtmlTeg::Tab(FILE* of, int level) {
	for (int i = 0; i<level; i++) fprintf(of, "\t\0");
}

//-----------------------------------------------------------------------------
// ���������������� ���� � ���������� ����
void Z�HtmlTeg::Open(FILE* of, int level) {
	// ������ �� ������ ���� ��������� - ���������� �������
	// ��� ���������� ���� ���������
	Tab(of, level);
	// ��� ����
	if (!n.length()) return;
	fprintf(of, "<%s\0", n.c_str());
	// �������� ����
	for each (ZSHtmlAttr ai in a){
		fprintf(of, " %s=\"%s\"\0", ai.n.c_str(), ai.v.c_str());
	}
	fprintf(of, ">\n\0");
}

//-----------------------------------------------------------------------------
void Z�HtmlTeg::Close(FILE* of, int level) {
	if (!n.length()) return;
	if (n == "hr" || n == "br") return;
	// ������ �� ������ ���� ��������� - ���������� �������
	// ��� ���������� ���� ���������
	Tab(of, level);
	// ��� ��������� �����������
	fprintf(of, "</%s>\n\0", n.c_str());
}

//-----------------------------------------------------------------------------
