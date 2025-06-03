#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include "list/XArrayList.h"
#include "list/DLinkedList.h"
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace std;

// -------------------- List1D --------------------
template <typename T>
class List1D
{
private:
    IList<T> *pList;

public:
    List1D();
    List1D(int num_elements);
    List1D(const T *array, int num_elements);
    List1D(const List1D<T> &other);
    virtual ~List1D();

    int size() const;
    T get(int index) const;
    void set(int index, T value);
    void add(const T &value);
    void removeAt(int index);
    string toString() const;
    List1D<T> &operator=(const List1D<T> &other);
    template <typename U>
    friend ostream &operator<<(ostream &os, const List1D<T> &list);
};

// -------------------- List2D --------------------
template <typename T>
class List2D
{
private:
    IList<IList<T> *> *pMatrix;

public:
    List2D();
    List2D(List1D<T> *array, int num_rows);
    List2D(const List2D<T> &other);
    virtual ~List2D();

    int rows() const;
    void setRow(int rowIndex, const List1D<T> &row);
    T get(int rowIndex, int colIndex) const;
    List1D<T> getRow(int rowIndex) const;
    void addRow(const List1D<T> &row);
    void removeRow(int rowIndex);
    string toString() const;
    List2D<T> &operator=(const List2D<T> &other);
    template <typename U>
    friend ostream &operator<<(ostream &os, const List2D<T> &matrix);
};

struct InventoryAttribute
{
    string name;
    double value;

    InventoryAttribute() : name(""), value(0.0) {}
    InventoryAttribute(const std::string &name, double value) : name(name), value(value) {}

    std::string toString() const { return name + ": " + std::to_string(value); }

    friend std::ostream &operator<<(std::ostream &os, const InventoryAttribute &attr)
    {
        os << attr.toString();
        return os;
    }
    bool operator==(const InventoryAttribute &other) const
    {
        return (name == other.name && value == other.value);
    }
};
// -------------------- InventoryManager --------------------
class InventoryManager
{
private:
    List2D<InventoryAttribute> attributesMatrix;
    List1D<string> productNames;
    List1D<int> quantities;

public:
    InventoryManager();
    InventoryManager(const List2D<InventoryAttribute> &matrix,
                     const List1D<string> &names,
                     const List1D<int> &quantities);
    InventoryManager(const InventoryManager &other);

    int size() const;
    List1D<InventoryAttribute> getProductAttributes(int index) const;
    string getProductName(int index) const;
    int getProductQuantity(int index) const;
    void updateQuantity(int index, int newQuantity);
    void addProduct(const List1D<InventoryAttribute> &attributes, const string &name, int quantity);
    void removeProduct(int index);

    List1D<string> query(string attributeName, const double &minValue,
                         const double &maxValue, int minQuantity, bool ascending) const;

    void removeDuplicates();

    static InventoryManager merge(const InventoryManager &inv1,
                                  const InventoryManager &inv2);

    void split(InventoryManager &section1,
               InventoryManager &section2,
               double ratio) const;

    List2D<InventoryAttribute> getAttributesMatrix() const;
    List1D<string> getProductNames() const;
    List1D<int> getQuantities() const;
    string toString() const;
};

// -------------------- List1D Method Definitions --------------------
template <typename T>
List1D<T>::List1D()
{
    // TODO
    this->pList = new XArrayList<T>();
}

template <typename T>
List1D<T>::List1D(int num_elements)
{
    // TODO
    pList = new XArrayList<T>();
    for (int i = 0; i < num_elements; i++)
    {
        pList->add(T());
    }
}

template <typename T>
List1D<T>::List1D(const T *array, int num_elements)
{
    // TODO
    pList = new XArrayList<T>();
    for (int i = 0; i < num_elements; i++)
    {
        pList->add(array[i]);
    }
}

template <typename T>
List1D<T>::List1D(const List1D<T> &other)
{
    // TODO
    pList = new XArrayList<T>();
    if (other.pList)
    {
        for (int i = 0; i < other.size(); i++)
        {
            pList->add(other.pList->get(i));
        }
    }
}

