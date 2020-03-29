#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>
using namespace std;

class polynom
{
	private:
		void set_value(double* a, int n)
		{
			power = n;
			v = new double[n + 1];
			for (int i=0; i <= n; i++)
				v[i] = a[i];
		};
	
	public:
		double* v; //Вектор коэффициентов
		int power; // Степень многочлена
		string name = ""; // Имя многочлена (не обязательно)
		
		polynom(double* a, int n)
		{ // Конструктор из вектора коэффициентов и степени многочлена
			set_value(a, n);
		};
		
		polynom(int pow)
		{ // Конструктор создает многочлен степени pow с нулевыми коэффициентами
			v = new double[pow+1];
			power = pow;
			for (int i = 0; i < power; i++)
				v[i] = 0.0;
		};
		polynom(double* a, int n, string id)
		{ // Конструктор с присвоением многочлену имени
			set_value(a, n);
			name = id;
		};
		
		void show()
		{ // Вывод многочлена на экран, начиная с нулевой степени с учетом имени
			string signum;
			
			cout << "\t";
			if (name[0] != 0)
				cout << name << " = ";
				
			if (v[0] != 0)
			{
				cout << v[0];
				if (v[1] != 0)
				{
					if (v[1] < 0)
						signum = " - ";
					else
						signum = " + ";
					cout << signum << abs(v[1]) << "*x";
				};
			};
			
			for (int i = 2; i <= power; i++)
				if (v[i] != 0)
					{
					if (v[i] < 0)
						signum = " - ";
					else
						signum = " + ";
					cout << signum << abs(v[i]) << "*x^" << i;
					}
				else
					continue;
			cout << endl;
			//cout << "Степень многочлена равна " << power << endl;
		};
		
		polynom operator* (double alpha)
		{ // Умножаем полином на число. Обязательно число - второй множитель!
			polynom tmp(v, power, "tmp");
			for (int i = 0; i <= tmp.power + 1; i++)
				tmp.v[i] *= alpha;
			return tmp;
		};
		
		polynom pow_up(int up)
		{ // Домножаем многочлен arg на x^up
			polynom tmp(v, power + up);
			for (int i = 0; i < up; i++)
				tmp.v[i] = 0.0;
			for (int i = up; i <= power + up; i++)
				tmp.v[i] = v[i - up];
			//tmp.show(); getchar();
			return tmp;
		};
		
		polynom operator- (polynom subtrahend)
		{ // Вычитание многочленов
			int result_power, limit;
			double* result;
			
			if (power < subtrahend.power)
			{ // Если степень уменьшаемого < степени вычитаемого
				result_power = subtrahend.power;
				limit = power;
			}
			else
			{ // Если степень уменьшаемого >= степени вычитаемого
				result_power = power;
				limit = subtrahend.power;
			};
			// Создается массив под результат вычитания максимальной длины
			result = new double[result_power + 1];
			
			// Записываем в результат коэфф. при младших степенях уменьшаемого
			for (int i = 0; i <= limit; i++)
				result[i] = this->v[i];
			
			
			if (power < subtrahend.power)
				for (int i = limit + 1; i <= result_power; i++)
					result[i] = 0;
			else
				for (int i = limit + 1; i <= result_power; i++)
					result[i] = this->v[i];
			
			if (power < subtrahend.power)
				for (int i = 0; i <= result_power; i++)
					result[i] -= subtrahend.v[i];
			else
				for (int i = 0; i <= subtrahend.power; i++)
					result[i] -= subtrahend.v[i];
			// Пересчет степени результата с учетом нулевых коэффициентов при старших степенях
			while (result_power >= 0 && result[result_power] == 0)
				result_power--;
			// Создаем объект многочлена со скорректированной степенью и возвращаем его
			polynom res(result, result_power);
			delete result;
			return res;
		};
//----------------------------------------------------------------------
		polynom operator+ (polynom term)
		{ // Сложение многочленов
			int result_power, limit;
			double* result;
			
			if (power < term.power)
			{ // Если степень уменьшаемого < степени вычитаемого
				result_power = term.power;
				limit = power;
			}
			else
			{ // Если степень уменьшаемого >= степени вычитаемого
				result_power = power;
				limit = term.power;
			};
			// Создается массив под результат вычитания максимальной длины
			result = new double[result_power + 1];
			
			// Записываем в результат коэфф. при младших степенях уменьшаемого
			for (int i = 0; i <= limit; i++)
				result[i] = this->v[i];
			
			
			if (power < term.power)
				for (int i = limit + 1; i <= result_power; i++)
					result[i] = 0;
			else
				for (int i = limit + 1; i <= result_power; i++)
					result[i] = this->v[i];
			
			if (power < term.power)
				for (int i = 0; i <= result_power; i++)
					result[i] += term.v[i];
			else
				for (int i = 0; i <= term.power; i++)
					result[i] += term.v[i];
			// Пересчет степени результата с учетом нулевых коэффициентов при старших степенях
			while (result_power >= 0 && result[result_power] == 0)
				result_power--;
			// Создаем объект многочлена со скорректированной степенью и возвращаем его
			polynom res(result, result_power);
			delete result;
			return res;
		};
		
