//-----------------------------------------------------------------------------
// Z�HtmlItem.h
//
// ������� ��������� � ������� HTML - 
// - ����� ��� � ���������� ����������, ������� � ���������� ��������� ������.
// ������ �������� ��� ����� ����� ����� �������� ����.
// ���� ��� � �������� �������� ��������������� ���������� � ����� 
// �������������. ������������ � ������������ ��������� ���� �������� ��� ���,
// ��������: "p", "div", "a" ...
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include <FlyHtmlWriter.h>
#include <FlyHtmlWriterSource\\ZSHtmlAtribute.h>
#include <FlyHtmlWriterSource\\Z�HtmlTeg.h>
#include <FlyHtmlWriterSource\\Z�HtmlItem.h>

//-----------------------------------------------------------------------------
Z�HtmlItem::Z�HtmlItem(const char* tegName, int textPos,
	char* text, vector<ZSHtmlAttr>* atribut) : Z�HtmlTeg(tegName) {
	txt = text ? text : "";
	level = 0;
	txtPos = textPos;
	if (atribut) a = std::move(*atribut);
}

//-----------------------------------------------------------------------------
Z�HtmlItem::~Z�HtmlItem() {
	nestedItems.clear();
}

//-----------------------------------------------------------------------------
// ����������� �����������
Z�HtmlItem::Z�HtmlItem(const Z�HtmlItem& I) {
	Copy(I);
}

//-----------------------------------------------------------------------------
// ����������� �����������
Z�HtmlItem::Z�HtmlItem(Z�HtmlItem&& I) {
	Move(std::move(I));
}

//-----------------------------------------------------------------------------
Z�HtmlItem& Z�HtmlItem::operator = (const Z�HtmlItem& I) {
	return(Copy(I));
}

//-----------------------------------------------------------------------------
Z�HtmlItem& Z�HtmlItem::operator = (Z�HtmlItem&& I) {
	return(Move(std::move(I)));
}

//-----------------------------------------------------------------------------
// �������� �������� ���.
// ��� ����������� � ����� ������ �������� ����� ������� 
// �����������, �� ���� �������� ��������� ������, ����������
// ����������� ��� ����� ���������� �������� ������� ������.
// ������������ vove-�������� C++ 11.
// ������ ������ ������������ ��� ��������� ���������������.
Z�HtmlItem& Z�HtmlItem::operator << (Z�HtmlItem& I) {
	I.level = level + 1;
	nestedItems.push_back(std::move(I));
	return *this;
}

//-----------------------------------------------------------------------------
Z�HtmlItem& Z�HtmlItem::operator << (char* s) {
	txt = s;
	return *this;
}

//-----------------------------------------------------------------------------
// ������ � �������� ����� �� �������
// ���� ������ �� ��������, �� ������������ ������ �� ��������� 
// �������� ���. ���� ������ ������� �� ������� �������, �� ������
// ������������� ����������� ������� ������.
Z�HtmlItem& Z�HtmlItem::operator() (const long i) {
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
int Z�HtmlItem::CountItems() const {
	return nestedItems.size();
}

//-----------------------------------------------------------------------------
void Z�HtmlItem::Open(FILE* of) {
	Z�HtmlTeg::Open(of, level);
	for (int i = 0; i < C.CountItem(); ++i)	C(i).Open(of, level);
}

//-----------------------------------------------------------------------------
void Z�HtmlItem::Close(FILE* of) {
	for (int i = C.CountItem() - 1; i >= 0; --i) C(i).Close(of, level);
	Z�HtmlTeg::Close(of, level);
}

//-----------------------------------------------------------------------------
void Z�HtmlItem::PrintText(FILE* of) {
	if (!txt.length()) return;
	Z�HtmlTeg::Tab(of, level + 1);
	fprintf(of, "%s\n\0", txt.c_str());
}

//-----------------------------------------------------------------------------
void Z�HtmlItem::Self(FILE* of) {
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
Z�HtmlItem& Z�HtmlItem::Move(Z�HtmlItem&& I) {
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
Z�HtmlItem& Z�HtmlItem::Copy(const Z�HtmlItem& I) {
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
