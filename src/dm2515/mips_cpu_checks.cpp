#include "mips.h"
#include "mips_cpu_checks.hpp"
#include "mips_cpu_basic_definitions.hpp"
#include <iostream>

mips_error check_overflow_addition(const uint32_t& regA, const uint32_t& regB, const uint32_t& answer) {
    if ((is_negative(regA) && (is_negative(regB)) && (!is_negative(answer))) ||
        ((!is_negative(regA)) && (!is_negative(regB)) && (is_negative(answer)))) {

        std::cout << "There is overflow" << std::endl;
        return mips_ExceptionArithmeticOverflow;

    }
    std::cout << "There is no overflow" << std::endl;

    return mips_Success;
}

mips_error check_overflow_subtraction(const uint32_t& regA, const uint32_t& regB, const uint32_t& answer) {
    if ((is_negative(regA) && (!is_negative(regB)) && (!is_negative(answer))) ||
        ((!is_negative(regA)) && (is_negative(regB)) && (is_negative(answer)))) {

        std::cout << "There is overflow" << std::endl;
        return mips_ExceptionArithmeticOverflow;

    }
    std::cout << "There is no overflow" << std::endl;

    return mips_Success;
}


bool is_negative(const uint32_t& parameter) {
    if (parameter > 0x7FFFFFFF)
        return true;
    else
        return false;
}

mips_error sign_extend(const uint32_t& value_1, const uint32_t& value_2, uint32_t& result) {
    if (is_negative(value_1)) {
        for (uint8_t n =0; n < value_2; n++) {
            result = ( (0x80000000 >> n) | result);
        }
    }
    return mips_Success;
}

void Logging(mips_cpu_h state, const uint32_t all_information[7]) {
    uint32_t pc;
    mips_cpu_get_pc(state, &pc);

    unsigned debuglevel;
    mips_cpu_get_debug_level(state, &debuglevel);

    if (debuglevel == 0) {
        std::cout << "Debug Level set to 0, will not print anything" << std::endl;
    }

    else if (debuglevel == 1) {
        std::cout << "############### Basic Test Information Requested ############" << std::endl << std::endl
                  << "Program Counter: " << pc << std::endl
                  << "Instruction Type Running: " << all_information[TYPE] << std::endl;

    }

    else if (debuglevel == 2) {
        std::cout << "############### Full Debug Information requested ############" << std::endl << std::endl
                  << "Instruction Type: " << all_information[TYPE] << std::endl
                  << "Opcode: " << all_information[OPCODE] << std::endl
                  << "RS: " << all_information[RS] << std::endl
                  << "RD: " << all_information[RD] << std::endl
                  << "RT/Immediate Constant: " << all_information[RT] << std::endl
                  << "Shift: " << all_information[SHIFT] << std::endl
                  << "Function: " << all_information[FUNCTION] << std::endl
                  << "Program Counter: " << pc << std::endl;
    }

}

uint32_t little_to_big_endian (uint32_t fulladdress) {
    return (((fulladdress<<24)&0xFF000000) | ((fulladdress<<8)&0x00FF0000) | ((fulladdress>>8)&0x0000FF00) | ((fulladdress>>24)&0x000000FF));
}

mips_error check_register(const uint32_t all_information[7]) {
    if ( (all_information[RD] == 31) && (!((all_information[RT] == 16) && (all_information[OPCODE] == 1)) || !(( all_information[RT] == 17)&& (all_information[OPCODE] == 1)) || !( all_information[OPCODE] == 3)) )
        return mips_ErrorInvalidArgument;

    return mips_Success;
}