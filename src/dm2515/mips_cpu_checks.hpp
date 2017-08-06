//
// Created by Sanjeev Manocha on 21/10/2016.
//

#ifndef COMPARCHMIPS_MIPS_CPU_CHECKS_HPP
#define COMPARCHMIPS_MIPS_CPU_CHECKS_HPP

#define MAXIMUMVALUE 0xFFFFFFFF

bool is_negative(const uint32_t& parameter);

mips_error check_overflow(const uint32_t& regA, const uint32_t& regB, const uint32_t& answer);
void Logging(mips_cpu_h state, const uint32_t all_information[7]);
mips_error sign_extend(const uint32_t& value_1, const uint32_t& value_2, uint32_t& result);
uint32_t little_to_big_endian (uint32_t fulladdress);
mips_error branch_after_delay(mips_cpu_h state, const uint32_t& pcN);
mips_error check_register(const uint32_t all_information[7]);
mips_error check_overflow_addition(const uint32_t& regA, const uint32_t& regB, const uint32_t& answer);
mips_error check_overflow_subtraction(const uint32_t& regA, const uint32_t& regB, const uint32_t& answer);

#endif //COMPARCHMIPS_MIPS_CPU_CHECKS_HPP
