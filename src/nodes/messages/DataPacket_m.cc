//
// Generated file, do not edit! Created by nedtool 5.3 from nodes/messages/DataPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "DataPacket_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

EXECUTE_ON_STARTUP(
    omnetpp::cEnum *e = omnetpp::cEnum::find("DataPacketType");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("DataPacketType"));
    e->insert(DP_ERROR, "DP_ERROR");
    e->insert(DP_DATA, "DP_DATA");
    e->insert(DP_FEEDBACK, "DP_FEEDBACK");
    e->insert(DP_REQ, "DP_REQ");
    e->insert(DP_EOF, "DP_EOF");
    e->insert(DP_BROADCAST, "DP_BROADCAST");
    e->insert(DP_CONFIRM, "DP_CONFIRM");
)

EXECUTE_ON_STARTUP(
    omnetpp::cEnum *e = omnetpp::cEnum::find("ErrorType");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("ErrorType"));
    e->insert(DP_ERR_NOTFOUND, "DP_ERR_NOTFOUND");
    e->insert(DP_ERR_OUTOFRANGE, "DP_ERR_OUTOFRANGE");
    e->insert(DP_ERR_UNKNOWN, "DP_ERR_UNKNOWN");
)

Register_Class(ErrorDataPacket)

ErrorDataPacket::ErrorDataPacket(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->fileId = 0;
    this->error = 0;
}

ErrorDataPacket::ErrorDataPacket(const ErrorDataPacket& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

ErrorDataPacket::~ErrorDataPacket()
{
}

ErrorDataPacket& ErrorDataPacket::operator=(const ErrorDataPacket& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void ErrorDataPacket::copy(const ErrorDataPacket& other)
{
    this->fileId = other.fileId;
    this->error = other.error;
}

void ErrorDataPacket::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->fileId);
    doParsimPacking(b,this->error);
}

void ErrorDataPacket::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->fileId);
    doParsimUnpacking(b,this->error);
}

int ErrorDataPacket::getFileId() const
{
    return this->fileId;
}

void ErrorDataPacket::setFileId(int fileId)
{
    this->fileId = fileId;
}

int ErrorDataPacket::getError() const
{
    return this->error;
}

void ErrorDataPacket::setError(int error)
{
    this->error = error;
}

class ErrorDataPacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    ErrorDataPacketDescriptor();
    virtual ~ErrorDataPacketDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(ErrorDataPacketDescriptor)

ErrorDataPacketDescriptor::ErrorDataPacketDescriptor() : omnetpp::cClassDescriptor("ErrorDataPacket", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

ErrorDataPacketDescriptor::~ErrorDataPacketDescriptor()
{
    delete[] propertynames;
}

bool ErrorDataPacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<ErrorDataPacket *>(obj)!=nullptr;
}

const char **ErrorDataPacketDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *ErrorDataPacketDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int ErrorDataPacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount() : 2;
}

unsigned int ErrorDataPacketDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *ErrorDataPacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "fileId",
        "error",
    };
    return (field>=0 && field<2) ? fieldNames[field] : nullptr;
}

int ErrorDataPacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "fileId")==0) return base+0;
    if (fieldName[0]=='e' && strcmp(fieldName, "error")==0) return base+1;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *ErrorDataPacketDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : nullptr;
}

const char **ErrorDataPacketDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 1: {
            static const char *names[] = { "enum",  nullptr };
            return names;
        }
        default: return nullptr;
    }
}

const char *ErrorDataPacketDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 1:
            if (!strcmp(propertyname,"enum")) return "ErrorType";
            return nullptr;
        default: return nullptr;
    }
}

int ErrorDataPacketDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    ErrorDataPacket *pp = (ErrorDataPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *ErrorDataPacketDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    ErrorDataPacket *pp = (ErrorDataPacket *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string ErrorDataPacketDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    ErrorDataPacket *pp = (ErrorDataPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getFileId());
        case 1: return enum2string(pp->getError(), "ErrorType");
        default: return "";
    }
}

bool ErrorDataPacketDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    ErrorDataPacket *pp = (ErrorDataPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setFileId(string2long(value)); return true;
        case 1: pp->setError((ErrorType)string2enum(value, "ErrorType")); return true;
        default: return false;
    }
}

