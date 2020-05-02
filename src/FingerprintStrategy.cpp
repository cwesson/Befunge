/**
 * @file FingerprintStrategy.cpp
 * Strategy for fingerprint commands.
 * @author Conlan Wesson
 */

#include "FingerprintStrategy.h"
#include "FingerprintBITW.h"
#include "FingerprintBOOL.h"
#include "FingerprintHRTI.h"
#include "FingerprintMODU.h"
#include "FingerprintNULL.h"
#include "FingerprintORTH.h"
#include "FingerprintREFC.h"
#include "FingerprintROMA.h"

namespace Funge {

FingerprintStrategy::FingerprintStrategy(Field& f, InstructionPointer& i, StackStack& s) :
	FungeStrategy(f, i, s),
	available(),
	loaded()
{
	available[0x42495457] = new FingerprintBITW(f, i, s);
	available[0x424F4F4C] = new FingerprintBOOL(f, i, s);
	available[0x48525449] = new FingerprintHRTI(f, i, s);
	available[0x4d4f4455] = new FingerprintMODU(f, i, s);
	available[0x4e554c4c] = new FingerprintNULL(f, i, s);
	available[0x4f525448] = new FingerprintORTH(f, i, s);
	available[0x52454643] = new FingerprintREFC(f, i, s);
	available[0x524f4d41] = new FingerprintROMA(f, i, s);
}

FingerprintStrategy::~FingerprintStrategy(){
	for(auto f : available){
		delete f.second;
	}
}

bool FingerprintStrategy::execute(inst_t cmd){
	bool done = false;
	auto found = loaded.find(cmd);
	if(found != loaded.cend()){
		done = found->second.top()->execute(cmd);
	}
	return done;
}

bool FingerprintStrategy::load(uint64_t fingerprint){
	auto found = available.find(fingerprint);
	if(found != available.cend()){
		for(auto i : found->second->instructions()){
			auto exist = loaded.find(i);
			if(exist == loaded.cend()){
				loaded[i] = std::stack<Fingerprint*>();
			}
			loaded[i].push(found->second);
		}
		return true;
	}
	return false;
}

bool FingerprintStrategy::unload(uint64_t fingerprint){
	auto found = available.find(fingerprint);
	if(found != available.cend()){
		for(auto i : found->second->instructions()){
			auto exist = loaded.find(i);
			if(exist != loaded.cend()){
				exist->second.pop();
			}
		}
		return true;
	}
	return false;
}

}