#include "Target.h"
#include <optional>
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
