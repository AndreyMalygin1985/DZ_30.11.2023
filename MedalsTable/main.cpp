#include <iostream>
#include <cassert>
#include <string>

using namespace std;

// Реализуйте в классе MedalsTable из примера 18 метод пакета урока 
// 4ре возможность динамически задавать размер таблицы медалей.
// Текущая реализация для упрощения использует статический массив
// на 10 элементов.
// Замените его на динамически выделяемый массив.
// Для класса MedalsTable реализуйте семантику копирования 
// и семантику перемещения(две пары конструктор / оператор присваивания).

class MedalRow
{
	char country[4];
	int medals[3];
public:
	static const int GOLD{ 0 };
	static const int SILVER{ 1 };
	static const int BRONZE{ 2 };
	MedalRow(const char* countryP, const int* medalsP)
	{
		strcpy_s(country, 4, countryP ? countryP : "NON");
		for (int i{ 0 }; i < 3; ++i)
		{
			medals[i] = medalsP ? medalsP[i] : 0;
		}
	}
	MedalRow() : MedalRow(nullptr, nullptr) {}
	MedalRow& setCountry(const char* countryP)
	{
		if (countryP)
		{
			strcpy_s(country, 4, countryP);
		}
		return *this;
	}
	const char* getCountry()const { return country; }
	int& operator[](int idx)
	{
		assert((idx >= 0 and idx < 3) and "Index out "
			"of range!");
		return medals[idx];
	}
	int operator[](int idx)const
	{
		assert((idx >= 0 and idx < 3) and "Index out "
			"of range!");
		return medals[idx];
	}
	void print()const
	{
		cout << '[' << country << "]-( ";
		for (int i{ 0 }; i < 3; ++i)
		{
			cout << medals[i];
			if (i < 2) { cout << '\t'; }
		}
		cout << " )\n";
	}
};
class MedalsTable
{
	int size;
	int maxSize;
	MedalRow* medalRows;

	int findCountry(const char* country)const
	{
		for (int i{ 0 }; i < size; ++i)
		{
			if (strcmp(medalRows[i].getCountry(), country) == 0)
			{
				return i;
			}
		}
		return -1;
	}
public:
	
	MedalsTable() : size{ 0 }, maxSize{ 10 }, medalRows(new MedalRow[maxSize]) {}

	MedalsTable(const MedalsTable& other) : size{ other.size }, maxSize{ other.maxSize }, medalRows(new MedalRow[maxSize])
	{
		for (int i = 0; i < size; ++i)
		{
			medalRows[i] = other.medalRows[i];
		}
	}

	MedalsTable& operator=(const MedalsTable& other)
	{
		if (this != &other)
		{
			delete[] medalRows;
			size = other.size;
			maxSize = other.maxSize;
			medalRows = new MedalRow[maxSize];
			for (int i = 0; i < size; ++i)
			{
				medalRows[i] = other.medalRows[i];
			}
		}
		return *this;
	}

	MedalsTable(MedalsTable&& other) noexcept : size{ other.size }, maxSize{ other.maxSize }, medalRows{ other.medalRows }
	{
		other.size = 0;
		other.maxSize = 0;
		other.medalRows = nullptr;
	}
	MedalRow& operator[](const char* country)
	{
		int idx{ findCountry(country) };
		if (idx == -1)
		{
			assert(size < MedalsTable::maxSize and
				"Table is FULL!");
			idx = size++;
			medalRows[idx].setCountry(country);
		}
		return medalRows[idx];
	}
	const MedalRow& operator[](const char* country)const
	{
		int idx{ findCountry(country) };
		assert(idx != -1 and "Country not found on const "
			"table");
		return medalRows[idx];
	}
	void print()const
	{
		for (int i{ 0 }; i < size; ++i)
		{
			medalRows[i].print();
		}
	}

	MedalsTable& operator=(MedalsTable&& other) noexcept
	{
		if (this != &other)
		{
			delete[] medalRows;
			size = other.size;
			maxSize = other.maxSize;
			medalRows = other.medalRows;

			other.size = 0;
			other.maxSize = 0;
			other.medalRows = nullptr;
		}
		return *this;
	}

	~MedalsTable()
	{
		delete[] medalRows;
	}

	void resize(int newSize)
	{
		MedalRow* temp = new MedalRow[newSize];
		for (int i = 0; i < newSize && i < size; ++i)
		{
			temp[i] = medalRows[i];
		}
		delete[] medalRows;
		medalRows = temp;
		maxSize = newSize;
	}	
};

int main()
{
	MedalsTable mt1;
	cout << "Medals table #1:\n";
	mt1["UKR"][MedalRow::GOLD] = 1;
	mt1["UKR"][MedalRow::SILVER] = 2;
	mt1["HUN"][MedalRow::BRONZE] = 3;
	mt1["HUN"][MedalRow::GOLD] = 4;
	mt1["POL"][MedalRow::GOLD] = 5;
	mt1["POL"][MedalRow::SILVER] = 6;
	mt1.print();
	cout << "\nMedals table #2:\n";
	const MedalsTable mt2{ mt1 };
	mt2.print();
	
	return 0;
}