
#include <modbus/modbus.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>

int main(int argc, char* argv[]) {
    if(argc < 11) {
        std::cout << "Usage: " << argv[0] << " <port> <baud> <data bits> <parity N/E/O> "
                  << "<stop bits> <slave id> <start reg> <num reg> <timeout ms> <type H/I>\n";
        std::cout << "Type: H=holding, I=input\n";
        return 1;
    }

    const char* port = argv[1];
    int baud = std::stoi(argv[2]);
    int data_bits = std::stoi(argv[3]);
    char parity = argv[4][0];
    int stop_bits = std::stoi(argv[5]);
    int slave_id = std::stoi(argv[6]);
    int start_reg = std::stoi(argv[7]);
    int num_reg = std::stoi(argv[8]);
    int timeout_ms = std::stoi(argv[9]);
    char reg_type = toupper(argv[10][0]);

    modbus_t* ctx = modbus_new_rtu(port, baud, parity, data_bits, stop_bits);
    if(!ctx) { std::cerr << "Unable to create Modbus context\n"; return 1; }

    modbus_set_slave(ctx, slave_id);
    modbus_set_response_timeout(ctx, timeout_ms / 1000, (timeout_ms % 1000) * 1000);

    if(modbus_connect(ctx) == -1) {
        std::cerr << "Connection failed: " << modbus_strerror(errno) << "\n";
        modbus_free(ctx);
        return 1;
    }

    std::vector<uint16_t> regs(num_reg);
    int rc = -1;

    if(reg_type == 'H') {
        rc = modbus_read_registers(ctx, start_reg, num_reg, regs.data());
    } else if(reg_type == 'I') {
        rc = modbus_read_input_registers(ctx, start_reg, num_reg, regs.data());
    } else {
        std::cerr << "Invalid register type. Use H for holding, I for input.\n";
        modbus_close(ctx);
        modbus_free(ctx);
        return 1;
    }

    if(rc == -1) {
        std::cerr << "Read failed: " << modbus_strerror(errno) << "\n";
    } else {
        for(int i = 0; i < rc; ++i) {
            std::cout << "Reg " << (start_reg + i) << ": "
                      << "0x" << std::hex << regs[i] << std::dec
                      << " (" << regs[i] << ")\n";
        }
    }

    modbus_close(ctx);
    modbus_free(ctx);

    return 0; // fully frees memory, exits, no background processes
}

