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
	{ 0xcea0a119, "kmalloc_caches" },
	{ 0x527e538b, "sock_setsockopt" },
	{ 0xdeab4304, "led_classdev_register" },
	{ 0xdefe4ad0, "sock_release" },
	{ 0x41793eb3, "sock_recvmsg" },
	{ 0x91715312, "sprintf" },
	{ 0xe7a0f60c, "current_task" },
	{ 0x50eedeb8, "printk" },
	{ 0x20c55ae0, "sscanf" },
	{ 0xe35ef521, "sock_sendmsg" },
	{ 0xb4390f9a, "mcount" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x41ad0272, "kmem_cache_alloc_trace" },
	{ 0x3755bc92, "led_classdev_unregister" },
	{ 0x37a0cba, "kfree" },
	{ 0xc656ee78, "sock_create" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "BC96F92B52923CC696AC164");
