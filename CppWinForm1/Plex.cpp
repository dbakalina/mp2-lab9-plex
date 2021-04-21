#include "Plex.h"

void TChart::Show(Graphics^ gr, Pen^ pen)
{
	TLine CurrLine;
	TRoot* pR;
	TPoint* pP;
	CurrLine.pChart = this;
	CurrLine.pFp = NULL;
	CurrLine.pLp = NULL;
	St.Clear();
	St.Push(CurrLine);
	while (!St.Empty())
	{
		CurrLine = St.Pop();
		while (CurrLine.pFp == NULL)
		{
			pR = CurrLine.pChart->GetFirst();
			pP = dynamic_cast<TPoint*>(pR);
			if (pP != NULL)
				CurrLine.pFp = pP;
			else
			{
				St.Push(CurrLine);
				CurrLine.pChart = dynamic_cast<TChart*>(pR);
			}
		}
		if (CurrLine.pLp == NULL)
		{
			pR = CurrLine.pChart->GetLast();
			pP = dynamic_cast<TPoint*>(pR);
			if (pP != NULL)
				CurrLine.pLp = pP;
			else
			{
				St.Push(CurrLine);
				CurrLine.pChart = dynamic_cast<TChart*>(pR);
				CurrLine.pFp = NULL;
				St.Push(CurrLine);
			}
		}
		CurrLine.pChart->SetVisible(true);
		if (CurrLine.pFp != NULL && CurrLine.pLp != NULL)
		{
			if (CurrLine.pChart->GetVisible())
			{
				gr->DrawLine(pen, CurrLine.pFp->GetX(), CurrLine.pFp->GetY(), CurrLine.pLp->GetX(), CurrLine.pLp->GetY());
				CurrLine.pFp->Show(gr, pen);
				CurrLine.pLp->Show(gr, pen);
			}
			pP = CurrLine.pLp;
			if (!St.Empty())
			{
				CurrLine = St.Pop();
				if (CurrLine.pFp == NULL)
					CurrLine.pFp = pP;
				else
					CurrLine.pLp = pP;
				St.Push(CurrLine);
			}
		}
	}
}

void TChart::Hide(Graphics^ gr, Pen^ pen)
{
	TLine CurrLine;
	TRoot* pR;
	TPoint* pP;
	CurrLine.pChart = this;
	CurrLine.pFp = NULL;
	CurrLine.pLp = NULL;
	St.Clear();
	St.Push(CurrLine);

	while (!St.Empty())
	{
		CurrLine = St.Pop();
		while (CurrLine.pFp == NULL)
		{
			pR = CurrLine.pChart->GetFirst();
			pP = dynamic_cast<TPoint*>(pR);
			if (pP != NULL)
				CurrLine.pFp = pP;
			else
			{
				St.Push(CurrLine);
				CurrLine.pChart = dynamic_cast<TChart*>(pR);
			}
		}
		if (CurrLine.pLp == NULL)
		{
			pR = CurrLine.pChart->GetLast();
			pP = dynamic_cast<TPoint*>(pR);
			if (pP != NULL)
				CurrLine.pLp = pP;
			else
			{
				St.Push(CurrLine);
				CurrLine.pChart = dynamic_cast<TChart*>(pR);
				CurrLine.pFp = NULL;
				St.Push(CurrLine);
			}
		}

		if (CurrLine.pFp != NULL && CurrLine.pLp != NULL)
		{
			gr->DrawLine(pen, CurrLine.pFp->GetX(), CurrLine.pFp->GetY(), CurrLine.pLp->GetX(), CurrLine.pLp->GetY());
			CurrLine.pFp->Show(gr, pen);
			CurrLine.pLp->Show(gr, pen);

			pP = CurrLine.pLp;
			if (!St.Empty())
			{
				CurrLine = St.Pop();
				if (CurrLine.pFp == NULL)
					CurrLine.pFp = pP;
				else
					CurrLine.pLp = pP;
				St.Push(CurrLine);
			}
		}
	}
}

