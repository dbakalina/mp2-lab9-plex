#pragma once
#include <string>
#include <iostream>
#include "Stack.h"

using namespace System;
using namespace System::Drawing;

class TChart;
class TPoint;
struct TLine 
{
	TChart* pChart;
	TPoint* pFp, * pLp;
};

class TRoot
{
protected:
	bool Visible;
	bool Active;
	bool sm;
public:
	TRoot()
	{
		Visible = true;
		Active = false;
		sm = false;
	};
	bool IsVisible()
	{
		return Visible;
	};
	bool IsActive()
	{
		return Active;
	};
	void SetActive(bool _Active = true)
	{
		Active = _Active;
	};
	void SetVisible(bool _Visible = true)
	{
		Visible = _Visible;
	};
	bool GetActive()
	{
		return Active;
	};
	bool GetVisible()
	{
		return Visible;
	};
	bool Get()
	{
		return sm;
	}
	void Set(bool _sm)
	{
		sm = _sm;
	}
	virtual void Show(Graphics^ gr, Pen^ pen) = 0;
	virtual void Hide(Graphics^ gr, Pen^ pen) = 0;
};

class TPoint : public TRoot
{
protected:
	int x, y;
public:
	TPoint(int _x = 0, int _y = 0)
	{
		x = _x; y = _y;
	}
	virtual void Show(Graphics^ gr, Pen^ pen)
	{
		gr->DrawEllipse(pen, (int)(x - 6), (int)(y - 6), 9, 9);
		Visible = true;
	}
	virtual void Hide(Graphics^ gr, Pen^ pen)
	{
		gr->DrawEllipse(Pens::White, (int)(x - 6), int(y - 6), 9, 9);
		Visible = false;
	}
	int GetX()
	{
		return x - 2 > 0 ? x - 2 : 0;
	}
	int GetY()
	{
		return y - 2 > 0 ? y - 2 : 0;
	}
	void SetX(int _x) { x = _x; }
	void SetY(int _y) { y = _y; }
};

class TChart : public TRoot
{
protected:
	TRoot* pFirst;
	TRoot* pLast;
	Stack <TLine> St;
public:
	TChart(TRoot* _pFirst = NULL, TRoot* _pLast = NULL)
	{
		pFirst = _pFirst; pLast = _pLast; this->Visible = true;
	}
	void SetFirst(TRoot* pf)
	{
		TPoint* p = dynamic_cast<TPoint*>(pf);
		TChart* c = dynamic_cast<TChart*>(pf);
		if (!p || !c)
			pFirst = pf;
	}
	void SetLast(TRoot* pf)
	{
		TPoint* p = dynamic_cast<TPoint*>(pf);
		TChart* c = dynamic_cast<TChart*>(pf);
		if (!p || !c)
			pLast = pf;
	}
	TRoot* GetFirst()
	{
		return pFirst;
	}
	TRoot* GetLast()
	{
		return pLast;
	}
	TRoot* Show(Graphics^ gr, Pen^ pen, TRoot* p)
	{
		TPoint* pF, * pL;
		TChart* pNode;
		if (dynamic_cast<TPoint*>(p))
			return p;
		else
		{
			pNode = dynamic_cast<TChart*>(p);
			pF = dynamic_cast<TPoint*>(Show(gr, pen, pNode->pFirst));
			pL = dynamic_cast<TPoint*>(Show(gr, pen, pNode->pLast));
			gr->DrawLine(pen, (int)(pF->GetX()), (int)(pF->GetY()), (int)(pL->GetX()), (int)
				(pL->GetY()));
			return pL;
		}
	}
	void Show(Graphics^ gr, Pen^ pen);
	void Hide(Graphics^ gr, Pen^ pen);
	TPoint* SetCurrPoint(Graphics^ gr, Pen^ pen, int _x, int _y);
	void AddLine(Graphics^ gr, Pen^ pen, TPoint* PointActive, TChart* NewLine);
	virtual void MoveTo(int X, int Y)
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
				if (!CurrLine.pFp->Get())
				{
					CurrLine.pFp->Set(true);
					CurrLine.pFp->SetX(CurrLine.pFp->GetX() + X);
					CurrLine.pFp->SetY(CurrLine.pFp->GetY() + Y);
				}
				if (!CurrLine.pLp->Get())
				{
					CurrLine.pLp->Set(true);
					CurrLine.pLp->SetX(CurrLine.pLp->GetX() + X);
					CurrLine.pLp->SetY(CurrLine.pLp->GetY() + Y);
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

	virtual void Reset()
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
				CurrLine.pFp->Set(false);
				CurrLine.pLp->Set(false);

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
};

