#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h> // Для получения числа Пи через константу M_PI
#define EARTH_RADIUS 6372795 // радиус Земли в метрах

using namespace std;

// Шаблонный класс Вектор
template<class T, size_t _dimension>
class Vector {
private:
	T _data[_dimension]; // Создание статического массива заданной размерности и типа

public:
	// Конструктор, использущий initializer_list
	// Нужен для инициализации вектора как массива с помощью скобок {} 
	Vector(const std::initializer_list<T>& list)
	{
		size_t i = 0;
		for (auto& element : list) {
			_data[i++] = element;
		}
	}
	// Перегрузка оператора присваивания с тем же initializer_list
	// для того присваивать значения через {}
	Vector& operator= (const std::initializer_list<T>& list) {
		size_t i = 0;
		for (auto& element : list) {
			_data[i++] = element;
		}

		return *this;
	}

	// Возвращает длину вектора
	// Формула длины вектора = Корень из суммы квадратов координат вектора
	double length() const {
		T sum = 0;
		for (const T& coor : _data) {
			sum += coor * coor;
		}

		return sqrt(sum);
	}

	// Возвращает размерность
	size_t dimension() const { return _dimension; }

	// Возвращает длину между векторами
	// Формула: корень из суммы квадратов разностей координат векторов
	double distanceBetween(Vector& other) const {
		// Вывод ошибки если вектора разной размерности и выход
		if (_dimension != other.dimension()) {
			std::cout << "ERROR: the vectors are in different dimensions...\n";
			exit(1);
		}
		// Считаем сумму квадратов разностей
		T sum = 0;
		for (int i = 0; i < _dimension; i++) {
			sum += (other[i] - _data[i]) * (other[i] - _data[i]);
		}

		return sqrt(sum);
	}

	// Метод доступа к элементу массива (возращает копию данных, т.е. их нельзя изменить)
	T  at(int index) const { return _data[index]; }
	// перегрузка оператора доступа к элементу массива (возвращает по ссылке, данные можно изменить)
	T& operator[](int index) { return _data[index]; }

	// Перегрузка оператора вывода - печатает данные в виде "[a, b, c]"
	friend std::ostream& operator<< (std::ostream& out, const Vector& vec) {
		out << "[";
		for (size_t i = 0; i < vec.dimension() - 1; i++) {
			out << vec.at(i) << ", ";
		}
		out << vec.at(vec.dimension() - 1) << "]";

		return out;
	}
};

// Класс Угол
class Angle {
private:
	// значение (в градусах)
	double _value;

public:
	// Конструктор. Принимает данные в градусах
	Angle(double value_in_degrees = 0) : _value(value_in_degrees)
	{
	}

	// Методы получения данных в градусах (deg) и радианах (rad) 
	double deg() const { return _value; }
	double rad() const { return _value * M_PI / 180; } // Перевод из градусов в радианы
};

// Специализация шаблона вектора, которые состоит из Углов и размерности 2
template<>
class Vector<Angle, 2> {
private:
	Angle _data[2];
	// _data[0] - угол широты в градусах
	// _data[1] - угол долготы в градусах

public:
	// Метод принимает широту и долготу в градусах
	Vector(double deg_latitude = 0, double deg_longitude = 0)
	{
		_data[0] = deg_latitude;
		_data[1] = deg_longitude;
	}

	// Расстояние между векторами
	double distance(Vector<Angle, 2>& other) const {
		// Широты в радианах (фи1 и фи2 из формулы)
		double rad_lat1 = this->latitude().rad();
		double rad_lat2 = other.latitude().rad();

		// Разница по долготе в радианах (дельта лямбда из формулы)
		double delta_long = abs(this->longitude().rad() - other.longitude().rad());

		// Возращаем угловую разницу, умноженную на радиус Земли
		return EARTH_RADIUS * atan(
			sqrt(
				(cos(rad_lat2) * sin(delta_long)) * (cos(rad_lat2) * sin(delta_long)) +
				(cos(rad_lat1) * sin(rad_lat1) - sin(rad_lat1) * cos(rad_lat2) * cos(delta_long)) * (cos(rad_lat1) * sin(rad_lat1) - sin(rad_lat1) * cos(rad_lat2) * cos(delta_long))
			)
			/
			(sin(rad_lat1) * sin(rad_lat2) + cos(rad_lat1) * cos(rad_lat2) * cos(delta_long))
		);
	}

	// Быстрый доступ к широте и долготе 
	Angle latitude()  const { return _data[0]; }
	Angle longitude() const { return _data[1]; }

	// Перегузка оператора вывода
	friend std::ostream& operator<< (std::ostream& out, const Vector<Angle, 2>& vec) {
		out << "[" << vec.latitude().deg() << ", " << vec.longitude().deg() << "]";

		return out;
	}
};
// используем N_Vector в качестве псевдонима 
using N_Vector = Vector<Angle, 2>;


int main() {

	double x, y; // Создание переменных для ввода координат векторов
	/* TODO: Сделать ввод координат красивее.Может быть через метод.А то выглядит ужасно. */
	cout << "Enter coordinates of the first N-vector\n"; //Запрос на ввод координат первого n-вектора
	cout << "Enter X: ";
	cin >> x;
	cout << "Enter Y: ";
	cin >> y;
	N_Vector nvec1(x, y);//Создание n-вектора

	cout << "Enter coordinates of the second N-vector\n"; //Запрос на ввод координат первого n-вектора
	cout << "Enter X: ";
	cin >> x;
	cout << "Enter Y: ";
	cin >> y;
	N_Vector nvec2(x, y); //СОздание n-вектора
	
	// выводим расстояние между ними
	std::cout << "Distance between two N-vectors: " << nvec1.distance(nvec2) << "\n";

	return 0;
}