TPoint* TChart::SetCurrPoint(Graphics^ gr, Pen^ pen, int _x, int _y)
{
	TPoint* PointActive = new TPoint;;
	TLine CurrLine;
	TRoot* pR;
	TPoint* pP;
	CurrLine.pChart = this;
	CurrLine.pFp = CurrLine.pLp = NULL;

	St.Clear();
	St.Push(CurrLine);
	PointActive->SetX(-1); PointActive->SetY(-1);

	while (!St.Empty())
	{
		CurrLine = St.Pop();
		while (CurrLine.pFp == NULL)
		{
			pR = CurrLine.pChart->GetFirst();
			pP = dynamic_cast<TPoint*>(pR);
			if (pP != NULL)
			{
				CurrLine.pFp = pP;
				CurrLine.pFp->SetActive(false);
				CurrLine.pFp->Show(gr, pen);

				if (abs(_x - CurrLine.pFp->GetX()) < 10 && abs(_y - CurrLine.pFp->GetY()) < 10)
				{
					CurrLine.pFp->SetActive(true);
					PointActive = CurrLine.pFp;
				}
			}
			else
			{
				St.Push(CurrLine);
				CurrLine.pChart = dynamic_cast<TChart*>(pR);
			}
		}
		if (CurrLine.pLp == NULL)
		{
			pR = CurrLine.pChart->GetLast();
			pP = dynamic_cast<TPoint*>(pR);

			if (pP != NULL)
			{
				CurrLine.pLp = pP;
				CurrLine.pLp->SetActive(false);
				CurrLine.pLp->Show(gr, pen);

				if (abs(_x - CurrLine.pLp->GetX()) < 3 && abs(_y - CurrLine.pLp->GetY()) < 3)
				{
					CurrLine.pLp->SetActive(true);
					PointActive = CurrLine.pLp;
				}
			}
			else
			{
				St.Push(CurrLine);
				CurrLine.pChart = dynamic_cast<TChart*>(pR);
				CurrLine.pFp = NULL;
				St.Push(CurrLine);
			}
		}
		if (CurrLine.pFp != NULL && CurrLine.pLp != NULL)
		{
			pP = CurrLine.pLp;

			if (!St.Empty())
			{
				CurrLine = St.Pop();
				if (CurrLine.pFp == NULL)
					CurrLine.pFp = pP;
				else
					CurrLine.pLp = pP;

				St.Push(CurrLine);
			}
		}
	}

	PointActive->Show(gr, pen);
	return PointActive;
}

void TChart::AddLine(System::Drawing::Graphics^ gr, System::Drawing::Pen^ pen, TPoint* PointActive, TChart* NewLine)
{
	TLine CurrLine;
	TRoot* pR;
	TPoint* pP;
	CurrLine.pChart = this;
	CurrLine.pFp = NULL;
	CurrLine.pLp = NULL;

	St.Clear();
	St.Push(CurrLine);

	while (!St.Empty())
	{
		CurrLine = St.Pop();
		while (CurrLine.pFp == NULL)
		{
			pR = CurrLine.pChart->GetFirst();
			pP = dynamic_cast<TPoint*>(pR);

			if (pP != NULL)
			{
				CurrLine.pFp = pP;
				if (abs(PointActive->GetX() - CurrLine.pFp->GetX()) < 3 && abs(PointActive->GetY() - CurrLine.pFp->GetY()) < 3)
				{
					CurrLine.pChart->SetFirst(NewLine);
					return;
				}
			}
			else
			{
				St.Push(CurrLine);
				CurrLine.pChart = dynamic_cast<TChart*>(pR);
			}
		}
		if (CurrLine.pLp == NULL)
		{
			pR = CurrLine.pChart->GetLast();
			pP = dynamic_cast<TPoint*>(pR);

			if (pP != NULL)
			{
				CurrLine.pLp = pP;
				if (abs(PointActive->GetX() - CurrLine.pLp->GetX()) < 3 && abs(PointActive->GetY() - CurrLine.pLp->GetY()) < 3)
				{
					CurrLine.pChart->SetLast(NewLine);
					return;
				}
			}
			else
			{
				St.Push(CurrLine);
				CurrLine.pChart = dynamic_cast<TChart*>(pR);
				CurrLine.pFp = NULL;
				St.Push(CurrLine);
			}
		}
		if (CurrLine.pFp != NULL && CurrLine.pLp != NULL)
		{
			pP = CurrLine.pLp;

			if (!St.Empty())
			{
				CurrLine = St.Pop();

				if (CurrLine.pFp == NULL)
					CurrLine.pFp = pP;
				else
					CurrLine.pLp = pP;

				St.Push(CurrLine);
			}
		}
	}
}