template <typename T>
List1D<T>::~List1D()
{
    // TODO
    delete pList;
}

template <typename T>
int List1D<T>::size() const
{
    // TODO
    return pList->size();
}

template <typename T>
T List1D<T>::get(int index) const
{
    // TODO
    return pList->get(index);
}

template <typename T>
void List1D<T>::set(int index, T value)
{
    // TODO
    if (index < 0)
        throw out_of_range("Index is out of range!");
    XArrayList<T> *pList = dynamic_cast<XArrayList<T> *>(this->pList);
    pList->set(index, value);
}

template <typename T>
void List1D<T>::add(const T &value)
{
    // TODO
    pList->add(value);
}
template <typename T>
void List1D<T>::removeAt(int index)
{
    // TODO
    if (index < 0 || index >= pList->size())
        throw out_of_range("Index is out of range!");
    pList->removeAt(index);
}

template <typename T>
List1D<T> &List1D<T>::operator=(const List1D<T> &other)
{
    // TODO
    if (this != &other)
    {
        delete pList;
        pList = new XArrayList<T>();
        for (int i = 0; i < other.size(); i++)
        {
            pList->add(other.pList->get(i));
        }
    }
    return *this;
}

template <typename T>
string List1D<T>::toString() const
{
    // TODO
    ostringstream oss;
    oss << "[";
    for (int i = 0; i < pList->size(); i++)
    {
        oss << get(i);
        if (i < pList->size() - 1)
        {
            oss << ", ";
        }
    }
    oss << "]";
    return oss.str();
}

template <typename T>
ostream &operator<<(ostream &os, const List1D<T> &list)
{
    // TODO
    os << list.toString();
    return os;
}

template <typename T>
class List1DAdapter : public IList<T>
{
private:
    List1D<T> list;
    mutable T tempValue;

public:
    List1DAdapter() : list() {}
    List1DAdapter(const List1D<T> &lst) : list(lst) {}
    List1DAdapter(const List1DAdapter<T> &other) : list(other.list) {}
    void add(T e) override
    {
        list.add(e);
    }

    void add(int index, T e) override
    {
        throw runtime_error("add(int, T) is not supported in List1D<T>");
    }

    T &get(int index) override
    {
        tempValue = list.get(index);
        return tempValue;
    }

    T removeAt(int index) override
    {
        T value = list.get(index);
        list.removeAt(index);
        return value;
    }
    bool removeItem(T item, void (*removeItemData)(T) = 0) override
    {
        throw runtime_error("removeItem() is not supported in List1D<T>");
    }
    void set(int index, T e)
    {
        list.set(index, e);
    }

    int size() override
    {
        return list.size();
    }

    void clear() override
    {
        throw runtime_error("clear() is not supported in List1D<T>");
    }

    bool empty() override
    {
        throw runtime_error("empty() is not supported in List1D<T>");
    }

    bool contains(T item) override
    {
        throw runtime_error("contains() is not supported in List1D<T>");
    }

    std::string toString(std::string (*item2str)(T &) = 0) override
    {
        return list.toString();
    }
    int indexOf(T item) override
    {
        throw runtime_error("indexOf() is not supported in List1D<T>");
    }
    List1D<T> getList() const { return list; }
    ~List1DAdapter() override {}
};

// -------------------- List2D Method Definitions --------------------
template <typename T>
List2D<T>::List2D()
{
    // TODO
    pMatrix = new DLinkedList<IList<T> *>();
}

template <typename T>
List2D<T>::List2D(List1D<T> *array, int num_rows)
{
    pMatrix = new DLinkedList<IList<T> *>();
    for (int i = 0; i < num_rows; i++)
    {
        pMatrix->add(new List1DAdapter<T>(array[i]));
    }
}

template <typename T>
List2D<T>::List2D(const List2D<T> &other)
{
    pMatrix = new DLinkedList<IList<T> *>();
    for (int i = 0; i < other.rows(); i++)
    {
        pMatrix->add(new List1DAdapter<T>(other.getRow(i)));
    }
}

