#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <memory>
#include <tuple>
#include <array>

typedef uint8_t byte;
typedef byte bshort[2];
typedef byte bint[4];
typedef byte blong[8];
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

std::string join(std::vector<std::string> array, std::string joint) {
    std::string newtext; 
    for (int i = 0; i < array.size(); i++) {
        newtext += array[i];
        if (i != array.size()-1) {
            newtext += joint;
        }
    }
    return newtext;
}

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
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

template<typename T> std::vector<byte> toBytes(T value) 
{
    std::vector<byte> bytes(sizeof(T));
    memcpy(&bytes[0], &value, bytes.size());
    return bytes;
}

template<typename T> void copyToBytes(T value, byte* bytes) 
{
    memcpy(bytes, &value, sizeof(T));
}

template<typename T> T copyBytesTo(byte* bytes) 
{
    T value;
    memcpy(&value, bytes, sizeof(T));
    return value;
}

template<typename T> T* bytesTo(byte* bytes) 
{
    T* value;
    value = (T*)bytes;
    return value;
}

bool isBinInByte(byte b, uint8_t slot) 
{
    return ((b/byteSlotValues[slot])%2 != 0);
}

std::array<bool, 8> byteToBinArray(byte b) 
{
    std::array<bool, 8> ba;
    for (int i = 0; i < 8; i++)
        ba[i] = ((b/byteSlotValues[i])%2 != 0);
    return ba;
}

template<typename T> std::array<bool, sizeof(T)*8> toBinArray(T value) 
{
    std::array<bool, sizeof(T)*8> arr;
    auto byteArray = toBytes(value);
    for (int i = 0; i < byteArray.size(); i++) {
        auto binArr = byteToBinArray(byteArray[i]);
        for (int e = 0; e < 8; e++) {
            arr[(i*8)+e] = binArr[e];
        }
    }
    return arr;
}

template<typename ...Types> class TypeArray 
{
    private:
        std::tuple<Types...> tu;
    public:
        void g() {
            std::cout << std::get<0>(tu) << '\n';
        }
        template <typename T>
        auto push_back(const T& t)
        {
            return std::tuple_cat(tu, std::make_tuple(t));
        }
        
        template <std::size_t ... Is>
        auto pop_front_impl(std::index_sequence<Is...>)
        {
            return std::make_tuple(std::get<1 + Is>(tu)...);
        }
        
        auto pop_front()
        {
            tu = pop_front_impl(std::make_index_sequence<std::tuple_size<decltype(tu)>::value - 1>());
        }

        template <std::size_t ... Is>
        auto pop_back_impl(std::index_sequence<Is...>)
        {
            return std::make_tuple(std::get<Is>(tu)...);
        }
        
        void pop_back()
        {
            tu = pop_back_impl(std::make_index_sequence<std::tuple_size<decltype(tu)>::value - 1>());
        }
};

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
