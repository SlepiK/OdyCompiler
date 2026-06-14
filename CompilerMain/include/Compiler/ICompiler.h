#ifndef SIERRA_ODYCOMPILER_V1_COMPILER_ICOMPILER_H
#define SIERRA_ODYCOMPILER_V1_COMPILER_ICOMPILER_H

namespace Sierra::OdyCompiler::v1::Compiler {
    class ICompiler {
    public:
        virtual ~ICompiler() = default;
        //virtual void compile() = 0;
        //virtual void setCallArguments(int argc, char* argv[]) = 0;
    };
} // Sierra::OdyCompiler::v1::Compiler

#endif //SIERRA_ODYCOMPILER_V1_COMPILER_ICOMPILER_H