template <typename T>
List2D<T>::~List2D()
{
    // TODO
    while (pMatrix->size() > 0)
    {
        delete pMatrix->get(0);
        pMatrix->removeAt(0);
    }
    delete pMatrix;
}

template <typename T>
int List2D<T>::rows() const
{
    // TODO
    return pMatrix->size();
}

template <typename T>
void List2D<T>::setRow(int rowIndex, const List1D<T> &row)
{
    // TODO

    if (rowIndex < 0)
        throw out_of_range("Index is out of range!");
    DLinkedList<IList<T> *> *pMatrix = dynamic_cast<DLinkedList<IList<T> *> *>(this->pMatrix);
    if (rowIndex >= pMatrix->size())
    {
        if (rowIndex == pMatrix->size())
            pMatrix->add(new List1DAdapter<T>(row));
        else
            throw out_of_range("Index is out of range!");
    }

    else
        pMatrix->set(rowIndex, new List1DAdapter<T>(row));
}

template <typename T>
T List2D<T>::get(int rowIndex, int colIndex) const
{
    // TODO
    if (rowIndex < 0 || rowIndex >= this->rows())
        throw out_of_range("Index is out of range!");
    return pMatrix->get(rowIndex)->get(colIndex);
}

template <typename T>
List1D<T> List2D<T>::getRow(int rowIndex) const
{
    // TODO
    if (rowIndex < 0 || rowIndex >= rows())
        throw out_of_range("Index is out of range!");
    List1DAdapter<T> *rowPtr = dynamic_cast<List1DAdapter<T> *>(pMatrix->get(rowIndex));
    return rowPtr->getList();
}
template <typename T>
void List2D<T>::addRow(const List1D<T> &row)
{
    pMatrix->add(new List1DAdapter<T>(row));
}

template <typename T>
void List2D<T>::removeRow(int rowIndex)
{
    // TODO
    if (rowIndex < 0 || rowIndex >= rows())
        throw out_of_range("Index is out of range!");
    delete pMatrix->get(rowIndex);
    pMatrix->removeAt(rowIndex);
}

template <typename T>
string List2D<T>::toString() const
{
    // TODO
    ostringstream oss;
    oss << "[";
    for (int i = 0; i < rows(); i++)
    {
        oss << "[";
        List1D<T> row = getRow(i);
        for (int j = 0; j < row.size(); j++)
        {
            oss << row.get(j);
            if (j < row.size() - 1)
            {
                oss << ", ";
            }
        }
        oss << "]";
        if (i < rows() - 1)
        {
            oss << ", ";
        }
    }
    oss << "]";
    return oss.str();
}
template <typename T>
List2D<T> &List2D<T>::operator=(const List2D<T> &other)
{
    // TODO
    if (this != &other)
    {
        while (pMatrix->size() > 0)
        {
            delete pMatrix->get(0);
            pMatrix->removeAt(0);
        }
        for (int i = 0; i < other.rows(); i++)
        {
            pMatrix->add(new List1DAdapter<T>(other.getRow(i)));
        }
    }
    return *this;
}

template <typename T>
ostream &operator<<(ostream &os, const List2D<T> &matrix)
{
    // TODO
    os << matrix.toString();
    return os;
}

// -------------------- InventoryManager Method Definitions --------------------
inline InventoryManager::InventoryManager()
    : attributesMatrix(), productNames(), quantities()
{
}

inline InventoryManager::InventoryManager(const List2D<InventoryAttribute> &matrix,
                                   const List1D<string> &names,
                                   const List1D<int> &quantities)
    : attributesMatrix(matrix), productNames(names), quantities(quantities)
{
}

inline InventoryManager::InventoryManager(const InventoryManager &other)
    : attributesMatrix(other.attributesMatrix),
      productNames(other.productNames),
      quantities(other.quantities)
{
}

inline int InventoryManager::size() const
{
    // TODO
    return productNames.size();
}

