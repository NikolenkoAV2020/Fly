//-----------------------------------------------------------------------------
// BMagicCalcU2.cpp: 
// �. ��������� 24.10.2018
//
//-----------------------------------------------------------------------------
#include <stdafx.h>

//-----------------------------------------------------------------------------
// ���������� Fly
#include <FlyMMath.h>		// �������������� �������

//-----------------------------------------------------------------------------
// ������ ���������� �� �������� "24" L,T + e,w
int BMagicCalcU24(double T0, double L0, double e0, double w0, double i0,
	double Tz, double Lz, double ez, double wz,
	int Vitz, ZSFlightRule& U, bool notCalcDV)
{
	// �������������� ��� ������ ����� � ��������, ������� � ���������
	// ������� � ��������. 
	// ����� ���������� ��������� � ���������� ������� ������������
	// � ��������, ���� �������� � ��/���.

	// ������� �� ��������������� � ��������� ������� 
	// � ������������ ������� ���������������
	double e10 = e0*Cos(w0, 1);
	double e20 = e0*Sin(w0, 1);
	double e1z = ez*Cos(wz, 1);
	double e2z = ez*Sin(wz, 1);

	// ����� ���������
	int Vit1 = U.vit[0];
	int Vit2 = U.vit[1];

	// �������� ��� ����������� ��������� ���� ���������
	double eps = 1e-14;
	// �������� ����������� ��������� ������
	double epsu = 1e-6;

	struct Sln {
		double u1, u2;
		double V1, V2;
	};
	Sln R[2];

	int  j;
	bool isOutSideRing = false;
	for (j = 0; j < 2; j++) {
		memset(&R[j], 0, sizeof(Sln));
		while (1){
			// ��������� ������ ���������� ���������
			double u1 = R[j].u1;
			double u2 = R[j].u2;

			// ������� ���������
			double b1 = Vitz - Vit1 - u1 / k_dwapi;
			double b2 = Vitz - Vit2 - u2 / k_dwapi + 0.5;

			// ������� ������ � ��������
			double Ts = 0.5*(T0 + Tz);
			// ������� ��������� �������� � ��
			double ps = pow(Ts*sqrt(k_mu_kms) / k_dwapi, 2.0 / 3.0);
			// ������� �������� � ��/���
			double Vs = sqrt(k_mu_kms / ps);
			double I2 = -1.082626716e-3;
			double r0 = 6378.136;
			// ������� ������� �������� �������� �����, ���/���.
			double Wt = 7.292115e-5;
			double z = 1.0 + I2*r0*k_mu_kms*Cos(i0, 1) / 3.0 / Wt / pow(ps, 4) / Vs;

			//-----------------------------------------------------------------
			// ���������� ������������� �������.
			// !!!! �������� ���� ������ ����� ���������� ������� ������� � 
			// ���, ��� ����������� ���� ���������� ������� ����� ���������� �
			// � ������������� ����������� �������� - ��/���. !!!!
			//-----------------------------------------------------------------

			// ������������� ������� ���������� ������� ���������������
			double de1 = Vs*0.5*(e1z - e10);
			double de2 = Vs*0.5*(e2z - e20);
			// ������������� ������� �� �������
			double dT = (Tz - T0)*k_mu_kms / 6.0 / k_pi / ps / ps;
			// ������������� ������� �� �������
			double dL = -(TO_RAD(Lz - L0)*k_mu_kms / 6.0 / k_pi / ps / ps / Wt
				+ de1 / 1.5 / k_pi) / z;

			double E = sqrt(de1*de1 + de2*de2);

			// � ������� ������ ����� ...
			if (fabs(dT) < eps && fabs(E) < eps){
				// ������� ������������ � ��� �������
				U.u0[0] = 180.0;
				U.u0[1] = 180.0;
				if (!notCalcDV) {
					U.dV[0] = dL / (b1 - b2);
					U.dV[1] = -U.dV[0];
				}
				return 0;
			}

			// � ������� ������ ����� ...
			if (fabs(2.0*dL - (b1 + b2)*dT) < eps && fabs(E) < eps){
				// ������� ������������ � ��� �������
				U.u0[0] = 90.0;
				U.u0[1] = 270.0;
				if (!notCalcDV) {
					U.dV[0] = dT * 0.5;
					U.dV[1] = U.dV[0];
				}
				return 0;
			}

			// � ����� ������...
			double V1 = notCalcDV ? U.dV[0] : (dL - b2*dT) / (b1 - b2);
			double V2 = notCalcDV ? U.dV[1] : dT - V1;
			double Vmin = fabs(fabs(V1) - fabs(V2));
			double Vmax = fabs(V1) + fabs(V2);
			double du1, du2;
			double su1, cu1, su2, cu2;

			// ���� "� �������� ������"
			if (Vmin <= E && E <= Vmax) {
				double a = V1*V1;
				double b = V2*V2;
				double z = 0.5*(E + (a - b) / E);
				double t = 0.5*(E - (a - b) / E);
				double x = sqrt((2 * (a + b) - E*E)*E*E -
					pow(a - b, 2.0)) / (2.0*E);

				// ���� ����� ����� ��� ��������� ������� ������� ������
				x = j ? -x : x;

				su1 = (de1*z + de2*x) / V1 / E;
				cu1 = (de2*z - de1*x) / V1 / E;
				su2 = (de1*t + de2*x) / V2 / E;
				cu2 = (de2*t - de1*x) / V2 / E;
			}
			else {
				isOutSideRing = true;
				// ���� "�� ��������� ������" �� ...

				// ������ ������� ������ �� ����������
				// ��� ��������� ���������� �������������� ������� E.
				double sa = de1 / E;
				double ca = de2 / E;
				double sV1 = Sign1(V1);
				double sV2 = Sign1(V1);

				// ���� "������� ������� ������" �� ...
				if (E > Vmax) {
					su1 = sV1*sa;
					cu1 = sV1*ca;
					su2 = sV2*sa;
					cu2 = sV2*ca;
				}
				// ���� "���������� ������� ������" �� ...
				else {
					double f = fabs(V1) > fabs(V2) ? 1 : -1;
					su1 = f*sV1*sa;
					cu1 = f*sV1*ca;
					su2 = -f*sV2*sa;
					cu2 = -f*sV2*ca;
				}
			}

			// ��������� ������ ���������� ���������
			u1 = Atan2(su1, cu1, 1);
			u2 = Atan2(su2, cu2, 1);

			// ���������� �������� ���������� ���������� ������ 
			// ���������� ��������� �� �������� �� ������� ��������.
			du1 = ValiDA(R[j].u1 - u1, 1);
			du2 = ValiDA(R[j].u2 - u2, 1);

			// ������� ������� ���������� ���������
			R[j].u1 = u1;
			R[j].u2 = u2;
			R[j].V1 = V1;
			R[j].V2 = V2;

			// ���� "��� ������" �� ������� ���� � ��� ������� 
			if (isOutSideRing) {
				j++;
				break;
			}

			// �������� ������������� ���������� ������������� �����
			if (fabs(du1) < epsu && fabs(du2) < epsu){
				// ������� ������� ... ���������� �����
				break;
			}

			// ��������� ������� � ������ ���������� ���������� ������ 
			// ���������� ��������� ... ������� � ����������� b1 � b2.
		}
	}

	if (isOutSideRing) j = 0;
	else {
		// ����� ������� ������� �� ���� ���������
		j = fabs(R[0].V1) + fabs(R[0].V2) > fabs(R[1].V1) + fabs(R[1].V2);
	}

	// ������������ ��������� �������� ����������
	U.n = 2;
	U.u0[0] = ValiA(R[j].u1, 1);
	U.u0[1] = ValiA(R[j].u2, 1);
	U.dV[0] = R[j].V1;
	U.dV[1] = R[j].V2;

	return 0;
}

