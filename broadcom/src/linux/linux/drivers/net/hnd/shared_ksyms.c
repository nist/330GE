#include <linux/config.h>
#include <linux/module.h>
extern void dma_addrwidth; EXPORT_SYMBOL(dma_addrwidth);
extern void dma_attach; EXPORT_SYMBOL(dma_attach);
extern void dma_txpioloopback; EXPORT_SYMBOL(dma_txpioloopback);
extern void bcm_atoi; EXPORT_SYMBOL(bcm_atoi);
extern void bcm_binit; EXPORT_SYMBOL(bcm_binit);
extern void bcm_bitcount; EXPORT_SYMBOL(bcm_bitcount);
extern void bcm_bprintf; EXPORT_SYMBOL(bcm_bprintf);
extern void bcm_ctype; EXPORT_SYMBOL(bcm_ctype);
extern void bcmdumpfields; EXPORT_SYMBOL(bcmdumpfields);
extern void bcmerrorstr; EXPORT_SYMBOL(bcmerrorstr);
extern void bcm_ether_atoe; EXPORT_SYMBOL(bcm_ether_atoe);
extern void bcm_ether_ntoa; EXPORT_SYMBOL(bcm_ether_ntoa);
extern void bcm_iovar_lencheck; EXPORT_SYMBOL(bcm_iovar_lencheck);
extern void bcm_iovar_lookup; EXPORT_SYMBOL(bcm_iovar_lookup);
extern void bcm_mdelay; EXPORT_SYMBOL(bcm_mdelay);
extern void bcm_mkiovar; EXPORT_SYMBOL(bcm_mkiovar);
extern void bcm_mw_to_qdbm; EXPORT_SYMBOL(bcm_mw_to_qdbm);
extern void bcm_next_tlv; EXPORT_SYMBOL(bcm_next_tlv);
extern void bcm_nvram_cache; EXPORT_SYMBOL(bcm_nvram_cache);
extern void bcm_nvram_vars; EXPORT_SYMBOL(bcm_nvram_vars);
extern void bcm_parse_ordered_tlvs; EXPORT_SYMBOL(bcm_parse_ordered_tlvs);
extern void bcm_parse_tlvs; EXPORT_SYMBOL(bcm_parse_tlvs);
extern void bcm_qdbm_to_mw; EXPORT_SYMBOL(bcm_qdbm_to_mw);
extern void bcmstrcat; EXPORT_SYMBOL(bcmstrcat);
extern void bcmstrstr; EXPORT_SYMBOL(bcmstrstr);
extern void bcm_strtoul; EXPORT_SYMBOL(bcm_strtoul);
extern void getgpiopin; EXPORT_SYMBOL(getgpiopin);
extern void getintvar; EXPORT_SYMBOL(getintvar);
extern void getvar; EXPORT_SYMBOL(getvar);
extern void hndcrc16; EXPORT_SYMBOL(hndcrc16);
extern void hndcrc32; EXPORT_SYMBOL(hndcrc32);
extern void hndcrc8; EXPORT_SYMBOL(hndcrc8);
extern void pktcopy; EXPORT_SYMBOL(pktcopy);
extern void pktlast; EXPORT_SYMBOL(pktlast);
extern void pktq_deq; EXPORT_SYMBOL(pktq_deq);
extern void pktq_deq_tail; EXPORT_SYMBOL(pktq_deq_tail);
extern void pktq_flush; EXPORT_SYMBOL(pktq_flush);
extern void pktq_init; EXPORT_SYMBOL(pktq_init);
extern void pktq_mdeq; EXPORT_SYMBOL(pktq_mdeq);
extern void pktq_mlen; EXPORT_SYMBOL(pktq_mlen);
extern void pktq_pdel; EXPORT_SYMBOL(pktq_pdel);
extern void pktq_pdeq; EXPORT_SYMBOL(pktq_pdeq);
extern void pktq_pdeq_tail; EXPORT_SYMBOL(pktq_pdeq_tail);
extern void pktq_peek; EXPORT_SYMBOL(pktq_peek);
extern void pktq_peek_tail; EXPORT_SYMBOL(pktq_peek_tail);
extern void pktq_penq; EXPORT_SYMBOL(pktq_penq);
extern void pktq_penq_head; EXPORT_SYMBOL(pktq_penq_head);
extern void pktq_pflush; EXPORT_SYMBOL(pktq_pflush);
extern void pktq_setmax; EXPORT_SYMBOL(pktq_setmax);
extern void pktsetprio; EXPORT_SYMBOL(pktsetprio);
extern void pkttotlen; EXPORT_SYMBOL(pkttotlen);
extern void printfbig; EXPORT_SYMBOL(printfbig);
extern void srom_parsecis; EXPORT_SYMBOL(srom_parsecis);
extern void srom_read; EXPORT_SYMBOL(srom_read);
extern void srom_var_init; EXPORT_SYMBOL(srom_var_init);
extern void srom_write; EXPORT_SYMBOL(srom_write);
extern void sb_alp_clock; EXPORT_SYMBOL(sb_alp_clock);
extern void sb_attach; EXPORT_SYMBOL(sb_attach);
extern void sb_backplane64; EXPORT_SYMBOL(sb_backplane64);
extern void sb_base; EXPORT_SYMBOL(sb_base);
extern void sb_boardtype; EXPORT_SYMBOL(sb_boardtype);
extern void sb_boardvendor; EXPORT_SYMBOL(sb_boardvendor);
extern void sb_btcgpiowar; EXPORT_SYMBOL(sb_btcgpiowar);
extern void sb_bus; EXPORT_SYMBOL(sb_bus);
extern void sb_buscorerev; EXPORT_SYMBOL(sb_buscorerev);
extern void sb_buscoretype; EXPORT_SYMBOL(sb_buscoretype);
extern void sb_chip; EXPORT_SYMBOL(sb_chip);
extern void sb_chipcrev; EXPORT_SYMBOL(sb_chipcrev);
extern void sb_chippkg; EXPORT_SYMBOL(sb_chippkg);
extern void sb_chiprev; EXPORT_SYMBOL(sb_chiprev);
extern void sb_clkctl_clk; EXPORT_SYMBOL(sb_clkctl_clk);
extern void sb_clkctl_fast_pwrup_delay; EXPORT_SYMBOL(sb_clkctl_fast_pwrup_delay);
extern void sb_clkctl_init; EXPORT_SYMBOL(sb_clkctl_init);
extern void sb_clkctl_xtal; EXPORT_SYMBOL(sb_clkctl_xtal);
extern void sb_clock; EXPORT_SYMBOL(sb_clock);
extern void sb_clock_rate; EXPORT_SYMBOL(sb_clock_rate);
extern void sb_commit; EXPORT_SYMBOL(sb_commit);
extern void sb_corebist; EXPORT_SYMBOL(sb_corebist);
extern void sb_core_disable; EXPORT_SYMBOL(sb_core_disable);
extern void sb_coreflags; EXPORT_SYMBOL(sb_coreflags);
extern void sb_coreflagshi; EXPORT_SYMBOL(sb_coreflagshi);
extern void sb_coreid; EXPORT_SYMBOL(sb_coreid);
extern void sb_coreidx; EXPORT_SYMBOL(sb_coreidx);
extern void sb_corelist; EXPORT_SYMBOL(sb_corelist);
extern void sb_corepciid; EXPORT_SYMBOL(sb_corepciid);
extern void sb_coreregs; EXPORT_SYMBOL(sb_coreregs);
extern void sb_core_reset; EXPORT_SYMBOL(sb_core_reset);
extern void sb_corerev; EXPORT_SYMBOL(sb_corerev);
extern void sb_core_tofixup; EXPORT_SYMBOL(sb_core_tofixup);
extern void sb_coreunit; EXPORT_SYMBOL(sb_coreunit);
extern void sb_corevendor; EXPORT_SYMBOL(sb_corevendor);
extern void sb_d11_devid; EXPORT_SYMBOL(sb_d11_devid);
extern void sb_detach; EXPORT_SYMBOL(sb_detach);
extern void sb_deviceremoved; EXPORT_SYMBOL(sb_deviceremoved);
extern void sb_devpath; EXPORT_SYMBOL(sb_devpath);
extern void sb_findcoreidx; EXPORT_SYMBOL(sb_findcoreidx);
extern void sb_gpiocontrol; EXPORT_SYMBOL(sb_gpiocontrol);
extern void sb_gpioin; EXPORT_SYMBOL(sb_gpioin);
extern void sb_gpiointmask; EXPORT_SYMBOL(sb_gpiointmask);
extern void sb_gpiointpolarity; EXPORT_SYMBOL(sb_gpiointpolarity);
extern void sb_gpioled; EXPORT_SYMBOL(sb_gpioled);
extern void sb_gpioout; EXPORT_SYMBOL(sb_gpioout);
extern void sb_gpioouten; EXPORT_SYMBOL(sb_gpioouten);
extern void sb_gpiorelease; EXPORT_SYMBOL(sb_gpiorelease);
extern void sb_gpioreserve; EXPORT_SYMBOL(sb_gpioreserve);
extern void sb_gpiosetcore; EXPORT_SYMBOL(sb_gpiosetcore);
extern void sb_gpiotimerval; EXPORT_SYMBOL(sb_gpiotimerval);
extern void sb_iscoreup; EXPORT_SYMBOL(sb_iscoreup);
extern void sb_kattach; EXPORT_SYMBOL(sb_kattach);
extern void sb_osh; EXPORT_SYMBOL(sb_osh);
extern void sb_pcie_readreg; EXPORT_SYMBOL(sb_pcie_readreg);
extern void sb_pcie_writereg; EXPORT_SYMBOL(sb_pcie_writereg);
extern void sb_pcirev; EXPORT_SYMBOL(sb_pcirev);
extern void sb_pci_setup; EXPORT_SYMBOL(sb_pci_setup);
extern void sb_pcmcia_init; EXPORT_SYMBOL(sb_pcmcia_init);
extern void sb_pcmciarev; EXPORT_SYMBOL(sb_pcmciarev);
extern void sb_pmuctl_force_ilp; EXPORT_SYMBOL(sb_pmuctl_force_ilp);
extern void sb_pmuctl_pllinit; EXPORT_SYMBOL(sb_pmuctl_pllinit);
extern void sb_pmu_fast_pwrup_delay; EXPORT_SYMBOL(sb_pmu_fast_pwrup_delay);
extern void sb_pmu_paref_ldo_enable; EXPORT_SYMBOL(sb_pmu_paref_ldo_enable);
extern void sb_pmu_set_ldo_voltage; EXPORT_SYMBOL(sb_pmu_set_ldo_voltage);
extern void sb_pmu_set_res_mask; EXPORT_SYMBOL(sb_pmu_set_res_mask);
extern void sb_register_intr_callback; EXPORT_SYMBOL(sb_register_intr_callback);
extern void sb_setcore; EXPORT_SYMBOL(sb_setcore);
extern void sb_setcoreidx; EXPORT_SYMBOL(sb_setcoreidx);
extern void sb_set_initiator_to; EXPORT_SYMBOL(sb_set_initiator_to);
extern void sb_setosh; EXPORT_SYMBOL(sb_setosh);
extern void sb_set_switcher_voltage; EXPORT_SYMBOL(sb_set_switcher_voltage);
extern void sb_size; EXPORT_SYMBOL(sb_size);
extern void sb_socram_size; EXPORT_SYMBOL(sb_socram_size);
extern void sb_war16165; EXPORT_SYMBOL(sb_war16165);
extern void sb_war32414_forceHT; EXPORT_SYMBOL(sb_war32414_forceHT);
extern void sb_watchdog; EXPORT_SYMBOL(sb_watchdog);
extern void osl_attach; EXPORT_SYMBOL(osl_attach);
extern void osl_delay; EXPORT_SYMBOL(osl_delay);
extern void osl_detach; EXPORT_SYMBOL(osl_detach);
extern void osl_dma_alloc_consistent; EXPORT_SYMBOL(osl_dma_alloc_consistent);
extern void osl_dma_free_consistent; EXPORT_SYMBOL(osl_dma_free_consistent);
extern void osl_dma_map; EXPORT_SYMBOL(osl_dma_map);
extern void osl_dma_unmap; EXPORT_SYMBOL(osl_dma_unmap);
extern void osl_error; EXPORT_SYMBOL(osl_error);
extern void osl_malloc; EXPORT_SYMBOL(osl_malloc);
extern void osl_malloced; EXPORT_SYMBOL(osl_malloced);
extern void osl_malloc_failed; EXPORT_SYMBOL(osl_malloc_failed);
extern void osl_mfree; EXPORT_SYMBOL(osl_mfree);
extern void osl_pci_bus; EXPORT_SYMBOL(osl_pci_bus);
extern void osl_pci_read_config; EXPORT_SYMBOL(osl_pci_read_config);
extern void osl_pci_slot; EXPORT_SYMBOL(osl_pci_slot);
extern void osl_pci_write_config; EXPORT_SYMBOL(osl_pci_write_config);
extern void osl_pcmcia_read_attr; EXPORT_SYMBOL(osl_pcmcia_read_attr);
extern void osl_pcmcia_write_attr; EXPORT_SYMBOL(osl_pcmcia_write_attr);
extern void osl_pktalloced; EXPORT_SYMBOL(osl_pktalloced);
extern void osl_pktdup; EXPORT_SYMBOL(osl_pktdup);
extern void osl_pktfree; EXPORT_SYMBOL(osl_pktfree);
extern void osl_pktget; EXPORT_SYMBOL(osl_pktget);
extern void bcm_robo_attach; EXPORT_SYMBOL(bcm_robo_attach);
extern void bcm_robo_config_vlan; EXPORT_SYMBOL(bcm_robo_config_vlan);
extern void bcm_robo_detach; EXPORT_SYMBOL(bcm_robo_detach);
extern void bcm_robo_enable_device; EXPORT_SYMBOL(bcm_robo_enable_device);
extern void bcm_robo_enable_switch; EXPORT_SYMBOL(bcm_robo_enable_switch);
extern void pdesc25; EXPORT_SYMBOL(pdesc25);
extern void pdesc97; EXPORT_SYMBOL(pdesc97);
