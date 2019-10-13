//-------------------------------------------------------------------------------
// FlightThisAndThat.h
// �. ��������� 15.02.2019
//-------------------------------------------------------------------------------
#ifndef FLIGHT_THIS_AND_THAT_H
#define FLIGHT_THIS_AND_THAT_H

//-------------------------------------------------------------------------------
// ���������� ��������� ������� ��� ������� ���� ���������� - ��������� ������ �
// ����� �����.
//		NU	-	�� �� ������� ����� ��������������
//		LSF	-`	����� ��� � ������� ����������� �������
//		dVit-	�������� ������ ��������������� �� ������� ������������� �������
//
int FlightCalcHightContour(ZNU& NU, ZLSF& LSF, int dVit, 
						   char* FilePrintSticker = nullptr);

//-------------------------------------------------------------------------------
// ��������������� ��������� ������������ � ����������� ����� �� �����
// �� �������� ��������� ������
//		NU		- �� �� ������� ����� ��������������
//		LSF		- ����� ��� � ������� ����������� �������
//		maxTime	- �������� ������������� � ������
//		SkipVit	- ���������� ������ ����� ������� �� ������� ��������������� 
//				  ������������� ������
// 
int FlightCalcHightSimplified(ZNU& NU, ZLSF& LSF, 
							  char* FilePrintSticker = nullptr,
							  double maxTime=366, int SkipVit=15) ;

//-------------------------------------------------------------------------------
// ������� �������
//		NU	-	�� �� ������� ����� ��������������
//		LSF	-`	����� ��� � ������� ����������� �������
//		dt	-	�������� ��������������� (7 ����� ��������)
//		dL, dLs, Hmin, ... - ����������� ����������, �������� ����� �����,
//				���. ������, ... �� ����� ��
//	
int FlightCalcPrediction(ZNU& NU, ZLSF& LSF, 
						 double dt = 7, char* FilePrintSticker = nullptr,
						 double* dL = nullptr, double* dLs = nullptr, 
						 double* Hmin = nullptr, double* Hmax = nullptr, 
						 double* Hs = nullptr);

//-------------------------------------------------------------------------------
#endif // #define FLIGHT_THIS_AND_THAT_H