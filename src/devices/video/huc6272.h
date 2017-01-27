// license:BSD-3-Clause
// copyright-holders:Wilbert Pol, Angelo Salese
/***************************************************************************

    Hudson/NEC HuC6272 "King" device

***************************************************************************/

#pragma once

#ifndef __huc6272DEV_H__
#define __huc6272DEV_H__

#include "bus/scsi/scsi.h"
#include "bus/scsi/scsicd.h"


//**************************************************************************
//  INTERFACE CONFIGURATION MACROS
//**************************************************************************

#define MCFG_HUC6272_ADD(_tag,_freq) \
	MCFG_DEVICE_ADD(_tag, huc6272, _freq)

#define MCFG_HUC6272_IRQ_CHANGED_CB(_devcb) \
	devcb = &huc6272_device::set_irq_changed_callback(*device, DEVCB_##_devcb);


//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

// ======================> huc6272_device

class huc6272_device :  public device_t,
						public device_memory_interface
{
public:
	// construction/destruction
	huc6272_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	template<class _Object> static devcb_base &set_irq_changed_callback(device_t &device, _Object object) { return downcast<huc6272_device &>(device).m_irq_changed_cb.set_callback(object); }

	// I/O operations
	DECLARE_WRITE32_MEMBER( write );
	DECLARE_READ32_MEMBER( read );

protected:
	// device-level overrides
	virtual void device_validity_check(validity_checker &valid) const override;
	virtual void device_start() override;
	virtual void device_reset() override;
	virtual machine_config_constructor device_mconfig_additions() const override;
	virtual const address_space_config *memory_space_config(address_spacenum spacenum = AS_PROGRAM) const override;

private:
	uint8_t m_register;
	uint32_t m_kram_addr_r, m_kram_addr_w;
	uint16_t m_kram_inc_r,m_kram_inc_w;
	uint8_t m_kram_page_r,m_kram_page_w;
	uint32_t m_page_setting;
	
	struct{
		uint32_t bat_address;
		uint32_t cg_address;
		uint8_t mode;
		uint16_t height;
		uint16_t width;
		uint16_t xscroll;
		uint16_t yscroll;
		uint8_t priority;
	}m_bg[4];
	
	struct{
		uint32_t bat_address;
		uint32_t cg_address;
		uint16_t height;
		uint16_t width;
	}m_bg0sub;
	
	
	struct{
		uint8_t index;
		uint8_t ctrl;
	}m_micro_prg;

	const address_space_config      m_program_space_config;
	const address_space_config      m_data_space_config;
	required_shared_ptr<uint16_t> 	m_microprg_ram;
	required_shared_ptr<uint32_t>   m_kram_page0;
	required_shared_ptr<uint32_t>   m_kram_page1;
	required_device<SCSI_PORT_DEVICE> m_scsibus;
	required_device<input_buffer_device> m_scsi_data_in;
	required_device<output_latch_device> m_scsi_data_out;
	required_device<input_buffer_device> m_scsi_ctrl_in;

	/* Callback for when the irq line may have changed (mandatory) */
	devcb_write_line    m_irq_changed_cb;
	
	uint32_t read_dword(offs_t address);
	void write_dword(offs_t address, uint32_t data);
	void write_microprg_data(offs_t address, uint16_t data);
};

// device type definition
extern const device_type huc6272;



//**************************************************************************
//  GLOBAL VARIABLES
//**************************************************************************



#endif
