type tsan_go.cc ..\rtl\tsan_interface_atomic.cc ..\rtl\tsan_clock.cc ..\rtl\tsan_flags.cc ..\rtl\tsan_md5.cc ..\rtl\tsan_mutex.cc ..\rtl\tsan_report.cc ..\rtl\tsan_rtl.cc ..\rtl\tsan_rtl_mutex.cc ..\rtl\tsan_rtl_report.cc ..\rtl\tsan_rtl_thread.cc ..\rtl\tsan_stat.cc ..\rtl\tsan_suppressions.cc ..\rtl\tsan_sync.cc ..\rtl\tsan_stack_trace.cc ..\..\sanitizer_common\sanitizer_allocator.cc ..\..\sanitizer_common\sanitizer_common.cc ..\..\sanitizer_common\sanitizer_flags.cc ..\..\sanitizer_common\sanitizer_stacktrace.cc ..\..\sanitizer_common\sanitizer_libc.cc ..\..\sanitizer_common\sanitizer_printf.cc ..\..\sanitizer_common\sanitizer_suppressions.cc ..\..\sanitizer_common\sanitizer_thread_registry.cc ..\rtl\tsan_platform_windows.cc ..\..\sanitizer_common\sanitizer_win.cc ..\..\sanitizer_common\sanitizer_deadlock_detector1.cc ..\..\sanitizer_common\sanitizer_stackdepot.cc ..\..\sanitizer_common\sanitizer_persistent_allocator.cc > gotsan.cc

gcc -c -o race_windows_amd64.syso gotsan.cc -I..\rtl -I..\.. -I..\..\sanitizer_common -I..\..\..\include -m64 -Wall -fno-exceptions -fno-rtti -DSANITIZER_GO -Wno-error=attributes -Wno-attributes -Wno-format -DSANITIZER_DEBUG=0 -O3 -fomit-frame-pointer

