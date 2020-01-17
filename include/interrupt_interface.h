#ifndef _interrupt_interface_h
#define _interrupt_interface_h

enum class Interrupt_priority_mask
{
    ALL = 0xff,
    EVEN = 0xfe,
    STEPS_4 = 0xfc,
    STEPS_8 = 0xf8,
    STEPS_16 = 0xf0
};

enum class Interrupt_binary_point
{
    GROUP_7_SUB_1 = 0,
    GROUP_6_SUB_2,
    GROUP_5_SUB_3,
    GROUP_4_SUB_4,
    GROUP_3_SUB_5,
    GROUP_2_SUB_6,
    GROUP_1_SUB_7,
    NO_PREEMPTION
};

struct Map_interrupt_interface
{
    unsigned int iccicr; // interface control register
    unsigned int iccpmr; // priority mask register
    unsigned int iccbpr; // binary point register
    unsigned int icciar; // acknowledge register
    unsigned int icceoir; // end of interrupt register
    unsigned int iccrpr; // running priority register
    unsigned int icchpir; // highest pending register
    unsigned int iccabpr; // aliased binady point
    unsigned int reserved0[8];
    unsigned int integ_en;
    unsigned int interrupt_out;
    unsigned int reserved1[2];
    unsigned int match_c;
    unsigned int enable_c;
    unsigned int reserved2[41];
    unsigned int cpu_if_ident;
    unsigned int reserved3[944];
    unsigned int periph_id_8;
    unsigned int reserved4[3];
    unsigned int periph_id_7_4[4];
    unsigned int periph_id_3_0[4];
    unsigned int component_id[4];
};

struct Acknowledge_pair {unsigned int cpu; unsigned int id;};

class Interrupt_interface
{
public:
    Interrupt_interface(unsigned int address) : _map((Map_interrupt_interface *) address) {}
    ~Interrupt_interface() {}

    void enable(unsigned char value);
    void disable();
    bool is_enabled();

    void priority_mask(Interrupt_priority_mask mask);
    Interrupt_priority_mask priority_mask();

    void binary_point(Interrupt_binary_point point);
    Interrupt_binary_point binary_point();

    Acknowledge_pair interrupt_acknowledge();
    void interrupt_end(unsigned int cpu, unsigned int id);

    unsigned char running_priority();
    Interrupt_binary_point binary_point_aliased();
    Acknowledge_pair highest_pending_interrupt();

private:
    Map_interrupt_interface * _map;

};

extern Interrupt_interface interrupt_interface;

#endif
