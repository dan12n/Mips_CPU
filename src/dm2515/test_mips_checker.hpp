//
// Created by Sanjeev Manocha on 19/10/2016.
//

#ifndef COMPARCHMIPS_TEST_MIPS_HPP
#define COMPARCHMIPS_TEST_MIPS_HPP
#include "mips.h"
#include <string>
#include <vector>


#define RTYPE 0
#define ITYPE 1
#define JTYPE 2


struct test_checker {
    char instr_type;
    std::string instr_name;
    uint16_t Imm_Constant;
    uint32_t value_RD;
    unsigned RD;
    unsigned RS;
    unsigned RT;
    uint32_t value_RS;
    uint32_t value_RT;
    uint32_t fulladdress;
    bool constant_is_negative;
    bool answer_is_negative;
};

static bool previous_value;

void create_test_r_i(mips_cpu_h cpu, mips_mem_h mem, const std::vector <test_checker>& vec_check_tests, unsigned int testindex, uint32_t& pc,
                     unsigned int type);
void check_test_r_i(const unsigned int& testindex, int& passed, const std::vector <test_checker>& vec_check_tests, mips_cpu_h cpu,
                    unsigned int type);
void check_test_j(const unsigned int& testindex, int& passed, const std::vector <test_checker>& vec_check_tests, mips_cpu_h cpu);

void printall_vector(const std::vector <test_checker>& vec_check_test);

void write_to_vec(std::vector <test_checker>& vec_check_tests);

bool is_register_changed(mips_cpu_h cpu, const uint32_t& register_value_before, unsigned RD);

void create_memory_test(mips_mem_h mem, const std::vector <test_checker>& vec_check_tests, unsigned int testindex);

void check_test_r_i(const unsigned int& testindex, int& passed, const std::vector <test_checker>& vec_check_tests, mips_cpu_h cpu, unsigned int type, mips_mem_h mem);

void create_register_test(mips_cpu_h cpu, const std::vector <test_checker>& vec_check_tests, unsigned int testindex);

void check_memory_test(const unsigned int& testindex, int& passed, const std::vector <test_checker>& vec_check_tests, mips_mem_h mem);

uint32_t change_endian (uint32_t fulladdress);

#endif //COMPARCHMIPS_TEST_MIPS_HPP