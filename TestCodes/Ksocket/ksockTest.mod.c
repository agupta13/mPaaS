#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x35ec255d, "module_layout" },
	{ 0xcea0a119, "kmalloc_caches" },
	{ 0x31f8978c, "sock_setsockopt" },
	{ 0x28dfa244, "sock_release" },
	{ 0x54c017df, "sock_recvmsg" },
	{ 0x91715312, "sprintf" },
	{ 0x20c55ae0, "sscanf" },
	{ 0x2b0f21c7, "sock_sendmsg" },
	{ 0xb4390f9a, "mcount" },
	{ 0x41ad0272, "kmem_cache_alloc_trace" },
	{ 0x6597f602, "sock_create" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "6A62F4236C7999411221E96");
