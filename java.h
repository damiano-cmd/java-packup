#include <iostream>
#include <string.h>
#include <tuple>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <array>
#include <functional>

typedef uint8_t byte;
int byteSlotValues[8] = {1, 2, 4, 8, 16, 32, 64, 128};
enum Error {noError, outOfMemory};


int readFile(std::string filePath, std::string& fileData) 
{
    std::fstream file;
    file.open(filePath);

    if (file.fail()) 
    {
        return 0;
    }

    std::string fileText;
    
    while (getline(file, fileText))
    {
        fileData += fileText + "\n";
    }
    
    return 1;
}

// string operators
std::vector<std::string> splitString(std::string str, const std::string& split) {
    std::vector<std::string> strings;
    int e = 0;
    for (int i = 0; i < str.size(); i++) {
        if (e >= strings.size()) {
            strings.push_back("");
        }
        if (split[0] == str[i]) {
            bool isSplit = true;
            int newi = 0;
            for (int x = 0; x < split.size(); x++) {
                if (split[x] != str[i+x]) {
                    isSplit = false;
                    break;
                }
                newi = i + x;
            }
            if (isSplit) {
                e++;
                i = newi;
            } else {
                strings[e] = strings[e] + str[i];
            }
        } else {
            strings[e] = strings[e] + str[i];
        }
    }
    return strings;
}

std::string join(std::vector<std::string> array, std::string joint) 
{
    std::string newtext; 
    for (int i = 0; i < array.size(); i++) {
        newtext += array[i];
        if (i != array.size()-1) {
            newtext += joint;
        }
    }
    return newtext;
}