inline List1D<InventoryAttribute> InventoryManager::getProductAttributes(int index) const
{
    // TODO
    if (index < 0 || index >= size())
        throw std::out_of_range("Index is invalid!");
    return attributesMatrix.getRow(index);
}

inline string InventoryManager::getProductName(int index) const
{
    // TODO
    if (index < 0 || index >= size())
        throw std::out_of_range("Index is invalid!");
    return productNames.get(index);
}

inline int InventoryManager::getProductQuantity(int index) const
{
    // TODO
    if (index < 0 || index >= size())
        throw std::out_of_range("Index is invalid!");
    return quantities.get(index);
}

inline void InventoryManager::updateQuantity(int index, int newQuantity)
{
    // TODO
    if (index < 0 || index >= size())
        throw std::out_of_range("Index is invalid!");
    quantities.set(index, newQuantity);
}

inline void InventoryManager::addProduct(const List1D<InventoryAttribute> &attributes, const string &name, int quantity)
{
    // TODO
    attributesMatrix.addRow(attributes);
    productNames.add(name);
    quantities.add(quantity);
}

inline void InventoryManager::removeProduct(int index)
{
    // TODO
    if (index < 0 || index >= size())
        throw out_of_range("Index is invalid!");
    attributesMatrix.removeRow(index);
    productNames.removeAt(index);
    quantities.removeAt(index);
}

inline List1D<string> InventoryManager::query(string attributeName, const double &minValue,
                                       const double &maxValue, int minQuantity, bool ascending) const
{
    List1D<string> result;
    List1D<double> resultValue;
    List1D<int> resultQuantity;

    for (int i = 0; i < size(); i++)
    {
        List1D<InventoryAttribute> attributes = attributesMatrix.getRow(i);
        int attributeIndex = -1;

        for (int j = 0; j < attributes.size(); j++)
        {
            if (attributes.get(j).name == attributeName)
            {
                attributeIndex = j;
                break;
            }
        }

        if (attributeIndex == -1)
        {
            continue;
        }

        double attributeValue = attributes.get(attributeIndex).value;

        if (attributeValue >= minValue && attributeValue <= maxValue && quantities.get(i) >= minQuantity)
        {
            result.add(productNames.get(i));
            resultValue.add(attributeValue);
            resultQuantity.add(quantities.get(i));
        }
    }

    for (int i = 0; i < result.size() - 1; i++)
    {
        for (int j = 0; j < result.size() - i - 1; j++)
        {
            bool shouldSwap = false;

            if (ascending)
            {
                if (resultValue.get(j) > resultValue.get(j + 1))
                {
                    shouldSwap = true;
                }
                else if (resultValue.get(j) == resultValue.get(j + 1) && resultQuantity.get(j) > resultQuantity.get(j + 1))
                {
                    shouldSwap = true;
                }
            }
            else
            {
                if (resultValue.get(j) < resultValue.get(j + 1))
                {
                    shouldSwap = true;
                }
                else if (resultValue.get(j) == resultValue.get(j + 1))
                {
                    if (resultQuantity.get(j) < resultQuantity.get(j + 1))
                        shouldSwap = true;
                    else if (resultQuantity.get(j) == resultQuantity.get(j + 1))
                        shouldSwap = true;
                }
            }

            if (shouldSwap)
            {
                string tempName = result.get(j);
                result.set(j, result.get(j + 1));
                result.set(j + 1, tempName);

                double tempValue = resultValue.get(j);
                resultValue.set(j, resultValue.get(j + 1));
                resultValue.set(j + 1, tempValue);

                int tempQuantity = resultQuantity.get(j);
                resultQuantity.set(j, resultQuantity.get(j + 1));
                resultQuantity.set(j + 1, tempQuantity);
            }
        }
    }

    return result;
}
inline void InventoryManager::removeDuplicates()
{
    // TODO
    for (int i = 0; i < productNames.size(); i++)
    {
        for (int j = i + 1; j < productNames.size();)
        {
            if (productNames.get(i) == productNames.get(j) && attributesMatrix.getRow(i).toString() == attributesMatrix.getRow(j).toString())
            {
                int newQuantity = quantities.get(i) + quantities.get(j);
                quantities.set(i, newQuantity);

                attributesMatrix.removeRow(j);
                productNames.removeAt(j);
                quantities.removeAt(j);
            }

            else
            {
                j++;
            }
        }
    }
}

