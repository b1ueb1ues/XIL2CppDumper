//
// Created by zhangshun on 2019/11/5.
//

#ifndef XIL2CPPDUMPER_XIL2CPPDUMPER_H
#define XIL2CPPDUMPER_XIL2CPPDUMPER_H

#include <string>
#include <unistd.h>
#include <fstream>
#include "xia0.h"
#import "IL2CppBinParser.h"

using namespace std;

#if DLOG
#define DLOG(log, ...)  printf("[debug] " log, ##__VA_ARGS__)
#else
#define DLOG(log, ...)
#endif

class XIL2CppDumper
{

private:
    XIL2CppDumper(){}
    static XIL2CppDumper *m_pInstance;
public:
    // metadata field
    void* metadata;
    const Il2CppGlobalMetadataHeader* metadataHeader;
    uint32_t metadataVersion;

    const Il2CppImageDefinition* metadataImageDefinitionTable;
    const Il2CppTypeDefinition* metadataTypeDefinitionTable;
    const Il2CppFieldDefinition* metadataFieldDefinitionTable;
    const Il2CppFieldDefaultValue* metadataFieldDefaultValueTable;
    const Il2CppPropertyDefinition* metadataPropertyTable;
    const Il2CppMethodDefinition* metadataMethodDefinitionTable;
    const Il2CppParameterDefinition* metadataParameterDefinitionTable;

    uint32_t* metadataInterfaceTable;

    // il2cpp field
    const Il2CppType** g_Il2CppTypeTable;
    int32_t g_Il2CppTypeTableCount;
    Il2CppMethodPointer * g_MethodPointers;
    uint32_t methodPointersCount;
    void** const * g_MetadataUsages;
    uint32_t metadataUsagesCount;

    // misc field
    ofstream outfile;
    ofstream scriptfile;

    // binary file
    IL2CppBinParser* binParser;
    void* il2cppbin;
    const Il2CppCodeRegistration* g_CodeRegistration;
    const Il2CppMetadataRegistration* g_MetadataRegistration;

    // single instance
    static XIL2CppDumper * GetInstance();
    void initMetadata(const char* metadataFile, const char* il2cpBinFile);

    // execute format file
    void* idaAddr2MemAddr(void* idaAddr);
    uint64_t RVA2RAW(void* rva); // convert ida adress to file offset
    void* RAW2RVA(uint64_t raw); // convert file offset to ida address

    // il2cpp function
    const Il2CppType* getTypeFromIl2CppTypeTableByIndex(TypeIndex index);
    Il2CppMethodPointer getMethodPointerFromMethodPointersByIndex(MethodIndex index);
    void* getMethodPointerIDAValueByIndex(MethodIndex index);
    void* getUsageAddressIDAValueByIndex(uint32_t index);

    string typeStringForID(int id);
    string getTypeName(const Il2CppType* type);
    string getTypeDefinitionName(const Il2CppTypeDefinition* typeDefinition);
    string getGenericTypeParams(const Il2CppGenericInst* genericInst);
    string getMethodAttribute(const Il2CppMethodDefinition* methodDef);

    const char* getStringByIndex(StringIndex index);
    char* getStringLiteralFromIndex(StringIndex index);
    const Il2CppImageDefinition* getImageDefinitionByIndex(ImageIndex index);
    const Il2CppTypeDefinition* getTypeDefinitionByIndex(TypeDefinitionIndex index);
    const Il2CppFieldDefinition* getFieldDefinitionByIndex(FieldIndex index);
    const Il2CppFieldDefaultValue* getFieldDefaultValueByIndex(DefaultValueIndex index);
    const Il2CppPropertyDefinition* getPropertyDefinitionByIndex(PropertyIndex index);
    const Il2CppMethodDefinition* getMethodDefinitionByIndex(MethodIndex index);
    const Il2CppParameterDefinition* getParameterDefinitionByIndex(ParameterIndex index);

    // misc
    const char* toEscapedString(char* str);
    inline string format(const char* fmt, ...);
    void write2File(string str);
    void write2Script(string str);
    void initScript();
    void clean();

    // dump
    void dumpUsage();
    void dumpAllImages();
    void dumpString();
    void dump();

    template <typename T>
    static T MetadataOffset(void* metadata, size_t sectionOffset, size_t itemIndex)
    {
        return reinterpret_cast<T> (reinterpret_cast<uint8_t*> (metadata) + sectionOffset) + itemIndex;
    }
};

#endif //XIL2CPPDUMPER_XIL2CPPDUMPER_H
