//
// Created by Sanjeev Manocha on 19/10/2016.
//

#include <iostream>
#include "mips_cpu_execute.hpp"
#include "mips_cpu_basic_definitions.hpp"
#include "mips_cpu_checks.hpp"

const unsigned ADD = 0,
               SUB = 1,
               XOR = 0,
               OR = 1,
               AND = 2,
               SHIFT_LEFT = 0,
               SHIFT_RIGHT = 1,
               SHIFT_ARITHMETIC = 2,
               IS_UNSIGNED = 0,
               IS_SIGNED = 1,
               FULLWORD = 0,
               HALFWORD = 1,
               BYTE = 2,
               BRANCH_BEQ =0,
               BRANCH_BGTZ =1,
               BRANCH_BLEZ = 2,
               BRANCH_BNE = 3,
               BRANCH_OTHER = 4;

mips_error name_code_finder (uint32_t& name_code, const uint32_t all_information[7]) {

    switch (all_information[TYPE]) {
        case R_TYPE :
            for (int i =0; i < 26; i++) {
                if (r_type_defs[i].opcode == all_information[FUNCTION]) {
                    std::cout << "Opcode: " << all_information[FUNCTION] << std::endl;
                    name_code = r_type_defs[i].name_code; }
            }
            return mips_Success;

        case I_TYPE :
            for (int i=0; i < 22; i++) {
                if (i_type_defs[i].opcode == all_information[OPCODE]) {
                    name_code = i_type_defs[i].name_code; }
            }
            return mips_Success;

        case J_TYPE :
            for (int i=0; i < 2; i++) {
                if (j_type_defs[i].opcode == all_information[OPCODE]) {
                    name_code = j_type_defs[i].name_code; }
            }
            return mips_Success;

        default :
            return mips_ErrorInvalidArgument;
    }
}

//Could replace this with a pointer in the constant array to relevant functions
mips_error main_execute(mips_cpu_h state, const uint32_t all_information[7], mips_mem_h mem, uint32_t& LO_register, uint32_t& HI_register, uint32_t& pcN, int& delay_slot) {
    mips_error errorcheck = mips_Success;
    uint32_t name_code = 50;
    errorcheck = name_code_finder(name_code, all_information);

    if (errorcheck)
        return errorcheck;

    errorcheck = check_register(all_information);
    if (errorcheck)
        return errorcheck;

    std::cout << "Name code: " << name_code << std::endl;
    //Grouping accordingly
    switch (name_code) {
        case 43:
        case 42:
            errorcheck= loadword_l_r(state, all_information, mem);
        case 41:
        case 40:
            errorcheck= branch_comparisons_delay_slot(state, all_information, pcN, J_TYPE, delay_slot);
            break;
        case 39:
            errorcheck = branch_comparisons_delay_slot(state, all_information, pcN, BRANCH_OTHER, delay_slot);
            break;
        case 38:
            errorcheck = branch_comparisons_delay_slot(state, all_information, pcN, BRANCH_BGTZ, delay_slot);
            break;
        case 37:
            errorcheck = branch_comparisons_delay_slot(state, all_information, pcN, BRANCH_BLEZ, delay_slot);
            break;
        case 36:
            errorcheck = branch_comparisons_delay_slot(state, all_information, pcN, BRANCH_BNE, delay_slot);
            break;
        case 35:
            errorcheck = branch_comparisons_delay_slot(state, all_information, pcN, BRANCH_BEQ, delay_slot);
            break;
        case 34:
            errorcheck = store_operations(state, all_information, FULLWORD, mem);
            break;
        case 33:
            errorcheck = store_operations(state, all_information, HALFWORD, mem);
            break;
        case 32:
            errorcheck = store_operations(state, all_information, BYTE, mem);
            break;
        case 31:
        case 30:
            errorcheck= load_operations(state, all_information, FULLWORD, mem);
            break;
        case 29:
        case 28:
            errorcheck= load_operations(state, all_information, HALFWORD, mem);
            break;
        case 27:
        case 26:
            errorcheck= load_operations(state, all_information, BYTE, mem);
            break;
        case 25:
        case 24:
            errorcheck= branch_comparisons_delay_slot(state, all_information, pcN, R_TYPE,delay_slot);
            break;
        case 23:
        case 22:
            errorcheck = write_hi_lo(state, all_information, HI_register, LO_register);
            break;
        case 21:
        case 20:
            errorcheck= read_hi_lo(state, all_information, HI_register, LO_register);
            break;
        case 19:
        case 18:
            errorcheck= basic_arithmetic_alu(state, all_information, SUB);
            break;
        case 17:
        case 16:
            errorcheck= basic_arithmetic_alu(state, all_information, ADD);
            break;
        case 15:
        case 14:
        case 13:
        case 12:
            errorcheck= mul_and_div(state, all_information, HI_register, LO_register);
            break;
        case 10:
            errorcheck= bitwise_operations_alu(state, all_information, XOR);
            break;
        case 9:
            errorcheck= bitwise_operations_alu(state, all_information, OR);
            break;
        case 8:
            errorcheck= bitwise_operations_alu(state, all_information, AND);
            break;
        case 6:
            errorcheck= set_operations(state, all_information, IS_SIGNED);
            break;
        case 7:
            errorcheck= set_operations(state, all_information, IS_UNSIGNED);
            break;
        case 5:
        case 4:
            errorcheck= shift_operations_alu(state, all_information, SHIFT_ARITHMETIC);
            break;
        case 3:
        case 2:
            errorcheck= shift_operations_alu(state, all_information, SHIFT_RIGHT);
            break;
        case 1:
        case 0:
            errorcheck= shift_operations_alu(state, all_information, SHIFT_LEFT);
            break;
        default:
            return mips_ExceptionInvalidInstruction;
    }

    if (errorcheck != 4097) {
        std::cout << "Yo the functions have errors: " << errorcheck << std::endl;
        return errorcheck;
    }

    return mips_Success;
}

