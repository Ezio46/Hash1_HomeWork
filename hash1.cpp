#include <iostream>
#include <vector>
#include <list>

using namespace std;

// Хеш-таблица с открытой адресацией и квадратичным пробингом
class HashTable {
private:
    enum EntryState { ACTIVE, EMPTY, DELETED }; // Состояния ячейки

    struct HashEntry {
        int key;
        int value;
        EntryState state;
    };

    vector<HashEntry> table;
    int size;

    // Хеш-функция
    int hashFunction(int key) {
        return key % size;
    }

    // Квадратичный пробинг
    int quadraticProbing(int index, int i) {
        return (index + i * i) % size;
    }

public:
    // Конструктор
    HashTable(int tableSize) : size(tableSize), table(tableSize, { 0, 0, EMPTY }) {}

    // Вставка элемента
    void insert(int key, int value) {
        int index = hashFunction(key);
        int i = 0;

        while (table[index].state == ACTIVE && i < size) {
            index = quadraticProbing(index, ++i);
        }

        if (i < size) {
            table[index] = { key, value, ACTIVE };
        }
        else {
            cout << "Error: HashTable overflowing\n";
        }
    }

    // Поиск элемента
    int search(int key) {
        int index = hashFunction(key);
        int i = 0;

        while (table[index].state != EMPTY && i < size) {
            if (table[index].state == ACTIVE && table[index].key == key) {
                return table[index].value;
            }
            index = quadraticProbing(index, ++i);
        }

        return -1; // Элемент не найден
    }

    // Ленивое удаление элемента
    void lazyDelete(int key) {
        int index = hashFunction(key);
        int i = 0;

        while (table[index].state != EMPTY && i < size) {
            if (table[index].state == ACTIVE && table[index].key == key) {
                table[index].state = DELETED;
                return;
            }
            index = quadraticProbing(index, ++i);
        }

        cout << "Error: not found\n";
    }

    // Вывод хеш-таблицы
    void print() {
        cout << "HashTable:\n";
        for (int i = 0; i < size; ++i) {
            cout << i << ": ";
            if (table[i].state == ACTIVE) {
                cout << table[i].key << " (" << table[i].value << ")\n";
            }
            else if (table[i].state == DELETED) {
                cout << "DELETED\n";
            }
            else {
                cout << "EMPTY\n";
            }
        }
    }
};

int main() {
    HashTable ht(10); // Хеш-таблица размера 10 ячеек

    ht.insert(12, 24);
    ht.insert(25, 50);
    ht.insert(37, 74);

    ht.print();

    cout << "Searh 25: " << ht.search(25) << endl;
    cout << "Searh 40: " << ht.search(40) << endl;

    ht.lazyDelete(25);

    ht.print();

    return 0;
}
