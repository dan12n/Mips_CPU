#include "test_mips_checker.hpp"
#include <iostream>

void check_test_r_i(const unsigned int& testindex, int& passed, const std::vector <test_checker>& vec_check_tests, mips_cpu_h cpu,
                    unsigned int type, mips_mem_h mem) {

    if ((vec_check_tests[testindex].instr_name == "SW") || (vec_check_tests[testindex].instr_name == "SH") || (vec_check_tests[testindex].instr_name == "SB")) {
        check_memory_test(testindex, passed, vec_check_tests, mem);
        return;
    }

    uint32_t result=32, input_A =32, input_B=32, signed_result = 0;
    int16_t cast_signed_halfword = 0;
    int8_t cast_signed_byte = 0;

    mips_cpu_get_register(cpu, vec_check_tests[testindex].RD, &result); //Get result
    mips_cpu_get_register(cpu, vec_check_tests[testindex].RS, &input_A);

    if (type == RTYPE)
        mips_cpu_get_register(cpu, vec_check_tests[testindex].RT, &input_B);

    else if (type == ITYPE)
        input_B = vec_check_tests[testindex].Imm_Constant;

    if ((vec_check_tests[testindex].instr_name == "LH")) {
        cast_signed_halfword = result;
        signed_result = cast_signed_halfword;
    }

    if (signed_result != 0) {
        fprintf(stderr, "Input A: %d  Input B: %d, from instructions file, to give %d. Expected = %d\n", input_A,
                input_B, signed_result, vec_check_tests[testindex].value_RD);

        if (vec_check_tests[testindex].value_RD != signed_result) {
            fprintf(stderr, "Answer is not as expected \n");
            passed = 0;
        } else {
            passed = 1;

        }
    }
    else {
        fprintf(stderr, "Input A: %d  Input B: %d, from instructions file, to give %d. Expected = %d\n", input_A,
                input_B, result, vec_check_tests[testindex].value_RD);

        if (vec_check_tests[testindex].value_RD != result) {
            fprintf(stderr, "Answer is not as expected \n");
            passed = 0;
        } else {
            passed = 1;

        }
    }
}

void check_test_j(const unsigned int& testindex, int& passed, const std::vector <test_checker>& vec_check_tests, mips_cpu_h cpu){

}

//Checks for store instructions
void check_memory_test(const unsigned int& testindex, int& passed, const std::vector <test_checker>& vec_check_tests, mips_mem_h mem) {
    uint32_t answer, expected_answer;
    uint32_t newoffset = vec_check_tests[testindex].value_RS + (int16_t)vec_check_tests[testindex].Imm_Constant;
    uint16_t temphalfword;
    uint8_t byte;

    if (vec_check_tests[testindex].instr_name == "SW") {
        mips_mem_read(mem, newoffset, 4, (uint8_t *) &answer);
        answer = change_endian(answer);
        expected_answer = vec_check_tests[testindex].value_RD;
    }

    if (vec_check_tests[testindex].instr_name == "SH") {
        mips_mem_read(mem, newoffset, 2, (uint8_t *) &temphalfword);
        temphalfword = ((temphalfword >> 8) & 0x00FF) | ((temphalfword << 8) & 0xFF00);
        answer = temphalfword;
        expected_answer = (vec_check_tests[testindex].value_RD & 0xFFFFFFFF);
    }

    if (vec_check_tests[testindex].instr_name == "SB") {
            mips_mem_read(mem, newoffset, 1, (uint8_t*) &byte);
            answer = byte;
            expected_answer = (vec_check_tests[testindex].value_RD & 0xFFFF);
    }

    if (answer == expected_answer) {
        passed = 1;
        fprintf(stderr, "Answer as expected, got: %d, expected: %d\n", answer, vec_check_tests[testindex].value_RD);
    }
    else {
        passed = 0;
        fprintf(stderr, "Answer is not as expected, got: %d, expected: %d\n", answer, vec_check_tests[testindex].value_RD);
    }

}


bool is_register_changed(mips_cpu_h cpu, const uint32_t& register_value_before, unsigned RD) {
    uint32_t result;
    mips_cpu_get_register(cpu, RD, &result); //Get result
    if (register_value_before == result)
        return false;

    else
        return true;
}

uint32_t change_endian (uint32_t fulladdress) {
    return (((fulladdress<<24)&0xFF000000) | ((fulladdress<<8)&0x00FF0000) | ((fulladdress>>8)&0x0000FF00) | ((fulladdress>>24)&0x000000FF));
}