// Basic arithmetic operations: Implements ADDU, ADDIU, ADD, ADDI, SUBU, SUB
mips_error basic_arithmetic_alu(mips_cpu_h state, const uint32_t *all_information, int operation_type) {
    uint32_t value_1 = 0, value_2 = 0, answer = 0;
    int32_t signed_value_1, signed_value_2 = 0, signed_answer = 0;

    mips_error errorcheck = mips_Success;
    mips_cpu_get_register (state, all_information[RS], &value_1);
    //Differentiate between immediate and not
    //Set values accordingly
    switch (all_information[TYPE]) {
        case R_TYPE:

            if (all_information[SHIFT] != 0)
                return mips_ExceptionInvalidInstruction;

            std::cout << std::endl << "Register RT: " << all_information[RT] << std::endl << std::endl;
            mips_cpu_get_register (state, all_information[RT], &value_2); //Getting value 2 for ADDU and SUBU
            //Getting signed values for ADD and SUB
            if ((all_information[FUNCTION] == 32) || (all_information[FUNCTION] == 34)) {
                std::cout << std::endl << "Using signed" << std::endl << std::endl;
                signed_value_2 = (int32_t)value_2;
                signed_value_1 = (int32_t)value_1; }
            break;

        case I_TYPE:
            if (all_information[OPCODE] == 8) {
                signed_value_2 = (int16_t)all_information[IMM_CONSTANT]; //Getting value 2 for ADDI
                signed_value_1 = (int32_t)value_1;
            }
            else if (all_information[OPCODE] == 9)
                value_2 = (int16_t)all_information[IMM_CONSTANT]; //Getting value 2 for ADDIU

            break;
    }

    std::cout << "Immediate " << all_information[IMM_CONSTANT] << " to register: " << all_information[RD] << std::endl;

    switch (operation_type) {
        case ADD:
            answer = value_1 + value_2; //ADDIU or ADDU
            if (((all_information[TYPE] == R_TYPE) && (all_information[FUNCTION] == 32)) || ((all_information[TYPE] == I_TYPE) && (all_information[OPCODE] == 8))) { //ADD or ADDI
                signed_answer = (int32_t)signed_value_2 + (int32_t)signed_value_1; //Should be zero if unsigned used
                errorcheck = check_overflow_addition(signed_value_1, signed_value_2, signed_answer);
            }
            break;

        case SUB:
            answer = value_1 - value_2;
            if (all_information[FUNCTION] == 34) {
                signed_answer = signed_value_1 - signed_value_2; // Should be 0 if unsigned
                errorcheck = check_overflow_subtraction(signed_value_1, signed_value_2, signed_answer);
            }
            break;
    }

    if (errorcheck)
        return errorcheck;

    if (signed_answer == 0) {
        std::cout << "Writing " << answer << " to register: " << all_information[RD] << std::endl;
        mips_cpu_set_register(state, all_information[RD], answer);
    }
    else {
        std::cout << "Writing " << signed_answer << " (signed) to register: " << all_information[RD] << std::endl;
        mips_cpu_set_register(state, all_information[RD], signed_answer);
    }

    return mips_Success;
}


