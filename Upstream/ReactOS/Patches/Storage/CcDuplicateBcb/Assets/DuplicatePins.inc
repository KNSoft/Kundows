static void run(const char *name, int pin, int when, ULONG flags, int deny, int oom, int present)
{
    void *b=NULL; reset();
    toPin=pin; race=(when==2); busy=deny; allocFail=oom; resident=present;
    if (when==1) seed();
    BOOLEAN ok=request(pin,flags,&b);
    int expected=!oom && present && !(deny && !(flags & PIN_WAIT));
    int check=(!!ok == expected);
    PINTERNAL_BCB target=when ? &winner : &temp;
    if (ok) {
        check &= b == &target->PFCB && (!pin || (lastLock==&target->Lock &&
                 exclusive==!!(flags&PIN_EXCLUSIVE) && (!when || waited==!!(flags&PIN_WAIT))));
        CcUnpinData(b);
    }
    if (when && !oom)
        check &= winner.RefCount==1 && winner.PinCount==(ULONG)pin && !winner.Lock.depth;
    verdict(name,check && !lockedDelete && !missingUnlock && !map.BcbSpinLock &&
            refs==!!when && allocs==frees && frees==deletes && IsListEmpty(&map.BcbList)==!when);
}

static void checkDuplicatePins(void)
{
    run("new pinned",1,0,PIN_WAIT,0,0,1);
    run("new mapped",0,0,MAP_WAIT,0,0,1);
    run("initial lookup hit",1,1,PIN_WAIT,0,0,1);
    run("duplicate map",0,2,MAP_WAIT,0,0,1);
    run("duplicate shared pin",1,2,PIN_WAIT,0,0,1);
    run("duplicate exclusive pin",1,2,PIN_WAIT|PIN_EXCLUSIVE,0,0,1);
    run("duplicate busy no-wait",1,2,PIN_EXCLUSIVE,1,0,1);
    run("duplicate busy shared no-wait",1,2,0,1,0,1);
    run("pin allocation failure",1,0,PIN_WAIT,0,1,1);
    run("map allocation failure",0,0,MAP_WAIT,0,1,1);
    run("pin residency failure",1,0,PIN_WAIT,0,0,0);
    run("duplicate pin residency failure",1,2,PIN_WAIT,0,0,0);
}
