#ifndef _interrupt_distributor_h
#define _interrupt_distributor_h

struct Map_interrupt_distributor
{
    unsigned int icddcr; // control register
    unsigned int icdictr; // controller type register
    unsigned int icdiidr; // inplementer indentyfication register
    unsigned int reserved0[29];
    unsigned int icdisr[32]; // security register
    unsigned int icdiser[32]; // set enable register
    unsigned int icdicer[32]; // clear enable register
    unsigned int icdispr[32]; // set pending register
    unsigned int icdicpr[32]; // clear enable register
    unsigned int icdabr[32]; // active bit register
    unsigned int reserved1[32];
    unsigned int icdipr[255]; // priority register
    unsigned int reserved2;
    unsigned int icdiptr[255]; // processor target register
    unsigned int reserved3;
    unsigned int icdicr[64]; // configuration register
    unsigned int ppisr;
    unsigned int spisr[31];
    unsigned int reserved4[21];
    unsigned int legacy_int;
    unsigned int reserved5[2];
    unsigned int match_d;
    unsigned int enable_d;
    unsigned int reserved6[70];
    unsigned int icdsgir; // software generated interrupt register
    unsigned int reserved7[47];
    unsigned int periph_id_8;
    unsigned int reserved8[3];
    unsigned int periph_id_7_4[4];
    unsigned int periph_id_3_0[4];
    unsigned int component_id[4];
};

enum class Interrupt_config
{
    LEVEL = 0,
    EDGE
};

enum class Interrupt_target
{
    CPU0 = 0x01,
    CPU1 = 0x02,
    CPU2 = 0x04,
    CPU3 = 0x08,
    CPU4 = 0x10,
    CPU5 = 0x20,
    CPU6 = 0x40,
    CPU7 = 0x80,
    ALL
};

enum class Interrupt_security
{
	SECURE = 0,
	NON_SECURE
};

enum class Interrupt_swi_target
{
	TO_LISTED = 0,
	TO_OTHERS,
	TO_ME
};

class Interrupt_distributor
{
public:
	Interrupt_distributor(unsigned int address) : _map((Map_interrupt_distributor *) address) {}
    ~Interrupt_distributor() {}

    void enable();
    void disable();
    bool is_enabled();

    void irq_security(unsigned int id, Interrupt_security security);
    Interrupt_security irq_security(unsigned int id);

    void irq_enable(unsigned int id);
    void irq_disable(unsigned int id);
    bool irq_is_enabled(unsigned int id);

    void irq_pending_set(unsigned int id);
    void irq_pending_clear(unsigned int id);
    bool irq_is_pending(unsigned int id);

    bool irq_is_active(unsigned int id);

    void irq_priority(unsigned int id, unsigned char priority);
    unsigned char irq_priority(unsigned int id);

    void irq_target(unsigned int id, Interrupt_target target_cpu);
    Interrupt_target irq_target(unsigned int id);

    void irq_config(unsigned int id, Interrupt_config config);
    Interrupt_config irq_config(unsigned int id);

    void swi_generate(unsigned int id, Interrupt_swi_target target_cpu = Interrupt_swi_target::TO_OTHERS, Interrupt_target target_list = Interrupt_target::CPU0, Interrupt_security security = Interrupt_security::SECURE);

private:
    Map_interrupt_distributor * _map;

};

extern Interrupt_distributor interrupt_distributor;

#endif