//Bitwise operations: AND, ANDI, OR, ORI, XOR, XORI
//Check immediates are unsigned, because the rest are always signed
mips_error bitwise_operations_alu(mips_cpu_h state, const uint32_t all_information[7], int operation_type) {
    uint32_t value_1 = 0, value_2 = 0, answer = 0;

    mips_error errorcheck = mips_Success;
    errorcheck = mips_cpu_get_register (state, all_information[RS], &value_1);

    switch (all_information[TYPE]) {
        case R_TYPE:
            errorcheck = mips_cpu_get_register (state, all_information[RT], &value_2);
            std::cout << "RS: " << value_2 << "from " << all_information[RS]<< std::endl;
            break;
            //All is 32 bits, and therefore should be zero extended. If need be, could do a check for this?
        case I_TYPE:
            value_2 = all_information[IMM_CONSTANT];
            std::cout << "Exec2" << std::endl;
            break;
    }

    switch (operation_type) {
        case AND:
            answer = value_1 & value_2;
            break;

        case XOR:
            answer = value_1 ^ value_2;
            break;

        case OR:
            answer = value_1 | value_2;
            break;
    }

    errorcheck = mips_cpu_set_register(state, all_information[RD], answer);
    if (errorcheck) {
        return errorcheck;
    }

    return mips_Success;
}

//Implements SLL, SRL, SRA, SLLV, SRLV, SRAV
//May attempt to combine all but arithmetic operations as divide, or vice versa
mips_error shift_operations_alu(mips_cpu_h state, const uint32_t all_information[7], int operation_type) {
    uint32_t value_1 = 0, value_2 = 0, answer = 0;
    //value 2 is 5 bits shift information, if a variable
    mips_error errorcheck = mips_Success;

    if ((all_information[FUNCTION] >= 4) && (all_information[FUNCTION] <= 7)) {
        errorcheck = mips_cpu_get_register(state, all_information[RS], &value_2);
        errorcheck = (mips_cpu_get_register(state, all_information[RT], &value_1));
        value_2 = (value_2 & 0x0000001F);
        std::cout << "Exec2" << std::endl;
    }

    else if ((all_information[FUNCTION] < 4)) {
        errorcheck = mips_cpu_get_register(state, all_information[RT], &value_1);
        value_2 = all_information[SHIFT];
        std::cout << "SHIFT: " << value_2 << "from " << all_information[SHIFT] << std::endl;
    }

    switch (operation_type) {
        case SHIFT_LEFT:
            answer = (value_1 << value_2);
            break;
        case SHIFT_RIGHT:
            answer = (value_1 >> value_2);
            break;
        case SHIFT_ARITHMETIC:
            answer = (value_1 >> value_2);
            sign_extend(value_1, value_2, answer);
            break;
    }

    errorcheck = mips_cpu_set_register(state, all_information[RD], answer);

    if (errorcheck)
        return errorcheck;

    return mips_Success;
}

//Implements set instructions
mips_error set_operations(mips_cpu_h state, const uint32_t all_information[7], int operation_type) {
    uint32_t value_1 = 0, value_2 = 0;
    int32_t signed_value_1, signed_value_2 = 0;

    uint8_t answer = 0;
    mips_error errorcheck = mips_Success;
    mips_cpu_get_register (state, all_information[RS], &value_1);

    switch (all_information[TYPE]) {
        case R_TYPE:
            errorcheck = mips_cpu_get_register (state, all_information[RT], &value_2);
            std::cout << "RS: " << value_2 << "from " << all_information[RS]<< std::endl;
            break;
            //All is 32 bits, and therefore should be zero extended. If need be, could do a check for this?
        case I_TYPE:
            value_2 = all_information[IMM_CONSTANT];
            std::cout << "Exec2" << std::endl;
            break;
    }

    if (operation_type) {
        signed_value_1 = value_1;
        signed_value_2 = value_2;
        if (signed_value_1 < signed_value_2)
            answer = 1;
    }
    else if (!operation_type){
        if (value_1 < value_2)
            answer = 1;
    }
    errorcheck = mips_cpu_set_register(state, all_information[RD], answer);
    if (errorcheck)
        return errorcheck;

    return mips_Success;
}

