// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include "cmath"
#include <string>

TBitField::TBitField(int len): BitLen(len)
{
	if (len < 1) throw len;
	len % 32 == 0 ? MemLen = len / 32 : MemLen = len / 32 + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
	pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / 32;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n % 32);//индекс бита в элементе
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0 || n >= BitLen) throw n;
	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || n >= BitLen) throw n;
	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] & (~(GetMemMask(n)));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || n >= BitLen) throw n;
	return (pMem[GetMemIndex(n)] >> ( n % 32 )) & 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this != &bf)
	{
		if ( MemLen != bf.MemLen)
		{
			delete[] pMem;
			MemLen = bf.MemLen;
			pMem = new TELEM[MemLen];
		}
		BitLen = bf.BitLen;
		for (int i = 0; i < MemLen; ++i)
			pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen == bf.BitLen)
	{
		for (int i = 0; i < MemLen; i++)
		{
			if (pMem[i] != bf.pMem[i])
				return 0;
		}
		return 1;
	}
  return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	if (BitLen > bf.BitLen)
	{
		TBitField res(BitLen);
		int r = MemLen - bf.MemLen;

		for (int i = 0; i < r ; i++)
		{
			res.pMem[i] = pMem[i];
		}
		for (int i = r ; i < MemLen; i++)
		{
			res.pMem[i] = pMem[i] | bf.pMem[i - r];
		}
		return res;
	}
	else
	{
		TBitField res(bf.BitLen);
		int r = bf.MemLen - MemLen;

		for (int i = 0; i < r; i++)
		{
			res.pMem[i] = bf.pMem[i];
		}
		for (int i = r; i < MemLen; i++)
		{
			res.pMem[i] = bf.pMem[i] | pMem[i - r];
		}
		return res;
	}
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	if (BitLen > bf.BitLen)
	{
		TBitField res(BitLen);
		int r = MemLen - bf.MemLen;

		for (int i = 0; i < r; i++)
		{
			res.pMem[i] = 0;
		}
		for (int i = r; i < MemLen; i++)
		{
			res.pMem[i] = pMem[i] & bf.pMem[i - r];
		}
		return res;
	}
	else
	{
		TBitField res(bf.BitLen);
		int r = bf.MemLen - MemLen;

		for (int i = 0; i < r; i++)
		{
			res.pMem[i] = 0;
		}
		for (int i = r; i < MemLen; i++)
		{
			res.pMem[i] = bf.pMem[i] & pMem[i - r];
		}
		return res;
	}
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(BitLen);
	for (int i = 0; i < BitLen; i++)
	{
		if (GetBit(i) == 0)
		{
			res.SetBit(i);
		}
	}
	return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	string bits;
	istr >> bits;
	TBitField bf1(bits.length());
	for (int i = 0; i < bits.length(); i++)
	{
		if (bits[i] == '1')
			bf1.SetBit(i);
	}
	bf = bf1;
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; ++i)
	{
		ostr << bf.GetBit(i);
	}
	ostr << std::endl;
	return ostr;
}
