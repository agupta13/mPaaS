#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x35ec255d, "module_layout" },
	{ 0x15692c87, "param_ops_int" },
	{ 0x3e7910c7, "nf_unregister_hook" },
	{ 0x999872e2, "remove_proc_entry" },
	{ 0xea43a7ae, "nf_register_hook" },
	{ 0xfc635aa2, "create_proc_entry" },
	{ 0x2febb866, "proc_mkdir" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x91715312, "sprintf" },
	{ 0x50eedeb8, "printk" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "D6136FFB7E0B4F2F7C97672");