//Implements load instructions - all I types : LW, LH, lhu, lbu, lb, LUI
mips_error load_operations(mips_cpu_h state, const uint32_t all_information[7], int operation_type, mips_mem_h mem) {
    uint32_t answer = 0, newoffset = 0, RS_value = 0;
    int32_t  signed_offset = 0, cast_signed_value = 0;
    int16_t  half_word_signed = 0;
    uint16_t half_unsigned_word = 0;
    int8_t   byte_signed = 0;

    mips_error errorcheck = mips_Success;
    mips_cpu_get_register (state, all_information[RS], &RS_value);

    //If RS = 0, then LUI
    if (all_information[RS] == 0) {
        answer = (all_information[IMM_CONSTANT] << 16) & 0xFFFF0000;
        std::cout << "LUI being executed " << answer << std::endl;
    }

    //Need to load registers: LW, LH, lhu, lbu, lb
    else {
        signed_offset = all_information[IMM_CONSTANT];
        newoffset = RS_value + signed_offset;

        // LW
        if (operation_type == FULLWORD) {
            if (((newoffset&0x01) == 1) || ((newoffset&0x02) == 2))
                return mips_ExceptionInvalidAddress;

            errorcheck = mips_mem_read(mem, newoffset, 4, (uint8_t*) &answer);
            answer = little_to_big_endian(answer);
        }

        //LH, LHU
        else if (operation_type == HALFWORD) {
            if ((newoffset & 0x01) == 1)
                return mips_ExceptionInvalidAddress;

            if (all_information[OPCODE] == 0x21) {
                errorcheck = mips_mem_read(mem, newoffset, 2, (uint8_t *) &half_word_signed);
                half_word_signed = ((half_word_signed >> 8)&0x00FF) | ((half_word_signed << 8)&0xFF00);
                cast_signed_value = (int16_t)half_word_signed;
            }
            else {
                std::cout << "newoffset: " << newoffset << std::endl;

                errorcheck = mips_mem_read(mem, newoffset, 2, (uint8_t *) &half_unsigned_word);
                std::cout << "answer: " << half_unsigned_word << std::endl;
                half_unsigned_word = (half_unsigned_word >> 8) | (half_unsigned_word << 8);
                answer = half_unsigned_word;
                std::cout << "answer: " << answer << std::endl;
            }
        }

        //LB, lBU
        else if (operation_type == BYTE) {
            if (all_information[OPCODE] == 0x20) {
                errorcheck = mips_mem_read(mem, newoffset, 1, (uint8_t *) &byte_signed);
                cast_signed_value = (int8_t)byte_signed;
            }
            else {
                errorcheck = mips_mem_read(mem, newoffset, 1, (uint8_t *) &answer);
            }
        }

    }

    if (errorcheck)
        return errorcheck;

    //Now determine if signed or unsigned
    if ((!cast_signed_value) && (all_information[RS] == 0))
        errorcheck = mips_cpu_set_register(state, all_information[RD], answer);

    if ((!cast_signed_value) && (all_information[RS] != 0)) {
        errorcheck = mips_cpu_set_register(state, all_information[RD], answer);
    }
    if (!answer) {
        errorcheck = mips_cpu_set_register(state, all_information[RD], cast_signed_value);
    }

    if (errorcheck)
        return errorcheck;

    return mips_Success;
}

