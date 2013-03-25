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
	{ 0xdefe4ad0, "sock_release" },
	{ 0x41793eb3, "sock_recvmsg" },
	{ 0x6339a8bc, "mutex_unlock" },
	{ 0x9fbf4bb2, "platform_device_register_full" },
	{ 0x91715312, "sprintf" },
	{ 0x2b9e43cb, "sysfs_remove_group" },
	{ 0x8b914fa0, "kthread_create_on_node" },
	{ 0x34dff4ec, "input_set_abs_params" },
	{ 0xf976537b, "input_event" },
	{ 0xe7a0f60c, "current_task" },
	{ 0xc5c74531, "__mutex_init" },
	{ 0x50eedeb8, "printk" },
	{ 0x20c55ae0, "sscanf" },
	{ 0x623264ff, "sysfs_create_group" },
	{ 0xe35ef521, "sock_sendmsg" },
	{ 0xb4390f9a, "mcount" },
	{ 0xcf510c4a, "mutex_lock" },
	{ 0xb0fc0f27, "platform_device_unregister" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x38415842, "input_register_device" },
	{ 0x8890be5f, "wake_up_process" },
	{ 0x41ad0272, "kmem_cache_alloc_trace" },
	{ 0x4f68e5c9, "do_gettimeofday" },
	{ 0x37a0cba, "kfree" },
	{ 0x5b1f749d, "input_unregister_device" },
	{ 0xc656ee78, "sock_create" },
	{ 0x1c0ecb76, "input_allocate_device" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "922DC8FFBE23206CF42C599");
