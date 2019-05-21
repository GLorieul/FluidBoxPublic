
//The purpose of the Hdf5 wrapper is to provide a simple interface to the
//HDF5 C++ API, because the C++ API is too verbose.
//It does aim at covering all of HDF5's functionalities,
//but only those needed by FluidBox (so far).

#include <initializer_list>
#include <vector>
#include "H5Cpp.h"

namespace fb
{

class Hdf5Location
{
public:
    // Method(s) createAttribute_() are protected and hence hidden away
    // whereas the createAttribute() methods are accessible.
    // This is so as to restrict the types allowed,
    // and force conversion where needed.
    // For instance calling createAttribute("abc", Point(1.0,2.0)) will
    // force the conversion from Point to std::vector<double>
    void createAttribute(std::string name, int data)
        { createAttribute_<int>(name, data); }
    void createAttribute(std::string name, double data)
        { createAttribute_<double>(name, data); }
    void createAttribute(std::string name, std::string data)
        { createAttribute_<const char*>(name, data.c_str()); }
    // Data is 1D array
    void createAttribute(std::string name, std::vector<int> data)
        { createAttribute_<int>(name, data); }
    void createAttribute(std::string name, std::vector<double> data)
        { createAttribute_<double>(name, data); }

    //Create an attribute as a scalar value
    template<typename data_T>
    void createAttribute(std::string name, data_T data, std::string units)
    {
        createAttribute(name, data);
        createAttribute(name + " units", units);
    }

    //Create an attribute as a 1D array
    template<typename data_T>
    void createAttribute(std::string name,
                         std::vector<data_T> data,
                         std::string units)
    {
        createAttribute(name, data);
        createAttribute(name + " units", units);
    }

protected:
    //Create an attribute as a scalar value
    template<typename data_T>
    void createAttribute_(std::string name, data_T data)
    {
        data_T data_arr[1] = {data};
        H5::Attribute attribute = getH5Location().createAttribute(
                name, getH5Type<data_T>(), H5::DataSpace(H5S_SCALAR));
        attribute.write( getH5Type<data_T>(), data_arr);
    }
    //Create an attribute as a 1D array
    template<typename data_T>
    void createAttribute_(std::string name,
                         std::vector<data_T> data)
    {
        hsize_t dims[1] = {data.size()};
        H5::Attribute attribute = getH5Location().createAttribute(
                name, getH5Type<data_T>(), H5::DataSpace(H5S_SIMPLE,dims));
        attribute.write( getH5Type<data_T>(), data.data());
    }

    //Custom type for variable-length null-terminated ASCII strings
    //(there is no "H5::PreType" for such strings:
    //see libhdf5-doc/html/_topic/create_vlen_strings.htm)
    static const H5::StrType string_type;

    //Binds typename in template arguments to an actual HDF5-valid type
    template<typename data_T> H5::DataType getH5Type()
    { raiseError(__FILE__, __LINE__, __FUNCTION__, "Illegal or unsupported type."); } //[ToDo] display the type, if that is possible?

    virtual H5::H5Location& getH5Location() = 0;
};

class Hdf5Group;
class Hdf5CommonFG
{
public:
    Hdf5Group createGroup(std::string groupName);

protected:
    virtual H5::CommonFG& getCommonFG() = 0;
};

const H5::StrType Hdf5Location::string_type = H5::StrType(H5T_C_S1, H5T_VARIABLE);

template<> H5::DataType Hdf5Location::getH5Type<int>()               { return H5::PredType::NATIVE_INT; }
template<> H5::DataType Hdf5Location::getH5Type<const int>()         { return H5::PredType::NATIVE_INT; }
template<> H5::DataType Hdf5Location::getH5Type<double>()            { return H5::PredType::NATIVE_DOUBLE; }
template<> H5::DataType Hdf5Location::getH5Type<const double>()      { return H5::PredType::NATIVE_DOUBLE; }
template<> H5::DataType Hdf5Location::getH5Type<char*>()             { return string_type; }
template<> H5::DataType Hdf5Location::getH5Type<const char*>()       { return string_type; }
template<> H5::DataType Hdf5Location::getH5Type<char[]>()            { return string_type; }
template<> H5::DataType Hdf5Location::getH5Type<const char[]>()      { return string_type; }
template<> H5::DataType Hdf5Location::getH5Type<std::string>()       { return string_type; }
template<> H5::DataType Hdf5Location::getH5Type<const std::string>() { return string_type; }

class Hdf5DataSet : public Hdf5Location
{
    public:
        Hdf5DataSet() = default;
        Hdf5DataSet(H5::DataSet other) : dataset_(other) {/*Do nothing*/}
        H5::DataSet getH5Object() { return dataset_; }
    protected:
        virtual H5::H5Location& getH5Location() { return dataset_; }
        H5::DataSet dataset_;
};

class Hdf5Group : public Hdf5Location, public Hdf5CommonFG
{
public:
    Hdf5Group() = default;
    Hdf5Group(H5::Group group) : group_(group) {/*Do nothing*/}

    //Create a 1D dataset from a (1D) C-array
    template<typename data_T>
    Hdf5DataSet createDatasetFromCArray(std::string name, data_T* data,
                                          size_t dataSize)
    {
        constexpr int rank = 1; //Is a one-dimensional array
        hsize_t dims[1] = {dataSize};
        H5::DataSet dataset = group_.createDataSet(
                name, getH5Type<data_T>(), H5::DataSpace(rank,dims));
        dataset.write(data, getH5Type<data_T>());
        return Hdf5DataSet(dataset);
    }

    //Create a n-dimensional dataset from a (1D) C-array
    template<typename data_T>
    Hdf5DataSet createDatasetFromCArray(std::string name, data_T* data,
                                          std::vector<hsize_t> shape_li)
    {
        std::vector<hsize_t> shape(shape_li);
        int rank = shape.size(); //Nb of dimensions
        H5::DataSet dataset = group_.createDataSet(
                name, getH5Type<data_T>(), H5::DataSpace(rank,shape.data()));
        dataset.write(data, getH5Type<data_T>());
        return Hdf5DataSet(dataset);
    }

protected:

    virtual H5::H5Location& getH5Location() { return group_; }
    virtual H5::CommonFG&   getCommonFG() { return group_; }

    H5::Group group_;
};


class Hdf5File : public Hdf5Location, public Hdf5CommonFG
{
public:
    Hdf5File() = default;
    Hdf5File(const std::string fileName)
        :file_( fileName, H5F_ACC_EXCL) //Fail if file already exists (See file:///usr/share/doc/libhdf5-doc/html/cpplus_RM/class_h5_1_1_h5_file.html)
    { /*Do nothing*/ }

    //Create a group
    Hdf5Group createGroup(std::string groupName)
        { return Hdf5Group( file_.createGroup(groupName) ); }

    Hdf5Group getRoot() { return file_.openGroup("/"); }
    H5::H5File& getFile() { return file_; }

private:
    virtual H5::H5Location& getH5Location() { return file_; }
    virtual H5::CommonFG&   getCommonFG()   { return file_; }
    H5::H5File file_;
};

Hdf5Group Hdf5CommonFG::createGroup(std::string groupName)
    { return Hdf5Group( getCommonFG().createGroup(groupName) ); }

}