mips_error store_operations(mips_cpu_h state, const uint32_t all_information[7], int operation_type, mips_mem_h mem) {
    uint32_t answer = 0, newoffset = 0, pc =0, value_rd = 0;
    int32_t signed_offset = 0;
    uint16_t temphalfword = 0;
    uint8_t byte = 0;

    mips_error errorcheck = mips_cpu_get_register(state, all_information[RD], &answer);

    signed_offset = all_information[IMM_CONSTANT];
    mips_cpu_get_register (state, all_information[RS], &value_rd);

    newoffset = signed_offset + value_rd;

        //SW
    if (operation_type == FULLWORD) {
        if (((newoffset&0x01) == 1) || ((newoffset&0x02) == 2))
            return mips_ExceptionInvalidAddress;
        answer = little_to_big_endian(answer);
        errorcheck = mips_mem_write(mem, newoffset, 4, (uint8_t *) &answer);
    }

        //SH
    else if (operation_type == HALFWORD) {
        if ((newoffset & 0x01) == 1)
            return mips_ExceptionInvalidAddress;
        temphalfword = (answer & 0xFFFFFFFF);
        temphalfword = ((temphalfword >> 8)&0x00FF) | ((temphalfword << 8)&0xFF00);
        errorcheck = mips_mem_write(mem, newoffset, 2, (uint8_t*) &temphalfword);
    }

        //SB
    else if (operation_type == BYTE) {
        byte = (answer & 0xFFFF);
        errorcheck = mips_mem_write(mem, newoffset, 1, (uint8_t*) &byte);
    }

    mips_cpu_get_pc(state, &pc);

    if (errorcheck)
        return errorcheck;

    return mips_Success;
}

//Implemenets MFHI, MFLO
mips_error read_hi_lo(mips_cpu_h state, const uint32_t all_information[7], const uint32_t& register_HI, const uint32_t& register_LO) {
    std::cout << "Executing mfhi" << std::endl;
    uint32_t answer = 0;
    //HI Register
    if ((all_information[FUNCTION] == 16) && (all_information[RS] == 0) && (all_information[RT] == 0) && (all_information[SHIFT] == 0)) {
        answer = register_HI;
    }
    //LO Register
    if ((all_information[FUNCTION] == 18) && (all_information[RS] == 0) && (all_information[RT] == 0) && (all_information[SHIFT] == 0)) {
        answer = register_LO;
    }

    mips_error errorcheck = mips_cpu_set_register(state, all_information[RD], answer);
    if (errorcheck)
        return errorcheck;

    return mips_Success;
}

//MTHI, MTLO
mips_error write_hi_lo(mips_cpu_h state, const uint32_t all_information[7], uint32_t& register_HI, uint32_t& register_LO) {
    std::cout << "executing mthi" << std::endl;
    uint32_t answer = 0;
    mips_error errorcheck = mips_cpu_get_register (state, all_information[RS], &answer);

    if (errorcheck)
        return errorcheck;

    if ((all_information[FUNCTION] == 17) && (all_information[RT] == 0) && (all_information[RD] == 0) && (all_information[SHIFT] == 0)) {
        register_HI = answer;
    }

    if ((all_information[FUNCTION] == 19) && (all_information[RT] == 0) && (all_information[RD] == 0) && (all_information[SHIFT] == 0)) {
        register_LO = answer;
    }

    return mips_Success;
}

//Implements MULT, MULTU, DIV, DIVU
mips_error mul_and_div(mips_cpu_h state, const uint32_t all_information[7], uint32_t& register_HI, uint32_t& register_LO) {
    uint64_t answer = 0;
    uint32_t remainder = 0;
    int64_t signed_answer = 0;
    uint32_t value_1 = 0, value_2 = 0;
    int32_t signed_value_1 = 0, signed_value_2 = 0;

    mips_error errorcheck = mips_cpu_get_register (state, all_information[RS], &value_1);
    errorcheck = mips_cpu_get_register (state, all_information[RT], &value_2);
    //Case: Multu
    if (all_information[FUNCTION] == 25 && (all_information[RD] == 0) && (all_information[SHIFT] == 0)) {
        answer = (uint64_t)value_1 * (uint64_t)value_2;
    }

    //Case: Mult
    if (all_information[FUNCTION] == 24 && (all_information[RD] == 0) && (all_information[SHIFT] == 0)) {
        signed_value_1 = value_1;
        signed_value_2 = value_2;
        signed_answer = (int64_t)signed_value_1 * (int64_t)signed_value_2;
    }

    //Div
    if (all_information[FUNCTION] == 26 && (all_information[RD] == 0) && (all_information[SHIFT] == 0)) {
        signed_value_1 = value_1;
        signed_value_2 = value_2;
        signed_answer = signed_value_1 / signed_value_2;
        remainder = signed_value_1 % signed_value_2;
    }

    //Divu
    if (all_information[FUNCTION] == 27 && (all_information[RD] == 0) && (all_information[SHIFT] == 0)) {
        answer = (uint64_t)value_1 / (uint64_t)value_2;
        remainder = (uint64_t)value_1 % (uint64_t)value_2;
    }

    if (answer!=0) {
        register_HI = ((answer>>32) & 0xFFFFFFFF);
        register_LO = (answer & 0xFFFFFFFF);
    }
    else if (signed_answer != 0) {
        register_HI = ((signed_answer>>32) & 0xFFFFFFFF);
        register_LO = (signed_answer & 0xFFFFFFFF);
    }
    if ((remainder != 0)) {
        register_HI = remainder;
    }

    return mips_Success;
}

