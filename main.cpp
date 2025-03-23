#include "Array.h"
#include <string>
#include <exception>
#include <iostream>
using namespace std;

template <typename T1>
void printArr(const Array<T1>& other, string massage){
    cout << "В контейнере "<< massage << " элементов - " << other.getSize() << endl;
    for (size_t i = 0; i < other.getSize(); i++)
    {
        cout << "Элемент N" << i << " = " << other.getNumberByIndex(i) << endl;
    }
}

int main(int argc, char const *argv[])
{
    setlocale(LC_ALL, "ru_RU.UTF-8");

    
    try {
        //IntegerArray arr(10000); //вызовет исключение поставил ограничение на 1000 элементов 4000 байт (примерно 3.91 КБ)
        //создаю контейнер
        Array<double> arr(5);
        //инициализирую
        arr[0] = 1.6;
        arr[1] = 2;
        arr[2] = 3.6;
        arr[3] = 4;
        arr[4] = 5;
        //вывод в консоль
        printArr(arr, string("arr"));
        //Изменяю элемент 0 для добавления метод elementIntoContainer
        arr.replaceElement(100.5, 0);
        //копирую
        Array<double> arr2 = arr;
        //сокращию размер до 2
        arr.changeSizeContainer(2);
        //arr.changeSizeContainer(-2); //вызовет исключение тк тип size_t а максимальный размер 1000
        //увеличиваю размер копии новые элементы = 0
        arr2.changeSizeContainer(10); //Error: Invalid container length если размер 0
         //так как заложил в логику, что массив должен состоять хотя бы из 1 элемента, в IntegerArray нету 0 массива

        //вывод в консоль
        printArr(arr, string("arr"));
        printArr(arr2, string("arr2"));
        arr2[5] = 70;
        arr2[6] = 60;
        arr2[7] = 0.6;
        arr2[8] = 4.6;
        arr2[9] = 33;
        //arr2[100] = 42; // Должно вызвать bad_range
        arr2.deleteElementOnTheContainer(0);//если удалить 0 элемент в массиве из 1 элемента будет исключение Error: Invalid container length
        //так как заложил в логику, что массив должен состоять хотя бы из 1 элемента, в IntegerArray нету 0 массива
        printArr(arr2, string("arr2"));
        //Добавляю элемент в коннец
        arr2.insertElementEnd(999);
        printArr(arr2, string("arr2"));

        //Элемент вставляется по индексу, а все элементы справа двигаются на 1 вправо
        //есть если вставить в конец, то последний элемент займет положение правее +1 от нового элемента
        //тоесть данным методом в конец не вставить элемент, он займет N индекса, а вставить в конец есть метод insertElementEnd
        arr2.elementIntoContainer(888, 5); //нелья вставлять эл вне диапозона Error: Index out of range
        printArr(arr2, string("arr2"));

        //Добавляю элемент в начало
        arr2.insertElementStart(1111);
        printArr(arr2, string("arr2"));

        //проверка есть ли элемент 888 индекс будет записан в переменной index метод вернет bool значение 1 - успех 0 - не найдено
        size_t index = 0;
        if (arr2.searchingElement(index, 888))
        {
            cout << "Элемент найден под индексом: " << index << endl;
        }
        else
        {
            cout << "Элемент не найден" << endl;
        }
        
        

    } 
    catch (const std::exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    return 0;
}