const char *ErrorDataPacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *ErrorDataPacketDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    ErrorDataPacket *pp = (ErrorDataPacket *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(FileDataPacket)

FileDataPacket::FileDataPacket(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->fileId = 0;
    this->blockId = 0;
}

FileDataPacket::FileDataPacket(const FileDataPacket& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

FileDataPacket::~FileDataPacket()
{
}

FileDataPacket& FileDataPacket::operator=(const FileDataPacket& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void FileDataPacket::copy(const FileDataPacket& other)
{
    this->fileId = other.fileId;
    this->blockId = other.blockId;
}

void FileDataPacket::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->fileId);
    doParsimPacking(b,this->blockId);
}

void FileDataPacket::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->fileId);
    doParsimUnpacking(b,this->blockId);
}

int FileDataPacket::getFileId() const
{
    return this->fileId;
}

void FileDataPacket::setFileId(int fileId)
{
    this->fileId = fileId;
}

int FileDataPacket::getBlockId() const
{
    return this->blockId;
}

void FileDataPacket::setBlockId(int blockId)
{
    this->blockId = blockId;
}

class FileDataPacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    FileDataPacketDescriptor();
    virtual ~FileDataPacketDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(FileDataPacketDescriptor)

FileDataPacketDescriptor::FileDataPacketDescriptor() : omnetpp::cClassDescriptor("FileDataPacket", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

FileDataPacketDescriptor::~FileDataPacketDescriptor()
{
    delete[] propertynames;
}

bool FileDataPacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<FileDataPacket *>(obj)!=nullptr;
}

const char **FileDataPacketDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *FileDataPacketDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int FileDataPacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount() : 2;
}

unsigned int FileDataPacketDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *FileDataPacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "fileId",
        "blockId",
    };
    return (field>=0 && field<2) ? fieldNames[field] : nullptr;
}

int FileDataPacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "fileId")==0) return base+0;
    if (fieldName[0]=='b' && strcmp(fieldName, "blockId")==0) return base+1;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *FileDataPacketDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : nullptr;
}

const char **FileDataPacketDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *FileDataPacketDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int FileDataPacketDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    FileDataPacket *pp = (FileDataPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *FileDataPacketDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    FileDataPacket *pp = (FileDataPacket *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string FileDataPacketDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    FileDataPacket *pp = (FileDataPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getFileId());
        case 1: return long2string(pp->getBlockId());
        default: return "";
    }
}

bool FileDataPacketDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    FileDataPacket *pp = (FileDataPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setFileId(string2long(value)); return true;
        case 1: pp->setBlockId(string2long(value)); return true;
        default: return false;
    }
}

const char *FileDataPacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *FileDataPacketDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    FileDataPacket *pp = (FileDataPacket *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(RequestDataPacket)

RequestDataPacket::RequestDataPacket(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->fileId = 0;
    this->startBlockId = 0;
}

RequestDataPacket::RequestDataPacket(const RequestDataPacket& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

RequestDataPacket::~RequestDataPacket()
{
}

RequestDataPacket& RequestDataPacket::operator=(const RequestDataPacket& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void RequestDataPacket::copy(const RequestDataPacket& other)
{
    this->fileId = other.fileId;
    this->startBlockId = other.startBlockId;
}

void RequestDataPacket::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->fileId);
    doParsimPacking(b,this->startBlockId);
}

void RequestDataPacket::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->fileId);
    doParsimUnpacking(b,this->startBlockId);
}

int RequestDataPacket::getFileId() const
{
    return this->fileId;
}

void RequestDataPacket::setFileId(int fileId)
{
    this->fileId = fileId;
}

int RequestDataPacket::getStartBlockId() const
{
    return this->startBlockId;
}

void RequestDataPacket::setStartBlockId(int startBlockId)
{
    this->startBlockId = startBlockId;
}

class RequestDataPacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    RequestDataPacketDescriptor();
    virtual ~RequestDataPacketDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(RequestDataPacketDescriptor)

RequestDataPacketDescriptor::RequestDataPacketDescriptor() : omnetpp::cClassDescriptor("RequestDataPacket", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

RequestDataPacketDescriptor::~RequestDataPacketDescriptor()
{
    delete[] propertynames;
}

bool RequestDataPacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<RequestDataPacket *>(obj)!=nullptr;
}

const char **RequestDataPacketDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *RequestDataPacketDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int RequestDataPacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount() : 2;
}

unsigned int RequestDataPacketDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *RequestDataPacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "fileId",
        "startBlockId",
    };
    return (field>=0 && field<2) ? fieldNames[field] : nullptr;
}

int RequestDataPacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "fileId")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "startBlockId")==0) return base+1;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *RequestDataPacketDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : nullptr;
}

const char **RequestDataPacketDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *RequestDataPacketDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int RequestDataPacketDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    RequestDataPacket *pp = (RequestDataPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *RequestDataPacketDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    RequestDataPacket *pp = (RequestDataPacket *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string RequestDataPacketDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    RequestDataPacket *pp = (RequestDataPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getFileId());
        case 1: return long2string(pp->getStartBlockId());
        default: return "";
    }
}

