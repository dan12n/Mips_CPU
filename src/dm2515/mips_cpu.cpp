#include "mips.h"
#include <iostream>
#include "mips_cpu_decoder.h"
#include "mips_cpu_execute.hpp"
#include "mips_cpu_basic_definitions.hpp"
#include "mips_cpu_checks.hpp"

struct mips_cpu_impl {
    uint32_t pc;
    uint32_t regs[32];
    
    uint32_t HI;
    uint32_t LO;
    unsigned debuglevel;
    
    uint32_t pcN;
    int delay_slot;
    
    mips_mem_h mem;
};

mips_cpu_h mips_cpu_create(mips_mem_h mem) {
    if (mem == 0) {
        std::cout << "No Memory Specified" << std::endl;
        return NULL;
    }
    mips_cpu_impl *state= new mips_cpu_impl;
    
    state->pc=0;
    state->pcN=4;
    for (unsigned i=0; i<32; i++){
        state->regs[i]=0;
    }
    state->mem=mem;
    
    return state;
}

void mips_cpu_free(mips_cpu_h state) {
    free(state);
}

mips_error mips_cpu_get_register( mips_cpu_h state, unsigned index, uint32_t *value) {
    if (state == 0) {
        return mips_ErrorInvalidHandle;
    }
    if (index >= 32) {
        return mips_ErrorInvalidArgument;
    }
    if (value == 0) {
        return mips_ErrorInvalidArgument;
    }
    
    *value = state->regs[index];
    return mips_Success;
}

//Modifies one of the registers
mips_error mips_cpu_set_register(mips_cpu_h state, unsigned index, uint32_t value) {
    if (state == 0) {
        return mips_ErrorInvalidHandle;
    }
    
    if(index >= 32) {
        return mips_ErrorInvalidArgument;
    }
    
    if (index == 0) {
        return mips_ErrorInvalidArgument;
    }
    
    state->regs[index] = value;
    
    return mips_Success;
}

mips_error mips_cpu_set_pc(mips_cpu_h state, uint32_t pc) {
    //Check if the address is a multiple of 4
    if (state == 0) {
        return mips_ErrorInvalidHandle;
    }
    
    if (pc%4 == 0) {
        state->pc = pc;
        state->pcN = pc + 4;
        return mips_Success;
    }
    
    return mips_ExceptionInvalidAddress;
}

mips_error mips_cpu_get_pc(mips_cpu_h state, uint32_t *pc) {
    if (state == 0) {
        return mips_ErrorInvalidHandle;
    }
    //Check if the address is a multiple of 4
    //if (((*pc % 4) != 0) && (*pc!=0)) {
    //    return mips_ExceptionInvalidAddress;
    //}
    
    *pc = state->pc;
    
    return mips_Success;
}

mips_error mips_cpu_set_debug_level(mips_cpu_h state, unsigned level, FILE *dest) {
    if (level > 2)
        return mips_ErrorInvalidArgument;
    if (state == 0)
        return mips_ErrorInvalidHandle;
    
    state->debuglevel = level;
    
    if ((level != 0) && (dest == 0)) {
        return mips_ErrorInvalidArgument;
    }
    
    return mips_Success;
}

//Should not be putting this here, get rid of it!!!
mips_error mips_cpu_get_debug_level(mips_cpu_h state, unsigned *level) {
    if (state == 0) {
        return mips_ErrorInvalidHandle;
    }
    *level = state->debuglevel;
    
    return mips_Success;
}

mips_error mips_cpu_reset(mips_cpu_h state) {
    state->pc = 0;
    state->pcN = 4;
    
    for (int i = 0; i <32; i++)
        state->regs[i] = 0;
    
    auto mem=state->mem;
    memset(state, 0, sizeof(*state));
    state->mem=mem;
    return mips_Success;
}

mips_error mips_cpu_step(mips_cpu_h state) {
    if (state == 0)
        return mips_ErrorInvalidHandle;
    
    uint32_t pc = 0;
    mips_error error = mips_Success;
    mips_cpu_get_pc(state, &pc);
    
    uint32_t fulladdress = 0x0;
    
    //This is the array that stores all the information
    uint32_t all_information[7] = {
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
    };
    
    error = mips_mem_read(state->mem, state->pc, 4, (uint8_t *) &fulladdress);
    
    if (error) {
        std::cout << "Returning error in read" << std::endl;
        std::cout << error << std::endl;
        return error;
    }
    
    fulladdress = little_to_big_endian(fulladdress);
    
    //Decoder
    error = instruction_type(fulladdress, all_information);
    if (error) {
        std::cout << "Returning error in decoder. PC will not be incremented by CPU." << std::endl;
        std::cout << error << std::endl;
        return error;
    }
    //Execute
    error = main_execute(state, all_information, state->mem, state->LO, state->HI, state->pcN, state->delay_slot);
    
    //Logs
    Logging(state, all_information);
    
    if (error) {
        std::cout << "Returning error in execute. PC will not be incremented by CPU." << std::endl;
        std::cout << error << std::endl;
        return error;
    }
    
    if (state->delay_slot == 1) {
        if (state->pc % 4 != 0)
            return mips_ExceptionInvalidAddress;  //Checks after the branch target is fetched
        
        std::cout << "pcN: " << state->pcN << std::endl;
        state->pc += 4;
        state->delay_slot = 0;
    }
    else if (state->delay_slot == 0){
        if (state->pcN % 4 != 0)
            return mips_ExceptionInvalidAddress;  //Checks after the branch target is fetched
        
        state->pc = state->pcN;
        state->pcN += 4;
    }
    
    return mips_Success;
}