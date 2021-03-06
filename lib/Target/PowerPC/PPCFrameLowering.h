//===-- PPCFrameLowering.h - Define frame lowering for PowerPC --*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_POWERPC_PPCFRAMELOWERING_H
#define LLVM_LIB_TARGET_POWERPC_PPCFRAMELOWERING_H

#include "PPC.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Target/TargetFrameLowering.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class PPCSubtarget;

class PPCFrameLowering: public TargetFrameLowering {
  const PPCSubtarget &Subtarget;
  const unsigned ReturnSaveOffset;
  const unsigned TOCSaveOffset;
  const unsigned FramePointerSaveOffset;
  const unsigned LinkageSize;
  const unsigned BasePointerSaveOffset;

public:
  PPCFrameLowering(const PPCSubtarget &STI);

  unsigned determineFrameLayout(MachineFunction &MF,
                                bool UpdateMF = true,
                                bool UseEstimate = false) const;

  /// emitProlog/emitEpilog - These methods insert prolog and epilog code into
  /// the function.
  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  bool hasFP(const MachineFunction &MF) const override;
  bool needsFP(const MachineFunction &MF) const;
  void replaceFPWithRealFP(MachineFunction &MF) const;

  void determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs,
                            RegScavenger *RS = nullptr) const override;
  void processFunctionBeforeFrameFinalized(MachineFunction &MF,
                                     RegScavenger *RS = nullptr) const override;
  void addScavengingSpillSlot(MachineFunction &MF, RegScavenger *RS) const;

  bool spillCalleeSavedRegisters(MachineBasicBlock &MBB,
                                 MachineBasicBlock::iterator MI,
                                 const std::vector<CalleeSavedInfo> &CSI,
                                 const TargetRegisterInfo *TRI) const override;

  void eliminateCallFramePseudoInstr(MachineFunction &MF,
                                  MachineBasicBlock &MBB,
                                  MachineBasicBlock::iterator I) const override;

  bool restoreCalleeSavedRegisters(MachineBasicBlock &MBB,
                                  MachineBasicBlock::iterator MI,
                                  const std::vector<CalleeSavedInfo> &CSI,
                                  const TargetRegisterInfo *TRI) const override;

  /// targetHandlesStackFrameRounding - Returns true if the target is
  /// responsible for rounding up the stack frame (probably at emitPrologue
  /// time).
  bool targetHandlesStackFrameRounding() const override { return true; }

  /// getReturnSaveOffset - Return the previous frame offset to save the
  /// return address.
  unsigned getReturnSaveOffset() const { return ReturnSaveOffset; }

  /// getTOCSaveOffset - Return the previous frame offset to save the
  /// TOC register -- 64-bit SVR4 ABI only.
  unsigned getTOCSaveOffset() const { return TOCSaveOffset; }

  /// getFramePointerSaveOffset - Return the previous frame offset to save the
  /// frame pointer.
  unsigned getFramePointerSaveOffset() const { return FramePointerSaveOffset; }

  /// getBasePointerSaveOffset - Return the previous frame offset to save the
  /// base pointer.
  unsigned getBasePointerSaveOffset() const { return BasePointerSaveOffset; }

  /// getLinkageSize - Return the size of the PowerPC ABI linkage area.
  ///
  unsigned getLinkageSize() const { return LinkageSize; }

  const SpillSlot *
  getCalleeSavedSpillSlots(unsigned &NumEntries) const override;
};
} // End llvm namespace

#endif
