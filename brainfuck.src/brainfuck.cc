/**
 * Brainfuck to x86_64 AT&T (GAS) assembler compiler
 * Author: Ondrej Budai <budai@mail.muni.cz>
 *
 * General info:
 * This program compiles BF program provided as first argument to
 * assembler in GAS syntax emitted on standard output. Compilation is
 * done in a very simple way and no optimizations are made.
 * Checking if all [] loops are closed is performed. If they are not
 * status code 2 is returned.
 *
 * Output assumes no c std lib is used, therefore you have to provide
 * -nostdlib parameter to GCC.
 *
 * Example of running:
 * brainfuck hanoi.bf | gcc -x assembler -nostdlib -o hanoi -
 *
 * Used registers:
 * rax: syscall type and return value
 * bl:  temporary register for increment and decrementing data
 * rcx: used by kernel in syscall, therefore not used by us
 * rdx: kernel syscall parameter (used for buffer length)
 * rdi: kernel syscall parameter (used for file descriptor)
 * rsi: kernel syscall parameter (used for buffer address)
 * r11: used by kernel in syscall, therefore not used by us
 * r12: stores pointer to tape (incrementable by < and > )
 *
 * Memory (tape):
 * Stack given by OS is used as tape. At the start of program
 * 65536 bytes of memory is set to 0. No bound checks are performed.
 * Usage of address lower than 0 or higher than 65535 is undefined.
 *
 * Example code of compiling a loop:
 *
 * BF:
 *
 * [**INSIDE**]
 *
 * ASSEMBLER:
 *
 * beg0:
 * mov (%r12), %bl
 * test %bl, %bl
 * jz end0
 *
 * **INSIDE**
 *
 * jmp beg0
 * end0:
 *
 */

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stack>
#include <vector>
#include <unordered_map>

std::string loadFile(const char* file){
    std::ifstream input{file};
    std::stringstream buffer;

    buffer << input.rdbuf();

    return buffer.str();
}

class Compiler {
public:
    Compiler() {
        // start symbol
        instructions_.emplace_back(".globl _start");
        instructions_.emplace_back("_start:");

        // init memory
        instructions_.emplace_back("mov %rsp, %r12");

        instructions_.emplace_back("sub $65536, %r12");
        instructions_.emplace_back("mov $8192, %rcx");
        instructions_.emplace_back("clean:");
        instructions_.emplace_back("movq $0, (%r12, %rcx, 8)");
        instructions_.emplace_back("loop clean");
    }

    void addInstruction(char c){
        static const std::unordered_map<char, void(Compiler::*)()> ptrs {
            {'>', &Compiler::addPointerIncrement},
            {'<', &Compiler::addPointerDecrement},
            {'+', &Compiler::addDataIncrement},
            {'-', &Compiler::addDataDecrement},
            {'.', &Compiler::addOutput},
            {',', &Compiler::addInput},
            {'[', &Compiler::addLoopBegin},
            {']', &Compiler::addLoopEnd},
        };

        if(const auto result = ptrs.find(c); result != ptrs.end()){
            const auto method = result->second;
            (this->*method)();
        }
    }

    void write(std::ostream& stream){
        // check if all loops are closed
        if(!loops.empty()){
            throw std::runtime_error{"Unclosed [ found!"};
        }

        // exit syscall
        instructions_.emplace_back("mov $60, %rax");
        instructions_.emplace_back("mov $0, %rdi");
        instructions_.emplace_back("syscall");

        // print and clear
        for(const auto& instruction: instructions_){
            stream << instruction << std::endl;
        }

        instructions_.clear();
    }


private:
    struct loop {
        std::string startLabel;
        std::string endLabel;
    };

    std::vector<std::string> instructions_;
    std::stack<loop> loops;
    int nextLoopId = 0;


    void addPointerIncrement() { instructions_.emplace_back("inc %r12"); }
    void addPointerDecrement() { instructions_.emplace_back("dec %r12"); }
    void addDataIncrement() {
        instructions_.emplace_back("mov (%r12), %bl");
        instructions_.emplace_back("inc %bl");
        instructions_.emplace_back("mov %bl, (%r12)");
    }
    void addDataDecrement() {
        instructions_.emplace_back("mov (%r12), %bl");
        instructions_.emplace_back("dec %bl");
        instructions_.emplace_back("mov %bl, (%r12)");
    }

    void addOutput() {
        instructions_.emplace_back("mov $1, %rax");   // write syscall
        instructions_.emplace_back("mov $1, %rdi");   // stdout
        instructions_.emplace_back("mov %r12, %rsi"); // address of buffer
        instructions_.emplace_back("mov $1, %rdx");   // length of output
        instructions_.emplace_back("syscall");
    }

    void addInput() {
        instructions_.emplace_back("mov $0, %rax");   // read syscall
        instructions_.emplace_back("mov $0, %rdi");   // stdin
        instructions_.emplace_back("mov %r12, %rsi"); // address of buffer
        instructions_.emplace_back("mov $1, %rdx");   // length of output
        instructions_.emplace_back("syscall");
    }

    void addLoopBegin(){
        // generate a new lebel
        const auto [beginLabel, endLabel] = createNewLoop();

        // add a label to output
        instructions_.emplace_back(beginLabel + ":");

        // jump logic
        instructions_.emplace_back("mov (%r12), %bl");
        instructions_.emplace_back("test %bl, %bl");
        instructions_.emplace_back("jz " + endLabel);
    }

    void addLoopEnd(){
        // retrieve the corresponding label
        const auto [beginLabel, endLabel] = getTopLoop();

        // jump and label
        instructions_.emplace_back("jmp " + beginLabel);
        instructions_.emplace_back(endLabel + ":");
    }

    loop createNewLoop(){
        auto loopId = nextLoopId;
        nextLoopId++;
        auto loopIdStr = std::to_string(loopId);
        loops.push(loop{"beg" + loopIdStr, "end" + loopIdStr});
        return loops.top();
    }

    loop getTopLoop(){
        // check if we still have loops on stack
        if(loops.empty()){
            throw std::runtime_error{"Extra ] found!"};
        }

        // get position of loop start
        const auto labelPos = loops.top();
        loops.pop();

        return labelPos;
    }
};

int main(int argc, char* argv[]){
    if(argc != 2){
        std::cerr << "Bad arguments." << std::endl;
        std::cerr << "Usage: brainfuck FILENAME";
        return 1;
    }

    auto file = loadFile(argv[1]);

    try {
        Compiler compiler;
        for(const auto c: file) {
            compiler.addInstruction(c);
        }

        compiler.write(std::cout);

    } catch(const std::runtime_error& e){
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Aborting..." << std::endl;
        return 2;
    }
    return 0;
}