//-----------------------------------------------------------------------------
// ��������� ����������� ������� F(x,y) ������� �������-���� (��������� MNM)
//-----------------------------------------------------------------------------
double F(double x, double y)
{
	return 1.0;
}

//-----------------------------------------------------------------------------
struct point
{
	double x;
	double y;
	double f;
};

//-----------------------------------------------------------------------------
double proverka(point p1, point p2, point p3)
{
	double max;
	max = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
	if (sqrt(pow(p2.x - p3.x, 2) + pow(p2.y - p3.y, 2)) > max)
		max = sqrt(pow(p2.x - p3.x, 2) + pow(p2.y - p3.y, 2));
	if (sqrt(pow(p3.x - p1.x, 2) + pow(p3.y - p1.y, 2)) > max)
		max = sqrt(pow(p3.x - p1.x, 2) + pow(p3.y - p1.y, 2));
	return max;
}

//-----------------------------------------------------------------------------
point MNM(double x, double y)
{
	double a = 1;
	double b = 0.5;
	double g = 2;
	double r = M_PI / 60;
	double d1, d2;
	point p, p1, p2, p3, ph, pg, pl, pc, pr, pe, ps;
	d1 = r / 2 / sqrt(2)*(sqrt(3) + 1);
	d2 = r / 2 / sqrt(2)*(sqrt(3) - 1);
	p1.x = x;
	p1.y = y;
	p1.f = F(x, y);
	p2.x = x + d1;
	p2.y = y + d2;
	p2.f = F(p2.x, p2.y);
	p3.x = x + d2;
	p3.y = y + d1;
	p3.f = F(p3.x, p3.y);    pg = p1;
	pl = p2;
	ph = p3;
	while (proverka(p1, p2, p3) > M_PI / 1800) {
		p1 = ph;
		p2 = pg;
		p3 = pl;
		if ((p1.f<p2.f) && (p2.f<p3.f)) { pl = p1; pg = p2; ph = p3; }
		if ((p1.f<p3.f) && (p3.f<p2.f)) { pl = p1; pg = p3; ph = p2; }
		if ((p2.f<p1.f) && (p1.f<p3.f)) { pl = p2; pg = p1; ph = p3; }
		if ((p2.f<p3.f) && (p3.f<p1.f)) { pl = p2; pg = p3; ph = p1; }
		if ((p3.f<p1.f) && (p1.f<p2.f)) { pl = p3; pg = p1; ph = p2; }
		if ((p3.f<p2.f) && (p2.f<p1.f)) { pl = p3; pg = p2; ph = p1; }
		pc.x = (pg.x + pl.x) / 2;
		pc.y = (pg.y + pl.y) / 2;
		pr.x = (1 + a) * pc.x - a * ph.x;
		pr.y = (1 + a) * pc.y - a * ph.y;
		pr.f = F(pr.x, pr.y);

		if (pr.f < pl.f) {
			pe.x = (1 - g) * pc.x + g * pr.x;
			pe.y = (1 - g) * pc.y + g * pr.y;

			pe.f = F(pe.x, pe.y);
			if (pe.f < pl.f)  ph = pe;  else  ph = pr;
		}
		else {
			if ((pl.f < pr.f) && (pr.f < pg.f))	{
				ph = pr;
			}
			else {
				if ((pg.f < pr.f) && (pr.f < ph.f))	{
					p = pr;
					pr = ph;
					ph = p;
				}
				ps.x = b * ph.x + (1 - b) * pc.x;
				ps.y = b * ph.y + (1 - b) * pc.y;
				ps.f = F(ps.x, ps.y);
				if (ps.f < ph.f)  ph = ps;
				else {
					pg.x = pl.x + (pg.x - pl.x) / 2;
					pg.y = pl.y + (pg.y - pl.y) / 2;
					pg.f = F(pg.x, pg.y);
					ph.x = pl.x + (ph.x - pl.x) / 2;
					ph.y = pl.y + (ph.y - pl.y) / 2;
					ph.f = F(ph.x, ph.y);
				}
			}
		}
	}
	return pl;
}