		polynom operator* (polynom factor)
		{ // Перемножение многочленов
			int i, j, k, m, n, i_start, i_finish;
			polynom* a;
			polynom* b;
			polynom c(power + factor.power);
			// с - многочлен - результат умножения
			// n - максимальная степень многочлена
			// m - минимальная степень многочлена
			// a - множитель с большей степенью
			// b - множитель с меньшей степенью
			// a = Pn(x) - многочлен степени n от x
			// b = Qm(x) - многочлен стtпени m от x
			if (power > factor.power)
			{
				n = power;
				m = factor.power;
				a = this;
				b = &factor;
			}
			else
			{
				n = factor.power;
				m = power;
				a = &factor;
				b = this;
			};
			
			for (k=0; k < n + m + 1; k++) 
			{
				i_start = 0;
				i_finish = k;
				if (k < m)
				{
					i_start = 0;
					i_finish = k;
				}
				else
					if (m <= k && k <= n)
					{
						i_start = 0;
						i_finish = m;
					}
					else
					{
						i_start = k-n;
						i_finish = m;
					}
				for (i = i_start; i <= i_finish; i++)
				{
					j = k-i;
					c.v[k] += a->v[j] * b->v[i];
					cout << "i = " << i << "\tj = " << j << endl;
				};
			};
			return c;
		};
		
		polynom operator% (polynom divider)
		{ // Деление многочленов. Аргумент функции - делитель
			int result_power; //Степень частного от деления
			result_power = this->power - divider.power; // Вычисляем степень частного
			polynom result(this->power); // Промежуточный результат
			polynom remaind(this->power); // Для хранения остатка от деления
			polynom quotient(result_power); // Частное от деления
			double a, b;
			
			result = divider;
			remaind = *this;
			
			for (int k = 0; k <= result_power; k++)
			{
				a = remaind.v[remaind.power]; // Коэффициент при старшей степени делимого
				b = divider.v[divider.power]; // Коэффициент при старшей степени делителя
				quotient.v[result_power - k] = a / b;
				if ( b != 0)
					result = divider * (a / b);
				else
					continue;
				result = result.pow_up(result_power - k);
				remaind = remaind - result;
			};
			//return quotient; // Возвращаем частное
			return remaind; // Возвращаем остаток
		};
			
		polynom operator/ (polynom divider)
		{ // Деление многочленов. Аргумент функции - делитель
			int result_power; //Степень частного от деления
			result_power = this->power - divider.power; // Вычисляем степень частного
			polynom result(this->power); // Промежуточный результат
			polynom remaind(this->power); // Для хранения остатка от деления
			polynom quotient(result_power); // Частное от деления
			double a, b;
			
			result = divider;
			remaind = *this;
			
			for (int k = 0; k <= result_power; k++)
			{
				a = remaind.v[remaind.power]; // Коэффициент при старшей степени делимого
				b = divider.v[divider.power]; // Коэффициент при старшей степени делителя
				quotient.v[result_power - k] = a / b;
				if ( b != 0)
					result = divider * (a / b);
				else
					continue;
				result = result.pow_up(result_power - k);
				remaind = remaind - result;
			};
			return quotient; // Возвращаем частное
			//return remaind; // Возвращаем остаток
		};

};

int main()
{
	// Более тщательно протестировать опреаторные функции / и %
	double a[] = {8.0, -4.0, 5.0, 0.0, 1.0, 3.0};
	double b[] = {-3.0, 2.0, 1.0, -2.0};
	double c[] = {12};
	double d[] = {5, 7};
	double e[] = {0, 0, 0, 5, 7, -1, -3};
	double f[] = {0, 0, 0, 0, 1, 1, 3};
	double g[] = {3, 0, -72, 12, 0, -1, 2};
	double h[] = {-1, 0, 2, 1};
	polynom test1(a, 5);
	polynom test2(b, 3);
	polynom test3(c, 0);
	polynom test4(d, 1);
	polynom test5(e, 6);
	polynom test6(f, 6);
	polynom test7(g, 6);
	polynom test8(h, 3);
	
	cout << "Деление многочленов\n\n";
	test7.show();
	cout << "Остаток от деления на" << endl;
	test8.show();
	cout <<"-------------------\n";
	//cout <<"Остаток равен  ";
	(test7 % test8).show();
	//(test2 * test1).show();
}
