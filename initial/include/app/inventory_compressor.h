#ifndef INVENTORY_COMPRESSOR_H
#define INVENTORY_COMPRESSOR_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <utility>
#include "inventory.h"
#include "hash/xMap.h"
#include "heap/Heap.h"
#include "list/XArrayList.h"

//! thêm này vô để chạy test

template <int treeOrder>

class HuffmanTree
{
public:
    struct HuffmanNode
    {
        char symbol;
        int freq;
        XArrayList<HuffmanNode *> children;
        int order;
        HuffmanNode(char s, int f, int ord = 0) : symbol(s), freq(f), order(ord) {
            children = XArrayList<HuffmanNode*>();
        }
        HuffmanNode(int f, const XArrayList<HuffmanNode*>& childs, int ord = 0) 
            : symbol('\0'), freq(f), children(childs), order(ord) {}
        bool operator<(const HuffmanNode &other) const
        {
            return freq > other.freq; // min-heap dựa trên tần số
        }
        
    };
    static int compareNodes(HuffmanNode*& a, HuffmanNode*& b) {
        if (a->freq < b->freq) return -1; // a nhỏ hơn b
        if (a->freq > b->freq) return 1;  // a lớn hơn b
        if (a->order < b->order) return -1; // a được đưa vào sớm hơn, ưu tiên a
        if (a->order > b->order) return 1;  // b được đưa vào sớm hơn, ưu tiên b
        return 0;
    }

    HuffmanTree();
    ~HuffmanTree();

    void build(XArrayList<pair<char, int>> &symbolsFreqs);
    void generateCodes(xMap<char, std::string> &table);
    string decode(const std::string &huffmanCode);

private:
    HuffmanNode *root;
    void clear(HuffmanNode *node)
    {
        if (node)
        {
            for (int i = 0; i < node->children.size(); ++i)
            {
                clear(node->children.get(i));
            }
            delete node;
        }
    }
    void dfs(HuffmanNode *node, std::string &code, xMap<char, std::string> &table)
    {
        if (node == nullptr)
            return;
        if (node->children.size() == 0)
        {
            table.put(node->symbol, code);
            if (node->symbol == '\0') return; // Skip dummy nodes
            return;
        }
        for (int i = 0; i < node->children.size(); ++i)
        {
            code += '0' + i;
            if (i >= 10) code[code.length() - 1] = 'a' + (i - 10);
            dfs(node->children.get(i), code, table);
            code.pop_back(); // Backtrack
        }
    }
};

template <int treeOrder>
class InventoryCompressor
{
public:
    InventoryCompressor(InventoryManager *manager);
    ~InventoryCompressor();

    void buildHuffman();
    void printHuffmanTable();
    std::string productToString(const List1D<InventoryAttribute> &attributes, const std::string &name);
    std::string encodeHuffman(const List1D<InventoryAttribute> &attributes, const std::string &name);
    std::string decodeHuffman(const std::string &huffmanCode, List1D<InventoryAttribute> &attributesOutput, std::string &nameOutput);

    static int comparator(pair<char, int> &a, pair<char, int> &b) {
        if (a.second != b.second)
            return (a.second > b.second) ? -1 : 1;
        return (a.first > b.first) ? -1 : (a.first < b.first ? 1 : 0);
    }
    
    

private:
    xMap<char, std::string> *huffmanTable;
    Heap<pair<char, int>> heap;
    InventoryManager *invManager;
    HuffmanTree<treeOrder> *tree;
};

#endif // INVENTORY_COMPRESSOR_H

template <int treeOrder>
HuffmanTree<treeOrder>::HuffmanTree()
{
    root = nullptr;
}

template <int treeOrder>
HuffmanTree<treeOrder>::~HuffmanTree()
{
    // TODO
    clear(root);
}

template <int treeOrder>
void HuffmanTree<treeOrder>::build(XArrayList<pair<char, int>> &symbolsFreqs)
{
    // TODO
    Heap<HuffmanNode*> minHeap(compareNodes);
    int orderCounter = 0;
    int L = symbolsFreqs.size();
    int d = (treeOrder - 1)  - ((L - 1) % (treeOrder - 1));
    if (d != treeOrder-1){
        for (int i = 0; i < d; ++i) {
            minHeap.push(new HuffmanNode('\0', 0, orderCounter++));
        }
    }
    else{
    }
    for (int i = 0; i < symbolsFreqs.size(); ++i)
    {
        auto &p = symbolsFreqs.get(i);
        minHeap.push(new HuffmanNode(p.first, p.second, orderCounter++));
    } 

    while (minHeap.size() > 1)
    {
        XArrayList<HuffmanNode *> children;
        int totalFreq = 0;
        for (int i = 0; i < treeOrder && !minHeap.empty(); ++i)
        {
            HuffmanNode *node = minHeap.peek();
            minHeap.pop();
            children.add(node);
            totalFreq += node->freq;
        }
        minHeap.push(new HuffmanNode(totalFreq, children, orderCounter++));
    }
    if (minHeap.empty()) {
        root = nullptr;
    } else {
        root = minHeap.peek();
    }
}

