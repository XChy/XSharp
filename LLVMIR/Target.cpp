#include "Target.h"
#include <cstdlib>
#include <optional>
#include <system_error>
#include <llvm-14/llvm/Support/FileSystem.h>
#include <llvm-14/llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/MC/TargetRegistry.h>

std::string target_triple() { return LLVM_DEFAULT_TARGET_TRIPLE; }

std::string data_layout()
{
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    std::string error;
    auto target = llvm::TargetRegistry::lookupTarget(target_triple(), error);

    if (!target) {
        llvm::errs() << error;
        return "";
    }

    auto CPU = "generic";
    auto features = "";

    llvm::TargetOptions opt;
    auto RM = llvm::Optional<llvm::Reloc::Model>();

    auto targetMachine =
        target->createTargetMachine(target_triple(), CPU, features, opt, RM);
    auto datalayout =
        targetMachine->createDataLayout().getStringRepresentation();
    delete targetMachine;
    return datalayout;
}

std::error_code emit_object_code(const std::string& path, llvm::Module& module)
{
    std::string error;
    std::error_code error_code;
    auto target = llvm::TargetRegistry::lookupTarget(target_triple(), error);

    if (!target) {
        llvm::errs() << error;
        return std::error_code();
    }

    auto CPU = "generic";
    auto features = "";

    llvm::TargetOptions opt;
    auto RM = llvm::Optional<llvm::Reloc::Model>();

    auto targetMachine =
        target->createTargetMachine(target_triple(), CPU, features, opt, RM);

    llvm::raw_fd_ostream dest(path, error_code, llvm::sys::fs::OF_None);
    if (error_code) {
        llvm::errs() << "Could not open file: " << error_code.message();
        delete targetMachine;
        return error_code;
    }
    llvm::legacy::PassManager pass;
    auto FileType = llvm::CGFT_ObjectFile;

    if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
        llvm::errs() << "TargetMachine can't emit a file of this type";
        delete targetMachine;
        return error_code;
    }

    pass.run(module);
    dest.flush();

    delete targetMachine;
    return error_code;
}

int link_object(const std::string& object_path, const std::string& lib_path,
                const std::string& exe_path)
{
    return system(
        fmt::format("gcc {} {} -o {}", object_path, lib_path, exe_path)
            .c_str());
}
