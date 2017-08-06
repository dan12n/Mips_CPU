#include "test_mips_checker.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

int main() {
    //************************* Test Initialisations ****************************
    //Set Debug Level
    unsigned DebugLevel = 2;
    FILE *dest = fopen("dump.txt", "w");

    //At the moment it sets debug level correctly only. Output is yet to be implemented correctly, and so is changing debugging without recompiling

    //const char *instr_src="test_instructions.txt";
    //Create Memory and CPU
    mips_mem_h mem = mips_mem_create_ram(4096);
    mips_cpu_h cpu = mips_cpu_create(mem);
    mips_error error_status;     //Used to get any returns from cpu/program

    std::vector <test_checker> vec_check_tests; //Vector to store all tests

    //Initialise all variables to zero
    int passed;
    uint32_t pc = 0, Instr = 0;

    write_to_vec(vec_check_tests);     //Load Vector with test instructions
    error_status = mips_cpu_set_debug_level(cpu, DebugLevel, dest);    //Setting Debug level
    uint32_t register_value_before;     //Checks the register value before


    //************************* Begin test ****************************
    mips_test_begin_suite();
    //error_status = mips_cpu_set_debug_level(cpu, DebugLevel, stderr);

    //Start testing all instructions now stored in the vector
    for (unsigned int test = 0; test < (vec_check_tests.size()); test++) {
        passed = 0;
        error_status = mips_Success;
        mips_cpu_get_pc(cpu, &pc);

        unsigned int testindex = pc/4;

        std::cout << "PC: " << pc << std::endl;

        std::cout << "testindex: " << testindex << std::endl;

        //Getting value of register previously
        mips_cpu_get_register(cpu, vec_check_tests[testindex].RD, &register_value_before);

        //std:: cout << "PC: " << pc << std::endl;
        const char *instruction_name = (vec_check_tests[testindex].instr_name).c_str();
        int testId=mips_test_begin_test(instruction_name);

        switch (vec_check_tests[testindex].instr_type) {
            case 'R':
                create_test_r_i(cpu, mem, vec_check_tests, testindex, pc, RTYPE);
                error_status = mips_cpu_step(cpu);
                check_test_r_i(testindex, passed, vec_check_tests, cpu, RTYPE, mem);
                break;

            case 'I':
                create_test_r_i(cpu, mem, vec_check_tests, testindex, pc, RTYPE);
                error_status = mips_cpu_step(cpu);
                check_test_r_i(testindex, passed, vec_check_tests, cpu, ITYPE, mem);
                break;

            case 'J':
                check_test_j(testindex, passed, vec_check_tests, cpu);
                break;

            default:
                fprintf(stderr, "Failed to find instruction type \n");
                exit(1);
        }

        if (error_status) {
            std::cout << "There was an error" << std::endl;

            switch (error_status) {
                case 0x2006:
                    std::cout << "It was arithmetic overflow, checking if value was changed at all" << error_status << std::endl;
                    passed = !(is_register_changed(cpu, register_value_before, vec_check_tests[testindex].RD));
            }

            std::cout << "Incrementing PC manually to test the next instruction" << std::endl;
            mips_cpu_set_pc(cpu, pc+4);
        }

        mips_test_end_test(testId, passed, NULL);
    }

//############################ End the testbench and deallocate memory ###############################
    mips_test_end_suite();
    fclose(dest);
    mips_cpu_free(cpu);
    mips_mem_free(mem);

}

void write_to_vec(std::vector <test_checker>& vec_check_tests) {
    //Read in the tests from a text file and store it in a vector
    //Vectors and Structs to create tester
    test_checker temp_check;

    std::ifstream instruction_data ("test_instructions.txt");
    std::string line;

    if (!(instruction_data.is_open())) {
        std::cout << "not open" << std::endl;
        exit(1);
    }
    //instruction_data.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );

    while (getline(instruction_data, line)) {
        std::istringstream ss(line);

        ss >> std::hex >> temp_check.fulladdress >> std::dec >> temp_check.value_RS >>
           temp_check.value_RT >> temp_check.instr_type >> temp_check.instr_name >> temp_check.RS >>
           temp_check.RT >> temp_check.RD >> temp_check.Imm_Constant >> temp_check.value_RD >> temp_check.constant_is_negative
           >> temp_check.answer_is_negative;

        if (temp_check.answer_is_negative)
            temp_check.value_RD = -temp_check.value_RD;
        if (temp_check.constant_is_negative)
            temp_check.Imm_Constant = -temp_check.Imm_Constant;

        vec_check_tests.push_back(temp_check);
    }

    instruction_data.close();

}