//-----------------------------------------------------------------------------
// �������� � ��������� 2-� ����������� ������ 
int BMagicCorrectionPulseU24(double Tr, double Lr, double i0,
	double Tz, double Lz,
	int Vitz, ZSFlightRule& U)
{
	// ����� ���������
	int Vit1 = U.vit[0];
	int Vit2 = U.vit[1];

	// ��������� ������ ���������� ���������
	double u1 = U.u0[0];
	double u2 = U.u0[1];

	// ������� ���������
	double b1 = Vitz - Vit1 - u1 / k_dwapi;
	double b2 = Vitz - Vit2 - u2 / k_dwapi + 0.5;

	// ������� ������ � ��������
	double Ts = 0.5*(Tr + Tz);
	// ������� ��������� �������� � ��
	double ps = pow(Ts*sqrt(k_mu_kms) / k_dwapi, 2.0 / 3.0);
	// ������� �������� � ��/���
	double Vs = sqrt(k_mu_kms / ps);
	double I2 = -1.082626716e-3;
	double r0 = 6378.136;
	// ������� ������� �������� �������� �����, ���/���.
	double Wt = 7.292115e-5;
	double z = 1.0 + I2*r0*k_mu_kms*Cos(i0, 1) / 3.0 / Wt / pow(ps, 4) / Vs;
	// ����������� �� ������� �� ��������
	double dTdV = 6.0*k_pi*ps*ps / k_mu_kms;
	// ����������� �� ������� �� ���������,
	// ����������� ���*���/�.
	double dLdV1 = -dTdV*Wt*(z*b1 + Sin(u1, 1) / 1.5 / k_pi);
	double dLdV2 = -dTdV*Wt*(z*b1 + Sin(u2, 1) / 1.5 / k_pi);

	// ����������� �������
	double dT = Tz - Tr;	// � ��������
	double dL = Lz - Lr;	// � ��������

	// �������� � ���������
	double ddV1 = (dTdV*TO_RAD(dL) - dLdV2*dT) / dTdV / (dLdV1 - dLdV2);
	double ddV2 = dT / dTdV - ddV1;

	// �������� �������� � �������� ���������
	U.dV[0] += ddV1;
	U.dV[1] += ddV1;

	return 0;
}

//-----------------------------------------------------------------------------