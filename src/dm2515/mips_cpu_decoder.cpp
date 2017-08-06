#include "mips_cpu_decoder.h"
#include "mips_cpu_basic_definitions.hpp"
#include <iostream>
// Must Delete

mips_error instruction_type(const uint32_t& fullword, uint32_t all_information[7]) {
    uint8_t opcode = ((fullword >> 26) & 0x3f);

    if (opcode == 0) {
        all_information[TYPE] = R_TYPE;
        all_information[OPCODE] = opcode;
        r_extractall(fullword, all_information);
    }

    else if ((opcode == 0x02) || (opcode == 0x03)) {
        all_information[TYPE] = J_TYPE;
        all_information[OPCODE] = opcode;
        j_extractall(fullword, all_information);
    }
    else {
        all_information[TYPE] = I_TYPE;
        all_information[OPCODE] = opcode;
        i_extractall(fullword, all_information);
    }

    print_all_information(all_information);
    return mips_Success;
}

void r_extractall(const uint32_t& fullword, uint32_t all_information[7]) {
    all_information[RS] = ((fullword >> 21) & 0x1f);
    all_information[RT] = ((fullword >> 16 ) & 0x1f );
    all_information[RD] = ((fullword >> 11) & 0x1f);
    all_information[SHIFT] = ((fullword >> 6) & 0x1f);
    all_information[FUNCTION] = ((fullword) & 0x3f);
}

void i_extractall(const uint32_t& fullword, uint32_t all_information[7]) {
    all_information[RS] = ((fullword >> 21) & 0x1f);
    all_information[RD] = ((fullword >> 16) & 0x1f);
    all_information[IMM_CONSTANT] = ((fullword) & 0xffff);
}

void j_extractall(const uint32_t& fullword, uint32_t all_information[7]) {
    all_information[MEMORY] = (fullword & 0x3FFFFFF);
}

//Must Delete
void print_all_information(const uint32_t all_information[7]) {

    std::cout << "Type: " << all_information[TYPE] << std::endl
            << "Opcode: " << all_information[OPCODE] << std::endl
            << "RS: " << all_information[RS] << std::endl
            << "RD: " << all_information[RD] << std::endl
            << "RT/Immediate Constant: " << all_information[RT] << std::endl
            << "Shift: " << all_information[SHIFT] << std::endl
            << "Function: " << all_information[FUNCTION] << std::endl;

}