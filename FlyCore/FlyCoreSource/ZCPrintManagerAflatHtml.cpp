//-----------------------------------------------------------------------------
// ZCPrintManagerAflatHtml.cpp
// А. Николенко 03.08.2018
// Горизонтальное документирование результатов прогноза
//-----------------------------------------------------------------------------
#pragma once
#include <stdafx.h>

#include <FlyCoreSource\\ZMSpaceCraft.h>
#include <FlyCoreSource\\ZCPrintManager.h>
#include <FlyCoreSource\\ZCPrintFormat.h>

//-----------------------------------------------------------------------------
int ZCPrintManager::PrintAflatMSChtml(ZMSC* pKA, ZSC_SI* FS, ZSC_VI* FV)
{
	// Если не задан КА, не заданы параметры движения на время FS и не
	// заданы характеристики витка, то значит документировать не чего,
	// просто выход из функции
	if (!pKA && !FS && !FV) return 0;

	if (!HtmlCreator)
		// Если объект "писатель" не создан, то понятно что выводить
		// информацию в html-формате не возможно - завершаем процедулу,
		// не считая это ошибкой, так как ситуация не фатальная.
		return 0;

	int  N  = MPK.Size();
	int  NF = MPK.SizeF();
	int  i, j = -1, k = 0, l;
	char S[128];

	// Создание заголовка таблицы
	if (!MPF.IsFirstLine()) {
		ZSMarkItem im = HtmlCreator->GetItemsMark("PredRez");
		j = im[0];
	}

	if (MPF.IsFirstLine() || j == -1) {
		// Индекс нового элемента документа
		j = HtmlCreator->CountItems();
		(*HtmlCreator)(j).n = "br"; j++;
		(*HtmlCreator)(j).n = "br"; j++;
		// Собственно создание самой таблицы
		ZСHtmlItem& I = (*HtmlCreator)(j);
		I.n = "table";
		I[0] << "class\n predByVit";
		// Создаём маркер нового элемента - таблицы с параметрами движения
		HtmlCreator->CreateItemsMark("PredRez", ZSMarkItem(j));

		// Строка заголовка
		I(0).n = "tr";

		string Name1, Name2;
		// Печать имен, документируемых параметров движения
		for (i = 0; i < NF; i++) {
			// Если задано документирование компонент радиуса или вектора 
			// скорости, то размерность соответствующего параметра печатается 
			// трижды - по числу компонент вектора.
			int ID = MPF.GetItemID(this, i);
			if (ID == MODPR_T_V75 || ID == MODPR_T_V00 || ID == MODPR_T_VTE ||
				ID == MODPR_T_VTES || ID == MODPR_T_VGSC ||
				ID == MODPR_T_R75 || ID == MODPR_T_R00 || ID == MODPR_T_RTE ||
				ID == MODPR_T_RTES || ID == MODPR_T_RGSC) {
				ID = ID;
				switch (ID) {
				case MODPR_T_R75:
					for (l = 0; l < 3; l++) {
						MPF.GetIDName(this, Name1, MODPR_T_X75 + l, true);
						MPF.GetIDDim (this, Name1, MODPR_T_X75 + l, true);
						sprintf(S, "%s<br>%s\0", Name1.c_str(), Name2.c_str());
						I(0)(i + k).n = "th";
						I(0)(i + k).txt = S;
						if (l < 2) k++;
					}
					break;
				case MODPR_T_V75:
					for (l = 0; l < 3; l++) {
						MPF.GetIDName(this, Name1, MODPR_T_VX75 + l, true);
						MPF.GetIDDim (this, Name2, MODPR_T_VX75 + l, true);
						sprintf(S, "%s<br>%s\0", Name1.c_str(), Name2.c_str());
						I(0)(i + k).n = "th";
						I(0)(i + k).txt = S;
						if (l < 2) k++;
					}
					break;
				case MODPR_T_R00:
					for (l = 0; l < 3; l++) {
						MPF.GetIDName(this, Name1, MODPR_T_X00 + l, true);
						MPF.GetIDDim (this, Name2, MODPR_T_X00 + l, true);
						sprintf(S, "%s<br>%s\0", Name1.c_str(), Name2.c_str());
						I(0)(i + k).n = "th";
						I(0)(i + k).txt = S;
						if (l < 2) k++;
					}
					break;
				case MODPR_T_V00:
					for (l = 0; l < 3; l++) {
						MPF.GetIDName(this, Name1, MODPR_T_VX00 + l, true);
						MPF.GetIDDim (this, Name2, MODPR_T_VX00 + l, true);
						sprintf(S, "%s<br>%s\0", Name1.c_str(), Name2.c_str());
						I(0)(i + k).n = "th";
						I(0)(i + k).txt = S;
						if (l < 2) k++;
					}
					break;
				case MODPR_T_RTE:
					for (l = 0; l < 3; l++) {
						MPF.GetIDName(this, Name1, MODPR_T_XTE + l, true);
						MPF.GetIDDim (this, Name2, MODPR_T_XTE + l, true);
						sprintf(S, "%s<br>%s\0", Name1.c_str(), Name2.c_str());
						I(0)(i + k).n = "th";
						I(0)(i + k).txt = S;
						if (l < 2) k++;
					}
					break;
				case MODPR_T_VTE:
					for (l = 0; l < 3; l++) {
						MPF.GetIDName(this, Name1, MODPR_T_VXTE + l, true);
						MPF.GetIDDim (this, Name2, MODPR_T_VXTE + l, true);
						sprintf(S, "%s<br>%s\0", Name1.c_str(), Name2.c_str());
						I(0)(i + k).n = "th";
						I(0)(i + k).txt = S;
						if (l < 2) k++;
					}
					break;
				case MODPR_T_RTES:
					for (l = 0; l < 3; l++) {
						MPF.GetIDName(this, Name1, MODPR_T_XTES + l, true);
						MPF.GetIDDim (this, Name2, MODPR_T_XTES + l, true);
						sprintf(S, "%s<br>%s\0", Name1.c_str(), Name2.c_str());
						I(0)(i + k).n = "th";
						I(0)(i + k).txt = S;
						if (l < 2) k++;
					}
					break;
				case MODPR_T_VTES:
					for (l = 0; l < 3; l++) {
						MPF.GetIDName(this, Name1, MODPR_T_VXTES + l, true);
						MPF.GetIDDim (this, Name2, MODPR_T_VXTES + l, true);
						sprintf(S, "%s<br>%s\0", Name1.c_str(), Name2.c_str());
						I(0)(i + k).n = "th";
						I(0)(i + k).txt = S;
						if (l < 2) k++;
					}
					break;
				case MODPR_T_RGSC:
					for (l = 0; l < 3; l++) {
						MPF.GetIDName(this, Name1, MODPR_T_XGSC + l, true);
						MPF.GetIDDim (this, Name2, MODPR_T_XGSC + l, true);
						sprintf(S, "%s<br>%s\0", Name1.c_str(), Name2.c_str());
						I(0)(i + k).n = "th";
						I(0)(i + k).txt = S;
						if (l < 2) k++;
					}
					break;
				case MODPR_T_VGSC:
					for (l = 0; l < 3; l++) {
						MPF.GetIDName(this, Name1, MODPR_T_VXGSC + l, true);
						MPF.GetIDDim (this, Name1, MODPR_T_VXGSC + l, true);
						sprintf(S, "%s<br>%s\0", Name1.c_str(), Name2.c_str());
						I(0)(i + k).n = "th";
						I(0)(i + k).txt = S;
						if (l < 2) k++;
					}
					break;
				}
			}
			else {
				S[0] = '\0';
				MPF.GetItemName(this, Name1, i, true);
				MPF.GetItemDim (this, Name2, i, true);
				if (Name1.length()) sprintf(S, "%s\0", Name1.c_str());
				if (Name2.length()) sprintf(&S[strlen(S)], "<br>%s\0", Name2.c_str());
				else				sprintf(&S[strlen(S)], "<br>\0");
				I(0)(i + k).n = "th";
				I(0)(i + k).txt = S;
			}
		}

		// В случае, когда в документе впервые отображаются параметры
		// движения КА, как по виткам так и по времени, соответствующая
		// таблица результатов должна начинаться с заголовка.
		// О необходимости документирования заголовка свидетельствует
		// MPF.IsFirstLine() == true.
		// Поэтому после вызова функций документирования в текстовый файл и
		// в файл html необходимо проверить наличие данного флага и если он
		// равен true - отключить его так-как соответствующий заголовок 
		// в этот момент в файл будет уже выведен соответственно мотодами 
		// PrintApeakMSCtext в текстовый документ и PrintApeakMSChtml в
		// документ html.
		MPF.FirstLineOff();
		// Итак флаг, сигнализирующий о необходимости вывода заголовка 
		// таблицы отключён, при последующих вызовах настоящего метода
		// в созданную в документе таблицу выводятся только параметры
		// движения.
	}

	//-------------------------------------------------------------------------
	// Маркер таблицы с результатами прогнозирования 
	ZSMarkItem im = HtmlCreator->GetItemsMark("PredRez");
	j = im[0];
	// Колличество строк в таблице считая заголовок
	int countRows = (*HtmlCreator)(j).CountItems();
	// Ссылка на новую строку таблицы, одновременное создание этой строки
	ZСHtmlItem& I = (*HtmlCreator)(j)(countRows);
	I.n = "tr";

	//-------------------------------------------------------------------------
	//  Заполнение строки таблицы результатами
	//-------------------------------------------------------------------------

	// Если массив FS параметров движения "на время" не задан, то  
	// в качестве такового массива используем результаты моделирования 
	// движения, то есть массив FSR класса ZMSC. Массив FSR формируется 
	// как результат выполнения любого "Go..." метода модели движения, 
	// например GoToTime и так далее.
	if (!FS) FS = &pKA->FSR;
	// Если массив FV характеристик витка не задан, то в качестве такового 
	// выбирается массив данных модели движения pKA.
	if (!FV) FV = &pKA->FV;
	// Если выбран вариант документирования по виткам, то несмотря 
	// ни на что в качестве массива параметров движения "на время"
	// выбирается массив FSUN параметров движения в восходящем узле орбиты
	// из состава принятых характеристик витка FV.
	FS = (MPK.fromVit ? &FV->FSUN : FS);

	// Выбор минимальных и максимальных высот и радиусов на витке
	// из число локально минимальных и максимальных высот и 
	// радиусов на витке.
	// Напомним что вдль витка может наблюдаться от 1-й до 3-х
	// локально минимальных высот, но в большинстве случаев для  
	// околокруговых орбит - 2, для высокоэллептических - 1. 
	// Аналогичная ситуация имеет место быть для локально 
	// макимальной высоты ирадиусов локально минимального и 
	// локально максимального
	ZSC_VP* Hmin = HMIN_VI(*FV);
	ZSC_VP* Hmax = HMAX_VI(*FV);
	ZSC_VP* Rmin = RMIN_VI(*FV);
	ZSC_VP* Rmax = RMAX_VI(*FV);

	// Документирование заданного набора элементов движения MPK,
	// размерность набора N = MPK.Size();
	k = 0;
	for (i = 0; i<N; i++) {
		double *dr = nullptr;
		double *dv = nullptr;
		int     dID = 0;
		int K = MPK[i];
		switch (K) {
		case MODPRDATETIME:
						MPF.PrintDateTime(this, nullptr, S, FS->ta, 0); break;
		case MODPRDATE:	MPF.PrintDateTime(this, nullptr, S, FS->ta, 1); break;
		case MODPRTIME:	MPF.PrintDateTime(this, nullptr, S, FS->ta, 2); break;
		case MODPR_KA:		  MPF.PrintKA(this, nullptr, S, pKA->GetNumKA()); break;
		case MODPR_VIT:		 MPF.PrintVit(this, nullptr, S, FS->VitTE); break;
		case MODPR_M:		   MPF.PrintM(this, nullptr, S, FS->m); break;
		case MODPR_MSSV:	MPF.PrintMSSV(this, nullptr, S, FS->tmss); break;
		case MODPR_Tosk:	 MPF.PrintTdr(this, nullptr, S, FS->THBLFR[0]); break;
		case MODPR_R:		 MPF.PrintRkm(this, nullptr, S, FS->THBLFR[5]); break;
		case MODPR_H:		 MPF.PrintHkm(this, nullptr, S, FS->THBLFR[1]); break;
		case MODPR_B:		 MPF.PrintBLF(this, nullptr, S, FS->THBLFR[2]); break;
		case MODPR_L:		 MPF.PrintBLF(this, nullptr, S, FS->THBLFR[3]); break;
			break;
		case MODPR_Aosk:	 MPF.PrintAkm(this, nullptr, S, TO_KM(FS->OSK1975[0])); break;
		case MODPR_eosk:	   MPF.PrintE(this, nullptr, S, FS->OSK1975[1]); break;
		case MODPR_exosk:	   MPF.PrintE(this, nullptr, S, FS->OSKTE[1] * Cos(FS->OSKTE[4], 0)); break;
		case MODPR_eyosk:	   MPF.PrintE(this, nullptr, S, FS->OSKTE[1] * Sin(FS->OSKTE[4], 0)); break;
		case MODPR_V_Tdr:	 MPF.PrintTdr(this, nullptr, S, FV->Td); break;
		case MODPR_V_Hsr:	 MPF.PrintHkm(this, nullptr, S, FV->HAvr); break;
		case MODPR_V_DL:	 MPF.PrintBLF(this, nullptr, S, FV->dL); break;
		case MODPR_V_DLS:	 MPF.PrintBLF(this, nullptr, S, FV->dLs); break;
		case MODPR_V_KR:	  MPF.PrintKR(this, nullptr, S, FV->Ks); break;
			// Восходящий узел
		case MODPR_V_VUdatetime:
						MPF.PrintDateTime(this, nullptr, S, FV->FSUN.ta, 0); break;
		case MODPR_V_VUdate:
						MPF.PrintDateTime(this, nullptr, S, FV->FSUN.ta, 1); break;
		case MODPR_V_VUtime:
						MPF.PrintDateTime(this, nullptr, S, FV->FSUN.ta, 2); break;
		case MODPR_V_VUR:	   MPF.PrintR(this, nullptr, S, FV->FSUN.THBLFR[5]); break;
		case MODPR_V_VUH:	 MPF.PrintHkm(this, nullptr, S, FV->FSUN.THBLFR[1]); break;
		case MODPR_V_VUL:	 MPF.PrintBLF(this, nullptr, S, FV->FSUN.THBLFR[3]); break;
		case MODPR_V_VUmssv:MPF.PrintMSSV(this, nullptr, S, FV->FSUN.tmss); break;
			// Нисходящий узел
		case MODPR_V_NUdatetime:
						MPF.PrintDateTime(this, nullptr, S, FV->FSDN.ta, 0); break;
		case MODPR_V_NUdate:
						MPF.PrintDateTime(this, nullptr, S, FV->FSDN.ta, 1); break;
		case MODPR_V_NUtime:
						MPF.PrintDateTime(this, nullptr, S, FV->FSDN.ta, 2); break;
		case MODPR_V_NUR:	   MPF.PrintR(this, nullptr, S, FV->FSDN.THBLFR[5]); break;
		case MODPR_V_NUH:	 MPF.PrintHkm(this, nullptr, S, FV->FSDN.THBLFR[1]); break;
		case MODPR_V_NUL:	 MPF.PrintBLF(this, nullptr, S, FV->FSDN.THBLFR[3]); break;
		case MODPR_V_NUmssv:MPF.PrintMSSV(this, nullptr, S, FV->FSDN.tmss); break;
			// Минимальная высота
		case MODPR_V_Hmint:
						MPF.PrintDateTime(this, nullptr, S, Hmin->t, 2); break;
		case MODPR_V_HminU:	 MPF.PrintBLF(this, nullptr, S, Hmin->ute); break;
		case MODPR_V_Hmin:	 MPF.PrintHkm(this, nullptr, S, Hmin->H); break;
		case MODPR_V_HminB:	 MPF.PrintBLF(this, nullptr, S, Hmin->B); break;
		case MODPR_V_HminL:	 MPF.PrintBLF(this, nullptr, S, Hmin->L); break;
			// Максимальная высота
		case MODPR_V_Hmaxt:
			MPF.PrintDateTime(this, nullptr, S, Hmax->t, 2); break;
		case MODPR_V_HmaxU:	 MPF.PrintBLF(this, nullptr, S, Hmax->ute); break;
		case MODPR_V_Hmax:	 MPF.PrintHkm(this, nullptr, S, Hmax->H); break;
		case MODPR_V_HmaxB:	 MPF.PrintBLF(this, nullptr, S, Hmax->B); break;
		case MODPR_V_HmaxL:	 MPF.PrintBLF(this, nullptr, S, Hmax->L); break;
			// Минимальный радиус
		case MODPR_V_Rmint:
						MPF.PrintDateTime(this, nullptr, S, Rmin->t, 2); break;
		case MODPR_V_RminU:	 MPF.PrintBLF(this, nullptr, S, Rmin->ute); break;
		case MODPR_V_Rmin:	   MPF.PrintR(this, nullptr, S, Rmin->R);	break;
		case MODPR_V_RminB:	 MPF.PrintBLF(this, nullptr, S, Rmin->B); break;
		case MODPR_V_RminL:	 MPF.PrintBLF(this, nullptr, S, Rmin->L); break;
			// Максимальный радиус
		case MODPR_V_Rmaxt:
						MPF.PrintDateTime(this, nullptr, S, Rmax->t, 2); break;
		case MODPR_V_RmaxU:	 MPF.PrintBLF(this, nullptr, S, Rmax->ute); break;
		case MODPR_V_Rmax:	   MPF.PrintR(this, nullptr, S, Rmax->R);	break;
		case MODPR_V_RmaxB:	 MPF.PrintBLF(this, nullptr, S, Rmax->B); break;
		case MODPR_V_RmaxL:	 MPF.PrintBLF(this, nullptr, S, Rmax->L); break;
			// Радиус и скорость в разных эпохах
		case MODPR_T_R75:	dID = MODPR_T_X75;  dr = &FS->ASK1975[j]; break;
		case MODPR_T_V75:	dID = MODPR_T_VX75; dv = &FS->ASK1975[3 + j]; break;
		case MODPR_T_R00:	dID = MODPR_T_X00;  dr = &FS->ASK2000[j]; break;
		case MODPR_T_V00:	dID = MODPR_T_VX00; dv = &FS->ASK2000[3 + j]; break;
		case MODPR_T_RTE:	dID = MODPR_T_XTE;  dr = &FS->ASKTE[j]; break;
		case MODPR_T_VTE:	dID = MODPR_T_VXTE; dv = &FS->ASKTE[3 + j]; break;
		case MODPR_T_RTES:	dID = MODPR_T_XTES; dr = &FS->ASKTES[j]; break;
		case MODPR_T_VTES:	dID = MODPR_T_VXTES; dv = &FS->ASKTES[3 + j]; break;
		case MODPR_T_RGSC:	dID = MODPR_T_XGSC; dr = &FS->GSK[j]; break;
		case MODPR_T_VGSC:	dID = MODPR_T_VXGSC; dv = &FS->GSK[3 + j]; break;
			// Угловые оскулирующие элементы орбиты в разных эпохах
		case MODPR_T_i75:	MPF.PrintGrad(this, nullptr, S, FS->OSK1975[2]); break;
		case MODPR_T_W75:	MPF.PrintGrad(this, nullptr, S, FS->OSK1975[3]); break;
		case MODPR_T_w75:	MPF.PrintGrad(this, nullptr, S, FS->OSK1975[4]); break;
		case MODPR_T_u75:	MPF.PrintGrad(this, nullptr, S, FS->OSK1975[5]); break;
		case MODPR_T_i00:	MPF.PrintGrad(this, nullptr, S, FS->OSK2000[2]); break;
		case MODPR_T_W00:	MPF.PrintGrad(this, nullptr, S, FS->OSK2000[3]); break;
		case MODPR_T_w00:	MPF.PrintGrad(this, nullptr, S, FS->OSK2000[4]); break;
		case MODPR_T_u00:	MPF.PrintGrad(this, nullptr, S, FS->OSK2000[5]); break;
		case MODPR_T_iTE:	MPF.PrintGrad(this, nullptr, S, FS->OSKTE[2]); break;
		case MODPR_T_WTE:	MPF.PrintGrad(this, nullptr, S, FS->OSKTE[3]); break;
		case MODPR_T_wTE:	MPF.PrintGrad(this, nullptr, S, FS->OSKTE[4]); break;
		case MODPR_T_uTE:	MPF.PrintGrad(this, nullptr, S, FS->OSKTE[5]); break;
		case MODPR_T_iTES:	MPF.PrintGrad(this, nullptr, S, FS->OSKTES[2]); break;
		case MODPR_T_WTES:	MPF.PrintGrad(this, nullptr, S, FS->OSKTES[3]); break;
		case MODPR_T_wTES:	MPF.PrintGrad(this, nullptr, S, FS->OSKTES[4]); break;
		case MODPR_T_uTES:	MPF.PrintGrad(this, nullptr, S, FS->OSKTES[5]); break;
		default: assert(0);
		}

		if (dID) {
			for (j = 0; j < 3; j++) {
				if (dv)	MPF.PrintV(this, nullptr, S, TO_KMS(dv[j]));
				else if (dr) MPF.PrintR(this, nullptr, S, TO_KM(dr[j]));
				else assert(0);
				I(i + k).n = "td";
				I(i + k).txt = S;
				if (j < 2) k++;
			}
		} else {
			I(i + k).n = "td";
			I(i + k).txt = S;
	}	}

	// Число сформированных ячеек в строке
	k = i + k;

	// Документирование дополнительных "пользовательских" параметров MPK.UPP.
	// Размерность массива таких параметров NF=MPK.SizeF()
	for (i = N; i<NF; i++) {
		int j = i - N;
		int Len = MPK.UPP(j).len;	// Длинна параметра
		int Dec = MPK.UPP(j).dec;	// Точность параметра - число знаков после запятой
		switch (MPK.UPP(j).type) {
			// Целое число
			case UPPint:    sprintf(S, "%*d",    Len, (int)MPK.UPP(j).value); break;
			// Число с фиксированной точкой
			case UPPdouble: sprintf(S, "%*.*lf", Len, Dec, MPK.UPP(j).value); break;
			// Число с плавающей точкой
			case UPPexp:    sprintf(S, "%*.*e",  Len, Dec, MPK.UPP(j).value); break;
			default:;
		}
		I(j + k).n = "td";
		I(j + k).txt = S;
	}

	return 0;
}

//-----------------------------------------------------------------------------