bool RequestDataPacketDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    RequestDataPacket *pp = (RequestDataPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setFileId(string2long(value)); return true;
        case 1: pp->setStartBlockId(string2long(value)); return true;
        default: return false;
    }
}

const char *RequestDataPacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *RequestDataPacketDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    RequestDataPacket *pp = (RequestDataPacket *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(ConfirmationDataPacket)

ConfirmationDataPacket::ConfirmationDataPacket(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->fileId = 0;
    this->numBlocks = 0;
    this->startBlockId = 0;
}

ConfirmationDataPacket::ConfirmationDataPacket(const ConfirmationDataPacket& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

ConfirmationDataPacket::~ConfirmationDataPacket()
{
}

ConfirmationDataPacket& ConfirmationDataPacket::operator=(const ConfirmationDataPacket& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void ConfirmationDataPacket::copy(const ConfirmationDataPacket& other)
{
    this->fileId = other.fileId;
    this->numBlocks = other.numBlocks;
    this->startBlockId = other.startBlockId;
}

void ConfirmationDataPacket::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->fileId);
    doParsimPacking(b,this->numBlocks);
    doParsimPacking(b,this->startBlockId);
}

void ConfirmationDataPacket::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->fileId);
    doParsimUnpacking(b,this->numBlocks);
    doParsimUnpacking(b,this->startBlockId);
}

int ConfirmationDataPacket::getFileId() const
{
    return this->fileId;
}

void ConfirmationDataPacket::setFileId(int fileId)
{
    this->fileId = fileId;
}

int ConfirmationDataPacket::getNumBlocks() const
{
    return this->numBlocks;
}

void ConfirmationDataPacket::setNumBlocks(int numBlocks)
{
    this->numBlocks = numBlocks;
}

int ConfirmationDataPacket::getStartBlockId() const
{
    return this->startBlockId;
}

void ConfirmationDataPacket::setStartBlockId(int startBlockId)
{
    this->startBlockId = startBlockId;
}

class ConfirmationDataPacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    ConfirmationDataPacketDescriptor();
    virtual ~ConfirmationDataPacketDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(ConfirmationDataPacketDescriptor)

ConfirmationDataPacketDescriptor::ConfirmationDataPacketDescriptor() : omnetpp::cClassDescriptor("ConfirmationDataPacket", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

ConfirmationDataPacketDescriptor::~ConfirmationDataPacketDescriptor()
{
    delete[] propertynames;
}

bool ConfirmationDataPacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<ConfirmationDataPacket *>(obj)!=nullptr;
}

const char **ConfirmationDataPacketDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *ConfirmationDataPacketDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int ConfirmationDataPacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount() : 3;
}

unsigned int ConfirmationDataPacketDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *ConfirmationDataPacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "fileId",
        "numBlocks",
        "startBlockId",
    };
    return (field>=0 && field<3) ? fieldNames[field] : nullptr;
}

int ConfirmationDataPacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "fileId")==0) return base+0;
    if (fieldName[0]=='n' && strcmp(fieldName, "numBlocks")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "startBlockId")==0) return base+2;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *ConfirmationDataPacketDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : nullptr;
}

const char **ConfirmationDataPacketDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *ConfirmationDataPacketDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int ConfirmationDataPacketDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    ConfirmationDataPacket *pp = (ConfirmationDataPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *ConfirmationDataPacketDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    ConfirmationDataPacket *pp = (ConfirmationDataPacket *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string ConfirmationDataPacketDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    ConfirmationDataPacket *pp = (ConfirmationDataPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getFileId());
        case 1: return long2string(pp->getNumBlocks());
        case 2: return long2string(pp->getStartBlockId());
        default: return "";
    }
}

bool ConfirmationDataPacketDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    ConfirmationDataPacket *pp = (ConfirmationDataPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setFileId(string2long(value)); return true;
        case 1: pp->setNumBlocks(string2long(value)); return true;
        case 2: pp->setStartBlockId(string2long(value)); return true;
        default: return false;
    }
}