void create_test_r_i(mips_cpu_h cpu, mips_mem_h mem, const std::vector <test_checker>& vec_check_tests, unsigned int testindex, uint32_t& pc,
                     unsigned int type) {
    uint32_t Instr = vec_check_tests[testindex].fulladdress;

    if ((vec_check_tests[testindex].instr_name == "LW") || (vec_check_tests[testindex].instr_name == "LHU") || (vec_check_tests[testindex].instr_name == "LH")
             || (vec_check_tests[testindex].instr_name == "LB") || (vec_check_tests[testindex].instr_name == "LBU"))
        create_memory_test(mem, vec_check_tests, testindex);

    else if ((vec_check_tests[testindex].instr_name == "SW") || (vec_check_tests[testindex].instr_name == "SH") || (vec_check_tests[testindex].instr_name == "SB")) {
        mips_cpu_set_register(cpu, vec_check_tests[testindex].RD, vec_check_tests[testindex].value_RD);
    }

    mips_cpu_set_register(cpu, vec_check_tests[testindex].RS, vec_check_tests[testindex].value_RS);            // Set first input

    if (type == RTYPE) { //Built for store purposes
        mips_cpu_set_register(cpu, vec_check_tests[testindex].RT, vec_check_tests[testindex].value_RT);            // Set second input
    }

    Instr = change_endian(Instr);

    mips_mem_write(mem, pc, 4, (uint8_t*)&Instr); //Writing instruction to register
}

//Built for Load purposes
void create_memory_test(mips_mem_h mem, const std::vector <test_checker>& vec_check_tests, unsigned int testindex) {
    uint32_t answer =vec_check_tests[testindex].value_RD;
    uint8_t tempbyte = (answer&0xFF);
    uint32_t memory_address = vec_check_tests[testindex].value_RS + (int32_t)vec_check_tests[testindex].Imm_Constant;
    std::cout << "Load answer: " << answer << std::endl;
    uint16_t temphalfword = (answer&0xFFFFFFFF);

    if (vec_check_tests[testindex].instr_name == "LW") {
        answer = change_endian(answer);
        mips_mem_write(mem, memory_address, 4, (uint8_t *) &answer);
    }

    if ((vec_check_tests[testindex].instr_name == "LH") || (vec_check_tests[testindex].instr_name == "LHU")) {
        std::cout << "Address on test: " << memory_address << std::endl;
        temphalfword = ((temphalfword >> 8)&0x00FF) | ((temphalfword << 8)&0xFF00);
        mips_mem_write(mem, memory_address, 2, (uint8_t *) &temphalfword);
    }

    if ((vec_check_tests[testindex].instr_name == "LB") || (vec_check_tests[testindex].instr_name == "LBU")) {
        mips_mem_write(mem, memory_address, 1, (uint8_t *) &tempbyte);
    }
}



void printall_vector(const std::vector <test_checker>& vec_check_test) {
    std::cout << "Checker: " << std::endl;

    for (unsigned int i = 0; i < vec_check_test.size(); i++) {
        std::cout << vec_check_test[i].instr_type << std::endl
                  << vec_check_test[i].instr_name << std::endl
                  << vec_check_test[i].Imm_Constant << std::endl
                  << vec_check_test[i].value_RD << std::endl
                  << vec_check_test[i].RD << std::endl
                  << vec_check_test[i].RS << std::endl
                  << vec_check_test[i].RT << std::endl
                  << vec_check_test[i].value_RT << std::endl
                  << vec_check_test[i].value_RS << std::endl
                  << vec_check_test[i].fulladdress << std::endl;
    }
}