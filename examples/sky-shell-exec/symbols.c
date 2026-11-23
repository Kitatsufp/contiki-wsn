#include "loader/symbols.h"

extern int abc_close();
extern int abc_input();
extern int abc_open();
extern int abc_send();
extern int abc_sent();
extern int announcement_bump();
extern int announcement_heard();
extern int announcement_init();
extern int announcement_list();
extern int announcement_listen();
extern int announcement_register();
extern int announcement_register_listen_callback();
extern int announcement_register_observer_callback();
extern int announcement_remove();
extern int announcement_remove_value();
extern int announcement_set_value();
extern int autostart_exit();
extern int autostart_processes();
extern int autostart_start();
extern int broadcast_announcement_beacon_interval();
extern int broadcast_announcement_init();
extern int broadcast_announcement_stop();
extern int broadcast_close();
extern int broadcast_open();
extern int broadcast_send();
extern int button_sensor();
extern int cc2420_aes_128_driver();
extern int cc2420_arch_init();
extern int cc2420_authority_level_of_sender();
extern int cc2420_driver();
extern int cc2420_get_channel();
extern int cc2420_get_txpower();
extern int cc2420_init();
extern int cc2420_interrupt();
extern int cc2420_last_correlation();
extern int cc2420_last_rssi();
extern int cc2420_off();
extern int cc2420_on();
extern int cc2420_port1_interrupt();
extern int cc2420_process();
extern int cc2420_rssi();
extern int cc2420_set_cca_threshold();
extern int cc2420_set_channel();
extern int cc2420_set_pan_addr();
extern int cc2420_set_txpower();
extern int cc2420_sfd_counter();
extern int cc2420_sfd_end_time();
extern int cc2420_sfd_start_time();
extern int cc2420_time_of_arrival();
extern int cc2420_time_of_departure();
extern int cfs_close();
extern int cfs_closedir();
extern int cfs_coffee_configure_log();
extern int cfs_coffee_format();
extern int cfs_coffee_reserve();
extern int cfs_open();
extern int cfs_opendir();
extern int cfs_read();
extern int cfs_readdir();
extern int cfs_remove();
extern int cfs_seek();
extern int cfs_write();
extern int chameleon_bitopt();
extern int chameleon_create();
extern int chameleon_hdrsize();
extern int chameleon_init();
extern int chameleon_parse();
extern int channel_close();
extern int channel_init();
extern int channel_lookup();
extern int channel_open();
extern int channel_set_attributes();
extern int clock_counter();
extern int clock_delay();
extern int clock_fine();
extern int clock_fine_max();
extern int clock_init();
extern int clock_seconds();
extern int clock_set();
extern int clock_set_seconds();
extern int clock_time();
extern int clock_wait();
extern int compower_accumulate();
extern int compower_accumulate_attrs();
extern int compower_attrconv();
extern int compower_clear();
extern int compower_idle_activity();
extern int compower_init();
extern int contikimac_debug_print();
extern int contikimac_driver();
extern int contikimac_framer();
extern int csma_driver();
extern int ctimer_expired();
extern int ctimer_init();
extern int ctimer_process();
extern int ctimer_reset();
extern int ctimer_restart();
extern int ctimer_set();
extern int ctimer_set_with_process();
extern int ctimer_stop();
extern int ds2411_id();
extern int ds2411_init();
extern int elfloader_arch_allocate_ram();
extern int elfloader_arch_allocate_rom();
extern int elfloader_arch_relocate();
extern int elfloader_arch_write_rom();
extern int elfloader_autostart_processes();
extern int elfloader_init();
extern int elfloader_load();
extern int elfloader_unknown();
extern int energest_current_mode();
extern int energest_current_time();
extern int energest_flush();
extern int energest_init();
extern int energest_leveldevice_leveltime();
extern int energest_total_count();
extern int energest_total_time();
extern int energest_type_set();
extern int energest_type_time();
extern int etimer_adjust();
extern int etimer_expiration_time();
extern int etimer_expired();
extern int etimer_next_expiration_time();
extern int etimer_pending();
extern int etimer_process();
extern int etimer_request_poll();
extern int etimer_reset();
extern int etimer_reset_with_new_interval();
extern int etimer_restart();
extern int etimer_set();
extern int etimer_start_time();
extern int etimer_stop();
extern int frame802154_check_dest_panid();
extern int frame802154_create();
extern int frame802154_extract_linkaddr();
extern int frame802154_get_pan_id();
extern int frame802154_has_panid();
extern int frame802154_hdrlen();
extern int frame802154_is_broadcast_addr();
extern int frame802154_parse();
extern int frame802154_set_pan_id();
extern int framer_802154();
extern int get_bits();
extern int help_command_process();
extern int init_platform();
extern int irq_p2();
extern int leds_arch_get();
extern int leds_arch_init();
extern int leds_arch_set();
extern int leds_blink();
extern int leds_get();
extern int leds_init();
extern int leds_off();
extern int leds_on();
extern int leds_set();
extern int leds_toggle();
extern int linkaddr_cmp();
extern int linkaddr_copy();
extern int linkaddr_node_addr();
extern int linkaddr_null();
extern int linkaddr_set_node_addr();
extern int list_add();
extern int list_chop();
extern int list_copy();
extern int list_head();
extern int list_init();
extern int list_insert();
extern int list_item_next();
extern int list_length();
extern int list_pop();
extern int list_push();
extern int list_remove();
extern int list_tail();
extern int mac_call_sent_callback();
extern int mac_sequence_is_duplicate();
extern int mac_sequence_register_seqno();
extern int main();
extern int memb_alloc();
extern int memb_free();
extern int memb_init();
extern int memb_inmemb();
extern int memb_numfree();
extern int memcmp();
void *memcpy();
void *memset();
extern int msp430_add_lpm_req();
extern int msp430_cpu_init();
extern int msp430_dco_required();
extern int msp430_init_dco();
extern int msp430_remove_lpm_req();
extern int msp430_sync_dco();
extern int node_id();
extern int node_id_burn();
extern int node_id_restore();
extern int nullsec_driver();
extern int packetbuf_addrs();
extern int packetbuf_attr_clear();
extern int packetbuf_attr_copyfrom();
extern int packetbuf_attr_copyto();
extern int packetbuf_attrs();
extern int packetbuf_clear();
extern int packetbuf_compact();
extern int packetbuf_copyfrom();
extern int packetbuf_copyto();
extern int packetbuf_datalen();
extern int packetbuf_dataptr();
extern int packetbuf_hdralloc();
extern int packetbuf_hdrlen();
extern int packetbuf_hdrptr();
extern int packetbuf_hdrreduce();
extern int packetbuf_holds_broadcast();
extern int packetbuf_set_datalen();
extern int packetbuf_totlen();
int printf(const char *, ...);
extern int process_alloc_event();
extern int process_current();
extern int process_exit();
extern int process_init();
extern int process_is_running();
extern int process_list();
extern int process_maxevents();
extern int process_nevents();
extern int process_poll();
extern int process_post();
extern int process_post_synch();
extern int process_run();
extern int process_start();
extern int putchar();
extern int puts();
extern int queuebuf_addr();
extern int queuebuf_attr();
extern int queuebuf_datalen();
extern int queuebuf_dataptr();
extern int queuebuf_debug_print();
extern int queuebuf_free();
extern int queuebuf_init();
extern int queuebuf_new_from_packetbuf();
extern int queuebuf_numfree();
extern int queuebuf_to_packetbuf();
extern int queuebuf_update_attr_from_packetbuf();
extern int queuebuf_update_from_packetbuf();
extern int rand();
extern int rand_r();
extern int random_init();
extern int random_rand();
extern int rime_driver();
extern int rime_output();
extern int rime_sniffer_add();
extern int rime_sniffer_remove();
extern int ringbuf_elements();
extern int ringbuf_get();
extern int ringbuf_init();
extern int ringbuf_put();
extern int ringbuf_size();
extern int rtimer_arch_init();
extern int rtimer_arch_now();
extern int rtimer_arch_schedule();
extern int rtimer_init();
extern int rtimer_run_next();
extern int rtimer_set();
extern int sbrk();
extern int sensors();
extern int sensors_changed();
extern int sensors_event();
extern int sensors_find();
extern int sensors_first();
extern int sensors_flags();
extern int sensors_next();
extern int sensors_process();
extern int serial_line_event_message();
extern int serial_line_init();
extern int serial_line_input_byte();
extern int serial_line_process();
extern int serial_shell_init();
extern int serial_shell_process();
extern int set_bits();
extern int shell_append_process();
extern int shell_base64_init();
extern int shell_binprint_process();
extern int shell_coffee_init();
extern int shell_dec64_process();
extern int shell_default_output();
extern int shell_echo_process();
extern int shell_event_input();
extern int shell_exec_init();
extern int shell_exec_process();
extern int shell_exit();
extern int shell_exit_process();
extern int shell_file_init();
extern int shell_format_process();
extern int shell_hd_process();
extern int shell_init();
extern int shell_input();
extern int shell_kill_process();
extern int shell_killall_process();
extern int shell_ls_process();
extern int shell_null_process();
extern int shell_output();
extern int shell_output_str();
extern int shell_process();
extern int shell_prompt();
extern int shell_quit();
extern int shell_read_process();
extern int shell_register_command();
extern int shell_rm_process();
extern int shell_server_process();
extern int shell_set_time();
extern int shell_size_process();
extern int shell_start();
extern int shell_start_command();
extern int shell_stop();
extern int shell_strtolong();
extern int shell_text_init();
extern int shell_time();
extern int shell_unregister_command();
extern int shell_write_process();
extern int sky_shell_process();
extern int snprintf();
extern int spi_busy();
extern int spi_init();
extern int splhigh_();
int sprintf(char *, const char *, ...);
extern int srand();
char *strchr();
extern int strcmp();
extern int strncmp();
extern int strncpy();
extern int symtab_lookup();
extern int timer_expired();
extern int timer_remaining();
extern int timer_reset();
extern int timer_restart();
extern int timer_set();
extern int timera0();
extern int timera1();
extern int uart1_active();
extern int uart1_init();
extern int uart1_set_input();
extern int uart1_writeb();
extern int uip_log();
extern int vprintf();
extern int vsnprintf();
extern int vsprintf();
extern int vuprintf();
extern int watchdog_init();
extern int watchdog_interrupt();
extern int watchdog_periodic();
extern int watchdog_reboot();
extern int watchdog_start();
extern int watchdog_stop();
extern int xmem_erase();
extern int xmem_init();
extern int xmem_pread();
extern int xmem_pwrite();


