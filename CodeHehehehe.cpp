#include <iostream>
#include <vector>

using namespace std;

int pow(int n) { // степени двойки
    return 1 << n;
}

int calculateDecodeParityBits(int messageSize) { // считаем количество битов чётности
    int m = 2;
    while ((1 << m) < (messageSize))
        m++;
    return m;
}

int calculateParityBits(int messageSize) { // считаем количество битов чётности
    int m = 2;
    while ((1 << m)  < (messageSize + m)) 
        m++;
    return m;
}

vector<bool> getInputMessage() { // перевод строки в вектор
    string input;
    cout << "Enter the message (0s and 1s): ";
    cin >> input;

    vector<bool> message;
    for (char bit : input) {
        message.push_back(bit - '0');
    }

    return message;
}

void introduceError(vector<bool>& message, int errorPosition) { // добавление ошибки в код
    if (errorPosition >= 0 && errorPosition < message.size()) {
        message[errorPosition] = !message[errorPosition];
        cout << "Error introduced at position " << (errorPosition + 1) << endl;
    }
    else {
        cout << "Invalid error position." << endl;
    }
}

int findErrorPosition(vector<bool>& receivedMessage) { // поиск ошибки
    int errorPosition = 0;

    for (int i = 0; i < receivedMessage.size(); i++) {
        if (receivedMessage[i] == 1) {
            errorPosition += (1 << i); // сейчас сделать // или записать в декодинг
        }
    }

    return errorPosition - 1;
}

void printMessage(const vector<bool>& decodedMessage) { // вывод сообщения после 
    cout << ": ";
    for (long bit : decodedMessage) {
        cout << bit;
    }
    cout << endl;
}

vector<bool> hammingDecode(vector<bool>& receivedMessage) { // декодирование ошибки
    int m = calculateDecodeParityBits(receivedMessage.size() - 1);
    vector<bool> decodedMessage(receivedMessage.size() - m -1);
    vector<bool> bitcheck(m+1);
    bool popa = false, mama = false; // начальные значения - ошибок нет
    int ero = 0;

    for (int i = 0; i < m; i++) { // считаем значения битов чётности
        int parityBit = 0;
        for (int j = 0; j < receivedMessage.size() - 1; j++) {
            if (((j + 1) & (1 << i)) != 0 && receivedMessage[j] == 1 && (j + 1) != (1 << i)) {
                parityBit ^= 1;
            }
        }
        bitcheck[i] = parityBit;
    }

    int additionalParityBit = 0, i = 0;     // Считаем значение контрольного бита чётности
    for (int j = 0; j < receivedMessage.size() - 1; j++) {
        if ((j + 1) == pow(i)) {
            i++;
            if (receivedMessage[j] == 1)
                additionalParityBit ^= 1;
        }
    }
    bitcheck[m] = additionalParityBit;

    for (int i = 0; i < m; i++)
        if (bitcheck[i] != receivedMessage[(1 << i) - 1])
        {
            popa = true; // есть ошибка
            ero += (1 << i); // значение в индекса ошибки
        }

    if (receivedMessage[receivedMessage.size() - 1] != bitcheck[m])
        mama = true; // есть ошибка в контрольном
    
    if (popa || mama) // если ошибки есть:
    {/*if (mama)
        {
            cout << "ошибка в битах чётности, исправления не внесены" << endl;
        }
        else
        {*/
        if (ero==0)
        {
            ero = receivedMessage.size();
        }
        receivedMessage[ero - 1] = !receivedMessage[ero - 1];
        cout << "Ошибка в бите " << ero << " исправлена" << endl;
        // }
    }

    int j = 0, pypa = 0; // вывод слова без устранения ошибки
    int max = receivedMessage.size() - 1;
    for (int i = 0; i < max; i++) {
        if (((i + 1) == pow(j))) 
            j++;
        else
        {
            decodedMessage[pypa] = receivedMessage[i];
            pypa++;
        }    
    }

    return decodedMessage;
}

vector<bool> hammingEncode(const vector<bool>& message) { // кодирование ошибки
    int m = calculateParityBits(message.size());
    vector<bool> encodedMessage(message.size() + m + 1, 0);

    int j = 0;
    for (int i = 0; i < encodedMessage.size() - 1; i++) {
        if (((i + 1) & i) == 0) {
            continue; // Skip parity bits
        }
        encodedMessage[i] = message[j++];
    }

    for (int i = 0; i < m; i++) { // считаем значения битов чётности
        int parityBit = 0;
        for (int j = 0; j < encodedMessage.size(); j++) {
            if (((j + 1) & (1 << i)) != 0 && encodedMessage[j] == 1) {
                parityBit ^= 1;
            }
        }
        encodedMessage[(1 << i) - 1] = parityBit;
    }

    int additionalParityBit = 0, i = 0;     // Считаем значение контрольного бита чётности
    for (int j = 1; j < encodedMessage.size() - 1; j++) {
        if (j == pow(i)) {
            i++;
            if (encodedMessage[j] == 1)
                additionalParityBit ^= 1;
        }
    }


    encodedMessage[encodedMessage.size() - 1] = additionalParityBit;

    return encodedMessage;
}

int main() {
    vector<bool> message, receivedMessage, decodedMessage, encodedMessage;
    int choice, errorPosition;
    setlocale(LC_ALL, "Russian");

    do {
        cout << "1. Введите сообщение\n2. Ввод ошибки\n3. Вывод раскодированого слова\n4. Вывод изначльного слова\n5. Вывод изменённого слова\n6. Авто тест\n7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            message = getInputMessage();
            encodedMessage = hammingEncode(message);
            receivedMessage = encodedMessage; // Иммитация передатчика
            break;
        case 2:
            cout << "Введите индекс ошибки: ";
            cin >> errorPosition;
            errorPosition -= 1;
            introduceError(receivedMessage, errorPosition);
            break;
        case 3:
            decodedMessage = hammingDecode(receivedMessage);
            printMessage(decodedMessage);
            break;
        case 4:
            printMessage(encodedMessage);
            break;
        case 5:
            printMessage(receivedMessage);
            break;
        case 6:
            for (int i = 0; i < receivedMessage.size(); i++)
            {
                receivedMessage = encodedMessage;
                cout << "Закодированое ";
                printMessage(encodedMessage);
                introduceError(receivedMessage, i);
                cout << "С ошибкой ";
                printMessage(receivedMessage);
                decodedMessage = hammingDecode(receivedMessage);
                cout << "Декодированое ";
                printMessage(decodedMessage);
                cout << "С исправленной ошибкой";
                printMessage(receivedMessage);
                cout << endl;
            }
            break;
        case 7:
            cout << "Exiting program.\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 7);

    return 0;
}
