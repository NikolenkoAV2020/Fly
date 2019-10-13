//-----------------------------------------------------------------------------
// Z�HtmlCssStyles.h
//
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include <FlyHtmlWriter.h>
#include <FlyHtmlWriterSource\\Z�HtmlCssStyles.h>

string Z�HtmlCssStyles::defautFlyProjectCssFile = "FlyProjectHtmlStyles";

//-----------------------------------------------------------------------------
// ������� ������� ������
//-----------------------------------------------------------------------------
Z�HtmlCssItem::Z�HtmlCssItem(const char* sn) :
Z�HtmlTeg(sn) {

}

// ���������������� ���� � ���������� ����
void Z�HtmlCssItem::Open(FILE* of, int level) {
	// ������ �� ������ ���� ��������� - ���������� �������
	// ��� ���������� ���� ���������
	fprintf(of, "\t\0");
	// ��� ����
	fprintf(of, "%s {\n\0", n.c_str());
	// �������� ����
	for each (ZSHtmlAttr ai in a){
		fprintf(of, "\t\t%s: %s;\n\0", ai.n.c_str(), ai.v.c_str());
	}
}

void Z�HtmlCssItem::Close(FILE* of, int level) {
	fprintf(of, "\t}\n\0");
}

//-----------------------------------------------------------------------------
// ������� ������
//-----------------------------------------------------------------------------
Z�HtmlCssStyles::Z�HtmlCssStyles() {
}

Z�HtmlCssStyles::~Z�HtmlCssStyles() {
	Clear();
}

//-----------------------------------------------------------------------------
void Z�HtmlCssStyles::Clear()    {
	t.clear(); 
	cssLocation = 0;
}

bool Z�HtmlCssStyles::isEmpty() const {
	return t.size() ? false : true;
}

int  Z�HtmlCssStyles::CetCount() const {
	return t.size();
}

//-----------------------------------------------------------------------------
// �������� ����� ���
Z�HtmlCssStyles& Z�HtmlCssStyles::Add(const char* sn, const char* sa) {
	int n = (int)t.size();
	t.resize(n + 1);
	t[n] = sn;
	t[n] << sa;
	return *this;
}

//-----------------------------------------------------------------------------
Z�HtmlCssStyles& Z�HtmlCssStyles::operator += (const char* sn) {
	return Add(sn, nullptr);
}

//-----------------------------------------------------------------------------
// ��������� ��������� ����������� ��� ���������� 
Z�HtmlCssStyles& Z�HtmlCssStyles::operator << (const char* ss) {
	int n = (int)t.size();
	if (n) {
		t[n - 1] << ss;
		if (t[n - 1].isEmpty()) t.resize(n - 1);
	}
	return *this;
}

//-----------------------------------------------------------------------------
// ������ � ��������� ����
Z�HtmlCssItem& Z�HtmlCssStyles::operator() (const long i) {
	int j = i;
	int n = t.size();
	if (j < 0) j = n ? n - 1 : 0;
	if (j >= n) t.resize(j + 1);
	return t[j];
}

//-----------------------------------------------------------------------------
void Z�HtmlCssStyles::Self(FILE* of) {
	if (!t.size()) return;
	if (!cssLocation) fprintf(of, "<style type=\"text/css\">\n\0");
	for each (Z�HtmlCssItem si in t){
		si.Open(of);
		si.Close(of);
	}
	if (!cssLocation) fprintf(of, "</style>\n\0");
}

//-----------------------------------------------------------------------------