const int symbols_nelts = 355;
const struct symbols symbols[355] = {
{ "abc_close", (void *)&abc_close },
{ "abc_input", (void *)&abc_input },
{ "abc_open", (void *)&abc_open },
{ "abc_send", (void *)&abc_send },
{ "abc_sent", (void *)&abc_sent },
{ "announcement_bump", (void *)&announcement_bump },
{ "announcement_heard", (void *)&announcement_heard },
{ "announcement_init", (void *)&announcement_init },
{ "announcement_list", (void *)&announcement_list },
{ "announcement_listen", (void *)&announcement_listen },
{ "announcement_register", (void *)&announcement_register },
{ "announcement_register_listen_callback", (void *)&announcement_register_listen_callback },
{ "announcement_register_observer_callback", (void *)&announcement_register_observer_callback },
{ "announcement_remove", (void *)&announcement_remove },
{ "announcement_remove_value", (void *)&announcement_remove_value },
{ "announcement_set_value", (void *)&announcement_set_value },
{ "autostart_exit", (void *)&autostart_exit },
{ "autostart_processes", (void *)&autostart_processes },
{ "autostart_start", (void *)&autostart_start },
{ "broadcast_announcement_beacon_interval", (void *)&broadcast_announcement_beacon_interval },
{ "broadcast_announcement_init", (void *)&broadcast_announcement_init },
{ "broadcast_announcement_stop", (void *)&broadcast_announcement_stop },
{ "broadcast_close", (void *)&broadcast_close },
{ "broadcast_open", (void *)&broadcast_open },
{ "broadcast_send", (void *)&broadcast_send },
{ "button_sensor", (void *)&button_sensor },
{ "cc2420_aes_128_driver", (void *)&cc2420_aes_128_driver },
{ "cc2420_arch_init", (void *)&cc2420_arch_init },
{ "cc2420_authority_level_of_sender", (void *)&cc2420_authority_level_of_sender },
{ "cc2420_driver", (void *)&cc2420_driver },
{ "cc2420_get_channel", (void *)&cc2420_get_channel },
{ "cc2420_get_txpower", (void *)&cc2420_get_txpower },
{ "cc2420_init", (void *)&cc2420_init },
{ "cc2420_interrupt", (void *)&cc2420_interrupt },
{ "cc2420_last_correlation", (void *)&cc2420_last_correlation },
{ "cc2420_last_rssi", (void *)&cc2420_last_rssi },
{ "cc2420_off", (void *)&cc2420_off },
{ "cc2420_on", (void *)&cc2420_on },
{ "cc2420_port1_interrupt", (void *)&cc2420_port1_interrupt },
{ "cc2420_process", (void *)&cc2420_process },
{ "cc2420_rssi", (void *)&cc2420_rssi },
{ "cc2420_set_cca_threshold", (void *)&cc2420_set_cca_threshold },
{ "cc2420_set_channel", (void *)&cc2420_set_channel },
{ "cc2420_set_pan_addr", (void *)&cc2420_set_pan_addr },
{ "cc2420_set_txpower", (void *)&cc2420_set_txpower },
{ "cc2420_sfd_counter", (void *)&cc2420_sfd_counter },
{ "cc2420_sfd_end_time", (void *)&cc2420_sfd_end_time },
{ "cc2420_sfd_start_time", (void *)&cc2420_sfd_start_time },
{ "cc2420_time_of_arrival", (void *)&cc2420_time_of_arrival },
{ "cc2420_time_of_departure", (void *)&cc2420_time_of_departure },
{ "cfs_close", (void *)&cfs_close },
{ "cfs_closedir", (void *)&cfs_closedir },
{ "cfs_coffee_configure_log", (void *)&cfs_coffee_configure_log },
{ "cfs_coffee_format", (void *)&cfs_coffee_format },
{ "cfs_coffee_reserve", (void *)&cfs_coffee_reserve },
{ "cfs_open", (void *)&cfs_open },
{ "cfs_opendir", (void *)&cfs_opendir },
{ "cfs_read", (void *)&cfs_read },
{ "cfs_readdir", (void *)&cfs_readdir },
{ "cfs_remove", (void *)&cfs_remove },
{ "cfs_seek", (void *)&cfs_seek },
{ "cfs_write", (void *)&cfs_write },
{ "chameleon_bitopt", (void *)&chameleon_bitopt },
{ "chameleon_create", (void *)&chameleon_create },
{ "chameleon_hdrsize", (void *)&chameleon_hdrsize },
{ "chameleon_init", (void *)&chameleon_init },
{ "chameleon_parse", (void *)&chameleon_parse },
{ "channel_close", (void *)&channel_close },
{ "channel_init", (void *)&channel_init },
{ "channel_lookup", (void *)&channel_lookup },
{ "channel_open", (void *)&channel_open },
{ "channel_set_attributes", (void *)&channel_set_attributes },
{ "clock_counter", (void *)&clock_counter },
{ "clock_delay", (void *)&clock_delay },
{ "clock_fine", (void *)&clock_fine },
{ "clock_fine_max", (void *)&clock_fine_max },
{ "clock_init", (void *)&clock_init },
{ "clock_seconds", (void *)&clock_seconds },
{ "clock_set", (void *)&clock_set },
{ "clock_set_seconds", (void *)&clock_set_seconds },
{ "clock_time", (void *)&clock_time },
{ "clock_wait", (void *)&clock_wait },
{ "compower_accumulate", (void *)&compower_accumulate },
{ "compower_accumulate_attrs", (void *)&compower_accumulate_attrs },
{ "compower_attrconv", (void *)&compower_attrconv },
{ "compower_clear", (void *)&compower_clear },
{ "compower_idle_activity", (void *)&compower_idle_activity },
{ "compower_init", (void *)&compower_init },
{ "contikimac_debug_print", (void *)&contikimac_debug_print },
{ "contikimac_driver", (void *)&contikimac_driver },
{ "contikimac_framer", (void *)&contikimac_framer },
{ "csma_driver", (void *)&csma_driver },
{ "ctimer_expired", (void *)&ctimer_expired },
{ "ctimer_init", (void *)&ctimer_init },
{ "ctimer_process", (void *)&ctimer_process },
{ "ctimer_reset", (void *)&ctimer_reset },
{ "ctimer_restart", (void *)&ctimer_restart },
{ "ctimer_set", (void *)&ctimer_set },
{ "ctimer_set_with_process", (void *)&ctimer_set_with_process },
{ "ctimer_stop", (void *)&ctimer_stop },
{ "ds2411_id", (void *)&ds2411_id },
{ "ds2411_init", (void *)&ds2411_init },
{ "elfloader_arch_allocate_ram", (void *)&elfloader_arch_allocate_ram },
{ "elfloader_arch_allocate_rom", (void *)&elfloader_arch_allocate_rom },
{ "elfloader_arch_relocate", (void *)&elfloader_arch_relocate },
{ "elfloader_arch_write_rom", (void *)&elfloader_arch_write_rom },
{ "elfloader_autostart_processes", (void *)&elfloader_autostart_processes },
{ "elfloader_init", (void *)&elfloader_init },
{ "elfloader_load", (void *)&elfloader_load },
{ "elfloader_unknown", (void *)&elfloader_unknown },
{ "energest_current_mode", (void *)&energest_current_mode },
{ "energest_current_time", (void *)&energest_current_time },
{ "energest_flush", (void *)&energest_flush },
{ "energest_init", (void *)&energest_init },
{ "energest_leveldevice_leveltime", (void *)&energest_leveldevice_leveltime },
{ "energest_total_count", (void *)&energest_total_count },
{ "energest_total_time", (void *)&energest_total_time },
{ "energest_type_set", (void *)&energest_type_set },
{ "energest_type_time", (void *)&energest_type_time },
{ "etimer_adjust", (void *)&etimer_adjust },
{ "etimer_expiration_time", (void *)&etimer_expiration_time },
{ "etimer_expired", (void *)&etimer_expired },
{ "etimer_next_expiration_time", (void *)&etimer_next_expiration_time },
{ "etimer_pending", (void *)&etimer_pending },
{ "etimer_process", (void *)&etimer_process },
{ "etimer_request_poll", (void *)&etimer_request_poll },
{ "etimer_reset", (void *)&etimer_reset },
{ "etimer_reset_with_new_interval", (void *)&etimer_reset_with_new_interval },
{ "etimer_restart", (void *)&etimer_restart },
{ "etimer_set", (void *)&etimer_set },
{ "etimer_start_time", (void *)&etimer_start_time },
{ "etimer_stop", (void *)&etimer_stop },
{ "frame802154_check_dest_panid", (void *)&frame802154_check_dest_panid },
{ "frame802154_create", (void *)&frame802154_create },
{ "frame802154_extract_linkaddr", (void *)&frame802154_extract_linkaddr },
{ "frame802154_get_pan_id", (void *)&frame802154_get_pan_id },
{ "frame802154_has_panid", (void *)&frame802154_has_panid },
{ "frame802154_hdrlen", (void *)&frame802154_hdrlen },
{ "frame802154_is_broadcast_addr", (void *)&frame802154_is_broadcast_addr },
{ "frame802154_parse", (void *)&frame802154_parse },
{ "frame802154_set_pan_id", (void *)&frame802154_set_pan_id },
{ "framer_802154", (void *)&framer_802154 },
{ "get_bits", (void *)&get_bits },
{ "help_command_process", (void *)&help_command_process },
{ "init_platform", (void *)&init_platform },
{ "irq_p2", (void *)&irq_p2 },
{ "leds_arch_get", (void *)&leds_arch_get },
{ "leds_arch_init", (void *)&leds_arch_init },
{ "leds_arch_set", (void *)&leds_arch_set },
{ "leds_blink", (void *)&leds_blink },
{ "leds_get", (void *)&leds_get },
{ "leds_init", (void *)&leds_init },
{ "leds_off", (void *)&leds_off },
{ "leds_on", (void *)&leds_on },
{ "leds_set", (void *)&leds_set },
{ "leds_toggle", (void *)&leds_toggle },
{ "linkaddr_cmp", (void *)&linkaddr_cmp },
{ "linkaddr_copy", (void *)&linkaddr_copy },
{ "linkaddr_node_addr", (void *)&linkaddr_node_addr },
{ "linkaddr_null", (void *)&linkaddr_null },
{ "linkaddr_set_node_addr", (void *)&linkaddr_set_node_addr },
{ "list_add", (void *)&list_add },
{ "list_chop", (void *)&list_chop },
{ "list_copy", (void *)&list_copy },
{ "list_head", (void *)&list_head },
{ "list_init", (void *)&list_init },
{ "list_insert", (void *)&list_insert },
{ "list_item_next", (void *)&list_item_next },
{ "list_length", (void *)&list_length },
{ "list_pop", (void *)&list_pop },
{ "list_push", (void *)&list_push },
{ "list_remove", (void *)&list_remove },
{ "list_tail", (void *)&list_tail },
{ "mac_call_sent_callback", (void *)&mac_call_sent_callback },
{ "mac_sequence_is_duplicate", (void *)&mac_sequence_is_duplicate },
{ "mac_sequence_register_seqno", (void *)&mac_sequence_register_seqno },
{ "main", (void *)&main },
{ "memb_alloc", (void *)&memb_alloc },
{ "memb_free", (void *)&memb_free },
{ "memb_init", (void *)&memb_init },
{ "memb_inmemb", (void *)&memb_inmemb },
{ "memb_numfree", (void *)&memb_numfree },
{ "memcmp", (void *)&memcmp },
{ "memcpy", (void *)&memcpy },
{ "memset", (void *)&memset },
{ "msp430_add_lpm_req", (void *)&msp430_add_lpm_req },
{ "msp430_cpu_init", (void *)&msp430_cpu_init },
{ "msp430_dco_required", (void *)&msp430_dco_required },
{ "msp430_init_dco", (void *)&msp430_init_dco },
{ "msp430_remove_lpm_req", (void *)&msp430_remove_lpm_req },
{ "msp430_sync_dco", (void *)&msp430_sync_dco },
{ "node_id", (void *)&node_id },
{ "node_id_burn", (void *)&node_id_burn },
{ "node_id_restore", (void *)&node_id_restore },
{ "nullsec_driver", (void *)&nullsec_driver },
{ "packetbuf_addrs", (void *)&packetbuf_addrs },
{ "packetbuf_attr_clear", (void *)&packetbuf_attr_clear },
{ "packetbuf_attr_copyfrom", (void *)&packetbuf_attr_copyfrom },
{ "packetbuf_attr_copyto", (void *)&packetbuf_attr_copyto },
{ "packetbuf_attrs", (void *)&packetbuf_attrs },
{ "packetbuf_clear", (void *)&packetbuf_clear },
{ "packetbuf_compact", (void *)&packetbuf_compact },
{ "packetbuf_copyfrom", (void *)&packetbuf_copyfrom },
{ "packetbuf_copyto", (void *)&packetbuf_copyto },
{ "packetbuf_datalen", (void *)&packetbuf_datalen },
{ "packetbuf_dataptr", (void *)&packetbuf_dataptr },
{ "packetbuf_hdralloc", (void *)&packetbuf_hdralloc },
{ "packetbuf_hdrlen", (void *)&packetbuf_hdrlen },
{ "packetbuf_hdrptr", (void *)&packetbuf_hdrptr },
{ "packetbuf_hdrreduce", (void *)&packetbuf_hdrreduce },
{ "packetbuf_holds_broadcast", (void *)&packetbuf_holds_broadcast },
{ "packetbuf_set_datalen", (void *)&packetbuf_set_datalen },
{ "packetbuf_totlen", (void *)&packetbuf_totlen },
{ "printf", (void *)&printf },
{ "process_alloc_event", (void *)&process_alloc_event },
{ "process_current", (void *)&process_current },
{ "process_exit", (void *)&process_exit },
{ "process_init", (void *)&process_init },
{ "process_is_running", (void *)&process_is_running },
{ "process_list", (void *)&process_list },
{ "process_maxevents", (void *)&process_maxevents },
{ "process_nevents", (void *)&process_nevents },
{ "process_poll", (void *)&process_poll },
{ "process_post", (void *)&process_post },
{ "process_post_synch", (void *)&process_post_synch },
{ "process_run", (void *)&process_run },
{ "process_start", (void *)&process_start },
{ "putchar", (void *)&putchar },
{ "puts", (void *)&puts },
{ "queuebuf_addr", (void *)&queuebuf_addr },
{ "queuebuf_attr", (void *)&queuebuf_attr },
{ "queuebuf_datalen", (void *)&queuebuf_datalen },
{ "queuebuf_dataptr", (void *)&queuebuf_dataptr },
{ "queuebuf_debug_print", (void *)&queuebuf_debug_print },
{ "queuebuf_free", (void *)&queuebuf_free },
{ "queuebuf_init", (void *)&queuebuf_init },
{ "queuebuf_new_from_packetbuf", (void *)&queuebuf_new_from_packetbuf },
{ "queuebuf_numfree", (void *)&queuebuf_numfree },
{ "queuebuf_to_packetbuf", (void *)&queuebuf_to_packetbuf },
{ "queuebuf_update_attr_from_packetbuf", (void *)&queuebuf_update_attr_from_packetbuf },
{ "queuebuf_update_from_packetbuf", (void *)&queuebuf_update_from_packetbuf },
{ "rand", (void *)&rand },
{ "rand_r", (void *)&rand_r },
{ "random_init", (void *)&random_init },
{ "random_rand", (void *)&random_rand },
{ "rime_driver", (void *)&rime_driver },
{ "rime_output", (void *)&rime_output },
{ "rime_sniffer_add", (void *)&rime_sniffer_add },
{ "rime_sniffer_remove", (void *)&rime_sniffer_remove },
{ "ringbuf_elements", (void *)&ringbuf_elements },
{ "ringbuf_get", (void *)&ringbuf_get },
{ "ringbuf_init", (void *)&ringbuf_init },
{ "ringbuf_put", (void *)&ringbuf_put },
{ "ringbuf_size", (void *)&ringbuf_size },
{ "rtimer_arch_init", (void *)&rtimer_arch_init },
{ "rtimer_arch_now", (void *)&rtimer_arch_now },
{ "rtimer_arch_schedule", (void *)&rtimer_arch_schedule },
{ "rtimer_init", (void *)&rtimer_init },
{ "rtimer_run_next", (void *)&rtimer_run_next },
{ "rtimer_set", (void *)&rtimer_set },
{ "sbrk", (void *)&sbrk },
{ "sensors", (void *)&sensors },
{ "sensors_changed", (void *)&sensors_changed },
{ "sensors_event", (void *)&sensors_event },
{ "sensors_find", (void *)&sensors_find },
{ "sensors_first", (void *)&sensors_first },
{ "sensors_flags", (void *)&sensors_flags },
{ "sensors_next", (void *)&sensors_next },
{ "sensors_process", (void *)&sensors_process },
{ "serial_line_event_message", (void *)&serial_line_event_message },
{ "serial_line_init", (void *)&serial_line_init },
{ "serial_line_input_byte", (void *)&serial_line_input_byte },
{ "serial_line_process", (void *)&serial_line_process },
{ "serial_shell_init", (void *)&serial_shell_init },
{ "serial_shell_process", (void *)&serial_shell_process },
{ "set_bits", (void *)&set_bits },
{ "shell_append_process", (void *)&shell_append_process },
{ "shell_base64_init", (void *)&shell_base64_init },
{ "shell_binprint_process", (void *)&shell_binprint_process },
{ "shell_coffee_init", (void *)&shell_coffee_init },
{ "shell_dec64_process", (void *)&shell_dec64_process },
{ "shell_default_output", (void *)&shell_default_output },
{ "shell_echo_process", (void *)&shell_echo_process },
{ "shell_event_input", (void *)&shell_event_input },
{ "shell_exec_init", (void *)&shell_exec_init },
{ "shell_exec_process", (void *)&shell_exec_process },
{ "shell_exit", (void *)&shell_exit },
{ "shell_exit_process", (void *)&shell_exit_process },
{ "shell_file_init", (void *)&shell_file_init },
{ "shell_format_process", (void *)&shell_format_process },
{ "shell_hd_process", (void *)&shell_hd_process },
{ "shell_init", (void *)&shell_init },
{ "shell_input", (void *)&shell_input },
{ "shell_kill_process", (void *)&shell_kill_process },
{ "shell_killall_process", (void *)&shell_killall_process },
{ "shell_ls_process", (void *)&shell_ls_process },
{ "shell_null_process", (void *)&shell_null_process },
{ "shell_output", (void *)&shell_output },
{ "shell_output_str", (void *)&shell_output_str },
{ "shell_process", (void *)&shell_process },
{ "shell_prompt", (void *)&shell_prompt },
{ "shell_quit", (void *)&shell_quit },
{ "shell_read_process", (void *)&shell_read_process },
{ "shell_register_command", (void *)&shell_register_command },
{ "shell_rm_process", (void *)&shell_rm_process },
{ "shell_server_process", (void *)&shell_server_process },
{ "shell_set_time", (void *)&shell_set_time },
{ "shell_size_process", (void *)&shell_size_process },
{ "shell_start", (void *)&shell_start },
{ "shell_start_command", (void *)&shell_start_command },
{ "shell_stop", (void *)&shell_stop },
{ "shell_strtolong", (void *)&shell_strtolong },
{ "shell_text_init", (void *)&shell_text_init },
{ "shell_time", (void *)&shell_time },
{ "shell_unregister_command", (void *)&shell_unregister_command },
{ "shell_write_process", (void *)&shell_write_process },
{ "sky_shell_process", (void *)&sky_shell_process },
{ "snprintf", (void *)&snprintf },
{ "spi_busy", (void *)&spi_busy },
{ "spi_init", (void *)&spi_init },
{ "splhigh_", (void *)&splhigh_ },
{ "sprintf", (void *)&sprintf },
{ "srand", (void *)&srand },
{ "strchr", (void *)&strchr },
{ "strcmp", (void *)&strcmp },
{ "strncmp", (void *)&strncmp },
{ "strncpy", (void *)&strncpy },
{ "symtab_lookup", (void *)&symtab_lookup },
{ "timer_expired", (void *)&timer_expired },
{ "timer_remaining", (void *)&timer_remaining },
{ "timer_reset", (void *)&timer_reset },
{ "timer_restart", (void *)&timer_restart },
{ "timer_set", (void *)&timer_set },
{ "timera0", (void *)&timera0 },
{ "timera1", (void *)&timera1 },
{ "uart1_active", (void *)&uart1_active },
{ "uart1_init", (void *)&uart1_init },
{ "uart1_set_input", (void *)&uart1_set_input },
{ "uart1_writeb", (void *)&uart1_writeb },
{ "uip_log", (void *)&uip_log },
{ "vprintf", (void *)&vprintf },
{ "vsnprintf", (void *)&vsnprintf },
{ "vsprintf", (void *)&vsprintf },
{ "vuprintf", (void *)&vuprintf },
{ "watchdog_init", (void *)&watchdog_init },
{ "watchdog_interrupt", (void *)&watchdog_interrupt },
{ "watchdog_periodic", (void *)&watchdog_periodic },
{ "watchdog_reboot", (void *)&watchdog_reboot },
{ "watchdog_start", (void *)&watchdog_start },
{ "watchdog_stop", (void *)&watchdog_stop },
{ "xmem_erase", (void *)&xmem_erase },
{ "xmem_init", (void *)&xmem_init },
{ "xmem_pread", (void *)&xmem_pread },
{ "xmem_pwrite", (void *)&xmem_pwrite },
{ (const char *)0, (void *)0} };