//Executes Branch instructions: BEQ, BNE, BLEZ, BGTZ, BLTZ, BGEZ, J, JAL, JR, JALR
mips_error branch_comparisons_delay_slot(mips_cpu_h state, const uint32_t all_information[7], uint32_t& pcN, unsigned branch_type, int& delay_slot) {
    uint32_t value_1 = 0, value_2 = 0;
    int16_t signed_immediate = 0;
    uint32_t PC = 0, target_address = 0;

    mips_cpu_get_pc (state, &PC);
    mips_cpu_get_register (state, all_information[RS], &value_1);
    mips_cpu_get_register (state, all_information[RD], &value_2);

std::cout << "value1: " << value_1 << " value 2: " << value_2 << std::endl;
    //BEQ
    if ( ((value_1 == value_2) && (branch_type == BRANCH_BEQ)) || ((value_1 != value_2) && (branch_type == BRANCH_BNE)) || (((is_negative(value_1))
          || (value_1 == 0)) && (branch_type == BRANCH_BLEZ)) || ((!is_negative(value_1)) && (branch_type == BRANCH_BGTZ))
          || ((is_negative(value_1)) && (all_information[RT] == 0) && (branch_type == BRANCH_OTHER)) //BLTZ
          || (((!is_negative(value_1)) || (value_1 == 0)) && (all_information[RT] == 1) && (branch_type == BRANCH_OTHER)) ) { //BGEZ

        signed_immediate = (int16_t) (all_information[IMM_CONSTANT] << 2);
        pcN += signed_immediate;
        delay_slot = 1;
    }

    //J and JAL
    if (branch_type == J_TYPE) {
        target_address = (PC & 0xff0000000) | (0xFFFFFFF&all_information[MEMORY] << 2);
        pcN = target_address;
        delay_slot = 1;

        if (all_information[OPCODE] == 3)
            mips_cpu_set_register(state, 31, PC+8);
    }

    //JR and JALR
    else if (branch_type == R_TYPE) {
        pcN = all_information[RS];
        delay_slot = 1;

        if (all_information[FUNCTION] == 9)
            mips_cpu_set_register(state, all_information[RD], PC+8);
    }

    return mips_Success;
}

mips_error loadword_l_r(mips_cpu_h state, const uint32_t all_information[7], mips_mem_h mem) {
    int32_t signed_immediate = (int16_t)all_information[IMM_CONSTANT];
    uint32_t address = all_information[RS] + signed_immediate;
    uint8_t byte;
    uint32_t new_word = 0;
    unsigned byte_number;
    mips_error errorcheck;

    std::cout << "Going to Load Word" <<std::endl;

    if (all_information[OPCODE] == 34) {
        byte_number = 3;
        do {
            errorcheck = mips_mem_read(mem, address, 1, &byte);

            if (errorcheck)
                return errorcheck;
            new_word = ((byte << (byte_number * 8)) | new_word);

            address++;
            byte_number--;
        } while ((address&3) != 0);
        //Shift by however many bytes have been fetched
        uint32_t temp_l =  0xFFFFFFFF<<(byte_number*8);
        new_word = new_word & (0xFFFFFFFF >> ((3-byte_number)*8));
        mips_cpu_set_register(state, all_information[RD], (temp_l | new_word));
    }

    else if (all_information[OPCODE] == 38) {
        byte_number = 0;
            do {
                errorcheck = mips_mem_read(mem, address, 1, &byte);
                if (errorcheck)
                    return errorcheck;

                new_word = ((byte << (byte_number * 8)) | new_word);

                address++;
                byte_number++;
        } while ((address&3) != 0);
        //Shift by however many bytes have been fetched
        uint32_t temp_r = 0xFFFFFFFF>>(byte_number*8);
        new_word = new_word & (0xFFFFFFFF << ((byte_number)*8));
        mips_cpu_set_register(state, all_information[RD], (temp_r | new_word));
    }

    return mips_Success;

}