const char *ConfirmationDataPacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *ConfirmationDataPacketDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    ConfirmationDataPacket *pp = (ConfirmationDataPacket *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(FeedbackDataPacket)

FeedbackDataPacket::FeedbackDataPacket(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->fileId = 0;
    this->blockId = 0;
    this->ack = false;
    this->nextBlockId = 0;
}

FeedbackDataPacket::FeedbackDataPacket(const FeedbackDataPacket& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

FeedbackDataPacket::~FeedbackDataPacket()
{
}

FeedbackDataPacket& FeedbackDataPacket::operator=(const FeedbackDataPacket& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void FeedbackDataPacket::copy(const FeedbackDataPacket& other)
{
    this->fileId = other.fileId;
    this->blockId = other.blockId;
    this->ack = other.ack;
    this->nextBlockId = other.nextBlockId;
}

void FeedbackDataPacket::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->fileId);
    doParsimPacking(b,this->blockId);
    doParsimPacking(b,this->ack);
    doParsimPacking(b,this->nextBlockId);
}

void FeedbackDataPacket::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->fileId);
    doParsimUnpacking(b,this->blockId);
    doParsimUnpacking(b,this->ack);
    doParsimUnpacking(b,this->nextBlockId);
}

int FeedbackDataPacket::getFileId() const
{
    return this->fileId;
}

void FeedbackDataPacket::setFileId(int fileId)
{
    this->fileId = fileId;
}

int FeedbackDataPacket::getBlockId() const
{
    return this->blockId;
}

void FeedbackDataPacket::setBlockId(int blockId)
{
    this->blockId = blockId;
}

bool FeedbackDataPacket::getAck() const
{
    return this->ack;
}

void FeedbackDataPacket::setAck(bool ack)
{
    this->ack = ack;
}

int FeedbackDataPacket::getNextBlockId() const
{
    return this->nextBlockId;
}

void FeedbackDataPacket::setNextBlockId(int nextBlockId)
{
    this->nextBlockId = nextBlockId;
}

class FeedbackDataPacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    FeedbackDataPacketDescriptor();
    virtual ~FeedbackDataPacketDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(FeedbackDataPacketDescriptor)

FeedbackDataPacketDescriptor::FeedbackDataPacketDescriptor() : omnetpp::cClassDescriptor("FeedbackDataPacket", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

FeedbackDataPacketDescriptor::~FeedbackDataPacketDescriptor()
{
    delete[] propertynames;
}

bool FeedbackDataPacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<FeedbackDataPacket *>(obj)!=nullptr;
}

const char **FeedbackDataPacketDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *FeedbackDataPacketDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int FeedbackDataPacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount() : 4;
}

unsigned int FeedbackDataPacketDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *FeedbackDataPacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "fileId",
        "blockId",
        "ack",
        "nextBlockId",
    };
    return (field>=0 && field<4) ? fieldNames[field] : nullptr;
}

int FeedbackDataPacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "fileId")==0) return base+0;
    if (fieldName[0]=='b' && strcmp(fieldName, "blockId")==0) return base+1;
    if (fieldName[0]=='a' && strcmp(fieldName, "ack")==0) return base+2;
    if (fieldName[0]=='n' && strcmp(fieldName, "nextBlockId")==0) return base+3;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *FeedbackDataPacketDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "bool",
        "int",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : nullptr;
}

const char **FeedbackDataPacketDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *FeedbackDataPacketDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int FeedbackDataPacketDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    FeedbackDataPacket *pp = (FeedbackDataPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *FeedbackDataPacketDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    FeedbackDataPacket *pp = (FeedbackDataPacket *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string FeedbackDataPacketDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    FeedbackDataPacket *pp = (FeedbackDataPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getFileId());
        case 1: return long2string(pp->getBlockId());
        case 2: return bool2string(pp->getAck());
        case 3: return long2string(pp->getNextBlockId());
        default: return "";
    }
}

bool FeedbackDataPacketDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    FeedbackDataPacket *pp = (FeedbackDataPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setFileId(string2long(value)); return true;
        case 1: pp->setBlockId(string2long(value)); return true;
        case 2: pp->setAck(string2bool(value)); return true;
        case 3: pp->setNextBlockId(string2long(value)); return true;
        default: return false;
    }
}

const char *FeedbackDataPacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *FeedbackDataPacketDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    FeedbackDataPacket *pp = (FeedbackDataPacket *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(EOFDataPacket)

EOFDataPacket::EOFDataPacket(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->fileId = 0;
}

EOFDataPacket::EOFDataPacket(const EOFDataPacket& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

EOFDataPacket::~EOFDataPacket()
{
}

EOFDataPacket& EOFDataPacket::operator=(const EOFDataPacket& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void EOFDataPacket::copy(const EOFDataPacket& other)
{
    this->fileId = other.fileId;
}

void EOFDataPacket::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->fileId);
}

void EOFDataPacket::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->fileId);
}

int EOFDataPacket::getFileId() const
{
    return this->fileId;
}

void EOFDataPacket::setFileId(int fileId)
{
    this->fileId = fileId;
}

class EOFDataPacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    EOFDataPacketDescriptor();
    virtual ~EOFDataPacketDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(EOFDataPacketDescriptor)

EOFDataPacketDescriptor::EOFDataPacketDescriptor() : omnetpp::cClassDescriptor("EOFDataPacket", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

EOFDataPacketDescriptor::~EOFDataPacketDescriptor()
{
    delete[] propertynames;
}

bool EOFDataPacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<EOFDataPacket *>(obj)!=nullptr;
}

const char **EOFDataPacketDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *EOFDataPacketDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int EOFDataPacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount() : 1;
}

unsigned int EOFDataPacketDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *EOFDataPacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "fileId",
    };
    return (field>=0 && field<1) ? fieldNames[field] : nullptr;
}

int EOFDataPacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "fileId")==0) return base+0;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *EOFDataPacketDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : nullptr;
}

const char **EOFDataPacketDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *EOFDataPacketDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int EOFDataPacketDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    EOFDataPacket *pp = (EOFDataPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *EOFDataPacketDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    EOFDataPacket *pp = (EOFDataPacket *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string EOFDataPacketDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    EOFDataPacket *pp = (EOFDataPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getFileId());
        default: return "";
    }
}

bool EOFDataPacketDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    EOFDataPacket *pp = (EOFDataPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setFileId(string2long(value)); return true;
        default: return false;
    }
}

const char *EOFDataPacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *EOFDataPacketDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    EOFDataPacket *pp = (EOFDataPacket *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(DataPacket)

DataPacket::DataPacket(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->sourceId = 0;
    this->destinationId = 0;
    this->type = 0;
}

DataPacket::DataPacket(const DataPacket& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

DataPacket::~DataPacket()
{
}

DataPacket& DataPacket::operator=(const DataPacket& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void DataPacket::copy(const DataPacket& other)
{
    this->sourceId = other.sourceId;
    this->destinationId = other.destinationId;
    this->type = other.type;
}

void DataPacket::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->sourceId);
    doParsimPacking(b,this->destinationId);
    doParsimPacking(b,this->type);
}

void DataPacket::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->sourceId);
    doParsimUnpacking(b,this->destinationId);
    doParsimUnpacking(b,this->type);
}

int DataPacket::getSourceId() const
{
    return this->sourceId;
}

void DataPacket::setSourceId(int sourceId)
{
    this->sourceId = sourceId;
}

int DataPacket::getDestinationId() const
{
    return this->destinationId;
}

void DataPacket::setDestinationId(int destinationId)
{
    this->destinationId = destinationId;
}

int DataPacket::getType() const
{
    return this->type;
}

void DataPacket::setType(int type)
{
    this->type = type;
}

class DataPacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    DataPacketDescriptor();
    virtual ~DataPacketDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(DataPacketDescriptor)

DataPacketDescriptor::DataPacketDescriptor() : omnetpp::cClassDescriptor("DataPacket", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

DataPacketDescriptor::~DataPacketDescriptor()
{
    delete[] propertynames;
}

bool DataPacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<DataPacket *>(obj)!=nullptr;
}

const char **DataPacketDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *DataPacketDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int DataPacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount() : 3;
}

unsigned int DataPacketDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *DataPacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "sourceId",
        "destinationId",
        "type",
    };
    return (field>=0 && field<3) ? fieldNames[field] : nullptr;
}

int DataPacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourceId")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationId")==0) return base+1;
    if (fieldName[0]=='t' && strcmp(fieldName, "type")==0) return base+2;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *DataPacketDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : nullptr;
}

const char **DataPacketDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 2: {
            static const char *names[] = { "enum",  nullptr };
            return names;
        }
        default: return nullptr;
    }
}

const char *DataPacketDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 2:
            if (!strcmp(propertyname,"enum")) return "DataPacketType";
            return nullptr;
        default: return nullptr;
    }
}

int DataPacketDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    DataPacket *pp = (DataPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *DataPacketDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    DataPacket *pp = (DataPacket *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string DataPacketDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    DataPacket *pp = (DataPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSourceId());
        case 1: return long2string(pp->getDestinationId());
        case 2: return enum2string(pp->getType(), "DataPacketType");
        default: return "";
    }
}

bool DataPacketDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    DataPacket *pp = (DataPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setSourceId(string2long(value)); return true;
        case 1: pp->setDestinationId(string2long(value)); return true;
        case 2: pp->setType((DataPacketType)string2enum(value, "DataPacketType")); return true;
        default: return false;
    }
}

const char *DataPacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *DataPacketDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    DataPacket *pp = (DataPacket *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}