void replaceAll(std::string& str, const std::string& from, const std::string& to) 
{
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

std::array<std::string, 3> splitAt(std::string& source, int point1, int point2) 
{
    short pos = 0;
    std::array<std::string, 3> segments;
    for (int i = 0; i < source.size(); i++)
    {
        if (i == point1 || i == point2)
        {
            pos++;
        }
        segments[pos] += source[i];
    }
    return segments;
}

template<typename T> std::vector<T> slice(std::vector<T>& arr, int X, int Y)
{
    auto start = arr.begin() + X;
    auto end = arr.begin() + Y + 1;
 
    std::vector<T> result(Y - X + 1);
 
    copy(start, end, result.begin());
 
    return result;
}

class any;
template<typename T> class array 
{
    protected:
        T* _array;
        int16_t _size;
        int16_t _point;

    public: 
        array()
        {
            _point = 0;
            _size = 4;
            _array = new T[4];
        }

        array(int16_t s) 
        {
            _point = s;
            _size = s+4;
            _array = new T[s+4];
        }

        template<int sz> array(const T (&arr)[sz]) : array(0)
        {
            T* arr2 = const_cast<any*>(arr);
            std::cout << sz << '\n';
            for (int i = 0; i < sz; i++)
            {
                auto var = arr2[i];
                std::cout << var << '\n';
            }
        }

        ~array()
        {
            delete[] _array;
        }

        void push(T value)
        {
            if (_point >= _size) 
            {
                T* holdit = new T[_size];
                memcpy(holdit, _array, _size);
                delete[] _array;
                
                _size += 4;
                _array = new T[_size];
                memcpy(_array, holdit, _point);
                delete[] holdit;
            }
            _array[_point] = value;
            _point++;
        }

        void swap(array& arr)
        {
            array arrPH(arr.size());
            for (int16_t i = 0; i < arr.size(); i++) 
            {
                arrPH.push(arr[i]);
            }
            arr.clear();
            for (int16_t i = 0; i < _point; i++)
            {
                arr.push(_array[i]);
            }
            clear();
            for (int16_t i = 0; i < arrPH.size(); i++)
            {
                push(arrPH[i]);
            }
        }

        void copy(array<T> arr)
        {
            clear();
            for (int16_t i = 0; i < arr.size(); i++)
            {
                push(arr[i]);
            }
        }

        void pop()
        {
            _point -= 1;
        }

        void clear()
        {
            _point = 0;
        }

        int16_t size() {
            return _point;
        }

        int16_t capacity() {
            return _size;
        }

        T& operator[](int index) {

            if (index < _size)
            {
                return _array[index];
            } else 
            {
                printf("Index %i is an invalid index!", index);
                return _array[0];
            }
        }

        template<size_t sz> void operator=(const T (&arr)[sz])
        {
            for (int i = 0; i < sz; i++)
            {
                push(arr[i]);
            }
            
        }
};

class String : public array<char>
{
    public:
        String(int16_t s = 0) : array(s)
        {}

        String(char c) : array(0)
        {
            push(c);
        }

        String(const char * chs) : array(0)
        {
            int16_t sz = strlen(chs);
            for (int16_t i = 0; i < sz; i++)
            {
                push(chs[i]);
            }
            
        }

        void operator+(char ch)
        {
            push(ch);
        }

        void operator+(const char * chs)
        {
            int siz = strlen(chs);
            for (int16_t i = 0; i < siz; i++)
            {
                push(chs[i]);
            }
        }

        void operator=(const char * chs)
        {
            int siz = strlen(chs);
            clear();
            for (int16_t i = 0; i < siz; i++)
            {
                push(chs[i]);
            }
        }

        void operator=(char c)
        {
            clear();
            push(c);
        }

        bool operator==(const char * ccs)
        {
            int siz = strlen(ccs);
            if (siz > size() or siz < size())
            {
                return false;
            }
            for (int16_t i = 0; i < siz; i++)
            {
                if (ccs[i] != _array[i])
                {
                    return false;
                }
            }

            return true;
        }
};

std::ostream& operator<<(std::ostream& stream, String& other)
{
    for (int16_t i = 0; i < other.size(); i++)
    {
        stream << other[i];
    }
    
    return stream;
}

// code review
struct bracetOC {
    int open = 0;
    int close = 0;
};
struct bracetO {
    int character = 0;
    int line = 0;
    int position = 0;
};
std::vector<bracetOC> countBrackets(std::string source, char obr, char cbr, int& status) 
{

    int line = 1;
    int position = 1;
    std::vector<bracetO> obn;
    std::vector<bracetOC> bn;
    for (int i = 0; i < source.size(); i++) 
    {
        if (source[i] == obr) 
        {
            bracetO o;
            o.character = i;
            o.line = line;
            o.position = position;
            obn.push_back(o);
        }
        else if (source[i] == cbr)
        {
            bracetOC oc;
            oc.open = obn[obn.size()-1].position;
            oc.close = i;
            obn.pop_back();
            bn.push_back(oc);
        }
        else if (source[i] == '\n')
        {
            line++;
            position = 1;
        }
        position++;
    }
    if (obn.size() > 0)
    {
        for (int i = 0; i < obn.size(); i++) 
        {
            std::cout << "at line: " << obn[i].line << ':' << obn[i].position << " typeError: \'" << cbr << "\' no open bracket to close! " << '\n';
        }
        status = 1;
    }
    return bn;
}

std::vector<std::stringstream> compile(std::string source)
{
    replaceAll(source, ";", "\n");
    std::stringstream ss(source);

    int stat = 0;
    auto cbn = countBrackets(source, '{', '}', stat);
    auto sbn = countBrackets(source, '[', ']', stat);
    auto nbn = countBrackets(source, '(', ')', stat);
    if (stat > 0)
    {
        std::cout << "Quit compilation due to error" << '\n';
        exit(1);
    }

    std::vector<std::stringstream> map;
    std::string tmp;
    while(std::getline(ss, tmp, '\n')) {
        map.push_back(std::stringstream(tmp));
    }

    return map;
}


// convert in and out of bytes
template<typename T> std::vector<byte> toBytes(T value) 
{
    std::vector<byte> bytes(sizeof(T));
    memcpy(&bytes[0], &value, sizeof(T));
    return bytes;
}

template<typename T> T bytesTo(byte* bytes) 
{
    T value;
    memcpy(&value, bytes, sizeof(T));
    return value;
}

// copying 
template<typename T> void point2Bytes(T value, byte* bytes) 
{
    memcpy(bytes, &value, sizeof(T));
}

template<typename T> T* pointBytes2(byte* bytes) 
{
    T* value;
    value = (T*)bytes;
    return value;
}


// converting in and out of bin
template<typename T> std::array<bool, sizeof(T)*8> toBin(T value) 
{
    std::array<bool, sizeof(T)*8> arr;
    auto byteArray = toBytes(value);
    for (int i = 0; i < byteArray.size(); i++) 
    {
        for (int e = 0; e < 8; e++) {
            arr[(i*8)+e] = (byteArray[i]/byteSlotValues[e])%2 != 0;
        }
    }
    return arr;
}

template<typename T> T binTo(std::array<bool, sizeof(T)*8> arr) 
{
    std::array<byte, sizeof(T)> bytes;
    for (int i = arr.size()-1; i >= 0; i--) 
    {
        if (arr[i]) {
            bytes[i/8] += byteSlotValues[i];
        }
    } 
    return bytesTo<T>(&bytes[0]);
}

// else
template<typename T> bool isBinIn(T value, int slot) 
{
    auto bytes = toBytes(value, (int)(slot/8)+1);
    return ((bytes[slot/8]/(byteSlotValues[slot-((slot/8)*8)]))%2 != 0);
}


// data

template<typename T> class Memory 
{
    private:
        T max_size;
        std::vector<std::vector<byte>> RAM;

    public:
        Memory(T sizeOfVM = 1024) {
            max_size = sizeOfVM;
        }
        byte* operator ()(int ram, int slot) {
            if (RAM.size() == 0) {
                if (ram >= RAM.size()) {
                    ram = RAM.size()-1;
                }
                if (slot >= RAM[ram].size()) {
                    slot = RAM[ram].size()-1;
                }
                return &RAM[ram][slot];
            }
            return 0;
        }

        int size() {
            int s = 0;
            for (int i = 0; i < RAM.size(); i++) {
                s += RAM[i].capacity();
            }
            s += sizeof(this);
            return s;
        }

        void newRAMSlot() {
            RAM.push_back(std::vector<byte>(sizeof(T)*2));
        }

        template<typename E> Error addToMemory(T ram, E value) {
            if (size()+sizeof(value) > max_size) {
                return Error::outOfMemory;
            }
            auto bytes = toBytes(value);
            RAM[ram].reserve(RAM[ram].capacity() + sizeof(value));
            RAM[ram].insert(RAM[ram].end(), bytes.begin(), bytes.end());
            return Error::noError;
        }
};

class any;

class Number;
class BigInt;
class Bool;
class Undefined;
class Null;
class Object;

class any 
{
    private:
        std::vector<byte> data;

        char _type;
        byte _tsz;

        bool _isarray;
        int16_t _arrSize;

    public:
        any()
        {
            _isarray = false;
            _type = 'u';
            _arrSize = 0;
            _tsz = 0;
        }

        template<typename T> any(T var) : any()
        {
            set<T>(var);
        }
        
        template<typename T> void set(T var)
        {
            data = toBytes<T>(var);
            _type = typeid(var).name()[0];
            _tsz = sizeof(T);
        }

        int size() {
            return data.size();
        }

        String type()
        {
            return _type;
        }

        template<typename T> bool is(T var)
        {
            if (_type == typeid(T).name()[0])
            {
                T cv = bytesTo<T>(&data[0]);
                if (cv == var)
                {
                    return true;
                }
            }
            return false;
        }

        any method(String mname)
        {
            return this;
        }

        template<typename T> void operator=(T var) 
        {
            set<T>(var);
        }

        template<typename T> bool operator==(T var) 
        {
            T cv = bytesTo<T>(&data[0]);
            if (cv == var)
            {
                return true;
            }
            return false;
        }

        operator bool()
        {
            if (_type == 'u')
            {
                return false;
            } 
            else
            {
                return bytesTo<bool>(&data[0]);
            }
        }

        template<typename T> operator T()
        {
            return bytesTo<T>(&data[0]);
        }
};
std::ostream& operator<<(std::ostream& stream, any& other)
{
    String type = other.type();

    if (type == typeid(int).name())
    {
        stream << (int)other;
    }
    else if (type == typeid(short).name())
    {
        stream << (float)other;
    }
    else if (type == typeid(long).name())
    {
        stream << (float)other;
    }
    else if (type == typeid(float).name())
    {
        stream << (float)other;
    }
    else if (type == typeid(double).name())
    {
        stream << (double)other;
    }
    else if (type == typeid(char).name())
    {
        stream << (char)other;
    }
    else if (type == typeid(bool).name())
    {
        if ((bool)other)
        {
            stream << "true";
        }
        else
        {
            stream << "false";
        }
    }

    return stream;
}

class function
{
    private:
        std::function<any(std::vector<any>)> main;

    public:
        function()
        {}

        function(std::function<any(std::vector<any>)> maind)
        {
            main = maind;
        }

        void operator=(std::function<any(std::vector<any>)> h)
        {
            main = h;
        }

        void operator()(std::vector<any> arguments = {})
        {
            main(arguments);
        }
};