inline InventoryManager InventoryManager::merge(const InventoryManager &inv1,
                                         const InventoryManager &inv2)
{
    // TODO
    InventoryManager mergeInventory;
    for (int i = 0; i < inv1.productNames.size(); i++)
    {
        mergeInventory.addProduct(inv1.attributesMatrix.getRow(i), inv1.productNames.get(i), inv1.quantities.get(i));
    }
    for (int i = 0; i < inv2.productNames.size(); i++)
    {
        mergeInventory.addProduct(inv2.attributesMatrix.getRow(i), inv2.productNames.get(i), inv2.quantities.get(i));
    }
    // mergeInventory.removeDuplicates();
    return mergeInventory;
}

inline double myCeil(double x)
{
    int intPart = static_cast<int>(x);
    return (x > intPart) ? intPart + 0.99999 : intPart;
}

inline void InventoryManager::split(InventoryManager &section1,
                             InventoryManager &section2,
                             double ratio) const
{
    // TODO
    section1.attributesMatrix = List2D<InventoryAttribute>();
    section1.productNames = List1D<string>();
    section1.quantities = List1D<int>();

    section2.attributesMatrix = List2D<InventoryAttribute>();
    section2.productNames = List1D<string>();
    section2.quantities = List1D<int>();
    double finalratio;
    if (ratio < 0)
        finalratio = 0;
    else if (ratio > 1)
        finalratio = 1;
    else
        finalratio = ratio;
    int totalProducts = productNames.size();
    int section1size = myCeil((int)(totalProducts * finalratio + 0.9999));
    int section2size = totalProducts - section1size;
    for (int i = 0; i < section1size; i++)
    {
        section1.addProduct(attributesMatrix.getRow(i), productNames.get(i), quantities.get(i));
    }
    for (int i = section1size; i < totalProducts; i++)
    {
        section2.addProduct(attributesMatrix.getRow(i), productNames.get(i), quantities.get(i));
    }
}
inline List2D<InventoryAttribute> InventoryManager::getAttributesMatrix() const
{
    // TODO
    return attributesMatrix;
}

inline List1D<string> InventoryManager::getProductNames() const
{
    // TODO
    return productNames;
}

inline List1D<int> InventoryManager::getQuantities() const
{
    // TODO
    return quantities;
}
/*template <typename T>
ostream &operator<<(ostream &os, const InventoryAttribute &attr) {
    os << attr.toString();
    return os;
}*/

inline string InventoryManager::toString() const
{
    // TODO
    ostringstream oss;
    oss << "InventoryManager[\n ";
    oss << " AttributesMatrix: [";
    for (int i = 0; i < attributesMatrix.rows(); i++)
    {
        oss << "[";
        List1D<InventoryAttribute> row = attributesMatrix.getRow(i);
        for (int j = 0; j < row.size(); j++)
        {
            oss << row.get(j).toString();
            if (j < row.size() - 1)
            {
                oss << ", ";
            }
        }
        oss << "]";
        if (i < attributesMatrix.rows() - 1)
        {
            oss << ", ";
        }
    }
    oss << "],\n";
    oss << "  ProductNames: [";
    for (int i = 0; i < productNames.size(); i++)
    {
        oss << productNames.get(i);
        if (i < productNames.size() - 1)
        {
            oss << ", ";
        }
    }
    oss << "],\n";
    oss << "  Quantities: [";
    for (int i = 0; i < quantities.size(); i++)
    {
        oss << quantities.get(i);
        if (i < quantities.size() - 1)
        {
            oss << ", ";
        }
    }
    oss << "]\n";
    oss << "]";
    return oss.str();
}

#endif /* INVENTORY_MANAGER_H */
