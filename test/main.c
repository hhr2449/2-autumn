extern void(*hook1)();
void h(void (*callback)()) {
    hook1();
}