template <int treeOrder>
void HuffmanTree<treeOrder>::generateCodes(xMap<char, std::string> &table)
{
    // TODO
    if(!root)
        return;
    std::string current = "";
    dfs(root, current, table);
}

template <int treeOrder>
std::string HuffmanTree<treeOrder>::decode(const std::string &huffmanCode)
{
    if (!root)
        return std::string(1, '\0');
        
    std::string result;
    HuffmanNode *node = this->root;

    for (char c : huffmanCode)
    {
        if (!node || node->children.size() == 0)
            return std::string(1, '\0');

        int idx = c - '0';
        if (c >= 'a' && c <= 'f') idx = 10 + (c - 'a');
        if (idx < 0 || idx >= treeOrder)
            return std::string(1, '\0');

        node = node->children.get(idx);
        if (!node) return std::string(1, '\0');

        if (node->children.size() == 0)
        {
            if (node->symbol == '\0')
                return std::string(1, '\0');

            result += node->symbol;
            node = root;
        }
    }

    return result;
}


template <int treeOrder>
InventoryCompressor<treeOrder>::InventoryCompressor(InventoryManager *manager)
{
    // TODO
    this->invManager = manager;
    this->huffmanTable = new xMap<char, std::string>(xMap<char, std::string>::simpleHash);
    this->tree = new HuffmanTree<treeOrder>();
    this->heap = Heap<pair<char, int>>(comparator);
}

template <int treeOrder>
InventoryCompressor<treeOrder>::~InventoryCompressor()
{
    // TODO
    delete huffmanTable;
    delete tree;
}

template <int treeOrder>
void InventoryCompressor<treeOrder>::buildHuffman()
{
    // TODO
    xMap<char, int> freqMap(xMap<char, int>::simpleHash);
    List2D<InventoryAttribute> products = invManager->getAttributesMatrix();

    for (int i = 0; i < products.rows(); ++i)
    {
        auto row = products.getRow(i);
        std::string productStr = productToString(row, invManager->getProductNames().get(i));
        for (char c : productStr)
        {
            if (freqMap.containsKey(c))
                freqMap.put(c, freqMap.get(c) + 1);
            else
                freqMap.put(c, 1);
        }
    }

    XArrayList<pair<char, int>> symbolsFreqs;
    DLinkedList<char> keys = freqMap.keys();
    for (char c : keys)
    {
        symbolsFreqs.add(pair<char, int>(c, freqMap.get(c)));
    }
   heap.heapsortHuff(symbolsFreqs);
    tree->build(symbolsFreqs);
    tree->generateCodes(*huffmanTable);
}

template <int treeOrder>
void InventoryCompressor<treeOrder>::printHuffmanTable()
{
    DLinkedList<char> keys = huffmanTable->keys();
    for (char ch : keys) {
        std::cout << "'" << ch << "' : " << huffmanTable->get(ch) << std::endl;
    }
}

template <int treeOrder>
std::string InventoryCompressor<treeOrder>::productToString(const List1D<InventoryAttribute> &attributes, const std::string &name)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(6);
    oss << name << ":";
    for (int i = 0; i < attributes.size(); ++i) {
        if (i > 0) {
            oss << ", ";
        }
        oss << "(" << attributes.get(i).name << ": " << (double)attributes.get(i).value << ")";
    }
    return oss.str();
}

template <int treeOrder>
std::string InventoryCompressor<treeOrder>::encodeHuffman(const List1D<InventoryAttribute> &attributes, const std::string &name)
{
    std::string productStr = productToString(attributes, name);
    std::string huffmanCode;
    for (char c : productStr)
    {
        if (!huffmanTable->containsKey(c))
        {
            throw std::runtime_error("key (" + std::string(1, c) + ") is not found");
        }
        huffmanCode += huffmanTable->get(c);
    }
    return huffmanCode;
}
template <int treeOrder>
std::string InventoryCompressor<treeOrder>::decodeHuffman(const std::string &huffmanCode, List1D<InventoryAttribute> &attributesOutput, std::string &nameOutput)
{
    attributesOutput = List1D<InventoryAttribute>();
    nameOutput.clear();
    std::string decoded = tree->decode(huffmanCode);
    if (decoded.empty()) return std::string(1, '\0');

    size_t colonPos = decoded.find(':');
    if (colonPos == std::string::npos) return std::string(1, '\0');

    nameOutput = decoded.substr(0, colonPos);
    std::string attrPart = decoded.substr(colonPos + 1);

    size_t pos = 0;
    while (pos < attrPart.size()) {
        size_t open = attrPart.find('(', pos);
        size_t close = attrPart.find(')', open);
        if (open == std::string::npos || close == std::string::npos) break;

        std::string attrStr = attrPart.substr(open + 1, close - open - 1);
        size_t sep = attrStr.find(':');
        if (sep != std::string::npos) {
            InventoryAttribute attr;
            attr.name = attrStr.substr(0, sep);
            attr.value = std:: stod(attrStr.substr(sep + 1));
            attributesOutput.add(attr);
        }

        pos = close + 1;
    }
    return decoded;
    
}