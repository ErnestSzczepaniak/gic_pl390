#include "interrupt_distributor.h"

Interrupt_distributor interrupt_distributor(0xfffed000);

void Interrupt_distributor::enable()
{
    _map->icddcr |= 1;
}

void Interrupt_distributor::disable()
{
    _map->icddcr &= ~1;
}

bool Interrupt_distributor::is_enabled()
{
    return (_map->icddcr);
}

void Interrupt_distributor::irq_security(unsigned int id, Interrupt_security security)
{
	auto index = id / 32;
	auto bit = id % 32;

	security == Interrupt_security::NON_SECURE ? _map->icdisr[index] |= 1 << bit : _map->icdisr[index] &= ~1 << bit;
}

Interrupt_security Interrupt_distributor::irq_security(unsigned int id)
{
	auto index = id / 32;
	auto bit = id % 32;

	return (Interrupt_security)(_map->icdisr[index] >> bit & 1);
}

void Interrupt_distributor::irq_enable(unsigned int id)
{
	auto index = id / 32;
	auto bit = id % 32;

	_map->icdiser[index] |= 1 << bit;
}

void Interrupt_distributor::irq_disable(unsigned int id)
{
	auto index = id / 32;
	auto bit = id % 32;

	_map->icdicer[index] |= 1 << bit;
}

bool Interrupt_distributor::irq_is_enabled(unsigned int id)
{
	auto index = id / 32;
	auto bit = id % 32;

	return _map->icdiser[index] >> bit & 1 ? true : false;
}

void Interrupt_distributor::irq_pending_set(unsigned int id)
{
	auto index = id / 32;
	auto bit = id % 32;

	_map->icdispr[index] |= 1 << bit;
}

void Interrupt_distributor::irq_pending_clear(unsigned int id)
{
	auto index = id / 32;
	auto bit = id % 32;

	_map->icdicpr[index] |= 1 << bit;
}

bool Interrupt_distributor::irq_is_pending(unsigned int id)
{
	auto index = id / 32;
	auto bit = id % 32;

	return _map->icdispr[index] >> bit & 1 ? true : false;
}

bool Interrupt_distributor::irq_is_active(unsigned int id)
{
	auto index = id / 32;
	auto bit = id % 32;

	return _map->icdabr[index] >> bit & 1 ? true : false;
}

void Interrupt_distributor::irq_priority(unsigned int id, unsigned char priority)
{
    auto index = id / 4;
	auto block = id % 4;

    _map->icdipr[index] &= ~(0xff << block * 8);
	_map->icdipr[index] |= priority << block * 8;
}

unsigned char Interrupt_distributor::irq_priority(unsigned int id)
{
    auto index = id / 4;
	auto block = id % 4;

    return ((_map->icdipr[index] >> block * 8 ) & 0xff);
}

void Interrupt_distributor::irq_target(unsigned int id, Interrupt_target target_cpu)
{
    auto index = id / 4;
	auto block = id % 4;

    _map->icdiptr[index] &= ~(0xff << block * 8);
    _map->icdiptr[index] |= (unsigned int)target_cpu << block * 8;
}

Interrupt_target Interrupt_distributor::irq_target(unsigned int id)
{
    auto index = id / 4;
	auto block = id % 4;

    return (Interrupt_target)((_map->icdiptr[index] >> block * 8) & 0xff);
}

void Interrupt_distributor::irq_config(unsigned int id, Interrupt_config config)
{
    if (id > 31)
    {
        auto index = id / 16;
	    auto block = id % 16;

        config == Interrupt_config::EDGE ? _map->icdicr[index] |= 2 << block * 2 : _map->icdicr[index] &= ~2 << block * 2;
    }
    else
    {

    }
}

Interrupt_config Interrupt_distributor::irq_config(unsigned int id)
{
    auto index = id / 16;
    auto block = id % 16;

    return (Interrupt_config)(_map->icdicr[index] >> ((block * 2) + 1) & 0xff);
}

void Interrupt_distributor::swi_generate(unsigned int id, Interrupt_swi_target target_cpu, Interrupt_target target_list, Interrupt_security security)
{
    _map->icdsgir = ((unsigned int)target_cpu << 24) | ((unsigned int) target_list << 16) | ((unsigned int)security << 15) | id;
}
