#pragma once
#include <iostream>
#include <string>
#include <exception>
using namespace std;

//исключение выход за приделы массива
class Bad_range : public exception
{
public:
    Bad_range() = default;
    ~Bad_range() = default;
    const char* what() const noexcept override{
        return "Index out of range"; //Выход за пределы массива - пишу на английском чтобы избежать проблем с кодировками 
        //хотя кодировка Windows-1251 может использовать русские симолы в 1 байт -> 0-127 - соответствует ASCII -> 128-255 - содержит кириллицу
    }
};

//исключение не верное значение размера массива
class Bad_length : public exception
{
public:
    Bad_length() = default;
    ~Bad_length() = default;
    const char* what() const noexcept override{
        return "Invalid container length"; //Не верное значение размера массива- пишу на английском чтобы избежать проблем с кодировками 
        //хотя кодировка Windows-1251 может использовать русские симолы в 1 байт -> 0-127 - соответствует ASCII -> 128-255 - содержит кириллицу
    }
};

template <typename T> class Array
{
private:
    size_t _size;       //длинна массива
    T* _massRef;      //Указатель на массив
public:
    //Конструкторы и деструкторы
    //IntegerArray() = default; делаю обязательным указание длинны при создании
    explicit Array(size_t size){
        if (size == 0 || size > 1000) {  //ограничиваю размер 0 и 1000 элементами 4000 байт (примерно 3.91 КБ)
            throw Bad_length(); // Генерация исключения
        }
        _massRef = new T [size];
        _size = size;
    } //explicit - избежать случайных неявных преобразований числа
    /*
    Зачам? Без explicit могут возникуть проблемы с неяным преобразованием передаваемого значения в объект IntegerArray
    при помощи explicit я прошу компилятор использовать только явное преобразование для безопасности использования IntegerArray
    */
   
    ~Array(){
        delete [] _massRef;
        cout << "IntegerArray - Удален" << endl;
    }

    Array(const Array& other){
        _massRef = new T [other._size];
        _size = other._size;
        for (size_t i = 0; i < other._size; i++)
        {
            _massRef[i] = other._massRef[i];
        }
    }

    //Операторы
    Array& operator=(const Array& other){
            // Проверка на самоприсваивание (other == *this) arr = arr
        if (this != &other) {
            //Удаляем старые данные
            delete[] _massRef;
            //Копируем размер
            _size = other._size;
            //Выделяем новую память
            _massRef = new T[_size];

            //Копируем элементы
            for (size_t i = 0; i < _size; i++) {
                _massRef[i] = other._massRef[i];
            }
        }

        _massRef = new T [other._size];
        _size = other._size;
        for (size_t i = 0; i < other._size; i++)
        {
            _massRef[i] = other._massRef[i];
        }
        return *this;
    }

    T& operator[](size_t index){
        if (index >= _size) { //>= тк индекс начинается с 0, а размер с 1
            throw Bad_range(); // Генерация исключения
        }
        return _massRef[index];
    }                          // может бросить bad_range
    /*
    Использование:abortIntegerArray arr(5);
    arr[2] = 42;  // Запись
    int x = arr[3]; // Чтение
    */

    //Методы
    //получить доступ к любому элементу контейнера по индексу
    T getNumberByIndex(size_t index) const{
        if (index >= _size) { //>= тк индекс начинается с 0, а размер с 1
            throw Bad_range(); // Генерация исключения
        }
        return _massRef[index];
    }                // может бросить bad_range
    //получить размер
    size_t getSize() const{
        return _size;
    }
   
    //изменить размер контейнера на число
    void changeSizeContainer(size_t size){
        if (size == 0 || size > 1000) { //ограничиваю размер 1000 элементами 4000 байт (примерно 3.91 КБ)
            throw Bad_length(); // Генерация исключения
        }
        T* temporaryPtr = new T [size]; //создаю временный массив нового размера
        if (_size < size)
        {
            //копирую все элементы старого массива строго до длинны СТАРОГО массива тк она меньше
            for (size_t i = 0; i < _size; i++)
            {
                temporaryPtr[i] = _massRef[i];
            }
            //остаток забиваю 0
            for (size_t i = _size; i < size; i++)
            {
                temporaryPtr[i] = T(); // Универсальная инициализация по умолчанию
            }
        }
        else
        {
            //копирую все элементы старого массива строго до длинны НОВОГО массива тк она меньше
            for (size_t i = 0; i < size; i++)
            {
                temporaryPtr[i] = _massRef[i];
            }
        }
        //Удаляю старый массив
        delete [] _massRef;
        //прсваиваю ссылке новый массив
        _massRef = temporaryPtr;
        //обнулять старую ссылку не надо, она выйдет из зоны видисости, стек затрет сам
        //новый размер
        _size = size;
    }                  // может бросить bad_length

    //вставить элемент по индексу - сдвинет все вправо увеличивая размер
    void elementIntoContainer(T el, size_t index){
        if (index >= _size) { //>= тк индекс начинается с 0, а размер с 1
            throw Bad_range(); // Генерация исключения
        }
        //Увеличиваю размер +1
        this->changeSizeContainer(_size + 1);

        //копирую все элементы старого массива c конца на ячейку вправа до index а в индекс вставляю новый элемент
        for (size_t i = _size - 1; i != index; i--)
        {
            _massRef[i] = _massRef[i-1];
            if (i == 1) //выходим так как копируем 0 элемент в 1, а в 0 нужно добавить новый только при 0 элементе 
                break;
        }
        //Записываю новый элемент
        _massRef[index] = el;
    }        // может бросить bad_range

    //удалить элемент из контейнера по индексу
    void  deleteElementOnTheContainer(size_t index){
        if (index >= _size) { //>= тк индекс начинается с 0, а размер с 1
            throw Bad_range(); // Генерация исключения
        }
        //если последний элемент, то можно просто сократить размер на 1
        if (index == _size -1)
        {
            this->changeSizeContainer(_size -1);
            return; //Завершаем в этом случае
        }
        
        //сдвигаем все элементы влево начиная с index +1 тк уже проверил выше на последний элемент безопасно +1
        for (size_t i = index + 1; i < _size; i++)
        {
            _massRef[i-1] = _massRef[i];
        }
        //Удаляю последний элемент
        this->changeSizeContainer(_size -1);
    }        // может бросить bad_range

    //изменить элемент по индексу
    void replaceElement(T el, size_t index){
        if (index >= _size) { //>= тк индекс начинается с 0, а размер с 1
            throw Bad_range(); // Генерация исключения
        }
        _massRef[index] = el;
    }              // может бросить bad_range

    //Дополнительно вставка в начало и конец и поиск в контейнере элемента по значению
    //вставить элемент в конец контейнера
    void insertElementEnd(T el){
        //Увеличиваю размер на 1
        changeSizeContainer(_size + 1);
        //добавляю в конец _size - 1 так как уже новый размер _size на 1 больше после changeSizeContainer(_size + 1);
        _massRef[_size - 1] = el;
    }

    //вставить элемент в начало контейнера
    void insertElementStart(T el){
        this->elementIntoContainer(el, 0);//вопользуюсь уже готовой функцикй
    }

    //поиск элемента в контейнере записывает в size_t& index место элемента и возвращает true при успехе false при провале int el искомый
    bool searchingElement(size_t& index, T el){
        for (size_t i = 0; i < _size - 1; i++)
        {
            if (el == _massRef[i])
            {
                index = i;
                return true;
            }   
        }
        return false;
    }

};


